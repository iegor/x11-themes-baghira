/****************************************************************************
**
** DCOP Skeleton generated by dcopidl2cpp from bab_iface.kidl
**
** WARNING! All changes made in this file will be lost!
**
*****************************************************************************/

#include "./bab_iface.h"

#include <kdatastream.h>


static const char* const babInterface_ftable[8][3] = {
    { "void", "toggle()", "toggle()" },
    { "QString", "style()", "style()" },
    { "void", "setStyle(QString)", "setStyle(QString state)" },
    { "QString", "deco()", "deco()" },
    { "void", "setDeco(QString)", "setDeco(QString state)" },
    { "void", "start(QString,QString)", "start(QString name,QString settings)" },
    { "void", "emitStyleChanged()", "emitStyleChanged()" },
    { 0, 0, 0 }
};
static const int babInterface_ftable_hiddens[7] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

bool babInterface::process(const QCString &fun, const QByteArray &data, QCString& replyType, QByteArray &replyData)
{
    if ( fun == babInterface_ftable[0][1] ) { // void toggle()
	replyType = babInterface_ftable[0][0]; 
	toggle( );
    } else if ( fun == babInterface_ftable[1][1] ) { // QString style()
	replyType = babInterface_ftable[1][0]; 
	QDataStream _replyStream( replyData, IO_WriteOnly );
	_replyStream << style( );
    } else if ( fun == babInterface_ftable[2][1] ) { // void setStyle(QString)
	QString arg0;
	QDataStream arg( data, IO_ReadOnly );
	if (arg.atEnd()) return false;
	arg >> arg0;
	replyType = babInterface_ftable[2][0]; 
	setStyle(arg0 );
    } else if ( fun == babInterface_ftable[3][1] ) { // QString deco()
	replyType = babInterface_ftable[3][0]; 
	QDataStream _replyStream( replyData, IO_WriteOnly );
	_replyStream << deco( );
    } else if ( fun == babInterface_ftable[4][1] ) { // void setDeco(QString)
	QString arg0;
	QDataStream arg( data, IO_ReadOnly );
	if (arg.atEnd()) return false;
	arg >> arg0;
	replyType = babInterface_ftable[4][0]; 
	setDeco(arg0 );
    } else if ( fun == babInterface_ftable[5][1] ) { // void start(QString,QString)
	QString arg0;
	QString arg1;
	QDataStream arg( data, IO_ReadOnly );
	if (arg.atEnd()) return false;
	arg >> arg0;
	if (arg.atEnd()) return false;
	arg >> arg1;
	replyType = babInterface_ftable[5][0]; 
	start(arg0, arg1 );
    } else if ( fun == babInterface_ftable[6][1] ) { // void emitStyleChanged()
	replyType = babInterface_ftable[6][0]; 
	emitStyleChanged( );
    } else {
	return DCOPObject::process( fun, data, replyType, replyData );
    }
    return true;
}

QCStringList babInterface::interfaces()
{
    QCStringList ifaces = DCOPObject::interfaces();
    ifaces += "babInterface";
    return ifaces;
}

QCStringList babInterface::functions()
{
    QCStringList funcs = DCOPObject::functions();
    for ( int i = 0; babInterface_ftable[i][2]; i++ ) {
	if (babInterface_ftable_hiddens[i])
	    continue;
	QCString func = babInterface_ftable[i][0];
	func += ' ';
	func += babInterface_ftable[i][2];
	funcs << func;
    }
    return funcs;
}

