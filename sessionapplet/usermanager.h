/***************************************************************************
 *   Copyright (C) 2005 by Thomas Lübking   *
 *   thomas.luebking@web.de   *
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


#ifndef USERMANAGER_H
#define USERMANAGER_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <kpanelapplet.h>
#include <qstring.h>
#include <qiconset.h>
#include <qstringlist.h>
#include <kconfig.h>

class KMenuBar;
class QPopunMenu;
// class QLabel;

class UserManager : public KPanelApplet
{
   Q_OBJECT
    
public:
   UserManager(const QString& configFile, Type t = Normal, int actions = 0,
      QWidget *parent = 0, const char *name = 0);
   /** destructor */
   ~UserManager();

   virtual void about();
   int heightForWidth(int) const;
   int widthForHeight(int) const;
   
protected slots:
   void slotPopulateSessions();
   void slotSessionActivated( int );
   void slotPopulateLanguages();
   void slotLanguageActivated( int );
   
protected:
   void lock();
   void logout();
   void saveSession();
   void doNewSession(bool lock_);
    
private:
   KConfig *ksConfig;
   QWidget *mainView;
   KMenuBar *menu;
   QPopupMenu *sessionPopup;
   QPopupMenu *langPopup;
   QStringList langList;
   QIconSet exitIcon;
   QIconSet lockIcon;
   QIconSet fileSaveIcon;
//    QLabel *langLabel;
};

#endif
