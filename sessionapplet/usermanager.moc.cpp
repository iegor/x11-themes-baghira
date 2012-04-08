/****************************************************************************
** UserManager meta object code from reading C++ file 'usermanager.h'
**
** Created: Sun Apr 8 21:59:46 2012
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "usermanager.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *UserManager::className() const
{
    return "UserManager";
}

QMetaObject *UserManager::metaObj = 0;
static QMetaObjectCleanUp cleanUp_UserManager( "UserManager", &UserManager::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString UserManager::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "UserManager", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString UserManager::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "UserManager", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* UserManager::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = KPanelApplet::staticMetaObject();
    static const QUMethod slot_0 = {"slotPopulateSessions", 0, 0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"slotSessionActivated", 1, param_slot_1 };
    static const QUMethod slot_2 = {"slotPopulateLanguages", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"slotLanguageActivated", 1, param_slot_3 };
    static const QMetaData slot_tbl[] = {
	{ "slotPopulateSessions()", &slot_0, QMetaData::Protected },
	{ "slotSessionActivated(int)", &slot_1, QMetaData::Protected },
	{ "slotPopulateLanguages()", &slot_2, QMetaData::Protected },
	{ "slotLanguageActivated(int)", &slot_3, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"UserManager", parentObject,
	slot_tbl, 4,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_UserManager.setMetaObject( metaObj );
    return metaObj;
}

void* UserManager::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "UserManager" ) )
	return this;
    return KPanelApplet::qt_cast( clname );
}

bool UserManager::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: slotPopulateSessions(); break;
    case 1: slotSessionActivated((int)static_QUType_int.get(_o+1)); break;
    case 2: slotPopulateLanguages(); break;
    case 3: slotLanguageActivated((int)static_QUType_int.get(_o+1)); break;
    default:
	return KPanelApplet::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool UserManager::qt_emit( int _id, QUObject* _o )
{
    return KPanelApplet::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool UserManager::qt_property( int id, int f, QVariant* v)
{
    return KPanelApplet::qt_property( id, f, v);
}

bool UserManager::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
