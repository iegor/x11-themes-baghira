
#ifndef STARTMENU_H
#define STARTMENU_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qpoint.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qscrollview.h>
#include <qmap.h>
#include <qpixmap.h>
#include <qsortedlist.h>
#include <qptrlist.h>
#include <klineedit.h>
#include <kservicegroup.h>
#include "mykey.h"

class KIconLoader;
class QPaintEvent;
class QMouseEvent;
class QEvent;
class QPixmap;
class QVBoxLayout;
class QComboBox;
class StartMenuButton;
class KConfig;
class LinkConfig;

class Panel : public QWidget//QScrollView
{
   friend class StartMenu;
   friend class StartMenuButton;
Q_OBJECT
      
public:
   Panel(int size = 32, QWidget * parent = 0, const char * name = 0);
   Orientation orientation(){ return _orientation; }
   void setOrientation ( Orientation o );
//    void addItem ( QWidget *w );
   StartMenuButton* addIcon ( QString icon, QString title, QString command, QPoint pt = QPoint(-1,-1) );
   void save(KConfig* config);
   void repositionIcon ( StartMenuButton* bt, QPoint pt );
   void ensureVisible(QRect & rect);
   void reloadIcons(int size);
   void poof();
public slots:
   void updateSize(int);
protected:
   void wheelEvent ( QWheelEvent * );
   void dragEnterEvent ( QDragEnterEvent * );
   void dropEvent ( QDropEvent * );
   void mouseReleaseEvent ( QMouseEvent * e );
   void resizeEvent ( QResizeEvent * );
   LinkConfig *linkConfigDialog;
   int _size;
private:
   int _count;
   Orientation _orientation;
   bool _draggedMe;
   int _poofIndex;
   QPoint iconAddPosition;
   QPixmap *_poofPix;
   QPixmap *_poofAnimPix;
   QWidget *_poof;
private slots:
   void addIcon();
   void runPoof();
signals:
   void message(const QString&);
   void clearStatus();
};

class QTextDrag;

class StartMenuButton : public QWidget
{
   friend class Panel;
   Q_OBJECT

public:
   enum Orientation { Horizontal = 0, Vertical, Status };
   StartMenuButton ( int size, QString icon, QString title, QString command, Orientation orientation, QWidget* parent = 0, const char * name = 0);
   void smartMove(QPoint & pt); // prevents collisions with other childs of parentWidget()
   void smartMove(int x, int y);
   void reloadIcon(int size);
public slots:
   void edit();
protected:
   QString & command(){return m_command;}
   QString & title(){return m_title;}
   QString & icon(){return m_icon;}
   void mouseReleaseEvent ( QMouseEvent * e );
   void mouseMoveEvent ( QMouseEvent * e );
   void enterEvent( QEvent * );
   void leaveEvent( QEvent * );
   bool isMoving() {return _moving;}
private:
   Orientation m_orientation;
   bool _moving;
   QTextDrag *myDrag;
   QString m_command;
   QString m_title;
   QString m_icon;
   QLabel* m_titleLabel;
   QLabel* m_pixmapLabel;
   QPixmap m_pix;
   QPixmap m_hoverPix;
signals:
   void pressed(const QString &);
   void hovered(const QString &);
   void unhovered();
   void updateSize(int);
};

class KService;
class QDate;
class AppList;

class StartMenuEntry : public QWidget
{
   friend class AppList;
   Q_OBJECT
public:
   StartMenuEntry(KService * service, QString relPath, int size = 32, bool neewbie = false, QWidget * parent = 0);
   ~StartMenuEntry();
   void reloadIcon(int size);
   bool display;
   QString title();
   void saveStats();
   int rank;
   bool forNewbie;
   //--- operators to allow use of qHeapSort()
   bool operator==( const StartMenuEntry& se ) const;
   bool operator!=( const StartMenuEntry& se ) const;
   bool operator<( const StartMenuEntry& se ) const;
   bool operator>( const StartMenuEntry& se ) const;
   bool operator==( const double& d ) const;
   bool operator!=( const double& d ) const;
   bool operator<( const double& d ) const;
   bool operator>( const double& d ) const;
protected:
   void focusInEvent ( QFocusEvent * );
   void focusOutEvent ( QFocusEvent * );
   void mouseReleaseEvent ( QMouseEvent * e );
   void mouseMoveEvent ( QMouseEvent * mme );
   void keyPressEvent ( QKeyEvent * e );
   void enterEvent( QEvent * );
   void leaveEvent( QEvent * );
   KService* m_service;
   QString groupPath;
   QDate lastUse;
   uint usage;
private:
   void execute();
   QString exec;
   bool isCurrent;
   QLabel* m_titleLabel;
   QLabel* m_commentLabel;
   QLabel* m_pixmapLabel;
   QPixmap m_pix;
   QPixmap m_hoverPix;
signals:
   void closeMenu();
   void pressed();
   void appDown();
   void appUp();
   void appLeft();
   void hovered(const QString &);
   void sayText(const QString&);
   void unhovered();
   void popup(StartMenuEntry*);
   void executed();
};

class KPopupMenu;
class ConfigDialog;
class HelpDialog;

class AppList : public QScrollView
{
   friend class StartMenu;
Q_OBJECT

public:
   AppList(int size = 32, QWidget * parent = 0);
   StartMenuEntry* addApp(KService * service, QStringList & captions, QString relPath);
   void finish();
   StartMenuEntry* handledEntry; // for rightclick menu action
   void writeEntry(QString path, bool hidden = false); // for add/edit
   void reloadIcons(int size);
public slots:
   void addEntry();
   void addDialog();
   void removeEntry();
   void editEntry();
   void editDialog();
   void clear();
   void reset();
   void search(const QString & string);
   void appDown();
   void appUp();
   void appLeft();
   void sort();
   void showCategory(const QString & string);
   void unblockPopup(){popupBlocked_ = false;}
protected:
   QStringList categories;
   int favItemAmount;
   void save(KConfig* config);
   void mouseReleaseEvent ( QMouseEvent * e );
   void windowActivationChange ( bool oldActive );
private:
   void init();
   void insertGroup(KServiceGroup *g, QStringList & captions, QStringList & paths);
   KServiceGroup::Ptr m_root;
   KIconLoader *m_iconLoader;
   QLabel *infoLabel;
   QVBoxLayout * infoLayout;
   QVBoxLayout * m_VLayout;
   QFrame * m_widget;
   bool newbie;
   QStringList neewbieApps;
   KPopupMenu *m_popup;
   bool popupBlocked_;
   int _size;
   typedef QSortedList<StartMenuEntry> StartMenuEntryList;
   typedef QMap<QString,StartMenuEntryList> KeyWordList;
   StartMenuEntryList entryList;
   KeyWordList m_keywordList;
   KeyWordList m_groupList;
   ConfigDialog* configDialog_;
   HelpDialog* helpDialog_;
private slots:
   void popup(StartMenuEntry*);
signals:
   void looseKey();
   void message(const QString&);
   void sayText(const QString&);
   void clearStatus();
};

class QStringList;

class SearchLine : public KLineEdit
{
   Q_OBJECT
public:
   SearchLine( QWidget * parent );
   bool blocked;
protected slots:
   void makeCompletion (const QString &);
   void block(){blocked = true;}
signals:
   void typedTextChanged(const QString & string);
};

class KURIFilterData;
class StarterConfig;
class QSignalMapper;
class KSqueezedTextLabel;

class StartMenu : public QWidget
{
   friend class starter; // to allow setting the shortcutlis directly
   Q_OBJECT
public:
   enum PanelPosition { North = 0, South, West, East, Nowhere };
   StartMenu ( int size = 32, QWidget * parent = 0, WFlags f = 0 );
   ~StartMenu();
   void show();
   void hide();
   void reloadIcons(int size);
   QStringList & categories(){return appList->categories;};
   typedef QMap<MyKey,QString> ShortcutList;
   void updateShortcuts(ShortcutList &);
   void setFavItemAmount(int i) {if (appList) appList->favItemAmount = i;}
   void setPanelPosition(PanelPosition p);
public slots:
   void sayText(const QString &text);
   void toggleKTTS(bool);
   void setCategory(const QString & category);
   void save();
protected:
   bool eventFilter ( QObject * o, QEvent * e );
   ShortcutList shortcutList;
private slots:
   void message(const QString &text);
   void centerMessage(const QString &text);
   void clearStatus();
   void execute(const QString & command);
   void search(const QString & string);
   void endHistory();
//    void slotLock();
private:
   bool inMove;
   uint m_spokenText;
   int _size;
   QWidget *header;
   QPoint movePoint;
   KURIFilterData *_filterData;
   AppList *appList;
   QComboBox *categoryCombo;
   SearchLine *searchLine;
   KSqueezedTextLabel *statusBar;
   Panel *m_panel;
   QStringList history;
   QStringList::Iterator currentHistoryItem;
   PanelPosition m_panelPos;
   StartMenuButton *userButton;
   QGridLayout *panelLayout;
   
signals:
   void aboutToHide();
};

#endif
