/***************************************************************************
 *   Copyright (C) 2004 by Thomas L�bking                                  *
 *   thomas@home                                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "bab.h"
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qdir.h>
#include <qlayout.h>
#include <qsettings.h>
#include <qtabwidget.h>
#include <qtooltip.h>
#include <qtimer.h>
#include <qvbox.h> 
#include <qpainter.h>
#include <dcopclient.h>
#include <kapplication.h>
// #include <kconfig.h>
#include <kiconloader.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <kipc.h>
#include <klibloader.h>
#include <klocale.h>
#include <kprocess.h>
#if KDE_IS_VERSION(3,4,0)
#include <kwin.h>
#endif
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/extensions/shape.h>

#include "styleconfdialog.h"

static const char description[] =
    I18N_NOOP("Baghira Aqua/Brushed");

static const char version[] = "cvs";

static KCmdLineOptions options[] =
{
//    { "+[URL]", I18N_NOOP( "Document to open." ), 0 },
    KCmdLineLastOption
};

int main(int argc, char **argv)
{
   KAboutData about("bab", I18N_NOOP("BAB - Baghira Aqua/Brushed Switcher"), version, description,
                  KAboutData::License_GPL, "(C) 2004/2005 Thomas Lübking", 0, 0, "baghira-style@gmx.net");
   about.addAuthor( "Thomas Lübking", 0, "baghira-style@gmx.net" );
   KCmdLineArgs::init(argc, argv, &about);
   KCmdLineArgs::addCmdLineOptions( options );
   KApplication app;
   DCOPClient* client = app.dcopClient();
   client->registerAs( "bab", false);
   client->setDefaultObject( "babInterface" );
   bab* mainBAB = 0;
   mainBAB = new bab;
   app.setMainWidget(mainBAB);
   Corner leftCorner(Corner::left);
   Corner rightCorner(Corner::right);
#if KDE_IS_VERSION(3,4,0)
   KWin::setShadowSize(leftCorner.winId(), 0);
   KWin::setShadowSize(rightCorner.winId(), 0);
#endif
   if (!mainBAB->cornerCheck->isChecked())
   {
      leftCorner.hide();
      rightCorner.hide();
   }
   leftCorner.connect( mainBAB->cornerCheck, SIGNAL(toggled(bool)), SLOT(toggle(bool)) );
   rightCorner.connect( mainBAB->cornerCheck, SIGNAL(toggled(bool)), SLOT(toggle(bool)) );

   int ret = app.exec();
   if (mainBAB) delete mainBAB;
   return ret;
}


bab::bab() : KMainWindow(0,"bab",Qt::WStyle_StaysOnTop | Qt::WX11BypassWM ){
   bSwitcher = new BabSwitcher(this,"switcher");
   QToolTip::add(bSwitcher, i18n("leftclick toggles state<br>rightclick toggles setupmenu"));
   bSwitcher->show();
    
   QDir d( QDir::homeDirPath() + "/.baghira");
   if (!d.exists())
       d.mkdir(QDir::homeDirPath() + "/.baghira");
   d = QDir( QDir::homeDirPath() + "/.baghira/.bab");
   if (!d.exists())
       d.mkdir(QDir::homeDirPath() + "/.baghira/.bab");
   FILE *file = NULL;
   QString tmpString = QDir::homeDirPath() + "/.baghira/Jaguar";
   if (!QFile::exists(tmpString))
   {
      if( (file = fopen(tmpString.latin1(), "w")) != NULL )
      {
         fprintf(file,"0\n0\n0\n0\n0\n1\n-1\n-1\n-1\n-1\n-1\n-1\n-1\n-1\n1\n");
         fclose(file);
      }
   }
   tmpString = QDir::homeDirPath() + "/.baghira/Panther";
   if (!QFile::exists(tmpString))
   {
      if( (file = fopen(tmpString.latin1(), "w")) != NULL )
      {
         fprintf(file,"1\n1\n1\n1\n1\n1\n-1\n-1\n-1\n-1\n-1\n-1\n-1\n-1\n1\n");
         fclose(file);
      }
   }
   tmpString = QDir::homeDirPath() + "/.baghira/Brushed";
   if (!QFile::exists(tmpString))
   {
      if( (file = fopen(tmpString.latin1(), "w")) != NULL )
      {
         fprintf(file,"2\n2\n1\n2\n1\n1\n-1\n-1\n-1\n-1\n-1\n-1\n-1\n-1\n0\n");
         fclose(file);
      }
   }
   tmpString = QDir::homeDirPath() + "/.baghira/Tiger";
   if (!QFile::exists(tmpString))
   {
      if( (file = fopen(tmpString.latin1(), "w")) != NULL )
      {
         fprintf(file,"3\n3\n1\n1\n0\n1\n-1\n-1\n-1\n-1\n-1\n-1\n-1\n-1\n1\n");
         fclose(file);
      }
   }
   tmpString = QDir::homeDirPath() + "/.baghira/Milk";
   if (!QFile::exists(tmpString))
   {
      if( (file = fopen(tmpString.latin1(), "w")) != NULL )
      {
         fprintf(file,"4\n4\n4\n4\n1\n0\n-592138\n-4930834\n-1\n-16777216\n-13009982\n-1\n-16777216\n-1116417\n1\n");
         fclose(file);
      }
   }
   
   QVBox *vb = new QVBox(this);
   vb->setLineWidth(2);
   vb->setMargin(2);
//    vb->setFrameStyle(QFrame::Panel | QFrame::Plain );
   
   QLabel *DefStyleLabel = new QLabel("Default Style is", vb);
   DefStyle = new QComboBox(vb);
   DefStyle->insertItem ( "Jaguar" );
   DefStyle->insertItem ( "Panther" );
   DefStyle->insertItem ( "Brushed" );
   DefStyle->insertItem ( "Tiger" );
   DefStyle->insertItem ( "Milk" );
   cornerCheck = new QCheckBox("Round upper screen corners", vb);
   QSettings config;
   config.beginGroup("/baghira/BAB");
   DefStyle->setCurrentItem(config.readNumEntry("defaultState", BabSwitcher::Panther ));
   cornerCheck->setChecked( config.readBoolEntry("roundCorners", true ) );
   config.endGroup();
   QPushButton *configBaghira = new QPushButton("Configure Baghira", vb);
   QPushButton *Quit = new QPushButton("Quit BAB", vb);
   QPushButton *Close = new QPushButton("Close Menu", vb);
   vb->adjustSize();
   bSwitcher->setStyle(DefStyle->currentText());
   bSwitcher->setDeco(DefStyle->currentText());
   
   connect(DefStyle, SIGNAL(activated(int)), this, SLOT(saveSettings()));
   connect(cornerCheck, SIGNAL(toggled(bool)), this, SLOT(saveSettings()));
   connect(configBaghira, SIGNAL(clicked()), this, SLOT(configureBaghira()));
   connect(Close, SIGNAL(clicked()), this, SLOT(hide()));
   connect(Quit, SIGNAL(clicked()), this, SLOT(close()));
   
   resize(vb->width(),vb->height());
   
//   show();
}

bab::~ bab()
{
   bSwitcher->setStyle(DefStyle->currentText());
   bSwitcher->setDeco(DefStyle->currentText());
   if (bSwitcher) delete bSwitcher;
}

/*Taken from kcontrol, kcmshell style*/
void bab::configureBaghira()
{

   
   // Use KLibLoader to get the library, handling
   // any errors that arise
   KLibLoader* loader = KLibLoader::self();
   
   KLibrary* library = loader->library( QFile::encodeName("kstyle_baghira_config") );
   if (!library)
   {
      qWarning("There was an error loading the configuration dialog for this style.");
      return;
   }
   
   void* allocPtr = library->symbol("allocate_kstyle_config");
   
   if (!allocPtr)
   {
      qWarning("There was an error loading the configuration dialog for this style.");
      return;
   }
   
	//Create the container dialog
   StyleConfigDialog* dial = new StyleConfigDialog(this, "Baghira Configuration");
   dial->enableButtonSeparator(true);
   
   typedef QWidget*(* factoryRoutine)( QWidget* parent );
   
	//Get the factory, and make the widget.
   factoryRoutine factory      = (factoryRoutine)(allocPtr);
   QWidget*  styleConfig = factory( dial );
   
   QTabWidget* tw = (QTabWidget*)styleConfig->child("tabWidget");
   
   QWidget*  decoConfig = 0;
   QObject *decoObject = 0;
   if (tw)
   {
      KLibrary* library2 = loader->library( QFile::encodeName("kwin_baghira_config") );
      if (!library2)
      {
         qWarning("There was an error loading the configuration dialog for the deco.");
         return;
      }
      void* allocPtr2 = library2->symbol("allocate_config");
      if (!allocPtr2)
      {
         qWarning("There was an error loading the configuration dialog for this style.");
         return;
      }
      typedef QObject*(* factoryRoutine2)( KConfig* conf, QWidget* parent );
      factoryRoutine2 factory2 = (factoryRoutine2)(allocPtr2);
      decoObject = factory2( 0, tw );
      decoConfig = (QWidget*)tw->child("ConfigDialog");
      tw->addTab(decoConfig, "Decoration");
   }
   
	//Insert it in...
   dial->setMainWidget( styleConfig );
   
   QWidget *desktop = QApplication::desktop();
   QPoint nPos((desktop->width() - dial->width())/2, (desktop->height() - dial->height() - 100)/2);
   dial->move(nPos);
	//..and connect it to the wrapper
   connect(styleConfig, SIGNAL(changed(bool)), dial, SLOT(setStyleDirty(bool)));
   connect(decoObject, SIGNAL(changed()), dial, SLOT(setDecoDirty()));
   connect(dial, SIGNAL(defaults()), styleConfig, SLOT(defaults()));
   connect(dial, SIGNAL(save()), styleConfig, SLOT(save()));
   connect(dial, SIGNAL(defaults()), decoObject, SLOT(defaults()));
   connect(dial, SIGNAL(save(KConfig*)), decoObject, SLOT(save(KConfig*)));
   
   if (dial->exec() == QDialog::Accepted)
   {
      //For now, ask all KDE apps to recreate their styles to apply the setitngs
      if (dial->isStyleDirty())
      {
         KIPC::sendMessageAll(KIPC::StyleChanged);
         KIPC::sendMessageAll(KIPC::ToolbarStyleChanged);
      }
      if (dial->isDecoDirty())
         kapp->dcopClient()->send("kwin", "KWinInterface", "reconfigure()", QByteArray());
   }
   tw->removeChild( decoObject );
   dial->insertChild( decoObject );
   decoConfig->reparent(dial, QPoint(0,0));
   delete dial;
}

/* overwrite the show event for repositioning the window on top
 of the icon, adapted from kkeyled */
void bab::showEvent(QShowEvent *e){
    if ( bSwitcher->globalPos->x() >= 0) {
       QWidget *desktop = QApplication::desktop();
       int w = desktop->width();
       QPoint nPos;
       if (bSwitcher->globalPos->y() - height() >= 0 )
       { // it's not on top
          nPos.setY(bSwitcher->globalPos->y() -height());
          nPos.setX(bSwitcher->globalPos->x() - (width() / 2));
       }
       else if ( bSwitcher->globalPos->y() >= 0 )
       {
          nPos.setY(bSwitcher->globalPos->y() + bSwitcher->height() );
          nPos.setX(bSwitcher->globalPos->x() - (width() / 2));
       }
       if ( nPos.x() < 0 )
       {
          nPos.setX(bSwitcher->globalPos->x() + bSwitcher->width());
       }
       else if ( bSwitcher->globalPos->x() + (width() / 2)  > w )
       {
          nPos.setX(bSwitcher->globalPos->x() - width());
       }
       move(nPos);
    }
    
   // If it was set by the normal sessionmanagement it needs
   // to set the Flags again
   setWFlags(Qt::WStyle_StaysOnTop | Qt::WX11BypassWM); 
   QWidget::showEvent(e); // execute the normal showevent
   raise();
}

void bab::saveSettings(){
   QSettings *config = new QSettings();
//    KConfig *config = new KConfig("baghirarc");
   config->beginGroup("/baghira/BAB");
   config->writeEntry("defaultState", DefStyle->currentItem());
   config->writeEntry("roundCorners", cornerCheck->isChecked());
   config->endGroup();
   delete config;
}

QPoint *BabSwitcher::globalPos = new QPoint(0,0);

BabSwitcher::BabSwitcher(bab *parent, const char *name) 
                                          : KSystemTray(parent,name), DCOPObject("babInterface") {
   KIconLoader* iLoader = KGlobal::iconLoader();
   JaguarIcon = iLoader->loadIcon("bab_jaguar", KIcon::Small, 22);
   PantherIcon = iLoader->loadIcon("bab_panther", KIcon::Small, 22);
   iTunesIcon = iLoader->loadIcon("bab_itunes", KIcon::Small, 22);
   TigerIcon = iLoader->loadIcon("bab_tiger", KIcon::Small, 22);
   MilkIcon = iLoader->loadIcon("bab_milk", KIcon::Small, 22);
   installEventFilter(this);
}

BabSwitcher::~BabSwitcher(){
}

void BabSwitcher::toggle()
{
   if ( state_ < Milk )
      state_++;
   else
      state_ = Jaguar;
   decoState_ = state_;
   QString nameString;
   switch (state_)
   {
   case Jaguar:
      nameString = "Jaguar";
      break;
   case Panther:
      nameString = "Panther";
      break;
   case iTunes:
      nameString = "Brushed";
      break;
   case Tiger:
      nameString = "Tiger";
      break;
   case Milk:
      nameString = "Milk";
   }
   QString tmpString = QDir::homeDirPath() + "/.baghira/" + nameString;
   QString tmpString2 = QDir::homeDirPath() + "/.baghira/.bab/.style";
   remove(tmpString2.latin1());
   symlink (tmpString.latin1(), tmpString2.latin1());
   tmpString2 = QDir::homeDirPath() + "/.baghira/.bab/.deco";
   remove(tmpString2.latin1());
   symlink (tmpString.latin1(), tmpString2.latin1());
   repaint();
}

void BabSwitcher::setStyle(QString style)
{
   if (style == "Jaguar")
      state_ = Jaguar;
   else if (style == "Brushed")
      state_ = iTunes;
   else if (style == "Tiger")
      state_ = Tiger;
   else if (style == "Milk")
      state_ = Milk;
   else
   {
      if (style != "Panther")
         style = "Panther";
      state_ = Panther;
   }
   QString tmpString = QDir::homeDirPath() + "/.baghira/" + style;
   QString tmpString2 = QDir::homeDirPath() + "/.baghira/.bab/.style";
   remove(tmpString2.latin1());
   symlink (tmpString.latin1(), tmpString2.latin1());
   repaint();
}

void BabSwitcher::setDeco(QString deco)
{
   if (deco == "Jaguar")
      decoState_ = Jaguar;
   else if (deco == "Brushed")
      decoState_ = iTunes;
   else if (deco == "Tiger")
      decoState_ = Tiger;
   else if (deco == "Milk")
      decoState_ = Milk;
   else
   {
      if (deco != "Panther")
         deco = "Panther";
      decoState_ = Panther;
   }
   QString tmpString = QDir::homeDirPath() + "/.baghira/" + deco;
   QString tmpString2 = QDir::homeDirPath() + "/.baghira/.bab/.deco";
   remove(tmpString2.latin1());
   symlink (tmpString.latin1(), tmpString2.latin1());
}

QString BabSwitcher::style()
{
   switch (state_)
   {
   case Jaguar:
      return "Jaguar";
   case Panther:
      return "Panther";
   case iTunes:
      return "Brushed";
   case Tiger:
      return "Tiger";
   case Milk:
      return "Milk";
   }
}

QString BabSwitcher::deco()
{
   switch (decoState_)
   {
   case Jaguar:
      return "Jaguar";
   case Panther:
      return "Panther";
   case iTunes:
      return "Brushed";
   case Tiger:
      return "Tiger";
   case Milk:
      return "Milk";
   }
}

void BabSwitcher::start(QString name, QString settings)
{
   KProcess proc;
   QString section; int i = 0; bool done = false;
   if (name.contains(' '))
   {
      while (!done)
      {
         section = name.section(' ',i,i);
         if (i == 0)
         {
            QString tmpString = QDir::homeDirPath() + "/.baghira/" + settings;
            QString tmpString2 = QDir::homeDirPath() + "/.baghira/.bab/" + section;
            remove(tmpString2.latin1());
            symlink (tmpString.latin1(), tmpString2.latin1());
         }
         if (!section.isEmpty())
            proc << section;
         else
            done = true;
         i++;
      }
   }
   else
   {
      QString tmpString = QDir::homeDirPath() + "/.baghira/" + settings;
      QString tmpString2 = QDir::homeDirPath() + "/.baghira/.bab/" + name;
      remove(tmpString2.latin1());
      symlink (tmpString.latin1(), tmpString2.latin1());
      proc << name;
   }
   proc.start(KProcess::DontCare);
   proc.detach();
}

void BabSwitcher::emitStyleChanged()
{
   KIPC::sendMessageAll(KIPC::StyleChanged);
   KIPC::sendMessageAll(KIPC::ToolbarStyleChanged);
}

void BabSwitcher::mousePressEvent(QMouseEvent *e){

  QWidget *daddy = parentWidget();

  globalPos->setX(e->globalX() - e->x());
  globalPos->setY(e->globalY() - e->y());

   switch ( e->button() ) 
   {
   case LeftButton:
   {
      toggle();
      break;
   }
   case RightButton:
   default:
   {
      if ( daddy->isVisible() )
         daddy->hide();
      else
         daddy->show();
   }
   }
}

void BabSwitcher::paintEvent(QPaintEvent *ev){
   KSystemTray::paintEvent(ev);
   QPainter icyApainter(this);
   switch (state_)
   {
   case Jaguar:
      JaguarIcon.isNull() ? icyApainter.fillRect(rect(), Qt::blue) : icyApainter.drawPixmap(0, 0, JaguarIcon);
      break;
   case Panther:
      PantherIcon.isNull() ? icyApainter.fillRect(rect(), Qt::black) : icyApainter.drawPixmap(0, 0, PantherIcon);
      break;
   case iTunes:
      iTunesIcon.isNull() ? icyApainter.fillRect(rect(), Qt::gray) : icyApainter.drawPixmap(0, 0, iTunesIcon);
      break;
   case Tiger:
      TigerIcon.isNull() ? icyApainter.fillRect(rect(), Qt::yellow) : icyApainter.drawPixmap(0, 0, TigerIcon);
      break;
   case Milk:
      MilkIcon.isNull() ? icyApainter.fillRect(rect(), Qt::white) : icyApainter.drawPixmap(0, 0, MilkIcon);
   default:
      break;
   }
   icyApainter.end();
}

Corner::Corner(Side side) : QWidget(0, (side == left)?"left_corner":"right_corner", Qt::WType_TopLevel | Qt::WStyle_NoBorder | Qt::WStyle_StaysOnTop | Qt::WX11BypassWM )
{
    if (side == left) {
        setFixedSize(5,4);
        move(0,0);
        show();
        setPaletteBackgroundColor(Qt::black);
        XRectangle* xrects = new XRectangle[4];
        xrects[ 0 ].x = 0;
        xrects[ 0 ].y = 0;
        xrects[ 0 ].width = 5;
        xrects[ 0 ].height = 1;
        xrects[ 1 ].x = 0;
        xrects[ 1 ].y = 1;
        xrects[ 1 ].width = 3;
        xrects[ 1 ].height = 1;
        xrects[ 2 ].x = 0;
        xrects[ 2 ].y = 2;
        xrects[ 2 ].width = 2;
        xrects[ 2 ].height = 1;
        xrects[ 3 ].x = 0;
        xrects[ 3 ].y = 3;
        xrects[ 3 ].width = 1;
        xrects[ 3 ].height = 2;
        XShapeCombineRectangles( qt_xdisplay(), winId(), ShapeBounding, 0, 0,
                                 xrects, 4, ShapeSet, 0 );
        delete[] xrects;
    }
    else {
        setFixedSize(5,4);
        move(QApplication::desktop()->width()-5,0);
        show();
        setPaletteBackgroundColor(Qt::black);
        XRectangle* xrects = new XRectangle[4];
        xrects[ 0 ].x = 0;
        xrects[ 0 ].y = 0;
        xrects[ 0 ].width = 5;
        xrects[ 0 ].height = 1;
        xrects[ 1 ].x = 2;
        xrects[ 1 ].y = 1;
        xrects[ 1 ].width = 3;
        xrects[ 1 ].height = 1;
        xrects[ 2 ].x = 3;
        xrects[ 2 ].y = 2;
        xrects[ 2 ].width = 2;
        xrects[ 2 ].height = 1;
        xrects[ 3 ].x = 4;
        xrects[ 3 ].y = 3;
        xrects[ 3 ].width = 1;
        xrects[ 3 ].height = 2;
        XShapeCombineRectangles( qt_xdisplay(), winId(), ShapeBounding, 0, 0,
                                 xrects, 4, ShapeSet, 0 );
        delete[] xrects;
    }
}

void Corner::toggle(bool b)
{
   b?show():hide();
}

#include "bab.moc"
