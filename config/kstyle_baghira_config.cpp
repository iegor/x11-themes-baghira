/***************************************************************************
 *   Copyright (C) 2004 by Thomas Lübking                                  *
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

#include <qdesktopwidget.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qdir.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qgroupbox.h>
#include <kfiledialog.h>
#include <qpainter.h>
//#include <qpalette.h> 
// #include <qsettings.h>
#include <qslider.h>
#include <qspinbox.h>
#include <qstringlist.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpixmap.h>
#include <klistview.h>
#include <qtabwidget.h>
#include <qtimer.h>
#include <dcopclient.h>
#include <kapplication.h>
#include <kcolorbutton.h>
#include <kconfig.h>
#include <kimageeffect.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kglobal.h>
#include <kglobalsettings.h>
#include <kprocess.h>
#include <kparts/genericfactory.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <fixx11h.h>

#include "kstyle_baghira_config.h"
#include "pixmaps.h"
#include "colordialog.h"
#include "colorpicker.h"

#include "config.h"

//#define VERSION "0.7"

#ifndef KDE_VERSION_STRING
#define KDE_VERSION_STRING "error"
#endif
#ifndef QT_BUILD_KEY
#define QT_BUILD_KEY "error error error error"
#endif

#define CLAMP(x,l,u) x < l ? l :\
		     x > u ? u :\
		     x

#define SATURATION_COLOR(R,G,B) \
    grey = (299 * R + 587 * G + 114 * B) / 1000; \
    delta = 255 - grey; \
    grey = (grey *(10 - 5)) / 10; \
    iGrey = 255 - grey;\
    destR = (iGrey * (srcR - delta) + grey * R) / 255; \
    destG = (iGrey * (srcG - delta) + grey * G) / 255; \
    destB = (iGrey * (srcB - delta) + grey * B) / 255; 

#define SATURATION_COLOR2(S,R,G,B) \
   int max = (int)(255+0.65*(100-S)); \
   destR = CLAMP((srcR + R - 128), 0, max); \
   destG = CLAMP((srcG + G - 128), 0, max); \
   destB = CLAMP((srcB + B - 128), 0, max); \
   destR = (S*destR + (100-S)*R)/100; \
   destG = (S*destG + (100-S)*G)/100; \
   destB = (S*destB + (100-S)*B)/100;
   
#define COLOR_SPACE(R,G,B) \
    if ( R < 0 ) R = 0; else if ( R > 255 ) R = 255; \
    if ( G < 0 ) G = 0; else if ( G > 255 ) G = 255; \
    if ( B < 0 ) B = 0; else if ( B > 255 ) B = 255;


void kstyle_baghira_config::openHtml1()
{
   QStringList args;
   args << "exec" << "http://baghira.sf.net/";
   kapp->kdeinitExec("kfmclient", args);
}

void kstyle_baghira_config::openHtml2()
{
   QStringList args;
   args << "exec" << "http://www.sf.net/projects/baghira/files/";
   kapp->kdeinitExec("kfmclient", args);
}

void kstyle_baghira_config::startBab()
{
   kapp->kdeinitExec("bab");
}

void kstyle_baghira_config::startKRuler()
{
   kapp->kdeinitExec("kruler");
}

QImage* kstyle_baghira_config::tintButton(QImage &src, QColor c)
{
    QImage *dest = new QImage( src.width(), src.height(), 32, 0 );
    dest->setAlphaBuffer( true );
    unsigned int *data = ( unsigned int * ) src.bits();
    unsigned int *destData = ( unsigned int* ) dest->bits();
    int total = src.width() * src.height();
    int red, green, blue;
    int destR, destG, destB, alpha;
    int srcR = c.red();
    int srcG = c.green();
    int srcB = c.blue();
    int hue, s, v;
    c.getHsv( &hue, &s, &v );
    int sq = CLAMP((int)((45.0/128.0)*s+55),0,100);
    // float srcPercent, destPercent;
    for ( int current = 0 ; current < total ; ++current ) {
        alpha = qAlpha( data[ current ] );
        blue = qBlue( data[ current ] );
    	red = qRed( data[ current ] );
    	green = qGreen( data[ current ] );
	SATURATION_COLOR2(sq, red, green, blue);
	// force back to valid colorspace !
	COLOR_SPACE(destR, destG, destB);
	destData[ current ] = qRgba( destR, destG, destB, alpha );
    }
    return ( dest );
}

QImage* kstyle_baghira_config::tintBrush( QImage &img, QColor c)
{
    QImage *dest = new QImage( img.width(), img.height(), 32, 0 );
    unsigned int *data = ( unsigned int * ) img.bits();
    unsigned int *destData = ( unsigned int* ) dest->bits();
    int total = img.width() * img.height();
    int current;
    int delta;
    int red, green, blue, grey, iGrey;
    int destR, destG, destB, alpha;
 
    int srcR = c.red();
    int srcG = c.green();
    int srcB = c.blue();
    
    
    // float srcPercent, destPercent;
    for ( current = 0 ; current < total ; ++current ) {
        alpha = qAlpha( data[ current ] );
        blue = qBlue( data[ current ] );
    	red = qRed( data[ current ] );
    	green = qGreen( data[ current ] );
	SATURATION_COLOR(red, green, blue);
	// force back to valid colorspace !
	COLOR_SPACE(destR, destG, destB);
	destData[ current ] = qRgba( destR, destG, destB, alpha );
    }
    return ( dest );
}

void kstyle_baghira_config::sliderButtonRedChanged(int value)
{
    sliderButtonRedValue = value;
    dialog_->valueButtonRed->setNum(value);
    dialog_->pixmapButtonPreview->setPixmap(*tintButton(baseImage, QColor(sliderButtonRedValue, sliderButtonGreenValue, sliderButtonBlueValue)));
    dialog_->pixmapButtonPreview->repaint(false);
    configChanged();
}

void kstyle_baghira_config::sliderButtonGreenChanged(int value)
{
    sliderButtonGreenValue = value;
    dialog_->valueButtonGreen->setNum(value);
    dialog_->pixmapButtonPreview->setPixmap(*tintButton(baseImage, QColor(sliderButtonRedValue, sliderButtonGreenValue, sliderButtonBlueValue)));
    dialog_->pixmapButtonPreview->repaint(false);
    configChanged();
}

void kstyle_baghira_config::sliderButtonBlueChanged(int value)
{
    sliderButtonBlueValue = value;
    dialog_->valueButtonBlue->setNum(value);
    dialog_->pixmapButtonPreview->setPixmap(*tintButton(baseImage, QColor(sliderButtonRedValue, sliderButtonGreenValue, sliderButtonBlueValue)));
    dialog_->pixmapButtonPreview->repaint(false);
    configChanged();
}

void kstyle_baghira_config::sliderBrushRedChanged(int value)
{
    sliderBrushRedValue = value;
    dialog_->pixmapBrushPreview->setPixmap(*tintBrush(baseImage2, QColor(sliderBrushRedValue, sliderBrushGreenValue, sliderBrushBlueValue)));
    dialog_->pixmapBrushPreview->repaint(false);
    configChanged();
}

void kstyle_baghira_config::sliderBrushGreenChanged(int value)
{
    sliderBrushGreenValue = value;
    dialog_->pixmapBrushPreview->setPixmap(*tintBrush(baseImage2, QColor(sliderBrushRedValue, sliderBrushGreenValue, sliderBrushBlueValue)));
    dialog_->pixmapBrushPreview->repaint(false);
    configChanged();
}

void kstyle_baghira_config::sliderBrushBlueChanged(int value)
{
    sliderBrushBlueValue = value;
    dialog_->pixmapBrushPreview->setPixmap(*tintBrush(baseImage2, QColor(sliderBrushRedValue, sliderBrushGreenValue, sliderBrushBlueValue)));
    dialog_->pixmapBrushPreview->repaint(false);
    configChanged();
}

// void kstyle_baghira_config::sliderBrushSaturationChanged(int value)
// {
// 	sliderBrushSaturationValue = value;
// 	dialog_->pixmapBrushPreview->setPixmap(*tintBrush(baseImage2, QColor(sliderBrushRedValue, sliderBrushGreenValue, sliderBrushBlueValue), sliderBrushSaturationValue));
// 	dialog_->pixmapBrushPreview->repaint(false);
// 	emit changed(true);
// }

// button color preset table - internal only, AND DON'T CHANGE ONLY ONE!

enum ColorPreset {Aqua = 0, Graphite, GraphiteDark, Orange, Lime, Black, Smoke, Girlie, Smaragd, Emerald, Yellow, DeepBlueSea, Blood, Turquoise, Lilac, NUMBEROFCOLORS};

const char * presetColorName[NUMBEROFCOLORS] = {
    "Aqua", "Graphite", "Graphite (Dark)", "Orange", "Lime", "Black", "Smoke", "Girlie", "Smaragd", "Emerald", "Yellow", "Deep Blue Sea", "Blood", "Turquoise", "Lilac"
};

int presetColor[NUMBEROFCOLORS][3] = {{84,144,218},{130,170,190},{103,118,134},{200,130,30},{130,180,80},{0,0,0},{106,110,120},{255,130,200},{100,160,75},{255,100,90},{160,160,70},{50,100,200},{150,25,25},{60,170,190},{150,130,220}};

// --------------------------------

typedef KGenericFactory<kstyle_baghira_config, QWidget> kstyle_baghira_configFactory;
K_EXPORT_COMPONENT_FACTORY( kcm_kstyle_baghira_config, kstyle_baghira_configFactory("kcmkstyle_baghira_config"))

//extern QImage uic_findImage(const QString& name);
//extern void qInitImages_baghira();
//extern void qCleanupImages_baghira();

void kstyle_baghira_config::updateCR()
{
   if (!about_->isShown())
   {
     timer->stop();
      return;
   }
   if (crCurrent <  2)
      crCurrent++;
   else
      crCurrent = 0;
   about_->crLabel->setText(crNotes[crCurrent]);
}

void kstyle_baghira_config::showAbout()
{
   crCurrent = 0;
   about_->crLabel->setText(crNotes[0]);
   timer->start( 4000 );
   about_->exec();
}

kstyle_baghira_config::kstyle_baghira_config(QWidget *parent, const char *name, const QStringList&) : QWidget(parent, name )//, myAboutData(0)
{
   loadDone = false;
   if (parent) parent->setCaption("Configure Your Cat");
   QVBoxLayout *layout = new QVBoxLayout(this);
   dialog_ = new Config(this);

   for (int i = 0; i < NUMBEROFCOLORS; i++)
      dialog_->colorPresets->insertItem ( presetColorName[i], i );
   
   QWidget *rulesWidget = new QWidget(dialog_->tabWidget);
   QHBoxLayout *rwlh = new QHBoxLayout(rulesWidget);
   applist = new KListView(rulesWidget);
   applist->setFullWidth(true);
#if KDE_IS_VERSION(3,3,91)
   applist->setShadeSortColumn(false);
#endif
   applist->setAllColumnsShowFocus( true );
   applist->setSelectionMode(QListView::Single);
   applist->addColumn ( i18n("Name") );
   applist->addColumn ( i18n("Links to") );
   QDir d( QDir::homeDirPath() + "/.baghira", 0L, QDir::Name | QDir::IgnoreCase, QDir::Files | QDir::Readable | QDir::Writable );
   if (!d.exists())
      d.mkdir(QDir::homeDirPath() + "/.baghira");
   else
   {
      for ( uint i = 0; i < d.count(); i++ )
      {
         QFileInfo FI = QFileInfo(d, d[i]);
         if (FI.isSymLink())
            applist->insertItem(new KListViewItem(applist, d[i], FI.readLink().section('/',-1,-1)));
         else
            applist->insertItem(new KListViewItem(applist, d[i]));
      }
   }
   rwlh->addWidget(applist);
//     rwlh->addLayout(rwlv);
   QVBoxLayout *rwlv = new QVBoxLayout(rwlh);
   rwlv->addStretch();
   QPushButton *btnNew = new QPushButton("New...", rulesWidget);
   QPushButton *btnEdit = new QPushButton("Edit...", rulesWidget);
   QPushButton *btnDel = new QPushButton("Remove", rulesWidget);
   QPushButton *btnDet = new QPushButton("Detect", rulesWidget);
   rwlv->addWidget(btnNew);
   rwlv->addWidget(btnEdit);
   rwlv->addWidget(btnDel);
   rwlv->addSpacing( 5 );
   rwlv->addWidget(btnDet);
   rwlv->addStretch();
   dialog_->tabWidget->addTab( rulesWidget, QString("Rules") );
   appsetter = new AppSetter(this);
   connect(btnNew, SIGNAL(clicked()), appsetter, SLOT(show()));
   connect(btnEdit, SIGNAL(clicked()), this, SLOT(editApp()));
   connect(this, SIGNAL(editApp(QString, QString)), appsetter, SLOT(show(QString, QString)));
   connect(btnDel, SIGNAL(clicked()), this, SLOT(removeApp()));
   connect(btnDet, SIGNAL(clicked()), this, SLOT(selectWindow()));
   connect(appsetter, SIGNAL(addApp(QString, QString)), this, SLOT(addApp(QString, QString)));
   connect(appsetter, SIGNAL(removeApp(QString)), this, SLOT(removeApp(QString)));
   // preview stuff
   baseImage = uic_findImage("button-base");
   
   layout->addWidget(dialog_);
   
   dialog_->show();
   help_ = new Help(dialog_);
   about_ = new About(dialog_);
   about_->versionLabel->setText(VERSION);
   about_->kdeLabel->setText(KDE_VERSION_STRING);
   crCurrent = 0;
   crNotes[0] = "<qt>© 2003-2005 by Thomas Lübking<br>http://baghira.SourceForge.net</qt>";
   crNotes[1] = "<qt>© \"Design\" 2001-2005 by Apple inc.<br>http://www.apple.com";
   crNotes[2] = "<qt>© \"Milk\" 2002-2005 by Max Rudberg<br>http://www.maxthemes.com</qt>";
   timer = new QTimer();
   connect( timer, SIGNAL(timeout()), this, SLOT(updateCR()) );
   QString tmpString(QT_BUILD_KEY);
   about_->systemLabel->setText(tmpString.section(' ',1,1));
   about_->cpuLabel->setText(tmpString.section(' ',0,0));
   about_->setFixedSize (380, 430);
    
   // connections
   // buttons
   connect(dialog_->buttonHelp, SIGNAL(clicked()), help_, SLOT(exec()));
   connect(dialog_->buttonAbout, SIGNAL(clicked()), this, SLOT(showAbout()));
   connect(about_->buttonInfo, SIGNAL(clicked()), this, SLOT(openHtml1()));
   connect(about_->buttonDownload, SIGNAL(clicked()), this, SLOT(openHtml2()));
   connect(help_->startBAB, SIGNAL(clicked()), this, SLOT(startBab()));
   connect(dialog_->startKRuler, SIGNAL(clicked()), this, SLOT(startKRuler()));
   connect(dialog_->buttonLoad, SIGNAL(clicked()), this, SLOT(buttonLoadAction()));
   connect(dialog_->buttonSaveAs, SIGNAL(clicked()), this, SLOT(buttonSaveAsAction()));
   connect(dialog_->colorPresets, SIGNAL(activated(int)), this, SLOT(setPresetColor(int)));
    
   // changes
   connect(dialog_->defaultStyle, SIGNAL(activated(int)), this, SLOT(configChanged()));
   connect(dialog_->buttonStyle, SIGNAL(activated(int)), this, SLOT(handleButtonStyles(int)));
   connect(dialog_->toolbuttonStyle, SIGNAL(activated(int)), this, SLOT(handleButtonStyles(int)));
   connect(dialog_->animateButtons, SIGNAL(stateChanged(int)), this, SLOT(configChanged()));
   connect(dialog_->stippleBackground, SIGNAL(stateChanged(int)), this, SLOT(configChanged()));
   connect(dialog_->bevelHighlights, SIGNAL(stateChanged(int)), this, SLOT(configChanged()));
   connect(dialog_->stippleContrast, SIGNAL(valueChanged (int)), this, SLOT(configChanged()));
   connect(dialog_->shadowGroups,  SIGNAL(stateChanged(int)), this, SLOT(configChanged()));
   connect(dialog_->shadowIntensity, SIGNAL(valueChanged (int)), this, SLOT(configChanged()));
   connect(dialog_->centerTabs,  SIGNAL(stateChanged(int)), this, SLOT(configChanged()));
   connect(dialog_->colorMode, SIGNAL(activated(int)), this, SLOT(configChanged()));
   connect(dialog_->inactiveColorType, SIGNAL(activated(int)), this, SLOT(configChanged()));
   connect(dialog_->animateSlider,  SIGNAL(stateChanged(int)), this, SLOT(configChanged()));
   connect(dialog_->shadowSlider,  SIGNAL(stateChanged(int)), this, SLOT(configChanged()));
   connect(dialog_->squeezeSlider,  SIGNAL(stateChanged(int)), this, SLOT(configChanged()));
   connect(dialog_->unhoveredToolButtons,  SIGNAL(activated(int)), this, SLOT(configChanged()));
   connect(dialog_->aquaTabs,  SIGNAL(activated(int)), this, SLOT(configChanged()));
   connect(dialog_->brushedTabs,  SIGNAL(activated(int)), this, SLOT(configChanged()));
   connect(dialog_->drawTreeLines,  SIGNAL(stateChanged(int)), this, SLOT(configChanged()));
   connect(dialog_->useCustomExpanderColor,  SIGNAL(stateChanged(int)), this, SLOT(configChanged()));
   connect(dialog_->treeLineMode, SIGNAL(activated(int)), this, SLOT(configChanged()));
   connect(dialog_->expanderMode, SIGNAL(activated(int)), this, SLOT(configChanged()));
   connect(dialog_->useCustomExpanderColor,  SIGNAL(stateChanged(int)), this, SLOT(configChanged()));
   connect(dialog_->useRectLVH,  SIGNAL(stateChanged(int)), this, SLOT(configChanged()));
   connect(dialog_->removeKickerBevel,  SIGNAL(stateChanged(int)), this, SLOT(configChanged()));
   connect(dialog_->replaceMenubar,  SIGNAL(toggled(bool)), this, SLOT(menuToggled(bool)));
   connect(dialog_->roundTasks,  SIGNAL(stateChanged(int)), this, SLOT(configChanged()));
   connect(dialog_->showProgressValue,  SIGNAL(stateChanged(int)), this, SLOT(configChanged()));
   connect(dialog_->progressType, SIGNAL(activated(int)), this, SLOT(configChanged()));
   connect(dialog_->menuBackground, SIGNAL(activated(int)), this, SLOT(configChanged()));
   connect(dialog_->menuOpacity, SIGNAL(valueChanged (int)), this, SLOT(configChanged()));
   connect(dialog_->shadowMenuText, SIGNAL(stateChanged(int)), this, SLOT(configChanged()));
   connect(dialog_->menuColorMode, SIGNAL(activated(int)), this, SLOT(configChanged()));
   connect(dialog_->useCustomColors, SIGNAL(stateChanged(int)), this, SLOT(configChanged()));
   connect(dialog_->drawMenuStripe, SIGNAL(stateChanged(int)), this, SLOT(configChanged()));
   connect(dialog_->glossyMenus, SIGNAL(stateChanged(int)), this, SLOT(configChanged()));
             
   connect(dialog_->customInactiveColor, SIGNAL(changed(const QColor &)), this, SLOT(configChanged()));
connect(dialog_->toolbuttonHighColor, SIGNAL(changed(const QColor &)), this, SLOT(changeTooluttonHighlightColor(const QColor &)));
   connect(dialog_->treeLineColor, SIGNAL(changed(const QColor &)), this, SLOT(configChanged()));
   connect(dialog_->customExpanderColor, SIGNAL(changed(const QColor &)), this, SLOT(configChanged()));
   connect(dialog_->menuTextColor, SIGNAL(changed(const QColor &)), this, SLOT(configChanged()));
   connect(dialog_->menuTextColorHigh, SIGNAL(changed(const QColor &)), this, SLOT(configChanged()));
   connect(dialog_->menuColor, SIGNAL(changed(const QColor &)), this, SLOT(configChanged()));
   connect(dialog_->menuColorHigh, SIGNAL(changed(const QColor &)), this, SLOT(configChanged()));
   connect(dialog_->menuColor2, SIGNAL(changed(const QColor &)), this, SLOT(configChanged()));
   connect(dialog_->radioOffColor, SIGNAL(changed(const QColor &)), this, SLOT(configChanged()));
   connect(dialog_->radioOnColor, SIGNAL(changed(const QColor &)), this, SLOT(configChanged()));
   connect(dialog_->checkOffColor, SIGNAL(changed(const QColor &)), this, SLOT(configChanged()));
   connect(dialog_->checkOnColor, SIGNAL(changed(const QColor &)), this, SLOT(configChanged()));
   connect(dialog_->sliderColor, SIGNAL(changed(const QColor &)), this, SLOT(configChanged()));
   connect(dialog_->hoverSliderColorColor, SIGNAL(changed(const QColor &)), this, SLOT(configChanged()));
   connect(dialog_->pressedSliderColor, SIGNAL(changed(const QColor &)), this, SLOT(configChanged()));
   connect(dialog_->inactiveTabColor, SIGNAL(changed(const QColor &)), this, SLOT(configChanged()));
   connect(dialog_->activeTabColor, SIGNAL(changed(const QColor &)), this, SLOT(configChanged()));
   connect(dialog_->menuStripeColor, SIGNAL(changed(const QColor &)), this, SLOT(configChanged()));
   
   connect(dialog_->sliderButtonRed, SIGNAL(valueChanged (int)), this, SLOT(sliderButtonRedChanged(int)));
   connect(dialog_->sliderButtonGreen, SIGNAL(valueChanged (int)), this, SLOT(sliderButtonGreenChanged(int)));
   connect(dialog_->sliderButtonBlue, SIGNAL(valueChanged (int)), this, SLOT(sliderButtonBlueChanged(int)));

   connect(dialog_->sliderBrushRed, SIGNAL(valueChanged (int)), this, SLOT(sliderBrushRedChanged(int)));
   connect(dialog_->sliderBrushGreen, SIGNAL(valueChanged (int)), this, SLOT(sliderBrushGreenChanged(int)));
   connect(dialog_->sliderBrushBlue, SIGNAL(valueChanged (int)), this, SLOT(sliderBrushBlueChanged(int)));
   
   // GUI
   connect (dialog_->defaultStyle, SIGNAL(activated(int)), this, SLOT(setDesign(int)));
   connect (dialog_->inactiveColorType, SIGNAL(activated(int)), this, SLOT(enableInButtonColor(int)));
   connect (dialog_->drawTreeLines, SIGNAL(toggled(bool)), dialog_->treeLineMode, SLOT(setEnabled(bool)));
   connect (dialog_->drawTreeLines, SIGNAL(toggled(bool)), dialog_->treeLineColor, SLOT(setEnabled(bool)));
   connect (dialog_->useCustomExpanderColor, SIGNAL(toggled(bool)), dialog_->customExpanderColor, SLOT(setEnabled(bool)));
   connect (dialog_->menuColorMode, SIGNAL(activated(int)), this, SLOT(enableMenuColorStuff(int)));
   connect (dialog_->menuBackground, SIGNAL(activated(int)), this, SLOT(enableMenuColorStuff(int)));
   connect (dialog_->drawMenuStripe, SIGNAL(toggled(bool)), dialog_->menuStripeColor, SLOT(setEnabled(bool)));
//     connect (dialog_->tintBrush, SIGNAL(toggled(bool)), dialog_->brushBox, SLOT(setEnabled(bool)));
   connect (dialog_->useCustomColors, SIGNAL(toggled(bool)), dialog_->colorFrame, SLOT(setEnabled(bool)));
   
   // load settings
   load();
   loadDone = true;
   
   dialog_->pixmapButtonPreview->setPixmap(*tintButton(baseImage, QColor(sliderButtonRedValue, sliderButtonGreenValue, sliderButtonBlueValue)));

};

kstyle_baghira_config::~kstyle_baghira_config()
{
   if (timer) delete timer;
}

void kstyle_baghira_config::setPresetColor(int i)
{
    dialog_->sliderButtonRed->setValue(presetColor[i][0]);
    dialog_->sliderButtonGreen->setValue(presetColor[i][1]);
    dialog_->sliderButtonBlue->setValue(presetColor[i][2]);
}

void kstyle_baghira_config::load()
{
    QString tmpString = QDir::homeDirPath() + "/.qt/baghirarc";
    load (tmpString);
}

void kstyle_baghira_config::load(QString &fileName)
{
   KConfig *config = new KConfig(fileName);
//    QSettings config;
   config->setGroup("BAB");
   // general Design
    dialog_->defaultStyle->setCurrentItem(config->readNumEntry("defaultState", 0 ));
//     config->endGroup();
    config->setGroup("Style");
   dialog_->buttonStyle->setCurrentItem(config->readNumEntry("Special_ButtonStyle", 1 ));
   dialog_->toolbuttonStyle->setCurrentItem(config->readNumEntry("Special_ToolbuttonStyle", 1 ));
    dialog_->stippleBackground->setChecked( config->readBoolEntry( "Design_StippleBackground", true));
   dialog_->animateButtons->setChecked( config->readBoolEntry( "Design_AnimateButtons", true));
    dialog_->stippleContrast->setValue( config->readNumEntry( "Design_StippleContrast", 3));
    dialog_->shadowGroups->setChecked( config->readBoolEntry( "Design_ShadowGroupBoxes", true));
    dialog_->shadowIntensity->setValue( config->readNumEntry( "Design_GroupBoxeShadowDarkness", 6));
    dialog_->bevelHighlights->setChecked( config->readBoolEntry( "Design_BevelAsHighlight", true));
    dialog_->colorMode->setCurrentItem(config->readNumEntry("Design_ButtonStyle", 0));
    dialog_->inactiveColorType->setCurrentItem(config->readNumEntry( "Design_InactiveButtonStyle", 1));
    dialog_->customInactiveColor->setColor(QColor(config->readNumEntry("Design_InactiveButtonColor",(int)colorGroup().background().rgb())));
    dialog_->centerTabs->setChecked( config->readBoolEntry( "Design_CenterTabs", true));
    // button color stuff
    QColor tmpColor = QColor(config->readNumEntry("Design_ButtonColor",(int)colorGroup().button().rgb()));
    dialog_->sliderButtonRed->setValue(sliderButtonRedValue = tmpColor.red());
    dialog_->sliderButtonGreen->setValue(sliderButtonGreenValue = tmpColor.green());
    dialog_->sliderButtonBlue->setValue(sliderButtonBlueValue = tmpColor.blue());
    dialog_->valueButtonRed->setNum(sliderButtonRedValue);
    dialog_->valueButtonGreen->setNum(sliderButtonGreenValue);
    dialog_->valueButtonBlue->setNum(sliderButtonBlueValue);
    
    // special Widgets
    //ListViews
    dialog_->useRectLVH->setChecked( config->readBoolEntry( "Special_UseFlatLVH", false));
    dialog_->drawTreeLines->setChecked( config->readBoolEntry( "Special_DrawTreeLines", false));
    dialog_->treeLineMode->setCurrentItem(config->readNumEntry( "Special_TreelineStyle", 0));
    dialog_->treeLineColor->setColor( QColor( config->readNumEntry( "Special_TreelineColor", (int) colorGroup().mid().rgb())));
    dialog_->expanderMode->setCurrentItem(config->readNumEntry( "Special_ExpanderStyle", 0));
    dialog_->useCustomExpanderColor->setChecked( config->readBoolEntry( "Special_CustomExpanderColor", false));
    dialog_->customExpanderColor->setColor( QColor( config->readNumEntry( "Special_ExpanderColor", (int) colorGroup().text().rgb())));
    //Kicker
    dialog_->removeKickerBevel->setChecked( config->readBoolEntry( "Special_RemoveKickerBevel", true));
    dialog_->roundTasks->setChecked( config->readBoolEntry( "Special_RoundTaskbuttons", false));
    QFile file(QDir::homeDirPath() + "/.kde/share/apps/kicker/applets/menuapplet.desktop");
    dialog_->replaceMenubar->setChecked( menuReplaced = file.exists() );
    //Scrollbars
    dialog_->animateSlider->setChecked( config->readBoolEntry( "Special_AnimateSlider", true));
    dialog_->shadowSlider->setChecked( config->readBoolEntry( "Special_ShadowSlider", false));
    dialog_->squeezeSlider->setChecked( config->readBoolEntry( "Special_SqueezeSlider", false));
    //Progress
    dialog_->progressType->setCurrentItem( config->readNumEntry( "Special_ProgressStyle", 0));
    dialog_->showProgressValue->setChecked( config->readBoolEntry( "Special_ShowProgressValue", false));
    //Toolbars
    dialog_->unhoveredToolButtons->setCurrentItem(config->readNumEntry( "Special_UnhoveredToolButtons", 2));
     //Tabs
    dialog_->aquaTabs->setCurrentItem(config->readNumEntry("Design_TabStyleAqua", 0 ));
    dialog_->brushedTabs->setCurrentItem(config->readNumEntry("Design_TabStyleBrushed", 1 ));
    
    // menus
    dialog_->menuBackground->setCurrentItem( config->readNumEntry( "Menu_Background", 0));
    dialog_->menuOpacity->setValue( config->readNumEntry( "Menu_Opacity", 70));
    //Look
    dialog_->shadowMenuText->setChecked( config->readBoolEntry( "Menu_ShadowText", true));
    dialog_->menuColorMode->setCurrentItem(config->readNumEntry("Menu_ColorStyle"));
    dialog_->menuTextColor->setColor( QColor( config->readNumEntry( "Menu_TextColor", (int) colorGroup().text().rgb())));
    dialog_->menuTextColorHigh->setColor( QColor( config->readNumEntry( "Menu_TextColorHighlight", (int) colorGroup().highlightedText().rgb())));
    dialog_->menuColor->setColor( QColor( config->readNumEntry( "Menu_Color1", (int) colorGroup().background().rgb())));
    dialog_->menuColor2->setColor( QColor( config->readNumEntry( "Menu_Color2", (int) colorGroup().background().dark(130).rgb())));
    dialog_->menuColorHigh->setColor( QColor( config->readNumEntry( "Menu_ColorHighlight", (int) colorGroup().highlight().rgb())));
    dialog_->drawMenuStripe->setChecked(config->readBoolEntry( "Menu_DrawMenuStripe", false));
   dialog_->glossyMenus->setChecked(config->readBoolEntry( "Menu_Glossy", true));
    dialog_->menuStripeColor->setColor( QColor( config->readNumEntry( "Menu_StripeColor", (int) Qt::white.rgb())));

    // custom colors
    dialog_->useCustomColors->setChecked( config->readBoolEntry( "Colors_UseCustomColors", false));
    dialog_->radioOffColor->setColor( QColor( config->readNumEntry( "Colors_RadioOff", ( int ) colorGroup().background().rgb())));
    dialog_->radioOnColor->setColor( QColor( config->readNumEntry( "Colors_RadioOn", ( int ) colorGroup().button().rgb())));
    dialog_->checkOffColor->setColor( QColor( config->readNumEntry( "Colors_CheckOff", ( int ) colorGroup().background().rgb())));
    dialog_->checkOnColor->setColor( QColor( config->readNumEntry( "Colors_CheckOn", ( int ) colorGroup().button().rgb())));
    dialog_->sliderColor->setColor( QColor( config->readNumEntry( "Colors_Slider", ( int ) colorGroup().button().rgb())));
    dialog_->hoverSliderColorColor->setColor(QColor(config->readNumEntry("Colors_SliderHovered",(int)colorGroup().button().rgb())));
    dialog_->pressedSliderColor->setColor(QColor(config->readNumEntry("Colors_SliderPressed",(int) colorGroup().button().dark(110).rgb())));
    dialog_->sliderGrooveColor->setColor(QColor(config->readNumEntry("Colors_SliderGroove",(int)colorGroup().background().rgb())));
    dialog_->inactiveTabColor->setColor(QColor(config->readNumEntry("Colors_TabInactive",(int) colorGroup().background().rgb())));
    dialog_->activeTabColor->setColor(QColor(config->readNumEntry("Colors_TabActive",(int)colorGroup().button().rgb())));
    dialog_->tintBrush->setChecked( config->readBoolEntry( "Colors_TintBrushedMetal", false));
    brushTint = QColor(config->readNumEntry("Colors_BrushTint",(int)colorGroup().background().rgb()));
//    dialog_->brushTintSaturaion->setValue(settings.readNumEntry( "/qt/Baghira/brushTintSaturation", 3));
    baseImage2 = uic_findImage("brush-me");
    dialog_->pixmapBrushPreview->setPixmap(*tintBrush(baseImage2, brushTint));
    dialog_->sliderBrushRed->setValue(sliderBrushRedValue = brushTint.red());
    dialog_->sliderBrushGreen->setValue(sliderBrushGreenValue = brushTint.green());
    dialog_->sliderBrushBlue->setValue(sliderBrushBlueValue = brushTint.blue());
   config->setGroup("Menubar");
   dialog_->menuMaxWidth->setValue(config->readNumEntry("Width", QDesktopWidget().availableGeometry().width()));
   config->setGroup("Sidebar");
   delete config;
   
   config = new KConfig("kdeglobals");
   config->setGroup( "Toolbar style" );
   dialog_->toolbuttonHighColor->setColor(config->readColorEntry("HighlightColor", new QColor(103,141,178)) );
   delete config;
}


void kstyle_baghira_config::defaults()
{
    dialog_->defaultStyle->setCurrentItem(0);
   dialog_->buttonStyle->setCurrentItem(1);
   dialog_->toolbuttonStyle->setCurrentItem(1);
   dialog_->animateButtons->setChecked( true);
    dialog_->stippleBackground->setChecked( true);
    dialog_->stippleContrast->setValue( 30);
    dialog_->shadowGroups->setChecked( true);
    dialog_->shadowIntensity->setValue( 50);
    dialog_->bevelHighlights->setChecked( true );
    dialog_->centerTabs->setChecked( true);
    dialog_->colorMode->setCurrentItem(0);
    dialog_->inactiveColorType->setCurrentItem(1);
    dialog_->customInactiveColor->setColor(colorGroup().background().rgb());
    // button color stuff
    dialog_->sliderButtonRed->setValue(sliderButtonRedValue = colorGroup().button().red());
    dialog_->sliderButtonGreen->setValue(sliderButtonGreenValue = colorGroup().button().green());
    dialog_->sliderButtonBlue->setValue(sliderButtonBlueValue = colorGroup().button().blue());
    dialog_->valueButtonRed->setNum(sliderButtonRedValue);
    dialog_->valueButtonGreen->setNum(sliderButtonGreenValue);
    dialog_->valueButtonBlue->setNum(sliderButtonBlueValue);
    
    // special Widgets
//    dialog_->animateSlider;
    dialog_->shadowSlider->setChecked( false);
    dialog_->squeezeSlider->setChecked( false);
    dialog_->unhoveredToolButtons->setCurrentItem(2);
    dialog_->toolbuttonHighColor->setColor( QColor(103,141,178) );
    dialog_->drawTreeLines->setChecked( false);
    dialog_->useCustomExpanderColor->setChecked( false);
    dialog_->treeLineMode->setCurrentItem(0);
    dialog_->treeLineColor->setColor( colorGroup().mid().rgb());
    dialog_->expanderMode->setCurrentItem(0);
    dialog_->customExpanderColor->setColor( colorGroup().text().rgb());
    dialog_->useRectLVH->setChecked( false);
    dialog_->removeKickerBevel->setChecked( true);
    dialog_->roundTasks->setChecked( false);
    dialog_->showProgressValue->setChecked( false);
    dialog_->progressType->setCurrentItem( 0);
    dialog_->aquaTabs->setCurrentItem( 0 );
    dialog_->brushedTabs->setCurrentItem(1);
    
    // menus
//    dialog_->transGroup;
    dialog_->menuBackground->setCurrentItem(0);
    dialog_->menuOpacity->setValue( 70);
    dialog_->shadowMenuText->setChecked( true);
    dialog_->menuColorMode->setCurrentItem(0);
    dialog_->menuTextColor->setColor( colorGroup().text().rgb());
    dialog_->menuTextColorHigh->setColor( colorGroup().highlightedText().rgb());
    dialog_->menuColor->setColor( colorGroup().background().rgb());
    dialog_->menuColor2->setColor( colorGroup().background().dark(130).rgb());
    dialog_->menuColorHigh->setColor( colorGroup().highlight().rgb());
    dialog_->drawMenuStripe->setChecked(false);
   dialog_->glossyMenus->setChecked(true);
    dialog_->menuStripeColor->setColor( Qt::white.rgb());
    
    // custom colors
    dialog_->useCustomColors->setChecked( false);
    dialog_->radioOffColor->setColor( colorGroup().background().rgb());
    dialog_->radioOnColor->setColor( colorGroup().button().rgb());
    dialog_->checkOffColor->setColor( colorGroup().background().rgb());
    dialog_->checkOnColor->setColor( colorGroup().button().rgb());
    dialog_->sliderColor->setColor( colorGroup().button().rgb());
    dialog_->hoverSliderColorColor->setColor(colorGroup().button().rgb());
    dialog_->pressedSliderColor->setColor(colorGroup().button().dark(110).rgb());
    dialog_->sliderGrooveColor->setColor(colorGroup().background().rgb());
    dialog_->inactiveTabColor->setColor(colorGroup().background().rgb());
    dialog_->activeTabColor->setColor(colorGroup().button().rgb());
    dialog_->tintBrush->setChecked( false);
    emit changed(true);
}

void kstyle_baghira_config::save()
{
    QString tmpString = QDir::homeDirPath() + "/.qt/baghirarc";
    save(tmpString);
//     emit changed(true);
}

void kstyle_baghira_config::changeTooluttonHighlightColor(const QColor & color)
{
   KConfig *config = new KConfig("kdeglobals");
   config->setGroup( "Toolbar style" );
   config->writeEntry("HighlightColor", color);
   delete config;
}

void kstyle_baghira_config::handleButtonStyles(int i)
{
   if (i != 2 && sender() == dialog_->buttonStyle && dialog_->toolbuttonStyle->currentItem() != 2)
         dialog_->toolbuttonStyle->setCurrentItem(i);
   else if (i != 2 && sender() == dialog_->toolbuttonStyle && dialog_->buttonStyle->currentItem() != 2)
      dialog_->buttonStyle->setCurrentItem(i);
   configChanged();
}

void kstyle_baghira_config::save(QString &fileName)
{
//     QSettings *config = new QSettings;
    KConfig *config = new KConfig(fileName);
    config->setGroup("BAB");
    //General Design
    config->writeEntry("defaultState", dialog_->defaultStyle->currentItem());
//     config->endGroup();
    config->setGroup("Style");
   config->writeEntry("Special_ButtonStyle", dialog_->buttonStyle->currentItem());
   config->writeEntry("Special_ToolbuttonStyle", dialog_->toolbuttonStyle->currentItem());
   config->writeEntry("Design_AnimateButtons", dialog_->animateButtons->isOn());
    config->writeEntry("Design_Default", dialog_->defaultStyle->currentItem());
    config->writeEntry("Design_StippleBackground", dialog_->stippleBackground->isOn());
    config->writeEntry("Design_StippleContrast", dialog_->stippleContrast->value());
    config->writeEntry("Design_ShadowGroupBoxes", dialog_->shadowGroups->isOn());
    config->writeEntry("Design_GroupBoxeShadowDarkness", dialog_->shadowIntensity->value());
    config->writeEntry("Design_BevelAsHighlight", dialog_->bevelHighlights->isOn());
    config->writeEntry("Design_ButtonStyle", dialog_->colorMode->currentItem());
    config->writeEntry("Design_InactiveButtonStyle", dialog_->inactiveColorType->currentItem());
    config->writeEntry("Design_InactiveButtonColor", (int)dialog_->customInactiveColor->color().rgb());
    config->writeEntry("Design_CenterTabs", dialog_->centerTabs->isOn());
    //button color stuff
    config->writeEntry("Design_ButtonColor", (int)(QColor(sliderButtonRedValue, sliderButtonGreenValue, sliderButtonBlueValue).rgb()));
    
    // special Widgets
    //listviews
//    dialog_->animateSlider;
    config->writeEntry("Special_UseFlatLVH", dialog_->useRectLVH->isOn());
    config->writeEntry("Special_DrawTreeLines", dialog_->drawTreeLines->isOn());
    config->writeEntry("Special_TreelineStyle", dialog_->treeLineMode->currentItem());
    config->writeEntry("Special_TreelineColor", (int)dialog_->treeLineColor->color().rgb());
    config->writeEntry("Special_ExpanderStyle", dialog_->expanderMode->currentItem());
    config->writeEntry("Special_CustomExpanderColor", dialog_->useCustomExpanderColor->isOn());
    config->writeEntry("Special_ExpanderColor", (int)dialog_->customExpanderColor->color().rgb());
    //Kicker
    config->writeEntry("Special_RemoveKickerBevel", dialog_->removeKickerBevel->isOn());
    config->writeEntry("Special_RoundTaskbuttons", dialog_->roundTasks->isOn());
   QDir tmpDir(QDir::homeDirPath() + "/.kde");
   if (!tmpDir.exists()) tmpDir.mkdir(QDir::homeDirPath() + "/.kde");
   tmpDir.setPath(QDir::homeDirPath() + "/.kde/share");
   if (!tmpDir.exists()) tmpDir.mkdir(QDir::homeDirPath() + "/.kde/share");
   tmpDir.setPath(QDir::homeDirPath() + "/.kde/share/apps");
   if (!tmpDir.exists()) tmpDir.mkdir(QDir::homeDirPath() + "/.kde/share/apps");
   tmpDir.setPath(QDir::homeDirPath() + "/.kde/share/apps/kicker");
   if (!tmpDir.exists()) tmpDir.mkdir(QDir::homeDirPath() + "/.kde/share/apps/kicker");
   tmpDir.setPath(QDir::homeDirPath() + "/.kde/share/apps/kicker/applets");
   if (!tmpDir.exists()) tmpDir.mkdir(QDir::homeDirPath() + "/.kde/share/apps/kicker/applets");
    QFile file(QDir::homeDirPath() + "/.kde/share/apps/kicker/applets/menuapplet.desktop");
    if (dialog_->replaceMenubar->isOn())
    {
        file.open(IO_WriteOnly);
        const char *msg  = 
"[Desktop Entry]\n\
Hidden=true\n\
Type=Plugin\n\
Encoding=UTF-8\n\
Name=BaghiraMenu\n\
X-KDE-Library=b_menu_panelapplet\n\
X-KDE-UniqueApplet=true\n";
        file.writeBlock( msg, qstrlen(msg) );
        file.close();
    }
    else
        file.remove();
    if (dialog_->replaceMenubar->isOn() != menuReplaced)
        KApplication::dcopClient()->send("kicker", "default", "restart()", "");
    //Srollbars
    config->writeEntry("Special_AnimateSlider", dialog_->animateSlider->isOn());
    config->writeEntry("Special_ShadowSlider", dialog_->shadowSlider->isOn());
    config->writeEntry("Special_SqueezeSlider", dialog_->squeezeSlider->isOn());
    //Progress
    config->writeEntry("Special_ProgressStyle", dialog_->progressType->currentItem());
    config->writeEntry("Special_ShowProgressValue", dialog_->showProgressValue->isOn());
    //Toolbars
    config->writeEntry("Special_UnhoveredToolButtons", dialog_->unhoveredToolButtons->currentItem());
    //Tabs
    config->writeEntry("Design_TabStyleAqua", dialog_->aquaTabs->currentItem() );
    config->writeEntry("Design_TabStyleBrushed", dialog_->brushedTabs->currentItem() );
    // menus
    config->writeEntry("Menu_Background", dialog_->menuBackground->currentItem());
    config->writeEntry("Menu_Opacity", dialog_->menuOpacity->value());
    //Look
    config->writeEntry("Menu_ShadowText", dialog_->shadowMenuText->isOn());
    config->writeEntry("Menu_ColorStyle", dialog_->menuColorMode->currentItem());
    config->writeEntry("Menu_TextColor", (int)dialog_->menuTextColor->color().rgb());
    config->writeEntry("Menu_TextColorHighlight", (int)dialog_->menuTextColorHigh->color().rgb());
    config->writeEntry("Menu_Color1", (int)dialog_->menuColor->color().rgb());
    config->writeEntry("Menu_Color2", (int)dialog_->menuColor2->color().rgb());
    config->writeEntry("Menu_ColorHighlight", (int)dialog_->menuColorHigh->color().rgb());
    config->writeEntry("Menu_DrawMenuStripe", dialog_->drawMenuStripe->isOn());
   config->writeEntry("Menu_Glossy", dialog_->glossyMenus->isOn());
    config->writeEntry("Menu_StripeColor", (int)dialog_->menuStripeColor->color().rgb());
    
    // custom colors
    config->writeEntry("Colors_UseCustomColors", dialog_->useCustomColors->isOn());
    config->writeEntry("Colors_RadioOff", (int)dialog_->radioOffColor->color().rgb());
    config->writeEntry("Colors_RadioOn", (int)dialog_->radioOnColor->color().rgb());
    config->writeEntry("Colors_CheckOff",(int) dialog_->checkOffColor->color().rgb());
    config->writeEntry("Colors_CheckOn", (int)dialog_->checkOnColor->color().rgb());
    config->writeEntry("Colors_Slider",(int) dialog_->sliderColor->color().rgb());
    config->writeEntry("Colors_SliderHovered", (int)dialog_->hoverSliderColorColor->color().rgb());
    config->writeEntry("Colors_SliderPressed", (int)dialog_->pressedSliderColor->color().rgb());
    config->writeEntry("Colors_SliderGroove",(int) dialog_->sliderGrooveColor->color().rgb());
    config->writeEntry("Colors_TabInactive",(int) dialog_->inactiveTabColor->color().rgb());
    config->writeEntry("Colors_TabActive", (int)dialog_->activeTabColor->color().rgb());
    config->writeEntry("Colors_TintBrushedMetal", dialog_->tintBrush->isOn());
    config->writeEntry("Colors_BrushTint",(int)qRgb(sliderBrushRedValue, sliderBrushGreenValue, sliderBrushBlueValue));
   config->setGroup("Menubar");
   config->writeEntry("Width", dialog_->menuMaxWidth->value());
    config->sync();
//     config->endGroup();
    delete config;
}


void kstyle_baghira_config::configChanged()
{
   if (loadDone)
    {
    dialog_->statusWarning->setText("Config changed");
    emit changed(true);
    }
}


QString kstyle_baghira_config::quickHelp() const
{
    return i18n("Configure Your Cat");
}

void kstyle_baghira_config::setDesign(int d)
{
    bool b = (d == 0);
    dialog_->stippleBackground->setEnabled(b);
    dialog_->stippleContrast->setEnabled(b);
    dialog_->shadowGroups->setEnabled(b);
    dialog_->shadowIntensity->setEnabled(b);    
}

void kstyle_baghira_config::enableInButtonColor(int d)
{
    dialog_->customInactiveColor->setEnabled(d == 3);
}

void kstyle_baghira_config::enableMenuColorStuff(int d)
{
   bool b = (d == 2);
   dialog_->menuTextColor->setEnabled(b);
   dialog_->menuColor->setEnabled(b && dialog_->menuBackground->currentItem() > 0);
   dialog_->menuColor2->setEnabled(b && dialog_->menuBackground->currentItem() > 1);
}

void kstyle_baghira_config::buttonLoadAction()
{
    QString tmpString = KFileDialog::getOpenFileName(QDir::homeDirPath(), "*", this, "Select a Baghira config file");
    if (!tmpString.isEmpty())
        load(tmpString);
}

void kstyle_baghira_config::buttonSaveAsAction()
{
    QString tmpString = KFileDialog::getSaveFileName(QDir::homeDirPath(), "*", this, "Save current Baghira configuration as");
    save(tmpString);
}

void kstyle_baghira_config::editApp()
{
   emit editApp(applist->currentItem()->text(0), applist->currentItem()->text(1));
}

void kstyle_baghira_config::removeApp()
{
   QString string = QDir::homeDirPath() + "/.baghira/" + applist->currentItem()->text(0);
   QFile::remove(string);
   applist->takeItem(applist->currentItem());
}

void kstyle_baghira_config::removeApp(QString name)
{
   QString string = QDir::homeDirPath() + "/.baghira/" + name;
   QFile::remove(string);
   applist->takeItem(applist->findItem(name, 0, Qt::ExactMatch));
}

void kstyle_baghira_config::addApp(QString string, QString linkString)
{
   applist->insertItem(new KListViewItem(applist, string, linkString));
   applist->sort();
}

void kstyle_baghira_config::selectWindow()
// taken from kcmkwin
{
   // use a dialog, so that all user input is blocked
   // use WX11BypassWM and moving away so that it's not actually visible
   // grab only mouse, so that keyboard can be used e.g. for switching windows
   grabber = new QDialog( NULL, NULL, true, WX11BypassWM );
   grabber->move( -1000, -1000 );
   grabber->show();
   grabber->grabMouse( crossCursor );
   grabber->installEventFilter( this );
}

Atom wm_state = XInternAtom( qt_xdisplay(), "WM_STATE", False );
// Atom wm_state = XInternAtom( qt_xdisplay(), "WM_COMMAND", False );

bool kstyle_baghira_config::eventFilter( QObject* o, QEvent* e )
// adapted from kcmkwin... again ;)
{
   if( o != grabber )
      return false;
   if( e->type() != QEvent::MouseButtonRelease )
      return false;
   delete grabber;
   grabber = NULL;
   if( static_cast< QMouseEvent* >( e )->button() != LeftButton )
      return true;
   WId winId = findWindow();
   XClassHint classHint;
   if (XGetClassHint(qt_xdisplay(), winId, &classHint))
   {
      QString tmpString = QString( classHint.res_class ).lower() + " (uncertain)";
      appsetter->show(tmpString, QString());
//       qWarning("class: %s",QString( classHint.res_class ).lower().ascii());
      XFree( classHint.res_name );
      XFree( classHint.res_class );
   }
   return true;
}

WId kstyle_baghira_config::findWindow()
// taken from kcmkwin... as well =)
{
   Window root;
   Window child;
   uint mask;
   int rootX, rootY, x, y;
   Window parent = qt_xrootwin();
//    Atom wm_state = XInternAtom( qt_xdisplay(), "WM_STATE", False );
   for( int i = 0;
        i < 10;
        ++i )
   {
      XQueryPointer( qt_xdisplay(), parent, &root, &child,
                     &rootX, &rootY, &x, &y, &mask );
      if( child == None )
         return 0;
      Atom type;
      int format;
      unsigned long nitems, after;
      unsigned char* prop;
      if( XGetWindowProperty( qt_xdisplay(), child, wm_state, 0, 0, False, AnyPropertyType, &type, &format, &nitems, &after, &prop ) == Success )
      {
         if( prop != NULL )
            XFree( prop );
         if( type != None )
            return child;
      }
      parent = child;
   }
   return 0;
}


void kstyle_baghira_config::menuToggled(bool active)
{
    if (!active || !loadDone)
        return;
    KConfigGroup menuConfig(KGlobal::config(), "KDE");
    if (!menuConfig.readBoolEntry("macStyle", false))
        if (KMessageBox::questionYesNo(0, i18n("<qt>You just selected to replace kickers menu applet for the Mac like Menubar on top of the screen, but the Mac like style isn't selected anyway<br>Do you want to activate it now?</qt>"), 0L, KStdGuiItem::yes(), KStdGuiItem::no()) == KMessageBox::Yes)
    {
        KProcess proc;
        proc << "kcmshell" << "desktopbehavior";
        proc.start(KProcess::DontCare);
        proc.detach();
    }
    configChanged();
}

//////////////////////////////////////////////////////////////////////////////
// Plugin Stuff                                                             //
//////////////////////////////////////////////////////////////////////////////
    
extern "C"
{   
    QWidget* allocate_kstyle_config(QWidget* parent){
         return(new kstyle_baghira_config(parent, "BaghiraConfig"));
    }
}

AppSetter::AppSetter(QWidget *parent, const char *name) : QWidget(parent, name, Qt::WType_Dialog | Qt::WShowModal )
{
   isEdit = FALSE;
   
   QLabel *label1 = new QLabel(i18n("<qt><b>Application Name</b></qt>"), this);
   command = new QLineEdit(this);
   
   QLabel *linkTo = new QLabel(i18n("<qt><b>Just like</b></qt>"),this);
   link = new QComboBox(this);
   
   box = new QGroupBox(2,Qt::Horizontal,i18n("Custom Settings"),this);
   box->setCheckable( true );
   
   new QLabel(i18n("<qt><b>Style</b></qt>"), box);
   new QLabel("<qt><b>Deco</b></qt>", box);
   
   style = new QComboBox(box);
   style->insertItem(i18n("Don't set"));
   style->insertItem("Jaguar");
   style->insertItem("Panther");
   style->insertItem("Brushed Metal");
   style->insertItem("Tiger");
   style->insertItem("Milk");
   
   deco = new QComboBox(box);
   deco->insertItem(i18n("Don't set"));
   deco->insertItem("Jaguar");
   deco->insertItem("Panther");
   deco->insertItem("Brushed Metal");
   deco->insertItem("Tiger");
   deco->insertItem("Milk");
   
   new QLabel(i18n("<qt><b>Buttons</b></qt>"), box);
   new QLabel(i18n("<qt><b>Toolbuttons</b></qt>"), box);
   
   buttons = new QComboBox(box);
   buttons->insertItem("Jaguar");
   buttons->insertItem("Panther");
   buttons->insertItem("iTunes");
   buttons->insertItem("Tiger");
   buttons->insertItem("Milk");
   
   
   toolButtons = new QComboBox(box);
   toolButtons->insertItem("Jaguar");
   toolButtons->insertItem("Panther");
   toolButtons->insertItem("iTunes");
   toolButtons->insertItem("Tiger");
   toolButtons->insertItem("Milk");
                
   new QLabel(i18n("<qt><b>Tab vs. Chooser</b></qt>"), box);
   new QLabel(i18n("<qt>Inactive Button</qt>"), box);
   
   tabs = new QComboBox(box);
   tabs->insertItem(i18n("Don't set"));
   tabs->insertItem("Tabs");
   tabs->insertItem("Adaptive");
   tabs->insertItem("Choosers");
   
   
   inbutton = new QComboBox(box);
   inbutton->insertItem("Fixed");
   inbutton->insertItem("Background");
   inbutton->insertItem("Active Button");
   
   scanlines = new QCheckBox("Show scanlines", box);
   
   QWidget *dumb = new QWidget(box);
   customColors = new QCheckBox(dumb);
   buttonColors = new QPushButton("Custom Colors", dumb);
   QHBoxLayout *clh = new QHBoxLayout(dumb);
   clh->addWidget(customColors);
   clh->addWidget(buttonColors);
   
   QVBoxLayout *lv = new QVBoxLayout(this,11,6);
   lv->addWidget(label1);
   lv->addWidget(command);
   QHBoxLayout *lh0 = new QHBoxLayout(lv);
   lh0->addWidget(linkTo);
   lh0->addWidget(link);
   lv->addWidget(box);
   QHBoxLayout *lh = new QHBoxLayout(lv);
   QPushButton *cancel = new QPushButton("Cancel",this);
   lh->addWidget(cancel);
   QPushButton *ok = new QPushButton("Ok",this);
   lh->addWidget(ok);
   
   colorDialog = new ColorDialog(this);
   
   connect(box, SIGNAL(toggled( bool )), link, SLOT(setDisabled(bool)));
   connect(box, SIGNAL(toggled( bool )), linkTo, SLOT(setDisabled(bool)));
   connect(style, SIGNAL(activated( int )), this, SLOT(handler(int)));
   connect(buttons, SIGNAL(activated( int )), this, SLOT(handleButtonStyles(int)));
   connect(toolButtons, SIGNAL(activated( int )), this, SLOT(handleButtonStyles(int)));
   connect(cancel, SIGNAL(clicked()), this, SLOT(reset()));
   connect(ok, SIGNAL(clicked()), this, SLOT(save()));
   connect(customColors, SIGNAL(toggled( bool )), buttonColors, SLOT(setEnabled(bool)));
   connect(buttonColors, SIGNAL(clicked()), colorDialog, SLOT(exec()));
   connect(colorDialog->buttonOk, SIGNAL(clicked()), this, SLOT(updateColors()));
   connect(colorDialog->buttonCancel, SIGNAL(clicked()), this, SLOT(resetColorPicker()));
   reset();
}

AppSetter::~AppSetter()
{
}

void AppSetter::handleButtonStyles(int i)
{
   if (i != 2 && sender() == buttons && toolButtons->currentItem() != 2)
      toolButtons->setCurrentItem(i);
   else if (i != 2 && sender() == toolButtons && buttons->currentItem() != 2)
      buttons->setCurrentItem(i);
}

void AppSetter::handler(int i)
{
   scanlines->setEnabled(i != 3);
}

void AppSetter::updateColors()
{
   for (int i = 0; i < 8; i++)
      colors[i] = (int)colorDialog->picker[i]->color().rgb();
}

void AppSetter::show()
{
   link->clear();
   QDir d( QDir::homeDirPath() + "/.baghira", 0L, QDir::Name | QDir::IgnoreCase, QDir::Files | QDir::NoSymLinks | QDir::Readable | QDir::Writable );
   for ( uint i = 0; i < d.count(); i++ )
      link->insertItem(d[i]);
   QWidget::show();
}

void AppSetter::show(QString appName, QString linkString)
{
   isEdit = true;
   oldName = appName;
   oldLink = linkString;
   command->setText(appName);
   FILE *file = NULL;
   QString tmpString = QDir::homeDirPath() + "/.baghira/" + appName;
   if( (file = fopen(tmpString.latin1(), "r")) != NULL )
   {
      uint s, d, b, t, tc, sc, ib;
      s = d = 4;
      b = t = sc = 2;
      tc = 3;
      for (int i = 0; i < 8; i++)
         colors[i] = -1;
      fscanf(file,"%u\n%u\n%u\n%u\n%u\n%u\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",&s,&d,&b,&t,&tc,&sc,&colors[0],&colors[1],&colors[2],&colors[3],&colors[4],&colors[5],&colors[6],&colors[7],&ib);
      fclose(file);
      if (s < 5) style->setCurrentItem(s+1);
      if (d < 5) deco->setCurrentItem(d+1);
      if (b < 5) buttons->setCurrentItem(b);
      if (t < 5) toolButtons->setCurrentItem(t);
      if (tc < 3) tabs->setCurrentItem(tc+1);
      if (ib < 3) inbutton->setCurrentItem(ib);
      scanlines->setChecked(sc < 2 ? sc : true);
      scanlines->setEnabled(s != 2);
      customColors->setChecked(colors[0] != -1);
      buttonColors->setEnabled(customColors->isChecked());
      resetColorPicker();
   }
   show();
   if (!linkString.isNull())
   {
      for (int i = 0; i < link->count(); i++)
      {
         if (link->text(i) == linkString)
         {
            link->setCurrentItem(i);
            break;
         }
      }
   }
   else
   {
      box->setChecked(true);
   }
}

void AppSetter::resetColorPicker()
{
   if (colors[0] != -1)
   {
      for (int i = 0; i < 8; i++)
      {
         colorDialog->picker[i]->blockSignals(true);
         colorDialog->picker[i]->setColor(QColor(colors[i]));
         colorDialog->picker[i]->blockSignals(false);
      }
      ((QWidget*)colorDialog->demo)->repaint(false);
   }
   else
   {
      for (int i = 0; i < 8; i++)
         colorDialog->picker[i]->blockSignals(true);
      colorDialog->picker[Back]->setColor(colorGroup().background());
      colorDialog->picker[Button]->setColor(colorGroup().button());
      colorDialog->picker[Base]->setColor(colorGroup().base());
      colorDialog->picker[Text]->setColor(colorGroup().text());
      colorDialog->picker[High]->setColor(colorGroup().highlight());
      colorDialog->picker[HighText]->setColor(colorGroup().highlightedText());
      colorDialog->picker[ButText]->setColor(colorGroup().buttonText ());
      colorDialog->picker[Alternate]->setColor(KGlobalSettings::alternateBackgroundColor());
      for (int i = 0; i < 8; i++)
         colorDialog->picker[i]->blockSignals(false);
      ((QWidget*)colorDialog->demo)->repaint(false);
   }
}

void AppSetter::save()
{
   if (command->text() != "" && (!box->isChecked() || (style->currentItem() != 0 && deco->currentItem() != 0)))
   {
      if (!isEdit)
         emit addApp(command->text(), box->isChecked() ? QString() : link->currentText());
      else if (command->text() != oldName || link->currentText() != oldLink)
      {
         emit removeApp(oldName);
         emit addApp(command->text(), box->isChecked() ? QString() : link->currentText());
      }
      if (!customColors->isChecked())
      {
         for (int i = 0; i < 8; i++)
            colors[i] = -1;
      }
      FILE *file = NULL;
      QString tmpString = QDir::homeDirPath() + "/.baghira/" + command->text();
      if (box->isChecked()) // custom settings
      {
         if( (file = fopen(tmpString.latin1(), "w")) != NULL )
         {
            fprintf(file,"%u\n%u\n%u\n%u\n%u\n%u\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",style->currentItem()-1, deco->currentItem()-1, buttons->currentItem(), toolButtons->currentItem(), tabs->currentItem()-1, scanlines->isChecked(), colors[0], colors[1], colors[2], colors[3], colors[4], colors[5], colors[6], colors[7], inbutton->currentItem());
            fclose(file);
         }
      }
      else // just link
      {
         QString tmpString2 = QDir::homeDirPath() + "/.baghira/" + link->currentText();
         QFile::remove(tmpString); // just to be sure ;)
         symlink (tmpString2.latin1(), tmpString.latin1());
      }
   }
   reset();
}

void AppSetter::reset()
{
   hide();
   isEdit = false;
   style->setCurrentItem(0);
   deco->setCurrentItem(0);
   buttons->setCurrentItem(1);
   inbutton->setCurrentItem(0);
   toolButtons->setCurrentItem(1);
   tabs->setCurrentItem(0);
   scanlines->setChecked(false);
   command->clear();
   customColors->setChecked(false);
   for (int i = 0; i < 8; i++)
      colors[i] = -1;
   box->setChecked ( false );
}

#include "kstyle_baghira_config.moc"
