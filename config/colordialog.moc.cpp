/****************************************************************************
** DemoWindow meta object code from reading C++ file 'colordialog.h'
**
** Created: Sun Apr 8 21:58:33 2012
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "colordialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *DemoWindow::className() const
{
    return "DemoWindow";
}

QMetaObject *DemoWindow::metaObj = 0;
static QMetaObjectCleanUp cleanUp_DemoWindow( "DemoWindow", &DemoWindow::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString DemoWindow::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "DemoWindow", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString DemoWindow::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "DemoWindow", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* DemoWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod slot_0 = {"smartRepaint", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "smartRepaint()", &slot_0, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ "c", &static_QUType_ptr, "Colors", QUParameter::In }
    };
    static const QUMethod signal_0 = {"selected", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "selected(Colors)", &signal_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"DemoWindow", parentObject,
	slot_tbl, 1,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_DemoWindow.setMetaObject( metaObj );
    return metaObj;
}

void* DemoWindow::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "DemoWindow" ) )
	return this;
    return QWidget::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL selected
void DemoWindow::selected( Colors t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

bool DemoWindow::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: smartRepaint(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool DemoWindow::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: selected((Colors)(*((Colors*)static_QUType_ptr.get(_o+1)))); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool DemoWindow::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool DemoWindow::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *ColorDialog::className() const
{
    return "ColorDialog";
}

QMetaObject *ColorDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ColorDialog( "ColorDialog", &ColorDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ColorDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ColorDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ColorDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ColorDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ColorDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"grabColor", 0, 0 };
    static const QUParameter param_slot_1[] = {
	{ "string", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"getFromOther", 1, param_slot_1 };
    static const QMetaData slot_tbl[] = {
	{ "grabColor()", &slot_0, QMetaData::Private },
	{ "getFromOther(const QString&)", &slot_1, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ "color", &static_QUType_varptr, "\x0a", QUParameter::In }
    };
    static const QUMethod signal_0 = {"colorChanged", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "colorChanged(QColor)", &signal_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"ColorDialog", parentObject,
	slot_tbl, 2,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ColorDialog.setMetaObject( metaObj );
    return metaObj;
}

void* ColorDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ColorDialog" ) )
	return this;
    return QDialog::qt_cast( clname );
}

// SIGNAL colorChanged
void ColorDialog::colorChanged( QColor t0 )
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

bool ColorDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: grabColor(); break;
    case 1: getFromOther((const QString&)static_QUType_QString.get(_o+1)); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ColorDialog::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: colorChanged((QColor)(*((QColor*)static_QUType_ptr.get(_o+1)))); break;
    default:
	return QDialog::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool ColorDialog::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool ColorDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
