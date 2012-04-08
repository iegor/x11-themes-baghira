/*
   Copyright (C) 2004 Oswald Buddenhagen <ossi@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the Lesser GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

   You should have received a copy of the Lesser GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Steet, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "dmctl.h"

#ifdef Q_WS_X11

#include <klocale.h>
#include <dcopclient.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

static enum { Dunno, NoDM, NewKDM, OldKDM/*, GDM*/ } DMType = Dunno;
static const char *ctl, *dpy;

DM::DM() : fd( -1 )
{
	const char *ptr;
	struct sockaddr_un sa;

	if (DMType == Dunno) {
		if (!(dpy = ::getenv( "DISPLAY" )))
			DMType = NoDM;
		else if ((ctl = ::getenv( "DM_CONTROL" )))
			DMType = NewKDM;
		else if ((ctl = ::getenv( "XDM_MANAGED" )) && ctl[0] == '/')
			DMType = OldKDM;
		else
			DMType = NoDM;
	}
	switch (DMType) {
	default:
		return;
	case NewKDM:
		if ((fd = ::socket( PF_UNIX, SOCK_STREAM, 0 )) < 0)
			return;
		sa.sun_family = AF_UNIX;
		if ((ptr = strchr( dpy, ':' )))
			ptr = strchr( ptr, '.' );
		snprintf( sa.sun_path, sizeof(sa.sun_path),
		          "%s/dmctl-%.*s/socket", ctl, ptr ? ptr - dpy : 512, dpy );
		if (::connect( fd, (struct sockaddr *)&sa, sizeof(sa) )) {
			::close( fd );
			fd = -1;
		}
		break;
	case OldKDM:
		{
			QString tf( ctl );
			tf.truncate( tf.find( ',' ) );
			fd = ::open( tf.latin1(), O_WRONLY );
		}
		break;
	}
}

DM::~DM()
{
	if (fd >= 0)
		close( fd );
}

bool
DM::exec( const char *cmd )
{
	QCString buf;

	return exec( cmd, buf );
}

/**
 * Execute a KDM remote control command.
 * @param cmd the command to execute. FIXME: undocumented yet.
 * @param ret the result buffer.
 * @return result:
 *  @li If true, the command was successfully executed.
 *   @p ret might contain addional results.
 *  @li If false and @p ret is empty, a communication error occurred
 *   (most probably KDM is not running).
 *  @li If false and @p ret is non-empty, it contains the error message
 *   from KDM.
 */
bool
DM::exec( const char *cmd, QCString &buf )
{
	bool ret = false;
	int tl;
	unsigned len = 0;

	if (fd < 0)
		goto busted;

	if (::write( fd, cmd, (tl = strlen( cmd )) ) != tl) {
	    bust:
		::close( fd );
		fd = -1;
	    busted:
		buf.resize( 0 );
		return false;
	}
	if (DMType == OldKDM) {
		buf.resize( 0 );
		return true;
	}
	for (;;) {
		if (buf.size() < 128)
			buf.resize( 128 );
		else if (buf.size() < len * 2)
			buf.resize( len * 2 );
		if ((tl = ::read( fd, buf.data() + len, buf.size() - len)) <= 0) {
			if (tl < 0 && errno == EINTR)
				continue;
			goto bust;
		}
		len += tl;
		if (buf[len - 1] == '\n') {
			buf[len - 1] = 0;
			if (len > 2 && buf[0] == 'o' && buf[1] == 'k' && buf[2] < 32)
				ret = true;
			break;
		}
	}
	return ret;
}

bool
DM::canShutdown()
{
	if (DMType == OldKDM)
		return strstr( ctl, ",maysd" ) != 0;

	QCString re;

	return exec( "caps\n", re ) && re.find( "\tshutdown" ) >= 0;
}

void
DM::shutdown( KApplication::ShutdownType shutdownType,
              KApplication::ShutdownMode shutdownMode,
              const QString &bootOption )
{
	if (!bootOption.isEmpty() && DMType != NewKDM)
		return;

	if (shutdownType != KApplication::ShutdownTypeNone) {
		QCString cmd( "shutdown\t" );
		cmd.append( shutdownType == KApplication::ShutdownTypeReboot ?
		            "reboot\t" : "halt\t" );
		if (!bootOption.isNull())
			cmd.append( "=" ).append( bootOption.local8Bit() ).append( "\t" );
		cmd.append( shutdownMode == KApplication::ShutdownModeInteractive ?
		            "ask\n" :
		            shutdownMode == KApplication::ShutdownModeForceNow ?
		            "forcenow\n" :
		            shutdownMode == KApplication::ShutdownModeTryNow ?
		            "trynow\n" : "schedule\n" );
		exec( cmd.data() );
	}
}

bool
DM::bootOptions( QStringList &opts, int &defopt, int &current )
{
	if (DMType != NewKDM)
		return false;

	QCString re;
	if (!exec( "listbootoptions\n", re ))
		return false;

	opts = QStringList::split( '\t', QString::fromLocal8Bit( re.data() ) );
	if (opts.size() < 4)
		return false;

	bool ok;
	defopt = opts[2].toInt( &ok );
	if (!ok)
		return false;
	current = opts[3].toInt( &ok );
	if (!ok)
		return false;

	opts = QStringList::split( ' ', opts[1] );
	for (QStringList::Iterator it = opts.begin(); it != opts.end(); ++it)
		(*it).replace( "\\s", " " );

	return true;
}

void
DM::setLock( bool on )
{
	exec( on ? "lock\n" : "unlock\n" );
}

bool
DM::isSwitchable()
{
	if (DMType == OldKDM)
		return dpy[0] == ':';

	QCString re;

	return exec( "caps\n", re ) && re.find( "\tlocal" ) >= 0;
}

int
DM::numReserve()
{
	if (DMType == OldKDM)
		return strstr( ctl, ",rsvd" ) ? 1 : -1;

	QCString re;
	int p;

	if (!(exec( "caps\n", re ) && (p = re.find( "\treserve " )) >= 0))
		return -1;
	return atoi( re.data() + p + 9 );
}

void
DM::startReserve()
{
	exec("reserve\n");
}

bool
DM::localSessions( SessList &list )
{
	if (DMType == OldKDM)
		return false;

	QCString re;

	if (!exec( "list\talllocal\n", re ))
		return false;
	QStringList sess = QStringList::split( QChar('\t'), re.data() + 3 );
	for (QStringList::ConstIterator it = sess.begin(); it != sess.end(); ++it) {
		QStringList ts = QStringList::split( QChar(','), *it, true );
		SessEnt se;
		se.display = ts[0];
		if (ts[1][0] == '@')
			se.from = ts[1].mid( 1 ), se.vt = 0;
		else
			se.vt = ts[1].mid( 2 ).toInt();
		se.user = ts[2];
		se.session = ts[3];
		se.self = (ts[4].find( '*' ) >= 0);
		se.tty = (ts[4].find( 't' ) >= 0);
		list.append( se );
	}
	return true;
}

void
DM::sess2Str2( const SessEnt &se, QString &user, QString &loc )
{
	if (se.tty) {
		user = i18n("user: ...", "%1: TTY login").arg( se.user );
		loc = se.vt ? QString("vt%1").arg( se.vt ) : se.display ;
	} else {
		user =
			se.user.isEmpty() ?
				se.session.isEmpty() ?
					i18n("Unused") :
					se.session == "<remote>" ?
						i18n("X login on remote host") :
						i18n("... host", "X login on %1").arg( se.session ) :
				i18n("user: session type", "%1: %2")
					.arg( se.user ).arg( se.session );
		loc =
			se.vt ?
				QString("%1, vt%2").arg( se.display ).arg( se.vt ) :
				se.display;
	}
}

QString
DM::sess2Str( const SessEnt &se )
{
	QString user, loc;

	sess2Str2( se, user, loc );
	return i18n("session (location)", "%1 (%2)").arg( user ).arg( loc );
}

bool
DM::switchVT( int vt )
{
	return exec( QString("activate\tvt%1\n").arg(vt).latin1() );
}

void
DM::lockSwitchVT( int vt )
{
	if (switchVT( vt ))
		kapp->dcopClient()->send( "kdesktop", "KScreensaverIface", "lock()", "" );
}

#endif // Q_WS_X11
