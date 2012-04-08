/****************************************************************************
** starter meta object code from reading C++ file 'starter.h'
**
** Created: Sun Apr 8 21:59:59 2012
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "starter.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *starter::className() const
{
    return "starter";
}

QMetaObject *starter::metaObj = 0;
static QMetaObjectCleanUp cleanUp_starter( "starter", &starter::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString starter::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "starter", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString starter::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "starter", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* starter::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = KPanelApplet::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_ptr, "KShortcut", QUParameter::In }
    };
    static const QUMethod slot_0 = {"addShortcut", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"updateShortcutButton", 1, param_slot_1 };
    static const QUMethod slot_2 = {"configureMenu", 0, 0 };
    static const QUMethod slot_3 = {"reloadImages", 0, 0 };
    static const QUMethod slot_4 = {"updateSettings", 0, 0 };
    static const QUMethod slot_5 = {"unblockPopupDelayed", 0, 0 };
    static const QUMethod slot_6 = {"unblockPopup", 0, 0 };
    static const QUParameter param_slot_7[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_7 = {"activateShortcutButton", 1, param_slot_7 };
    static const QMetaData slot_tbl[] = {
	{ "addShortcut(const KShortcut&)", &slot_0, QMetaData::Private },
	{ "updateShortcutButton(const QString&)", &slot_1, QMetaData::Private },
	{ "configureMenu()", &slot_2, QMetaData::Private },
	{ "reloadImages()", &slot_3, QMetaData::Private },
	{ "updateSettings()", &slot_4, QMetaData::Private },
	{ "unblockPopupDelayed()", &slot_5, QMetaData::Private },
	{ "unblockPopup()", &slot_6, QMetaData::Private },
	{ "activateShortcutButton(int)", &slot_7, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"starter", parentObject,
	slot_tbl, 8,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_starter.setMetaObject( metaObj );
    return metaObj;
}

void* starter::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "starter" ) )
	return this;
    if ( !qstrcmp( clname, "StarterIface" ) )
	return (StarterIface*)this;
    return KPanelApplet::qt_cast( clname );
}

bool starter::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: addShortcut((const KShortcut&)*((const KShortcut*)static_QUType_ptr.get(_o+1))); break;
    case 1: updateShortcutButton((const QString&)static_QUType_QString.get(_o+1)); break;
    case 2: configureMenu(); break;
    case 3: reloadImages(); break;
    case 4: updateSettings(); break;
    case 5: unblockPopupDelayed(); break;
    case 6: unblockPopup(); break;
    case 7: activateShortcutButton((int)static_QUType_int.get(_o+1)); break;
    default:
	return KPanelApplet::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool starter::qt_emit( int _id, QUObject* _o )
{
    return KPanelApplet::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool starter::qt_property( int id, int f, QVariant* v)
{
    return KPanelApplet::qt_property( id, f, v);
}

bool starter::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
