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

#ifndef _BAB_INTERFACE_H_
#define _BAB_INTERFACE_H_
 
#include <dcopobject.h>

class babInterface : virtual public DCOPObject
{
   K_DCOP
   k_dcop:

   virtual void toggle() = 0;
   virtual QString style() = 0;
   virtual void setStyle(QString state) = 0;
   virtual QString deco() = 0;
   virtual void setDeco(QString state) = 0;
   virtual void start(QString name, QString settings) = 0;
   virtual void emitStyleChanged() = 0;

};

#endif
