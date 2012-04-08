
#ifndef DNDLISTBOX_H
#define DNDLISTBOX_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <klistbox.h>
#include <qptrlist.h>
#include <qstringlist.h>
#include "baghirasidebariface.h"

class DCOPClient;
class QDragEnterEvent;
class QDragMoveEvent;
class QDragLeaveEvent;
class QDropEvent;
class QKeyEvent;
class QPoint;
class KPopupMenu;
class LinkConfig;
class ListBoxLink;
class ListBoxDevice;
class QResizeEvent;

class ResizingLinkBox : public KListBox
{
    Q_OBJECT
    
public:
   ResizingLinkBox( QWidget * parent = 0, const char * name = 0, WFlags f = 0 );
   ~ResizingLinkBox(){};
   void insertItem( const QListBoxItem *, int index=-1 );
   void insertItem( const QListBoxItem *lbi, const QListBoxItem *after );
   void insertItem( const QString & icon, const QString & title, const QString & url );
   void insertItem( const QString &text, int index=-1 )
   {
      insertItem( new QListBoxText(text), index );
   }
   void insertItem( const QPixmap &pixmap, int index=-1 )
   {
      insertItem( new QListBoxPixmap(pixmap), index );
   }
   void insertItem( const QPixmap &pixmap, const QString &text, int index=-1 ){
      insertItem( new QListBoxPixmap(pixmap, text), index );
   }
   
   void removeItem( int index );
protected:
   void mousePressEvent ( QMouseEvent * );
   void mouseReleaseEvent ( QMouseEvent * );
   void contentsWheelEvent ( QWheelEvent * );
   KPopupMenu *popupMenu;
   uint size_;
private slots:
   void setIconSize(int);
signals:
   void itemNumberChanged(bool);
   void scrolled(int,int);
};

class MediaListBox : public ResizingLinkBox, virtual public BaghiraSidebarIface
{
   Q_OBJECT
public:
   MediaListBox( QWidget * parent = 0, const char * name = 0, WFlags f = 0 );
   ~MediaListBox();
   ListBoxDevice *createListBoxDevice(QStringList & deviceProperties, uint n = 0);
   void mediumAdded(const QString &name);
   void mediumRemoved(const QString &name);
   void mediumChanged(const QString &name);
   int index (const QString & name );
   void removeItem( int index );
protected:
   void mousePressEvent ( QMouseEvent * );
   void resizeEvent ( QResizeEvent * );
private:
   KPopupMenu *devicePopup;
   DCOPClient *client;
   ListBoxDevice *currentFloppy;
   typedef QPtrList<ListBoxDevice> DeviceList;
   DeviceList deviceList;
   QStringList hiddenDevices;
private slots:
   void kfloppy();
   void toggleDevice(int id);
};

class DnDListBox : public ResizingLinkBox
{
    Q_OBJECT
    
public:
     DnDListBox( QWidget * parent = 0, const char * name = 0, WFlags f = 0 );

    /** destructor */
    ~DnDListBox();
   void poof(ListBoxLink *link);

protected:
   void mousePressEvent ( QMouseEvent * );
   void mouseReleaseEvent ( QMouseEvent * );
   void mouseMoveEvent ( QMouseEvent * e );
   void dragEnterEvent ( QDragEnterEvent * );
//    void dragMoveEvent ( QDragMoveEvent * );
//    void dragLeaveEvent ( QDragLeaveEvent * );
   void dropEvent ( QDropEvent * );
   void startDrag();
private:
   ListBoxLink *currentItem; //TODO: unshadow int KListBox::currentItem()
   LinkConfig *dialog;
   bool dragging_;
   void pasteURL(int mode, QListBoxItem *after = 0);
   bool _draggedMe;
   int _poofIndex;
   QPixmap *_poofPix;
   QPixmap *_poofAnimPix;
   QWidget *_poof;
private slots:
   void configureLink();
   void updateLink();
   void runPoof();
};

#endif
