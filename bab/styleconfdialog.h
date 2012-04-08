/*
 * KCMStyle's container dialog for custom style setup dialogs
 *
 * (c) 2003 Maksim Orlovich <maksim.orlovich@kdemail.net>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef STYLE_CONF_DIALOG
#define STYLE_CONF_DIALOG

#include <kdialogbase.h>

class KConfig;
class StyleConfigDialog: public KDialogBase
{
  Q_OBJECT
public:
  StyleConfigDialog(QWidget* parent, QString styleName);

  bool isStyleDirty() const;
  bool isDecoDirty() const;

public slots:
  void setStyleDirty(bool dirty);
  void setDecoDirty();
   
private slots:
   void saveDeco();

signals:
  void defaults();
  void save();
  void save(KConfig*);

private:
  bool m_styleDirty;
  bool m_decoDirty;
};

#endif
