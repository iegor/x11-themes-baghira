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

#ifndef _BAB_H_
#define _BAB_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
//#include <dcopclient.h>
#include "bab_iface.h"
#include <qwidget.h>
#include <qpixmap.h>
#include <qsettings.h>
#include <kmainwindow.h>
#include <ksystemtray.h>

class bab;

class BabSwitcher : public KSystemTray , public babInterface{
   Q_OBJECT
public: 
   
   enum State {Jaguar = 0, Panther, iTunes, Tiger, Milk};

   BabSwitcher(bab *parent=0, const char *name=0);
   ~BabSwitcher();

   void toggle();
   void setStyle(QString);
   QString style();
   void setDeco(QString);
   QString deco();
   void start(QString name, QString settings);
   void emitStyleChanged();
   static QPoint *globalPos;
   
protected:
   void mousePressEvent(QMouseEvent *e);
   void paintEvent(QPaintEvent *ev);

private:

   int state_;
   int decoState_;
   QPixmap JaguarIcon;
   QPixmap PantherIcon;
   QPixmap iTunesIcon;
   QPixmap TigerIcon;
   QPixmap MilkIcon;
};


class QComboBox;
class QCheckBox;
/**
 * @short Application Main Window
 * @author Thomas LÃ¼bking <baghira-style@gmx.net>
 * @version 0.1
 */
class bab : public KMainWindow
{
    Q_OBJECT
public:
    
    enum TabState {Tab = 0, Clever, Chooser};
    bab();
    void showEvent(QShowEvent *e);
   QCheckBox *cornerCheck;
    virtual ~bab();
private:
    BabSwitcher* bSwitcher;
    QComboBox *DefStyle;

private slots:
   void saveSettings();
   void configureBaghira();

};

class Corner : public QWidget
{
    Q_OBJECT
    public:
        enum Side {left = 0, right};
        Corner(Side side);
public slots:
    void toggle(bool);
};

#endif // _BAB_H_
