
#include <stdlib.h>
#include <qcursor.h>
#include <qsplitter.h>
#include <klocale.h>
#include <qdir.h>
#include <kconfig.h>
#include <qtimer.h>
#include "dndlistbox.h"
#include "listboxlink.h"
#include "linkview.h"
#include "linkview.moc"

// internal class to eat invalid leave envents (i.e. leave that does not leave the rect but just enters the splitter, as styles (like baghira ;) may post install eventfilters that'd cause useless repaints and therefore flicker if the scroller appereance changes ;)
class EventKiller : public QObject 
{
protected:
   virtual bool eventFilter( QObject *o, QEvent *e)
   {
      if (e->type() == QEvent::Leave)
         return ((QScrollView*)o)->rect().contains(((QScrollView*)o)->mapFromGlobal (QCursor::pos()));
      return false;
   }
};

LinkView::LinkView(QWidget * parent, const char * name, WFlags f):
                   QScrollView(parent, name, f)
{
   setFrameShape( QFrame::StyledPanel );
   setFrameShadow( QFrame::Sunken );
   setBackgroundMode(Qt::PaletteBase);
   _blocked = FALSE;
   splitter = new QSplitter( Qt::Vertical, viewport() );
   addChild(splitter);
   splitter->setMargin(5);
   splitter->setBackgroundMode(Qt::PaletteBase);
   splitter->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
   splitter->setFrameShape( QFrame::NoFrame );
   splitter->setChildrenCollapsible(TRUE);
   splitter->setHandleWidth( 3 );
   splitter->setOpaqueResize();
   hardware = new MediaListBox(splitter, "hardware");
   splitter->setResizeMode( hardware, QSplitter::KeepSize );
   hardware->setFrameShape( QFrame::NoFrame );
   hardware->setHScrollBarMode(QScrollView::AlwaysOff);
   hardware->setVScrollBarMode(QScrollView::AlwaysOff);
   locations = new DnDListBox(splitter, "locations");
//    splitter->setResizeMode( locations, QSplitter::KeepSize );
   locations->setFrameShape( QFrame::NoFrame );
   locations->setHScrollBarMode(QScrollView::AlwaysOff);
   locations->setVScrollBarMode(QScrollView::AlwaysOff);
   // custom area, locations
   loadLinks();
   locations->setCurrentItem(0);
   locations->setSelected( locations->selectedItem(), false );
   hardware->installEventFilter(this);
   connect (hardware, SIGNAL(highlighted( int )), this, SLOT(unselectLocations()));
   connect (locations, SIGNAL(highlighted( int )), this, SLOT(unselectHardware()));
   connect (hardware, SIGNAL(scrolled(int,int)), this, SLOT(scrollBy(int,int)));
   connect (locations, SIGNAL(scrolled(int,int)), this, SLOT(scrollBy(int,int)));
   connect (hardware, SIGNAL(itemNumberChanged(bool)), this, SLOT(adjustSplitter2Hardware(bool)));
   connect (locations, SIGNAL(itemNumberChanged(bool)), this, SLOT(adjustSplitter2Locations()));
   QTimer::singleShot(50, this, SLOT(adjustSplitter2Locations()));
   QTimer::singleShot(60, this, SLOT(postInstallEventFilter()));
}

static EventKiller *eventKiller = 0L;

LinkView::~LinkView()
{
   
   saveLinks();
   delete eventKiller; eventKiller = 0L;
}

void LinkView::postInstallEventFilter()
{
   eventKiller = new EventKiller;
   installEventFilter(eventKiller);
}

bool LinkView::eventFilter(QObject *o, QEvent *e)
{
   if (o != hardware)
      return QScrollView::eventFilter(o, e);
   if (_blocked || e->type() != QEvent::Resize)
      return FALSE; // not a resize - non of our business
   QResizeEvent *rev = (QResizeEvent*)e;
   if (rev->size().height() == rev->oldSize().height())
      return FALSE; // height didn't change
   int tmpH = rev->size().height() + locations->numRows()*locations->itemHeight()+20;
   if (tmpH < viewport()->height())
      tmpH = viewport()->height();
   if (tmpH != splitter->height())
   {
      _blocked = TRUE;
      splitter->resize ( splitter->width(), tmpH );
      _blocked = FALSE;
   }

   return FALSE;
}

void LinkView::adjustSplitter2Locations()
{
   int tmpH = hardware->height() + locations->numRows()*locations->itemHeight()+20;
   if (tmpH < viewport()->height())
      tmpH = viewport()->height();
   if (tmpH != splitter->height())
      splitter->resize ( viewport()->width(), tmpH );
}

void LinkView::adjustSplitter2Hardware(bool added)
{
   if (added)
   {
      if (hardware->height() < hardware->numRows()*hardware->itemHeight())
         hardware->resize ( hardware->width(), hardware->numRows()*hardware->itemHeight() );
   }
   else
      if (hardware->height() > hardware->numRows()*hardware->itemHeight())
         hardware->resize ( hardware->width(), hardware->numRows()*hardware->itemHeight() );
}

void LinkView::viewportResizeEvent( QResizeEvent *rev )
{
   int tmpH = hardware->height() + locations->numRows()*locations->itemHeight()+20;
   if (tmpH < rev->size().height())
      tmpH = rev->size().height();
   splitter->resize(rev->size().width(), tmpH);
}

void LinkView::unselectLocations()
{
   if (locations) locations->setSelected( locations->selectedItem(), FALSE );
}

void LinkView::unselectHardware()
{
   if (hardware) hardware->setSelected( hardware->selectedItem(), FALSE );
}

void LinkView::loadLinks()
{
   if (!locations)
      return;
   KConfig config(QDir::homeDirPath() + "/.qt/baghirarc");
   config.setGroup("Sidebar");
   splitter->setSizes(config.readIntListEntry ("Sizes"));
   loadedLinks = (uint)config.readNumEntry("NumLinks", 0);
   locations->blockSignals ( true );
   if (loadedLinks == 0) // no settings stored - load defaults
   {
      locations->insertItem("desktop", i18n("Desktop"), QDir::homeDirPath()+"/Desktop");
      locations->insertItem("folder_home", getenv("USER"), QDir::homeDirPath());
      locations->insertItem("kmenu", i18n("Programs"), "programs:/");
   }
   QString num;
   for (uint i = 0; i < loadedLinks; i++)
   {
      QString title;
      QString icon;
      QString url;
      num.setNum(i);
      title = config.readEntry("Link_"+num+"_Title", "???");
      icon = config.readEntry("Link_"+num+"_Icon", "empty");
      url = config.readEntry("Link_"+num+"_URL", QDir::homeDirPath());
      locations->insertItem(icon, title, url);
   }
   locations->blockSignals ( false );
}

void LinkView::saveLinks()
{
   if (!locations)
      return;
   KConfig *config = new KConfig(QDir::homeDirPath() + "/.qt/baghirarc");
   config->setGroup("Sidebar");
   config->writeEntry("Sizes", splitter->sizes());
   config->writeEntry("NumLinks", (int)locations->count());
   QString num;
   for (uint i = 0; i < locations->count(); i++)
   {
      num.setNum(i);
      ListBoxLink *current = (ListBoxLink*)locations->item(i);
      config->writeEntry("Link_"+num+"_Title", current->text());
      config->writeEntry("Link_"+num+"_Icon", current->icon());
      config->writeEntry("Link_"+num+"_URL", current->URL());
   }
   // reduced links, remove them from settings
   for (uint i = locations->count(); i < loadedLinks; i++)
   {
      num.setNum(i);
      config->deleteEntry("Link_"+num+"_Title");
      config->deleteEntry("Link_"+num+"_Icon");
      config->deleteEntry("Link_"+num+"_URL");
   }
   delete config;
}
