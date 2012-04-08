/****************************************************************************
** ColorPicker meta object code from reading C++ file 'colorpicker.h'
**
** Created: Sun Apr 8 21:58:40 2012
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "colorpicker.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ColorPicker::className() const
{
    return "ColorPicker";
}

QMetaObject *ColorPicker::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ColorPicker( "ColorPicker", &ColorPicker::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ColorPicker::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ColorPicker", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ColorPicker::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ColorPicker", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ColorPicker::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QGroupBox::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "color", &static_QUType_varptr, "\x0a", QUParameter::In }
    };
    static const QUMethod slot_0 = {"setColor", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "red", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"setRed", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "green", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"setGreen", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "blue", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"setBlue", 1, param_slot_3 };
    static const QUMethod slot_4 = {"reset", 0, 0 };
    static const QUMethod slot_5 = {"init", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "setColor(QColor)", &slot_0, QMetaData::Public },
	{ "setRed(int)", &slot_1, QMetaData::Public },
	{ "setGreen(int)", &slot_2, QMetaData::Public },
	{ "setBlue(int)", &slot_3, QMetaData::Public },
	{ "reset()", &slot_4, QMetaData::Public },
	{ "init()", &slot_5, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ "color", &static_QUType_varptr, "\x0a", QUParameter::In }
    };
    static const QUMethod signal_0 = {"colorChanged", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "colorChanged(QColor)", &signal_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ColorPicker", parentObject,
	slot_tbl, 6,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ColorPicker.setMetaObject( metaObj );
    return metaObj;
}

void* ColorPicker::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ColorPicker" ) )
	return this;
    return QGroupBox::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL colorChanged
void ColorPicker::colorChanged( QColor t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_varptr.set(o+1,&t0);
    activate_signal( clist, o );
}

bool ColorPicker::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: setColor((QColor)(*((QColor*)static_QUType_ptr.get(_o+1)))); break;
    case 1: setRed((int)static_QUType_int.get(_o+1)); break;
    case 2: setGreen((int)static_QUType_int.get(_o+1)); break;
    case 3: setBlue((int)static_QUType_int.get(_o+1)); break;
    case 4: reset(); break;
    case 5: init(); break;
    default:
	return QGroupBox::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ColorPicker::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: colorChanged((QColor)(*((QColor*)static_QUType_ptr.get(_o+1)))); break;
    default:
	return QGroupBox::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool ColorPicker::qt_property( int id, int f, QVariant* v)
{
    return QGroupBox::qt_property( id, f, v);
}

bool ColorPicker::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
