
#ifndef STARTER_H
#define STARTER_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <kpanelapplet.h>
#include <qmap.h>
#include <qstring.h>
#include <qpixmap.h>
// #include <qpainter.h>
#include <kconfig.h>
#include <kshortcut.h>
#include "starteriface.h"
#include "mykey.h"

class DCOPClient;
class QLabel;
class KPopupMenu;
class StartMenu;
class StarterConfig;

class starter : public KPanelApplet, virtual public StarterIface
{
    Q_OBJECT
    
public:
    starter(const QString& configFile, Type t = Normal, int actions = 0,
        QWidget *parent = 0, const char *name = 0);
    ~starter();

    virtual int widthForHeight(int height) const;
    virtual int heightForWidth(int width) const;
    virtual void preferences();
   void popupMenu();
   void showMenu();
protected:
    virtual void resizeEvent ( QResizeEvent * );

private:
   enum State {Default, Hover, Down};
   State state;
   int _iconSize;
   bool eventFilter( QObject *o, QEvent *e );
   bool popupBlocked;
//    bool isDialog_;
   QLabel *mainView;
//    QPainter m_painter;
   DCOPClient *client;
   KPopupMenu *configPopup;
   StartMenu *startMenu;
   QPixmap pixmap;
   QPixmap hoverPixmap;
   QPixmap downPixmap;
   StarterConfig *configDialog;
   typedef QMap<MyKey,QString> ShortcutList;
   ShortcutList shortcutList;
private slots:
   void addShortcut(const KShortcut&);
   void updateShortcutButton(const QString&);
   void configureMenu();
   void reloadImages();
   void updateSettings();
//    void blockMenu();
   void unblockPopupDelayed();
   void unblockPopup();
   void activateShortcutButton(int);
};

#endif
