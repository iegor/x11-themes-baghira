
#include <kinstance.h>
#include <qsplitter.h>
#include <kiconloader.h>
#include <stdlib.h>
#include "dndlistbox.h"
#include "listboxlink.h"
#include "linkview.h"
#include "baghirasidebar.h"
#include "baghirasidebar.moc"

baghiraSidebar::baghiraSidebar(KInstance *inst,QObject *parent,QWidget *widgetParent, QString &desktopName, const char* name):
                   KonqSidebarPlugin(inst,parent,widgetParent,desktopName,name)
{
   scrollView = new LinkView(widgetParent);
   scrollView->setHScrollBarMode(QScrollView::AlwaysOff);
   connect (scrollView->Hardware(), SIGNAL(clicked(QListBoxItem *)), this, SLOT(callURL(QListBoxItem *)));
   connect (scrollView->Locations(), SIGNAL(clicked(QListBoxItem *)), this, SLOT(callURL(QListBoxItem *)));
}

baghiraSidebar::~baghiraSidebar()
{
}

void baghiraSidebar::callURL(QListBoxItem *item)
{
   emit openURLRequest(KURL(((ListBoxLink*)(item))->URL()), KParts::URLArgs(true,0,0));
}

void baghiraSidebar::handleURL(const KURL &url)
{
   int item = scrollView->Hardware()->currentItem();
   if ( scrollView->Hardware()->isSelected(item) && 
      KURL(((ListBoxLink*)(scrollView->Hardware()->item(item)))->URL()).url() != url.url() )
   {
      scrollView->Hardware()->setSelected(item, FALSE);
      return;
   }
   item = scrollView->Locations()->KListBox::currentItem();
   if (scrollView->Locations()->isSelected(item) && 
      KURL(((ListBoxLink*)(scrollView->Locations()->item(item)))->URL()).url() != url.url() )
   {
      scrollView->Locations()->setSelected(item, FALSE);
      return;
   }
}

extern "C" {
   KDE_EXPORT bool add_konqsidebar_baghirasidebar(QString* fn, QString* param, QMap<QString,QString> *map) {
      Q_UNUSED(param);
      
      map->insert("Type", "Link");
      map->insert("Icon", "bab_brushed");
      map->insert("Name", "Baghira Sidebar");
      map->insert("Open", "true");
      map->insert("X-KDE-KonqSidebarModule","konqsidebar_baghirasidebar");
      fn->setLatin1("baghirasidebar%1.desktop");
      return true;
   }
}

extern "C"
{
    void* create_konqsidebar_baghirasidebar(KInstance *instance,QObject *par,QWidget *widp,QString &desktopname,const char *name)
    {
        return new baghiraSidebar(instance,par,widp,desktopname,name);
    }
};
