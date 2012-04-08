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

#ifndef _KSTYLE_BAGHIRA_CONFIG_H_
#define _KSTYLE_BAGHIRA_CONFIG_H_

//#include <kcmodule.h>
#include <qcolor.h>
#include <qimage.h>
#include <kaboutdata.h>
#include "configdialog.h"
#include "help.h"
#include "about.h"

class KListView;
class QPushButton;
class AppSetter;
class QTimer;
class QCheckBox;
class QComboBox;
class QGroupBox;

class kstyle_baghira_config: public QWidget
{
   Q_OBJECT

public:
   kstyle_baghira_config( QWidget *parent=0, const char *name=0, const QStringList& = QStringList() );
   ~kstyle_baghira_config();

   virtual void load();
   void load(QString &fileName);
   void save(QString &fileName);
//    virtual int buttons();
   virtual QString quickHelp() const;
   virtual const KAboutData *aboutData()const
   { return myAboutData; };

public slots:
   void configChanged();
   void openHtml1();
   void openHtml2();
   void startBab();
   void startKRuler();
   void sliderButtonRedChanged(int);
   void sliderButtonGreenChanged(int);
   void sliderButtonBlueChanged(int);
   void sliderBrushRedChanged(int);
   void sliderBrushGreenChanged(int);
   void sliderBrushBlueChanged(int);
   virtual void save();
   virtual void defaults();

signals:
   void changed(bool);
   void editApp(QString, QString);

protected:
   bool eventFilter( QObject* o, QEvent* e );

private:
   QString crNotes[3];
   AppSetter *appsetter;
   KListView *applist;
   QImage* tintButton(QImage &src, QColor c);
   QImage* tintBrush( QImage &img, QColor c/*, int saturation*/ );
   WId findWindow();
   KAboutData *myAboutData;
   Config *dialog_;
   Help* help_;
   About* about_;
   QImage baseImage, baseImage2;
   QDialog *grabber;
   QColor buttonColor;
   QColor brushTint;
   int sliderButtonRedValue, sliderButtonGreenValue, sliderButtonBlueValue;
   int sliderBrushRedValue, sliderBrushGreenValue, sliderBrushBlueValue/*, sliderBrushSaturationValue*/;
   bool loadDone;
   bool menuReplaced;
   QTimer *timer;
   int crCurrent;
private slots:
   void updateCR();
   void showAbout();
   void editApp();
   void addApp(QString, QString);
   void removeApp();
   void removeApp(QString);
   void setDesign(int);
   void menuToggled(bool);
   void enableInButtonColor(int d);
   void enableMenuColorStuff(int d);
   void buttonLoadAction();
   void buttonSaveAsAction();
   void setPresetColor(int i);
   void selectWindow();
   void changeTooluttonHighlightColor(const QColor &);
   void handleButtonStyles(int i);
};

class ColorDialog;

class AppSetter : public QWidget
{
Q_OBJECT
public:
   AppSetter(QWidget *parent=0, const char *name=0);
   ~AppSetter();
public slots:
   void show();
   void show(QString appName, QString link);

private:
   bool isEdit;
   int index_;
   int colors[8];
   QString oldName;
   QString oldLink;
   QLineEdit *command;
   QComboBox *link;
   QGroupBox *box;
   QComboBox *style;
   QComboBox *deco;
   QComboBox *buttons;
   QComboBox *toolButtons;
   QComboBox *tabs;
   QComboBox *inbutton;
   QCheckBox *scanlines;
   QCheckBox *customColors;
   QPushButton *buttonColors;
   ColorDialog *colorDialog;

signals:
   void addApp(QString, QString);
   void removeApp(QString);

private slots:
   void save();
   void reset();
   void handler(int);
   void updateColors();
   void resetColorPicker();
   void handleButtonStyles(int i);
};

#endif
