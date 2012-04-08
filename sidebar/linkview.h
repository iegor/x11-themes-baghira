
#ifndef LINKVIEW_H
#define LINKVIEW_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qscrollview.h>

class ListBoxLink;
class DnDListBox;
class MediaListBox;

class LinkView : public QScrollView
{
    Q_OBJECT
    
public:
     LinkView(QWidget * parent = 0, const char * name = 0, WFlags f = 0);

    /** destructor */
    ~LinkView();
   MediaListBox *Hardware(){return hardware;}
   DnDListBox *Locations(){return locations;}
   void loadLinks();
   void saveLinks();

protected:
   void viewportResizeEvent( QResizeEvent * );
   bool eventFilter(QObject *, QEvent *);
private:
   MediaListBox *hardware;
   DnDListBox *locations;
   QSplitter *splitter;
   bool _blocked;
   uint loadedLinks;
private slots:
   void postInstallEventFilter();
   void unselectLocations();
   void unselectHardware();
   void adjustSplitter2Locations();
   void adjustSplitter2Hardware(bool added);
};

#endif
