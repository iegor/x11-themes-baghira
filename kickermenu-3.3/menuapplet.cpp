/*****************************************************************

Copyright (c) 2002 Siegfried Nijssen <snijssen@liacs.nl>
Copyright (c) 2003 Lubos Lunak <l.lunak@suse.cz>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

******************************************************************/

#define _MENUAPPLET_CPP_

#include <qvariant.h> // avoid X11 #define's

#include "menuapplet.h"

#include <assert.h>
#include <qcursor.h>
#include <kconfig.h>
#include <qtooltip.h>
#include <kglobal.h>
#include <klocale.h>
#include <qlayout.h>
#include <kdebug.h>
#include <netwm.h>
#include <kapplication.h>
#include <kwin.h>

#include <X11/Xlib.h>

/*

 KMenuBar from KDE3.1 and older won't work very well with this applet.
 This is because QMenuBar tries really hard to keep its preffered size,
 se even if the X window for the menubar has the size enforced by this
 applet, Qt thinks it has the size Qt wants. This results in parts
 of the menubar not being repainted. Also, old KMenuBar always forced
 with to be the width of the screen, so even if the menubar has only
 few entries, this applet will still indicate the menubar doesn't
 fit completely in it. There's no way to fix this, besides upgrading
 to KDE3.2.

*/


extern Time qt_x_time;

extern "C"
{
    KPanelApplet* init( QWidget* parent_P, const QString& configFile_P )
    {
      KGlobal::locale()->insertCatalogue("kmenuapplet");
      return new KickerMenuApplet::Applet( configFile_P, parent_P );
    }
}

namespace KickerMenuApplet
{

static const int MOVE_DIFF = 100; // size increment for left/right menu moving
static const int GROW_WIDTH = 10; // width of grow buttons

const long SUPPORTED_WINDOW_TYPES = NET::NormalMask | NET::DesktopMask | NET::DockMask
                | NET::ToolbarMask | NET::MenuMask | NET::DialogMask | NET::OverrideMask
                | NET::TopMenuMask | NET::UtilityMask | NET::SplashMask;

Applet::Applet( const QString& configFile_P, QWidget* parent_P )
    :   KPanelApplet( configFile_P, Stretch, 0, parent_P, "menuapplet" ),
        DCOPObject( "menuapplet" ),
        module( NULL ),
	active_menu( NULL ),
	grow_indicator_left( this, LeftArrow ),
	grow_indicator_right( this, RightArrow ),
        selection( NULL ),
        selection_watcher( NULL ),
        desktop_menu( false )
    {
    // for windowindicator
    setBackgroundOrigin(AncestorOrigin);
    QIntDict<QMenuBar>(97); //allowing 97 entries - should be enough, if not - who cares: just a label and hashtables are slightly faster than lists...
    windowIndicator = new QMenuBar(this);
    windowList = new KWindowListMenu(windowIndicator);
    connect(windowList, SIGNAL(aboutToShow()), this, SLOT(initPopup()));
    windowIndicator->insertItem( "KDE", windowList, 0);
    windowIndicator->move(0,-1);
    windowIndicator->show();
    windowIndicator->adjustSize();
    windowIndicator->setFixedSize(windowIndicator->size());
    //==============
    dcopclient.registerAs( "menuapplet", false );
    grow_indicator_left.hide();
    grow_indicator_right.hide();
    connect( &grow_indicator_left, SIGNAL( clicked()), SLOT( growClicked()));
    connect( &grow_indicator_right, SIGNAL( clicked()), SLOT( growClicked()));
    // toolbarAppearanceChanged(int) is sent when changing macstyle
    connect( kapp, SIGNAL( toolbarAppearanceChanged( int )),
        this, SLOT( readSettings()));
    claimSelection();
    readSettings();
    }

Applet::~Applet()
    {
    lostSelection(); // release all menu's before really loosing the selection
    /*if (windowIndicator) */delete windowIndicator;
    delete selection;
    delete selection_watcher;
    delete module;
    KGlobal::locale()->removeCatalogue("kmenuapplet");
    }

void Applet::initPopup()
    // helper to allow connecting windowlist activation to reinit - must be, as the list doesn't update itself
    {
    windowList->init();
    }

void Applet::windowRemoved(WId w_P)
    // another helping slot
    {
        windowTitleDict.setAutoDelete(TRUE);
        windowTitleDict.remove(w_P);
    }
    
void Applet::windowAdded( WId w_P )
    {
    QString* title;
    NETWinInfo info( qt_xdisplay(), w_P, qt_xrootwin(), NET::WMWindowType);
    XClassHint classHint;
    if (!windowTitleDict[w_P] && w_P != qt_xrootwin() && XGetClassHint( qt_xdisplay(), w_P, &classHint ) ) 
    {
        // much better way to get a nice name!
        title = new QString( classHint.res_class );
        XFree( classHint.res_name );
        XFree( classHint.res_class );
        windowTitleDict.insert(w_P, title);
    }
    else
    { //fallback
    // for windowindicator
    if (!windowTitleDict[w_P] && w_P != qt_xrootwin()){
       KWin::WindowInfo tmpInfo = KWin::windowInfo( w_P, NET::WMName );
        //generate title:
        title = new QString(tmpInfo.name());
        if (title->contains("K3b")){
            // hack, weil herr trueg zwar ein erstklassiges brennprogramm schreiben, sich aber leider nicht an windowmanager konventionen halten kan... ;)
            *title = "K3b";
        }
        else if (title->contains("GIMP")){
            *title = "GIMP"; //seems as if the title is "The Gimp" in english versions?!?
        }
        else {
        if (title->contains(" - "))
            *title = title->section(" - ", -1, -1); // get rid of application subinfo
        if (title->contains(" "))
            *title = title->section(" ", 0, 0); // get rid of version numbers
        }
        // add title
        windowTitleDict.insert(w_P, title);
    }
    }
    // =========
    if( info.windowType() != NET::TopMenu )
	return;
//    kdDebug() << "embedding:" << w_P << endl;
    Window transient_for = KWin::transientFor( w_P );
    if( transient_for == None )
	return;
    MenuEmbed* embed;
    if( transient_for == qt_xrootwin())
        embed = new MenuEmbed( transient_for, true, this );
    else
        {
        KWin::WindowInfo info2 = KWin::windowInfo( transient_for, NET::WMWindowType );
        embed = new MenuEmbed( transient_for,
            info2.windowType( SUPPORTED_WINDOW_TYPES ) == NET::Desktop, this );
        }
    
    embed->hide();
    embed->move( 0, 0 );
    embed->resize(size());
    embed->embed( w_P );
    //embed->setMinimumSize( size());
    embed->setMinimumSize( 0,0);
    if( embed->embeddedWinId() == None )
	{
	delete embed;
	return;
	}
    embed->installEventFilter( this );
    menus.append( embed );
    // in case the app mapped its menu after its mainwindow, check which menu should be shown
    activeWindowChanged( module->activeWindow());
    }

// - if the active window has its topmenu -> show the menu
// - if desktop menu is enabled (i.e. explicitly in kdesktop) :
//   - show it
//   - otherwise show nothing
void Applet::activeWindowChanged( WId w_P )
    {
//    kdDebug() << "active:" << w_P << endl;
    QFont tmpFont;
    for( WId window = w_P;
	 window != None;
	 window = tryTransientFor( window ))
	{
	for( QValueList< MenuEmbed* >::ConstIterator it = menus.begin();
	     it != menus.end();
	     ++it )
	    {
	    if( window == (*it)->mainWindow())
                {
                tmpFont = (*it)->font();
                tmpFont.setBold(true);
                windowIndicator->setFont(tmpFont);
                QString *newTitle = windowTitleDict[w_P];
                newTitle ? 
                windowIndicator->changeItem(0, *newTitle):
                windowIndicator->changeItem(0, "KDE");
                windowIndicator->setMinimumSize(0, windowIndicator->height());
                windowIndicator->setMaximumSize(20000, windowIndicator->height());
                windowIndicator->adjustSize();
                windowIndicator->setFixedSize(windowIndicator->size());
                (*it)->setFixedSize(width() - windowIndicator->width()-1, height());
                activateMenu( *it );
	        return;
		}
	    }
	}
//    kdDebug() << "no active" << endl;
    // No menu for active window found - if desktop menu
    // (in kdesktoprc) is enabled, use kdesktop's menu instead of none.
    bool try_desktop = desktop_menu;
    if( !try_desktop && w_P != None )
        { // also use the desktop menu if the active window is desktop
        KWin::WindowInfo info = KWin::windowInfo( w_P, NET::WMWindowType );
        if( info.windowType( SUPPORTED_WINDOW_TYPES ) == NET::Desktop )
            try_desktop = true;
        }
    if( try_desktop )
        {
	for( QValueList< MenuEmbed* >::ConstIterator it = menus.begin();
	     it != menus.end();
	     ++it )
	    {
            if( (*it)->isDesktopMenu())
                {
                tmpFont = (*it)->font();
                tmpFont.setBold(true);
                windowIndicator->setFont(tmpFont);
                QString *newTitle = windowTitleDict[w_P];
                newTitle ? 
                windowIndicator->changeItem(0, *newTitle):
                windowIndicator->changeItem(0, "KDE");
                windowIndicator->setMinimumSize(0, windowIndicator->height());
                windowIndicator->setMaximumSize(20000, windowIndicator->height());
                windowIndicator->adjustSize();
                windowIndicator->setFixedSize(windowIndicator->size());
                (*it)->setFixedSize(width() - windowIndicator->width()-1, height());
                activateMenu( *it );
                return;
                }
            }
        }
        QString *newTitle = windowTitleDict[w_P];
        newTitle ? 
        windowIndicator->changeItem(0, *newTitle):
        windowIndicator->changeItem(0, "KDE");
        windowIndicator->setMinimumSize(0, windowIndicator->height());
        windowIndicator->setMaximumSize(20000, windowIndicator->height());
        windowIndicator->adjustSize();
        windowIndicator->setFixedSize(windowIndicator->size());
        if (active_menu) active_menu->setFixedSize(width() - windowIndicator->width()-1, height());
    activateMenu( NULL );
    }

void Applet::activateMenu( MenuEmbed* embed_P )
    {
    if( embed_P != active_menu )
	{
//        kdDebug() << "activate:" << embed_P << endl;
	if( active_menu != NULL ){
	    active_menu->hide();
        }
	active_menu = embed_P;
        if( active_menu != NULL ){
	    active_menu->show();
        }
	}
        moveMenu( NULL );
    }

bool Applet::eventFilter( QObject* obj_P, QEvent* ev_P )
    {
    if( ev_P->type() == QEvent::Resize && obj_P == active_menu )
	moveMenu( NULL );
    return false;
    }

void Applet::growClicked()
    {
    moveMenu( static_cast< const QWidget* >( sender()));
    }

void Applet::updateGrowIndicators()
    {
//    kdDebug() << "ACT:" << active_menu << ":" << ( active_menu ? active_menu->x() : -1 ) << ":" << ( active_menu ? active_menu->x() + active_menu->width() : -1 ) << ":" << width() << endl;
    if( active_menu != NULL && active_menu->x() < 0 )
        {
	grow_indicator_left.raise();
	grow_indicator_left.setGeometry( 0, 0, GROW_WIDTH, height());
	grow_indicator_left.show();
	}
    else
        grow_indicator_left.hide();
    if( active_menu != NULL && active_menu->x() + active_menu->width() > width())
        {
	grow_indicator_right.raise();
	grow_indicator_right.setGeometry( width() - GROW_WIDTH, 0, GROW_WIDTH, height());
	grow_indicator_right.show();
	}
    else
        grow_indicator_right.hide();
    }

void Applet::moveMenu( const QWidget* indicator_P )
    {
    if( active_menu == NULL )
        {
        updateGrowIndicators();
        return;
        }
    if( indicator_P == NULL ) // no indicator -> reset pos
        active_menu->move( 0 + windowIndicator->width() + 1, 0 );
    else if( indicator_P == &grow_indicator_right ) // -> move left
        {                          // negative x
        if( windowIndicator->width() + 1 + active_menu->width() + active_menu->x() > width())
            {
            int newx = windowIndicator->width() + 1 + active_menu->x() - MOVE_DIFF;
            if( active_menu->width() + newx < width())
                newx = windowIndicator->width() + 1 + width() - active_menu->width();
            active_menu->move( newx, 0 );
            }
        }
    else // indicator_P == &grow_indicator_left  // -> move right
        {
        if( active_menu->x() < 0 )
            {
            int newx = active_menu->x() + windowIndicator->width() + 1 + MOVE_DIFF;
            newx = (( newx - MOVE_DIFF + 1 ) / MOVE_DIFF ) * MOVE_DIFF;
            active_menu->move( newx, 0 );
            }
        }
    updateGrowIndicators();
    }

// If there's no menu for the window, try finding menu for its mainwindow
// (where the window's WM_TRANSIENT_FOR property points).
// If the window is modal (_NET_WM_STATE_MODAL), stop.
WId Applet::tryTransientFor( WId w_P )
    {
/*    KWin::WindowInfo info = KWin::windowInfo( w_P, NET::WMState );
    if( info.state() & NET::Modal )
        return None;*/
    WId ret = KWin::transientFor( w_P );
    if( ret == qt_xrootwin())
        ret = None;
    return ret;
    }

void Applet::menuLost( MenuEmbed* embed )
    {
    for( QValueList< MenuEmbed* >::Iterator it = menus.begin();
	 it != menus.end();
	 ++it )
	{
	if( *it == embed )
	    {
	    menus.remove( it );
	    embed->deleteLater();
//	    kdDebug() << "deleting:" << (*it)->mainWindow() << endl;
	    if( embed == active_menu )
		{
		active_menu = NULL;
                updateGrowIndicators();
		// trigger selecting new active menu
		activeWindowChanged( module->activeWindow());
		}
	    return;
	    }
	}
    }

void Applet::resizeEvent( QResizeEvent* ev )
    {
    for( QValueList< MenuEmbed* >::ConstIterator it = menus.begin();
	 it != menus.end();
	 ++it )
	(*it)->setFixedSize( width() - windowIndicator->width()-1 ,height());
    KPanelApplet::resizeEvent( ev );
    moveMenu( NULL );
    }

void Applet::paletteChange(const QPalette & /* oldPalette */)
    {
	if( active_menu != NULL )
	    {
	    active_menu->hide();
	    active_menu->show();
	    }
    }

void Applet::claimSelection()
    {
    assert( selection == NULL );
    selection = new KSelectionOwner( makeSelectionAtom(), DefaultScreen( qt_xdisplay()));
// force taking the selection, but don't kill previous owner
    if( selection->claim( true, false ))
	{
        delete selection_watcher;
        selection_watcher = NULL;
        connect( selection, SIGNAL( lostOwnership()), SLOT( lostSelection()));
        module = new KWinModule;
	connect( module, SIGNAL( windowAdded( WId )), this, SLOT( windowAdded( WId )));
        connect( module, SIGNAL( windowRemoved( WId )), this, SLOT( windowRemoved( WId )));
	connect( module, SIGNAL( activeWindowChanged( WId )),
	    this, SLOT( activeWindowChanged( WId )));
	QValueList< WId > windows = module->windows();
	for( QValueList< WId >::ConstIterator it = windows.begin();
	     it != windows.end();
	     ++it )
	    windowAdded( *it );
	activeWindowChanged( module->activeWindow());
	}
    else
        lostSelection();
    }

void Applet::lostSelection()
    {
    if( selection == NULL )
        return;
//    kdDebug() << "lost selection" << endl;
    for( QValueList< MenuEmbed* >::ConstIterator it = menus.begin();
	 it != menus.end();
	 ++it )
	delete (*it); // delete all MenuEmbed's = release all menus
    menus.clear();
    active_menu = NULL;
    if( selection_watcher == NULL )
        {
        selection_watcher = new KSelectionWatcher( makeSelectionAtom(), DefaultScreen( qt_xdisplay()));
        connect( selection_watcher, SIGNAL( lostOwner()), this, SLOT( claimSelection()));
        }
    delete module;
    module = NULL;
    selection->deleteLater();
    selection = NULL;
    // selection_watcher stays
    }

void Applet::readSettings()
    {
    KConfig cfg( "kdesktoprc", true );
    cfg.setGroup( "Menubar" );
    desktop_menu = cfg.readBoolEntry( "ShowMenubar", false );
    cfg.setGroup( "KDE" );
    if( cfg.readBoolEntry( "macStyle", false ) || desktop_menu )
        QToolTip::remove( this );
    else
        QToolTip::add( this, i18n(
            "You do not appear to have enabled the standalone menubar; "
            "enable it in the Behavior control module for desktop." ));
    if( !isDisabled() && active_menu == NULL )
        activeWindowChanged( module->activeWindow()); //enforce desktop_menu
    }

void Applet::configure()
    {
    readSettings();
    }

int Applet::widthForHeight( int ) const
    {
    return 200; // we're stretch applet
    }

int Applet::heightForWidth( int ) const
    {
    // *shrug* running this applet in vertical mode is a bad idea anyway
    return 50;
    }

static Atom selection_atom = None;
static Atom msg_type_atom = None;

static
void initAtoms()
    {
    char nm[ 100 ];
    sprintf( nm, "_KDE_TOPMENU_OWNER_S%d", DefaultScreen( qt_xdisplay()));
    char nm2[] = "_KDE_TOPMENU_MINSIZE";
    char* names[ 2 ] = { nm, nm2 };
    Atom atoms[ 2 ];
    XInternAtoms( qt_xdisplay(), names, 2, False, atoms );
    selection_atom = atoms[ 0 ];
    msg_type_atom = atoms[ 1 ];
    }

Atom Applet::makeSelectionAtom()
    {
    if( selection_atom == None )
	initAtoms();
    return selection_atom;
    }

MenuEmbed::MenuEmbed( WId mainwindow_P, bool desktop_P,
    QWidget* parent_P, const char* name_P )
    :   QXEmbed( parent_P, name_P ),
	main_window( mainwindow_P ),
        desktop( desktop_P )
    {
    setAutoDelete( false );
    setBackgroundOrigin(AncestorOrigin);
    }

void MenuEmbed::windowChanged( WId w_P )
    {
    if( w_P == None )
	static_cast< Applet* >( parent())->menuLost( this );
    }

bool MenuEmbed::x11Event( XEvent* ev_P )
    {
    if( ev_P->type == ConfigureRequest
	&& ev_P->xconfigurerequest.window == embeddedWinId()
        && ev_P->xconfigurerequest.value_mask & ( CWWidth | CWHeight ))
        {
	XConfigureRequestEvent& ev = ev_P->xconfigurerequest;
        QSize new_size = size();
        if( ev.value_mask & CWWidth )
            new_size.setWidth( ev.width );
        if( ev.value_mask & CWHeight )
            new_size.setHeight( ev.height );
	// resize when the embedded window resizes (still obey min size)
//	kdDebug() << "RES:" << embeddedWinId() << ":" << ev.width << ":" << ev.height << endl;
	if( ev.width != width() || ev.height != height())
	    resize( ev.width, ev.height );
	sendSyntheticConfigureNotifyEvent();
//        int x, y;
//        unsigned int w, h, d, b;
//        Window root;
//        XGetGeometry( qt_xdisplay(), embeddedWinId(), &root, &x, &y, &w, &h, &b, &d );
//        kdDebug() << "RES3:" << width() << ":" << height() << ":" << w << ":" << h << endl;
	return true;
	}
    return QXEmbed::x11Event( ev_P );
    }

void MenuEmbed::sendSyntheticConfigureNotifyEvent()
{
    QPoint globalPos = mapToGlobal(QPoint(0,0));
    if (embeddedWinId()) {
        XConfigureEvent c;
        memset(&c, 0, sizeof(c));
        c.type = ConfigureNotify;
        c.display = qt_xdisplay();
        c.send_event = True;
        c.event = embeddedWinId();
        c.window = winId();
        c.x = globalPos.x();
        c.y = globalPos.y();
        c.width = width();
        c.height = height();
        c.border_width = 0;
        c.above = None;
        c.override_redirect = 0;
        XSendEvent( qt_xdisplay(), c.event, TRUE, StructureNotifyMask, (XEvent*)&c );
    }
}

void MenuEmbed::setMinimumSize( int w, int h )
{
    QXEmbed::setMinimumSize( w, h );
    // tell the menubar also the allowed minimum size
    // the applet won't allow resizing to smaller size
    if( embeddedWinId() != None )
        {
//        kdDebug() << "RES2:" << width() << ":" << height() << ":" << minimumWidth() << ":" << minimumHeight() << endl;
        XEvent ev;
        ev.xclient.display = qt_xdisplay();
        ev.xclient.type = ClientMessage;
        ev.xclient.window = embeddedWinId();
        assert( msg_type_atom != None );
        ev.xclient.message_type = msg_type_atom;
        ev.xclient.format = 32;
        ev.xclient.data.l[0] = qt_x_time;
        ev.xclient.data.l[1] = minimumWidth();
        ev.xclient.data.l[2] = minimumHeight();
        ev.xclient.data.l[3] = 0;
        ev.xclient.data.l[4] = 0;
        XSendEvent( qt_xdisplay(), embeddedWinId(), False, NoEventMask, &ev );
        }
}

} // namespace

#include "menuapplet.moc"
