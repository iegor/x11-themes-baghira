/****************************************************************************
** AquariusButton meta object code from reading C++ file 'aquariusbutton.h'
**
** Created: Sun Apr 8 21:58:40 2012
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "aquariusbutton.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *AquariusButton::className() const
{
    return "AquariusButton";
}

QMetaObject *AquariusButton::metaObj = 0;
static QMetaObjectCleanUp cleanUp_AquariusButton( "AquariusButton", &AquariusButton::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString AquariusButton::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AquariusButton", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString AquariusButton::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AquariusButton", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* AquariusButton::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "c", &static_QUType_varptr, "\x0a", QUParameter::In }
    };
    static const QUMethod slot_0 = {"setColor", 1, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "setColor(QColor)", &slot_0, QMetaData::Public }
    };
    static const QUMethod signal_0 = {"clicked", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "clicked()", &signal_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"AquariusButton", parentObject,
	slot_tbl, 1,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_AquariusButton.setMetaObject( metaObj );
    return metaObj;
}

void* AquariusButton::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "AquariusButton" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL clicked
void AquariusButton::clicked()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

bool AquariusButton::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: setColor((QColor)(*((QColor*)static_QUType_ptr.get(_o+1)))); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool AquariusButton::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: clicked(); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool AquariusButton::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool AquariusButton::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
