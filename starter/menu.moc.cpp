/****************************************************************************
** Panel meta object code from reading C++ file 'menu.h'
**
** Created: Sun Apr 8 21:59:58 2012
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "menu.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Panel::className() const
{
    return "Panel";
}

QMetaObject *Panel::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Panel( "Panel", &Panel::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Panel::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Panel", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Panel::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Panel", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Panel::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"updateSize", 1, param_slot_0 };
    static const QUMethod slot_1 = {"addIcon", 0, 0 };
    static const QUMethod slot_2 = {"runPoof", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "updateSize(int)", &slot_0, QMetaData::Public },
	{ "addIcon()", &slot_1, QMetaData::Private },
	{ "runPoof()", &slot_2, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"message", 1, param_signal_0 };
    static const QUMethod signal_1 = {"clearStatus", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "message(const QString&)", &signal_0, QMetaData::Private },
	{ "clearStatus()", &signal_1, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"Panel", parentObject,
	slot_tbl, 3,
	signal_tbl, 2,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Panel.setMetaObject( metaObj );
    return metaObj;
}

void* Panel::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Panel" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL message
void Panel::message( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

// SIGNAL clearStatus
void Panel::clearStatus()
{
    activate_signal( staticMetaObject()->signalOffset() + 1 );
}

bool Panel::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: updateSize((int)static_QUType_int.get(_o+1)); break;
    case 1: addIcon(); break;
    case 2: runPoof(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Panel::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: message((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: clearStatus(); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool Panel::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool Panel::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *StartMenuButton::className() const
{
    return "StartMenuButton";
}

QMetaObject *StartMenuButton::metaObj = 0;
static QMetaObjectCleanUp cleanUp_StartMenuButton( "StartMenuButton", &StartMenuButton::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString StartMenuButton::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "StartMenuButton", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString StartMenuButton::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "StartMenuButton", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* StartMenuButton::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod slot_0 = {"edit", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "edit()", &slot_0, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"pressed", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_1 = {"hovered", 1, param_signal_1 };
    static const QUMethod signal_2 = {"unhovered", 0, 0 };
    static const QUParameter param_signal_3[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_3 = {"updateSize", 1, param_signal_3 };
    static const QMetaData signal_tbl[] = {
	{ "pressed(const QString&)", &signal_0, QMetaData::Private },
	{ "hovered(const QString&)", &signal_1, QMetaData::Private },
	{ "unhovered()", &signal_2, QMetaData::Private },
	{ "updateSize(int)", &signal_3, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"StartMenuButton", parentObject,
	slot_tbl, 1,
	signal_tbl, 4,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_StartMenuButton.setMetaObject( metaObj );
    return metaObj;
}

void* StartMenuButton::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "StartMenuButton" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL pressed
void StartMenuButton::pressed( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

// SIGNAL hovered
void StartMenuButton::hovered( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 1, t0 );
}

// SIGNAL unhovered
void StartMenuButton::unhovered()
{
    activate_signal( staticMetaObject()->signalOffset() + 2 );
}

// SIGNAL updateSize
void StartMenuButton::updateSize( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 3, t0 );
}

bool StartMenuButton::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: edit(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool StartMenuButton::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: pressed((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: hovered((const QString&)static_QUType_QString.get(_o+1)); break;
    case 2: unhovered(); break;
    case 3: updateSize((int)static_QUType_int.get(_o+1)); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool StartMenuButton::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool StartMenuButton::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *StartMenuEntry::className() const
{
    return "StartMenuEntry";
}

QMetaObject *StartMenuEntry::metaObj = 0;
static QMetaObjectCleanUp cleanUp_StartMenuEntry( "StartMenuEntry", &StartMenuEntry::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString StartMenuEntry::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "StartMenuEntry", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString StartMenuEntry::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "StartMenuEntry", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* StartMenuEntry::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod signal_0 = {"closeMenu", 0, 0 };
    static const QUMethod signal_1 = {"pressed", 0, 0 };
    static const QUMethod signal_2 = {"appDown", 0, 0 };
    static const QUMethod signal_3 = {"appUp", 0, 0 };
    static const QUMethod signal_4 = {"appLeft", 0, 0 };
    static const QUParameter param_signal_5[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_5 = {"hovered", 1, param_signal_5 };
    static const QUParameter param_signal_6[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_6 = {"sayText", 1, param_signal_6 };
    static const QUMethod signal_7 = {"unhovered", 0, 0 };
    static const QUParameter param_signal_8[] = {
	{ 0, &static_QUType_ptr, "StartMenuEntry", QUParameter::In }
    };
    static const QUMethod signal_8 = {"popup", 1, param_signal_8 };
    static const QUMethod signal_9 = {"executed", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "closeMenu()", &signal_0, QMetaData::Private },
	{ "pressed()", &signal_1, QMetaData::Private },
	{ "appDown()", &signal_2, QMetaData::Private },
	{ "appUp()", &signal_3, QMetaData::Private },
	{ "appLeft()", &signal_4, QMetaData::Private },
	{ "hovered(const QString&)", &signal_5, QMetaData::Private },
	{ "sayText(const QString&)", &signal_6, QMetaData::Private },
	{ "unhovered()", &signal_7, QMetaData::Private },
	{ "popup(StartMenuEntry*)", &signal_8, QMetaData::Private },
	{ "executed()", &signal_9, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"StartMenuEntry", parentObject,
	0, 0,
	signal_tbl, 10,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_StartMenuEntry.setMetaObject( metaObj );
    return metaObj;
}

void* StartMenuEntry::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "StartMenuEntry" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL closeMenu
void StartMenuEntry::closeMenu()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

// SIGNAL pressed
void StartMenuEntry::pressed()
{
    activate_signal( staticMetaObject()->signalOffset() + 1 );
}

// SIGNAL appDown
void StartMenuEntry::appDown()
{
    activate_signal( staticMetaObject()->signalOffset() + 2 );
}

// SIGNAL appUp
void StartMenuEntry::appUp()
{
    activate_signal( staticMetaObject()->signalOffset() + 3 );
}

// SIGNAL appLeft
void StartMenuEntry::appLeft()
{
    activate_signal( staticMetaObject()->signalOffset() + 4 );
}

// SIGNAL hovered
void StartMenuEntry::hovered( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 5, t0 );
}

// SIGNAL sayText
void StartMenuEntry::sayText( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 6, t0 );
}

// SIGNAL unhovered
void StartMenuEntry::unhovered()
{
    activate_signal( staticMetaObject()->signalOffset() + 7 );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL popup
void StartMenuEntry::popup( StartMenuEntry* t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 8 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,t0);
    activate_signal( clist, o );
}

// SIGNAL executed
void StartMenuEntry::executed()
{
    activate_signal( staticMetaObject()->signalOffset() + 9 );
}

bool StartMenuEntry::qt_invoke( int _id, QUObject* _o )
{
    return QWidget::qt_invoke(_id,_o);
}

bool StartMenuEntry::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: closeMenu(); break;
    case 1: pressed(); break;
    case 2: appDown(); break;
    case 3: appUp(); break;
    case 4: appLeft(); break;
    case 5: hovered((const QString&)static_QUType_QString.get(_o+1)); break;
    case 6: sayText((const QString&)static_QUType_QString.get(_o+1)); break;
    case 7: unhovered(); break;
    case 8: popup((StartMenuEntry*)static_QUType_ptr.get(_o+1)); break;
    case 9: executed(); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool StartMenuEntry::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool StartMenuEntry::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *AppList::className() const
{
    return "AppList";
}

QMetaObject *AppList::metaObj = 0;
static QMetaObjectCleanUp cleanUp_AppList( "AppList", &AppList::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString AppList::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AppList", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString AppList::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AppList", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* AppList::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QScrollView::staticMetaObject();
    static const QUMethod slot_0 = {"addEntry", 0, 0 };
    static const QUMethod slot_1 = {"addDialog", 0, 0 };
    static const QUMethod slot_2 = {"removeEntry", 0, 0 };
    static const QUMethod slot_3 = {"editEntry", 0, 0 };
    static const QUMethod slot_4 = {"editDialog", 0, 0 };
    static const QUMethod slot_5 = {"clear", 0, 0 };
    static const QUMethod slot_6 = {"reset", 0, 0 };
    static const QUParameter param_slot_7[] = {
	{ "string", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_7 = {"search", 1, param_slot_7 };
    static const QUMethod slot_8 = {"appDown", 0, 0 };
    static const QUMethod slot_9 = {"appUp", 0, 0 };
    static const QUMethod slot_10 = {"appLeft", 0, 0 };
    static const QUMethod slot_11 = {"sort", 0, 0 };
    static const QUParameter param_slot_12[] = {
	{ "string", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_12 = {"showCategory", 1, param_slot_12 };
    static const QUMethod slot_13 = {"unblockPopup", 0, 0 };
    static const QUParameter param_slot_14[] = {
	{ 0, &static_QUType_ptr, "StartMenuEntry", QUParameter::In }
    };
    static const QUMethod slot_14 = {"popup", 1, param_slot_14 };
    static const QMetaData slot_tbl[] = {
	{ "addEntry()", &slot_0, QMetaData::Public },
	{ "addDialog()", &slot_1, QMetaData::Public },
	{ "removeEntry()", &slot_2, QMetaData::Public },
	{ "editEntry()", &slot_3, QMetaData::Public },
	{ "editDialog()", &slot_4, QMetaData::Public },
	{ "clear()", &slot_5, QMetaData::Public },
	{ "reset()", &slot_6, QMetaData::Public },
	{ "search(const QString&)", &slot_7, QMetaData::Public },
	{ "appDown()", &slot_8, QMetaData::Public },
	{ "appUp()", &slot_9, QMetaData::Public },
	{ "appLeft()", &slot_10, QMetaData::Public },
	{ "sort()", &slot_11, QMetaData::Public },
	{ "showCategory(const QString&)", &slot_12, QMetaData::Public },
	{ "unblockPopup()", &slot_13, QMetaData::Public },
	{ "popup(StartMenuEntry*)", &slot_14, QMetaData::Private }
    };
    static const QUMethod signal_0 = {"looseKey", 0, 0 };
    static const QUParameter param_signal_1[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_1 = {"message", 1, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_2 = {"sayText", 1, param_signal_2 };
    static const QUMethod signal_3 = {"clearStatus", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "looseKey()", &signal_0, QMetaData::Private },
	{ "message(const QString&)", &signal_1, QMetaData::Private },
	{ "sayText(const QString&)", &signal_2, QMetaData::Private },
	{ "clearStatus()", &signal_3, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"AppList", parentObject,
	slot_tbl, 15,
	signal_tbl, 4,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_AppList.setMetaObject( metaObj );
    return metaObj;
}

void* AppList::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "AppList" ) )
	return this;
    return QScrollView::qt_cast( clname );
}

// SIGNAL looseKey
void AppList::looseKey()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

// SIGNAL message
void AppList::message( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 1, t0 );
}

// SIGNAL sayText
void AppList::sayText( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 2, t0 );
}

// SIGNAL clearStatus
void AppList::clearStatus()
{
    activate_signal( staticMetaObject()->signalOffset() + 3 );
}

bool AppList::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: addEntry(); break;
    case 1: addDialog(); break;
    case 2: removeEntry(); break;
    case 3: editEntry(); break;
    case 4: editDialog(); break;
    case 5: clear(); break;
    case 6: reset(); break;
    case 7: search((const QString&)static_QUType_QString.get(_o+1)); break;
    case 8: appDown(); break;
    case 9: appUp(); break;
    case 10: appLeft(); break;
    case 11: sort(); break;
    case 12: showCategory((const QString&)static_QUType_QString.get(_o+1)); break;
    case 13: unblockPopup(); break;
    case 14: popup((StartMenuEntry*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QScrollView::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool AppList::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: looseKey(); break;
    case 1: message((const QString&)static_QUType_QString.get(_o+1)); break;
    case 2: sayText((const QString&)static_QUType_QString.get(_o+1)); break;
    case 3: clearStatus(); break;
    default:
	return QScrollView::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool AppList::qt_property( int id, int f, QVariant* v)
{
    return QScrollView::qt_property( id, f, v);
}

bool AppList::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *SearchLine::className() const
{
    return "SearchLine";
}

QMetaObject *SearchLine::metaObj = 0;
static QMetaObjectCleanUp cleanUp_SearchLine( "SearchLine", &SearchLine::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString SearchLine::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SearchLine", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString SearchLine::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SearchLine", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* SearchLine::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = KLineEdit::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"makeCompletion", 1, param_slot_0 };
    static const QUMethod slot_1 = {"block", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "makeCompletion(const QString&)", &slot_0, QMetaData::Protected },
	{ "block()", &slot_1, QMetaData::Protected }
    };
    static const QUParameter param_signal_0[] = {
	{ "string", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"typedTextChanged", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "typedTextChanged(const QString&)", &signal_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"SearchLine", parentObject,
	slot_tbl, 2,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_SearchLine.setMetaObject( metaObj );
    return metaObj;
}

void* SearchLine::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "SearchLine" ) )
	return this;
    return KLineEdit::qt_cast( clname );
}

// SIGNAL typedTextChanged
void SearchLine::typedTextChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

bool SearchLine::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: makeCompletion((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: block(); break;
    default:
	return KLineEdit::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool SearchLine::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: typedTextChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    default:
	return KLineEdit::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool SearchLine::qt_property( int id, int f, QVariant* v)
{
    return KLineEdit::qt_property( id, f, v);
}

bool SearchLine::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *StartMenu::className() const
{
    return "StartMenu";
}

QMetaObject *StartMenu::metaObj = 0;
static QMetaObjectCleanUp cleanUp_StartMenu( "StartMenu", &StartMenu::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString StartMenu::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "StartMenu", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString StartMenu::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "StartMenu", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* StartMenu::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"sayText", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"toggleKTTS", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "category", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"setCategory", 1, param_slot_2 };
    static const QUMethod slot_3 = {"save", 0, 0 };
    static const QUParameter param_slot_4[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"message", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"centerMessage", 1, param_slot_5 };
    static const QUMethod slot_6 = {"clearStatus", 0, 0 };
    static const QUParameter param_slot_7[] = {
	{ "command", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_7 = {"execute", 1, param_slot_7 };
    static const QUParameter param_slot_8[] = {
	{ "string", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_8 = {"search", 1, param_slot_8 };
    static const QUMethod slot_9 = {"endHistory", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "sayText(const QString&)", &slot_0, QMetaData::Public },
	{ "toggleKTTS(bool)", &slot_1, QMetaData::Public },
	{ "setCategory(const QString&)", &slot_2, QMetaData::Public },
	{ "save()", &slot_3, QMetaData::Public },
	{ "message(const QString&)", &slot_4, QMetaData::Private },
	{ "centerMessage(const QString&)", &slot_5, QMetaData::Private },
	{ "clearStatus()", &slot_6, QMetaData::Private },
	{ "execute(const QString&)", &slot_7, QMetaData::Private },
	{ "search(const QString&)", &slot_8, QMetaData::Private },
	{ "endHistory()", &slot_9, QMetaData::Private }
    };
    static const QUMethod signal_0 = {"aboutToHide", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "aboutToHide()", &signal_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"StartMenu", parentObject,
	slot_tbl, 10,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_StartMenu.setMetaObject( metaObj );
    return metaObj;
}

void* StartMenu::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "StartMenu" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL aboutToHide
void StartMenu::aboutToHide()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

bool StartMenu::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: sayText((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: toggleKTTS((bool)static_QUType_bool.get(_o+1)); break;
    case 2: setCategory((const QString&)static_QUType_QString.get(_o+1)); break;
    case 3: save(); break;
    case 4: message((const QString&)static_QUType_QString.get(_o+1)); break;
    case 5: centerMessage((const QString&)static_QUType_QString.get(_o+1)); break;
    case 6: clearStatus(); break;
    case 7: execute((const QString&)static_QUType_QString.get(_o+1)); break;
    case 8: search((const QString&)static_QUType_QString.get(_o+1)); break;
    case 9: endHistory(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool StartMenu::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: aboutToHide(); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool StartMenu::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool StartMenu::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
