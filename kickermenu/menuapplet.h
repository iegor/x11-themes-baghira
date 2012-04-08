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

#ifndef _MENUAPPLET_H_
#define _MENUAPPLET_H_

#include <kpanelapplet.h>
#include <qintdict.h>
#include <qvaluelist.h>
#include <kmenubar.h>
#include <kwindowlistmenu.h>
#include <kwinmodule.h>
#include <kmanagerselection.h>
#include <dcopclient.h>
#include <dcopobject.h>
#include <qxembed.h>
#include <karrowbutton.h>
#include <assert.h>

class QHBoxLayout;
class QLabel;

namespace KickerMenuApplet
{

class MenuEmbed;

class Applet : public KPanelApplet, public DCOPObject
{
    Q_OBJECT
    K_DCOP
k_dcop:
   ASYNC configure();
   virtual void showWindowList();
public:
   friend class MenuEmbed;
   Applet( const QString& configFile, QWidget *parent );
   virtual ~Applet();
   virtual int widthForHeight( int height ) const;
   virtual int heightForWidth( int width ) const;
   void menuLost( MenuEmbed* embed );
   void updateMenuGeometry( MenuEmbed* embed );
protected:
   virtual void paletteChange(const QPalette& );
   virtual void positionChange( Position p );
   bool eventFilter(QObject *, QEvent *ev);
   QMenuBar *windowIndicator;
private slots:
   void moveMenu();
   void windowAdded( WId w );
   void activeWindowChanged( WId w );
   void lostSelection();
   void readSettings();
   void claimSelection();
   // to show current active window (not necessary equal to the menubar)
   void initPopup();
   void fontChanged();
private:
   bool isDisabled() const; // does it own the manager selection?
   WId tryTransientFor( WId w );
   void activateMenu( MenuEmbed* embed );
   static Atom makeSelectionAtom();
   void updateTopEdgeOffset();
   KWinModule* module;
   QValueList< MenuEmbed* > menus;
   MenuEmbed* active_menu;
   KSelectionOwner* selection;
   KSelectionWatcher* selection_watcher;
   bool desktop_menu;
   DCOPClient dcopclient;
   int topEdgeOffset;
   KWindowListMenu* windowList;
   QHBoxLayout *layout;
   QLabel *_menuMover;
   bool wannaMove;
   int menuDX;
};

class MenuEmbed : public QXEmbed
{
   Q_OBJECT
public:
   MenuEmbed( WId mainwindow, bool desktop, QWidget* parent = NULL, const char* name = NULL );
   WId mainWindow() const;
   bool isDesktopMenu() const;
   virtual void setMinimumSize( int w, int h );
   void setMinimumSize( const QSize& s ) { setMinimumSize( s.width(), s.height()); }
   QString title;
protected:
   virtual void windowChanged( WId w );
   virtual bool x11Event( XEvent* ev );
private:
   void sendSyntheticConfigureNotifyEvent();
   WId main_window;
   bool desktop;
};

inline bool Applet::isDisabled() const
{
   assert( ( selection == NULL && selection_watcher != NULL )
         || ( selection != NULL && selection_watcher == NULL ));
   return selection == NULL;
}

inline WId MenuEmbed::mainWindow() const
{
   return main_window;
}

inline bool MenuEmbed::isDesktopMenu() const
{
   return desktop;
}

} // namespace

#endif
