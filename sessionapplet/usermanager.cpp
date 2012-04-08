/***************************************************************************
 *   Copyright (C) 2005 by Thomas Lübking   *
 *   thomas.luebking@web.de   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <stdlib.h>
#include <kmenubar.h>
// #include <qlabel.h>
#include <qlayout.h>
#include <qpopupmenu.h>
#include <dcopclient.h>
#include <kglobal.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kconfig.h>
#include <ksimpleconfig.h>
#include <kapplication.h>
#include <kdeversion.h>
#include <kmessagebox.h>
#include <kservice.h>
#include <kstandarddirs.h>
#include "dmctl.h"
#include "usermanager.h"

#define _ICONSET_(i) KGlobal::iconLoader()->loadIconSet(i, KIcon::Small, 0, false)


UserManager::UserManager(const QString& configFile, Type type, int actions, QWidget *parent, const char *name)
    : KPanelApplet(configFile, type, actions, parent, name)
{

   // Get the current application configuration handle
   ksConfig = config();
   lockIcon = _ICONSET_("lock");
   fileSaveIcon = _ICONSET_("filesave");
   exitIcon = _ICONSET_("exit");
   setFixedWidth(600);
   
   menu = new KMenuBar(this);
   menu->setTopLevelMenu(false);
   menu->setFrameShape ( QFrame::NoFrame );
   menu->setMargin ( 0 );
   menu->setLineWidth ( 0 );

   // sessions
   sessionPopup = new QPopupMenu(this);
   menu->insertItem ( getenv("USER"), sessionPopup, 0);
   connect( sessionPopup, SIGNAL(aboutToShow()), SLOT(slotPopulateSessions()) );
   connect( sessionPopup, SIGNAL(activated(int)), SLOT(slotSessionActivated(int)) );
   
   // languages
   langPopup = new QPopupMenu(this);
   
   KConfigBase *config = KGlobal::config();
   config->setGroup("Locale");
   langList = config->readListEntry ("Language", ':');
//    QString flag( locate( "locale", QString::fromLatin1( "l10n/%1/flag.png" ).arg(langList[0]) ) );
//    langLabel = new QLabel(this);
//    langLabel->setPixmap(_ICONSET_(flag).pixmap());
   
   menu->insertItem ( '[' + langList[0].section('_',0,0) + ']', langPopup, 1 );
   menu->connectItem ( 1, this, SLOT(slotPopulateLanguages()) );
   connect( langPopup, SIGNAL(aboutToShow()), SLOT(slotPopulateLanguages()) );
   connect( langPopup, SIGNAL(activated(int)), SLOT(slotLanguageActivated(int)) );
   
   menu->adjustSize();
   
   setFixedWidth(menu->width()+5);

   mainView = menu;
   menu->move(menu->mapToParent(QPoint(0,0)).x(),0);
   menu->show();
}

UserManager::~UserManager()
{
}

int UserManager::widthForHeight(int) const
{
   return menu->width();
}

int UserManager::heightForWidth(int) const
{
   return menu->height();
}

void UserManager::about()
{
    KMessageBox::information(0, i18n("This is an about box"));
}

void UserManager::lock()
{
   QCString appname( "kdesktop" );
//    if ( kicker_screen_number )
//       appname.sprintf("kdesktop-screen-%d", kicker_screen_number);
   kapp->dcopClient()->send(appname, "KScreensaverIface", "lock()", "");
}

void UserManager::logout()
{
   kapp->requestShutDown();
}


void UserManager::slotPopulateLanguages()
{
   langPopup->clear();
   KConfig *config = new KConfig("kdeglobals");
   config->setGroup("Locale");
   langList = config->readListEntry ("Language", ':');
   int i = 0;
   for ( QStringList::ConstIterator it = langList.begin();
         it != langList.end(); ++it )
   {
      KConfig entry(locate( "locale", QString::fromLatin1( "%1/entry.desktop" ).arg(*it) ));
      entry.setGroup("KCM Locale");
      QString name = entry.readEntry("Name", i18n("without name"));
      
      QString flag( locate( "locale", QString::fromLatin1( "%1/flag.png" ).arg(*it) ) );
      langPopup->insertItem( _ICONSET_(flag), name, i );
      i++;
   }
//    langPopup->popup(langLabel->mapToGlobal(QPoint(0,height())));
}

void UserManager::slotLanguageActivated( int i )
{
   if (i == 0)
      return; // no change, no action
   menu->changeItem ( 1, '[' + langList[i].section('_',0,0) + ']'/**langPopup->pixmap(i)*/ );
//    langLabel->setPixmap(*langPopup->pixmap(i));
   KConfig *config = new KConfig("kdeglobals");
   config->setGroup("Locale");
   QString tmp = langList[i];
   langList.remove(langList.at(i));
   langList.prepend(tmp);
   config->writeEntry ("Language", langList, ':');
   config->sync();
//    KService::rebuildKSycoca(this);
   delete config;
}

void UserManager::slotPopulateSessions()
{
   int p = 0;
   DM dm;
   
   sessionPopup->clear();
      // lock
   if (kapp->authorize("lock_screen"))
   {
      sessionPopup->insertItem(lockIcon, i18n("Lock Session"), 102);
   }
   sessionPopup->insertSeparator();
   // switch
   if (kapp->authorize("start_new_session") && (p = dm.numReserve()) >= 0)
   {
      if (kapp->authorize("lock_screen"))
         sessionPopup->insertItem(/*_ICONSET_("lockfork"),*/ i18n("Lock&&Load Session..."), 100 );
      sessionPopup->insertItem(_ICONSET_("fork"), i18n("New Session..."), 101 );
      if (!p) {
         sessionPopup->setItemEnabled( 100, false );
         sessionPopup->setItemEnabled( 101, false );
      }
      sessionPopup->insertSeparator();
   }
   SessList sess;
   if (dm.localSessions( sess ))
      for (SessList::ConstIterator it = sess.begin(); it != sess.end(); ++it) {
         int id = sessionPopup->insertItem( DM::sess2Str( *it ), (*it).vt );
         if (!(*it).vt)
            sessionPopup->setItemEnabled( id, false );
         if ((*it).self)
            sessionPopup->setItemChecked( id, true );
      }
      // save
   sessionPopup->insertSeparator();
   KConfig ksmserver("ksmserverrc", false, false);
   ksmserver.setGroup("General");
   if (ksmserver.readEntry( "loginMode" ) == "restoreSavedSession")
   {
      sessionPopup->insertItem(fileSaveIcon, i18n("Save Session"), 103);
   }
   // logout
   if (kapp->authorize("logout"))
   {
      sessionPopup->insertItem(exitIcon, i18n("Log Out..."), 104);
   }
}

void UserManager::slotSessionActivated( int ent )
{
   switch (ent)
   {
   case 100:
      doNewSession( true );
      break;
   case 101:
      doNewSession( false );
      break;
   case 102:
      lock();
      break;
   case 103:
      saveSession();
      break;
   case 104:
      logout();
      break;
   default:
      if (!sessionPopup->isItemChecked( ent ))
         DM().lockSwitchVT( ent );
   }
}

void UserManager::doNewSession( bool lock_ )
{
   int result = KMessageBox::warningContinueCancel(
      kapp->desktop()->screen(kapp->desktop()->screenNumber(this)),
      i18n("<p>You have chosen to open another desktop session.<br>"
           "The current session will be hidden "
           "and a new login screen will be displayed.<br>"
           "An F-key is assigned to each session; "
           "F%1 is usually assigned to the first session, "
           "F%2 to the second session and so on. "
           "You can switch between sessions by pressing "
           "Ctrl, Alt and the appropriate F-key at the same time. "
           "Additionally, the KDE Panel and Desktop menus have "
           "actions for switching between sessions.</p>")
      .arg(7).arg(8),
      i18n("Warning - New Session"),
      KGuiItem(i18n("&Start New Session"), "fork"),
      ":confirmNewSession",
#if KDE_IS_VERSION(3,4,0) //3.4
      KMessageBox::PlainCaption | 
#endif
      KMessageBox::Notify);
   
   if (result==KMessageBox::Cancel)
      return;
   
   if (lock_)
      lock();
   
   DM().startReserve();
}

void UserManager::saveSession()
{
   QByteArray data;
   kapp->dcopClient()->send( "ksmserver", "default",
                             "saveCurrentSession()", data );
}


extern "C"
{
    KPanelApplet* init( QWidget *parent, const QString& configFile)
    {
        KGlobal::locale()->insertCatalogue("usermanager");
        return new UserManager(configFile, KPanelApplet::Normal, KPanelApplet::About, parent, "usermanager");
    }
}
