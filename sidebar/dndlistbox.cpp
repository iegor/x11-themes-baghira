
#include <dcopclient.h>
#include <qdragobject.h>
#include <kurl.h>
#include <qstrlist.h>
#include <qtimer.h>
#include <kapplication.h>
#include <klocale.h>
#include <kfileitem.h>
#include <kpopupmenu.h>
#include <kicondialog.h>
#include <kiconloader.h>
#include <klineedit.h>
#include <kprocess.h>
#include <kurlrequester.h>
#include <kstandarddirs.h>
#include <kwin.h>
#include <qclipboard.h>
#include "listboxlink.h"
#include "linkconfig.h"
#include "dndlistbox.h"
#include "baghiralinkdrag.h"
#include "dndlistbox.moc"

#define ID 0
#define NAME 1
#define LABEL 2
#define USER_LABEL 3
#define MOUNTABLE 4
#define DEVICE_NODE 5
#define MOUNT_POINT 6
#define FS_TYPE 7
#define MOUNTED 8
#define BASE_URL 9
#define MIME_TYPE 10
#define ICON_NAME 11
#define MEDIA_PROPS 12
#define MEDIALIST_PROPS 13

ResizingLinkBox::ResizingLinkBox( QWidget * parent, const char * name, WFlags f ) : KListBox( parent, name, f)
{
   KConfig config(QDir::homeDirPath() + "/.qt/baghirarc");
   config.setGroup("Sidebar");
   size_ = config.readNumEntry (QString(name) + "_IconSize", 48);
   popupMenu = new KPopupMenu;
   popupMenu->insertTitle (i18n("Icon Size"),122,122);
   popupMenu->insertItem("16x16", this, SLOT(setIconSize(int)),0,16,123);
   popupMenu->insertItem("22x22", this, SLOT(setIconSize(int)),0,22,124);
   popupMenu->insertItem("32x32", this, SLOT(setIconSize(int)),0,32,125);
   popupMenu->insertItem("48x48", this, SLOT(setIconSize(int)),0,48,126);
   popupMenu->insertItem("64x64", this, SLOT(setIconSize(int)),0,64,127);
   popupMenu->insertItem("128x128", this, SLOT(setIconSize(int)),0,128,128);
}

void ResizingLinkBox::insertItem( const QListBoxItem *lbi, int index )
{
   KListBox::insertItem( lbi, index );
   if (height() <= numRows()*itemHeight())
      emit itemNumberChanged(TRUE);
}

void ResizingLinkBox::insertItem( const QListBoxItem *lbi, const QListBoxItem *after )
{
   KListBox::insertItem( lbi, after );
   if (height() <= numRows()*itemHeight())
      emit itemNumberChanged(TRUE);
}


void ResizingLinkBox::insertItem( const QString & icon, const QString & title, const QString & url )
{
   insertItem( new ListBoxLink(icon, size_, title, url ) );
}

void ResizingLinkBox::removeItem( int index )
{
   blockSignals ( true );
   KListBox::removeItem(index);
   blockSignals ( false );
   emit itemNumberChanged(FALSE);
}

void ResizingLinkBox::setIconSize(int size)
{
   size_ = size;
   KConfig *config = new KConfig(QDir::homeDirPath() + "/.qt/baghirarc");
   config->setGroup("Sidebar");
   config->writeEntry (QString(name()) + "_IconSize", size);
   config->sync();
   ListBoxLink *runner;
   ListBoxDevice *current;
   blockSignals ( true );
   for (uint i = 0; i < count(); i++)
   {
      runner = (ListBoxLink*)item(i);
      if (dynamic_cast<ListBoxDevice*>(runner))
      {
         current = (ListBoxDevice*)runner;
         insertItem( new ListBoxDevice(current->icon(), size, current->text(), current->URL(), current->name(), current->mountPoint(), current->mounted(), current->ejectable(), current->removable(), current->id()), i );
      }
      else
      {
         insertItem( new ListBoxLink(runner->icon(), size, runner->text(), runner->URL()), i );
      }
      KListBox::removeItem(i+1);
   }
   blockSignals ( false );
}

void ResizingLinkBox::mousePressEvent ( QMouseEvent *mpe )
{
   KListBox::mousePressEvent( mpe );
}

void ResizingLinkBox::mouseReleaseEvent ( QMouseEvent *mpe )
{
   if (mpe->button() == Qt::LeftButton)
   {
      ListBoxLink *link = (ListBoxLink*)itemAt(mpe->pos());
      if (isSelected(link)) emit clicked(link);
      KListBox::mousePressEvent( mpe );
      return;
   }
   KListBox::mouseReleaseEvent( mpe );
}

void ResizingLinkBox::contentsWheelEvent ( QWheelEvent * we )
{
   if (we->state() == Qt::ControlButton)
      KListBox::contentsWheelEvent ( we );
   else
      emit scrolled(0, -we->delta());
}

MediaListBox::MediaListBox( QWidget * parent, const char * name, WFlags f ) : ResizingLinkBox(parent, name, f), DCOPObject("BaghiraSidebarIface")
{
   KConfig config(QDir::homeDirPath() + "/.qt/baghirarc");
   config.setGroup("Sidebar");
   hiddenDevices = config.readListEntry("HiddenDevices");
   currentFloppy = 0L;
   devicePopup = new KPopupMenu(this);
   devicePopup->setCheckable ( true );
   popupMenu->insertItem(i18n("Device List"), devicePopup, 1, 0);
   popupMenu->insertSeparator( 0 );

#if KDE_IS_VERSION(3,4,90)
   insertItem(new ListBoxDevice("system", size_, i18n("My Computer"), "system:/", "", "", TRUE, FALSE, FALSE));
   insertItem(new ListBoxDevice("network", size_, i18n("Network"), "remote:/", "", "", TRUE, FALSE, FALSE));
#else
   insertItem(new ListBoxDevice("system", size_, i18n("My Computer"), "media:/", "", "", TRUE, FALSE, FALSE));
   insertItem(new ListBoxDevice("network", size_, i18n("Network"), "lan:/localhost", "", "", TRUE, FALSE, FALSE));
#endif
   insertItem(new ListBoxDevice("hdd_mount", size_, i18n("Startvolume"), QDir::rootDirPath(), "", "", TRUE, FALSE, FALSE));
   client = KApplication::dcopClient();
   client->connectDCOPSignal("kded", "mediamanager", "mediumAdded(QString)", "BaghiraSidebarIface", "mediumAdded(QString)", FALSE);
   client->connectDCOPSignal("kded", "mediamanager", "mediumRemoved(QString)", "BaghiraSidebarIface", "mediumRemoved(const QString)", FALSE);
   client->connectDCOPSignal("kded", "mediamanager", "mediumChanged(QString)", "BaghiraSidebarIface", "mediumChanged(QString)", FALSE);
   /* Get the media info - huhhh ;) */
   QByteArray data, replyData;
   QCString replyType;
   QDataStream arg(data, IO_WriteOnly);
   arg << ""; // ask for the full list
   if (!client->call("kded", "mediamanager", "fullList()", data, replyType, replyData))
      qDebug("there was some error using DCOP.");
   else
   {
      QDataStream reply(replyData, IO_ReadOnly);
      if (replyType == "QStringList")
      {
         QStringList result;
         reply >> result;
         blockSignals ( true );
         for (uint i = 0; i < result.size()/MEDIALIST_PROPS; i++)
         {
            if (result[MEDIALIST_PROPS*i+MIME_TYPE] != "media/hdd_mounted" &&
                  result[MEDIALIST_PROPS*i+MIME_TYPE] != "media/hdd_unmounted" &&
                  result[MEDIALIST_PROPS*i+MIME_TYPE] != "media/nfs_mounted" &&
                  result[MEDIALIST_PROPS*i+MIME_TYPE] != "media/nfs_unmounted" &&
                  result[MEDIALIST_PROPS*i+MIME_TYPE] != "media/smb_mounted" &&
                  result[MEDIALIST_PROPS*i+MIME_TYPE] != "media/smb_unmounted")
            {
               ListBoxDevice *dev = createListBoxDevice(result, i);
               if (hiddenDevices.contains(dev->name()))
               {
                  deviceList.append(dev);
                  devicePopup->setItemChecked(dev->id(),false);
               }
               else
               {
                  insertItem(dev);
                  devicePopup->setItemChecked(dev->id(),true);
               }
            }
         }
         blockSignals ( false );
      }
      else
         qWarning("fullList() returned an unexpected type of reply!");
   }
//    setCurrentItem( 0 );
}

MediaListBox::~MediaListBox()
{
   hiddenDevices.clear();
   ListBoxDevice *runner;
   for ( runner = deviceList.first(); runner; runner = deviceList.next() )
      hiddenDevices.append(runner->name());
   KConfig config(QDir::homeDirPath() + "/.qt/baghirarc");
   config.setGroup("Sidebar");
   config.writeEntry("HiddenDevices", hiddenDevices);
}

void MediaListBox::removeItem( int index )
{
   devicePopup->removeItem(((ListBoxDevice*)item( index ))->id());
   ResizingLinkBox::removeItem(index);
}

void MediaListBox::kfloppy()
{
   if (currentFloppy)
   {
      KProcess proc;
      proc << "kfloppy" << currentFloppy->mountPoint();
      proc.start(KProcess::DontCare);
      proc.detach();
      currentFloppy = 0L;
   }
   return;
}

void MediaListBox::toggleDevice(int id)
{
   ListBoxDevice *runner;
   if (devicePopup->isItemChecked(id)) // remove the device!
   {
      devicePopup->setItemChecked(id, false);
      for (uint i = 0; i < count(); i++)
      {
         runner = (ListBoxDevice*)item(i);
         if (runner->id() == id)
         {
            deviceList.append(runner);
            blockSignals ( true );
            takeItem(runner);
            blockSignals ( false );
            break;
         }
      }
   }
   else  // add the device!
   {
      devicePopup->setItemChecked(id, true);
      for ( runner = deviceList.first(); runner; runner = deviceList.next() )
         if (runner->id() == id)
         {
            insertItem(deviceList.take());
            break;
         }
   }
}

ListBoxDevice *MediaListBox::createListBoxDevice(QStringList & deviceProperties, uint n)
{
   QString icon;
   icon = deviceProperties[MEDIALIST_PROPS*n+ICON_NAME];
   if (icon.isNull())
   {
      icon = deviceProperties[MEDIALIST_PROPS*n+MIME_TYPE];
      icon = icon.section( '/', -1 );
      icon.truncate( icon.length()-2 );
      if (icon.contains("floppy")) icon.prepend("3");
   }
   QString label;
   label = deviceProperties[MEDIALIST_PROPS*n+USER_LABEL];
   if (label.isNull())
   {
      label = deviceProperties[MEDIALIST_PROPS*n+LABEL];
      label = i18n(label.section( " (", 0, 0 ).utf8());
   }
#if KDE_IS_VERSION(3,4,90)
   return new ListBoxDevice( icon, size_, label, "system:/media/"+deviceProperties[MEDIALIST_PROPS*n+NAME], deviceProperties[MEDIALIST_PROPS*n+NAME], deviceProperties[MEDIALIST_PROPS*n+DEVICE_NODE], deviceProperties[MEDIALIST_PROPS*n+MOUNTED] == "true", icon.contains("dvd") || icon.contains("cdrom") || icon.contains("cdwriter"),icon.contains("floppy"), devicePopup->insertItem(deviceProperties[MEDIALIST_PROPS*n+NAME], this, SLOT(toggleDevice(int))));
#else
   return new ListBoxDevice( icon, size_, label, "media:/"+deviceProperties[MEDIALIST_PROPS*n+NAME], deviceProperties[MEDIALIST_PROPS*n+NAME], deviceProperties[MEDIALIST_PROPS*n+DEVICE_NODE], deviceProperties[MEDIALIST_PROPS*n+MOUNTED] == "true", icon.contains("dvd") || icon.contains("cdrom") || icon.contains("cdwriter"),icon.contains("floppy"),devicePopup->insertItem(deviceProperties[MEDIALIST_PROPS*n+NAME], this, SLOT(toggleDevice(int))));
#endif
}

int MediaListBox::index (const QString & name )
{
   ListBoxDevice *device;
   for (uint i = 0; i < count(); i++)
      {
         device = (ListBoxDevice*)item(i);
         if (device && device->name() == name) return i;
      }
   return -1;
}

void MediaListBox::mediumAdded(const QString &name)
{
   QByteArray data, replyData;
   QCString replyType;
   QDataStream arg(data, IO_WriteOnly);
   arg << name; // ask for this item only
   if (!client->call("kded", "mediamanager", "properties(QString)", data, replyType, replyData))
      qDebug("there was some error using DCOP.");
   else
   {
      QDataStream reply(replyData, IO_ReadOnly);
      if (replyType == "QStringList")
      {
         QStringList result;
         reply >> result;
         ListBoxDevice *dev = createListBoxDevice(result);
         if (hiddenDevices.contains(dev->name()))
         {
            deviceList.append(dev);
            devicePopup->setItemChecked(dev->id(),false);
         }
         else
         {
            insertItem(dev);
            devicePopup->setItemChecked(dev->id(),true);
         }
      }
      else
         qWarning("properties() returned an unexpected type of reply!");
   }
}

void MediaListBox::mediumRemoved(const QString &name)
{
   QByteArray data, replyData;
   QCString replyType;
   QDataStream arg(data, IO_WriteOnly);
   arg << name; // ask for this item only
   if (!client->call("kded", "mediamanager", "properties(QString)", data, replyType, replyData))
      qDebug("there was some error using DCOP.");
   else
   {
      QDataStream reply(replyData, IO_ReadOnly);
      if (replyType == "QStringList")
      {
         QStringList result;
         reply >> result;
         int i = index(name);
         if (i<0)
            return;
         if (i == currentItem()) setCurrentItem(0);
         removeItem(i);
      }
      else
         qWarning("properties() returned an unexpected type of reply!");
   }
}

void MediaListBox::mediumChanged(const QString &name)
{
   QByteArray data, replyData;
   QCString replyType;
   QDataStream arg(data, IO_WriteOnly);
   arg << name; // ask for this item only
   if (!client->call("kded", "mediamanager", "properties(QString)", data, replyType, replyData))
      qDebug("there was some error using DCOP.");
   else
   {
      QDataStream reply(replyData, IO_ReadOnly);
      if (replyType == "QStringList")
      {
         QStringList result;
         reply >> result;
         int i = index(name);
         if (i<0)
            return;
         
         ListBoxDevice *device = createListBoxDevice(result);
         if (hiddenDevices.contains(device->name()))
         {
            deviceList.append(device);
            devicePopup->setItemChecked(device->id(),false);
            return;
         }
         devicePopup->setItemChecked(device->id(),true);
         blockSignals(true);
         if (i == currentItem()) // changing current item - take some care of updating stuff
         {
            if (((ListBoxDevice*)item(i))->mounted() && !device->mounted()) // unmounted the device - we certainly do not wanna select it anymore
            {
               setCurrentItem(0);
               removeItem(i);
               insertItem(device, i);
            }
            else // we're selected and wanna keep selection
            {
               removeItem(i);
               insertItem(device, i);
               setSelected( i, true );
            }
         }
         else // ordinary change
         {
            removeItem(i);
            insertItem(device, i);
         }
         blockSignals(false);
      }
      else
         qWarning("properties() returned an unexpected type of reply!");
   }
}

#define _FLOPPYID_ 0
#define _FLOPPYINDEX_ 0

void MediaListBox::mousePressEvent ( QMouseEvent *mpe )
{
   if (mpe->button() == Qt::RightButton)
   {
      popupMenu->removeItem(_FLOPPYID_);
      ListBoxDevice *device = (ListBoxDevice*)itemAt(mpe->pos());
      if (device && device->name().contains("fd"))
      {
         currentFloppy = device;
         popupMenu->insertItem(i18n("Format disk..."), this, SLOT(kfloppy()),0,_FLOPPYID_,_FLOPPYINDEX_);
      }
      popupMenu->popup(mpe->globalPos());
      return;
   }
   if (mpe->button() == Qt::LeftButton && mpe->x() > width()-22)
   {
      ListBoxDevice *device = (ListBoxDevice*)itemAt(mpe->pos());
      int dy = itemRect(device).y();
      if ((device->removable() || device->ejectable()) && device->mounted() && mpe->y() > dy+11 && mpe->y() < dy+33)
      {
         KProcess proc;
         device->ejectable()?
            proc << "kdeeject" /*<< "-q"*/ << device->mountPoint():
            proc << "umount" << device->mountPoint(); // umount?
         proc.start(KProcess::DontCare);
         proc.detach();
         return;
      }
   }
   ResizingLinkBox::mousePressEvent( mpe );
}

void MediaListBox::resizeEvent ( QResizeEvent * re)
{
   if (width() != re->oldSize().width())
   {
      for (uint i = 0; i < count(); i++)
         if (((ListBoxDevice*)item(i))->ejectable() && ((ListBoxDevice*)item(i))->mounted() && !isSelected(i)) updateItem(i);
   }
   ResizingLinkBox::resizeEvent(re);
}

DnDListBox::DnDListBox( QWidget * parent, const char * name, WFlags f ):
ResizingLinkBox( parent, name, f), _poof(0), _poofIndex(0), _poofAnimPix(0), _poofPix(0)
{
   setAcceptDrops(true);
   dialog = new LinkConfig();
   connect(dialog->buttonOk, SIGNAL(clicked()), this, SLOT(updateLink()));
   setCursor(Qt::PointingHandCursor);
}

DnDListBox::~DnDListBox()
{
}

void DnDListBox::poof(ListBoxLink *link)
{
   _poofIndex = 0;
   _poofPix = new QPixmap(locateLocal("data", "baghira/poof.png"), "png");
   _poofAnimPix = new QPixmap(_poofPix->width(), _poofPix->width());
   if (!_poof)
      _poof = new QWidget(0,0, Qt::WType_TopLevel | Qt::WStyle_NoBorder | Qt::WStyle_StaysOnTop | Qt::WX11BypassWM);
   KWin::setShadowSize(_poof->winId(), 0);
   _poof->setFixedSize(_poofPix->width(), _poofPix->width());
   int x = QCursor::pos().x() - _poof->width()/2;
   int y = QCursor::pos().y() - _poof->height()/2;
   QPixmap bgPix = QPixmap::grabWindow( qt_xrootwin(), x, y, _poofPix->width(), _poofPix->width());
   _poof->move(x,y);
   _poof->show();
   _poof->setBackgroundOrigin(QWidget::WidgetOrigin);
   _poof->setPaletteBackgroundPixmap( bgPix );
   runPoof();
   removeItem ( index(link) );
}

void DnDListBox::runPoof()
{
   if (_poofIndex > 4)
   {
      _poof->hide();
      delete _poofPix;
      _poofPix = 0L;
//       delete _poof;
//       _poof = 0L;
      delete _poofAnimPix;
      _poofAnimPix = 0L;
      _poofIndex = 0;
      return;
   }
   _poof->erase();
   bitBlt(_poof, 0 ,0, _poofPix, 0, _poofIndex * _poofPix->width(), _poofPix->width(), _poofPix->width(), Qt::AndROP);
   ++_poofIndex;
   QTimer::singleShot ( 70, this, SLOT(runPoof()) ); // around 15 fps
}

void DnDListBox::dragEnterEvent ( QDragEnterEvent *dee )
{
//    dragging_ = true;
   if (QUriDrag::canDecode(dee) || BaghiraLinkDrag::canDecode(dee) || QTextDrag::canDecode(dee))
      dee->accept(true);
}

void DnDListBox::dropEvent ( QDropEvent *de )
{
   QStrList list;
   QString title;
   QString command;
   QString icon;
   int oldIndex;
   QCString subtype;
   if (BaghiraLinkDrag::decode(de, &title, &command, &icon, &oldIndex)) // internal move
   {
      BaghiraLinkDrag::setAccepted();
      QListBoxItem *after = itemAt(de->pos());
      int newIndex = index(after);
      if (!dragging_ || oldIndex < 0 || oldIndex > count()-2)
         insertItem (new ListBoxLink(icon, size_, title, command), after?newIndex:count());
      else if (oldIndex != newIndex)
      {
         insertItem (new ListBoxLink(*((ListBoxLink*)item(oldIndex))), after?newIndex:count());
         removeItem ( (newIndex < 0 || oldIndex < newIndex) ? oldIndex : oldIndex + 1 );
      }
   }
   else if ( QUriDrag::decode(de, list) )
   {
      char *uri;
      KURL url;
      QListBoxItem *after = itemAt(de->pos());
      for ( uri = list.first(); uri; uri = list.next() )
      {
         url = KURL(uri);
         if (url.protocol() == "http")
            insertItem(new ListBoxLink("html", size_, url.host()+(url.path()=="/"?QString(""):url.path()), uri), after?index(after):count());
         else
         {
            KFileItem item = KFileItem(KFileItem::Unknown, KFileItem::Unknown, url, true);
               insertItem(new ListBoxLink(item.iconName(), size_, url.fileName().isEmpty()?url.prettyURL():url.fileName(), uri), after?index(after):count());
         }
      }
   }
   else if (QTextDrag::decode(de, command, subtype))
   {
      KURL url(command);
      if (url.isValid())
      {
         QListBoxItem *after = itemAt(de->pos());
         if (url.protocol() == "http")
            insertItem(new ListBoxLink("html", size_, url.host()+(url.path()=="/"?QString(""):url.path()), command), after?index(after):count());
         else
         {
            KFileItem item = KFileItem(KFileItem::Unknown, KFileItem::Unknown, url, true);
            insertItem(new ListBoxLink(item.iconName(), size_, url.fileName().isEmpty()?url.prettyURL():url.fileName(), command), after?index(after):count());
         }
      }
      else if (command.contains('@'))
      {
         QListBoxItem *after = itemAt(de->pos());
         command.replace(" ","");
         insertItem(new ListBoxLink("kmail", size_, command, "mailto:"+command), after?index(after):count());
      }
      else if (command.contains("'at'")) //last chance for anti-spam addy
      {
         QListBoxItem *after = itemAt(de->pos());
         command.replace(" ","");
         command.replace("'at'","@");
         insertItem(new ListBoxLink("kmail", size_, command, "mailto:"+command), after?index(after):count());
      }
   }
}

void DnDListBox::mousePressEvent ( QMouseEvent *mpe )
{
   if (mpe->button() == Qt::RightButton)
   {
      currentItem = 0;
      popupMenu->removeItem(0);
      ListBoxDevice *device = (ListBoxDevice*)itemAt(mpe->pos());
      if (device)
      {
         currentItem = device;
         popupMenu->insertItem("Edit link...", this, SLOT(configureLink()),0,0,0);
      }
      popupMenu->popup(mpe->globalPos());
      return;
   }
   if (mpe->button() == Qt::MidButton)
   {
      pasteURL(QClipboard::Selection, itemAt(mpe->pos()));
      return;
   }
   ResizingLinkBox::mousePressEvent( mpe );
}

void DnDListBox::mouseReleaseEvent ( QMouseEvent *mre )
{
   if (!dragging_) ResizingLinkBox::mouseReleaseEvent( mre );
}

void DnDListBox::mouseMoveEvent ( QMouseEvent * mme )
{
   if (mme->state() & Qt::LeftButton)
   {
      if (!dragging_)
      {
         ListBoxLink *link = (ListBoxLink*)itemAt(mme->pos());
         if (link)
         {
            dragging_ = true;
            BaghiraLinkDrag *d = new BaghiraLinkDrag( link->text(), link->URL(), link->icon(), index(link), this );
            d->setPixmap(*link->pixmap(),QPoint(22,22));
            d->drag();
            if (mme->state() & Qt::ControlButton || BaghiraLinkDrag::accepted())
               return;
            poof(link);
            // do NOT delete d.
         }
      }
   }
   else // ensure to release from drag
      dragging_ = false;
}

void DnDListBox::pasteURL(int mode, QListBoxItem *after)
{
   QString string = qApp->clipboard()->text( (QClipboard::Mode)mode );
   KURL url(string);
   if (url.isValid())
   {
      if (url.protocol() == "http")
         insertItem(new ListBoxLink("html", size_, url.host()+(url.path()=="/"?QString(""):url.path()), string), after?index(after):count());
      else
      {
         KFileItem item = KFileItem(KFileItem::Unknown, KFileItem::Unknown, url, true);
         insertItem(new ListBoxLink(item.iconName(),size_, url.fileName().isEmpty()?url.prettyURL():url.fileName(), string), after?index(after):count());
      }
   }
   else if (string.contains('@'))
   {
      string.replace(" ","");
      insertItem(new ListBoxLink("kmail", size_, string, "mailto:"+string), after?index(after):count());
   }
   else if (string.contains("'at'")) //last chance for anti-spam addy
   {
      string.replace(" ","");
      string.replace("'at'","@");
      insertItem(new ListBoxLink("kmail", size_, string, "mailto:"+string), after?index(after):count());
   }
}

void DnDListBox::configureLink()
{
   if (currentItem == 0L)
      return;
   dialog->title->setText(currentItem->text());
   dialog->url->setURL(currentItem->URL());
   dialog->icon->setIcon(currentItem->icon());
   dialog->show();
}

void DnDListBox::updateLink()
{
   if (currentItem)
   {
      int index_ = index(currentItem);
      bool wasSelected = isSelected(index_);
      insertItem( new ListBoxLink(dialog->icon->icon(), size_, dialog->title->text(),dialog->url->url()), index_ +1);
      removeItem( index_ );
      setSelected(index_, wasSelected);
   }
}
