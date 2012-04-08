
#ifndef BAGHIRASIDEBAR_H
#define BAGHIRASIDEBAR_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <konqsidebarplugin.h>
#include <kconfig.h>

class LinkView;
class KListBox;
class QListBoxItem;

class baghiraSidebar : public KonqSidebarPlugin
{
    Q_OBJECT
    
public:
    /**
     * Construct a @ref KonqSidebarPlugin.
     *
     * @param inst The sidebar's kinstance class.
     * @param parent The sidebar internal button info class responsible for this plugin.
     * @param widgetParent The container which will contain the plugins widget.
     * @param desktopName The filename of the configuration file.
     * @param name A Qt object name for your plugin.
     **/
     baghiraSidebar(KInstance *inst,QObject *parent, QWidget *widgetParent, QString &desktopName, const char* name=0);

    /** destructor */
    ~baghiraSidebar();
    
   virtual QWidget *getWidget(){return scrollView;}
   virtual void *provides(const QString &) {return 0;}

protected:
   LinkView *scrollView;
   virtual void handleURL(const KURL &url);

private slots:
   void callURL(QListBoxItem *);

signals:
   void openURLRequest(const KURL &url, const KParts::URLArgs &args);

};

#endif
