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

#include "styleconfdialog.h"
#include <klocale.h>

StyleConfigDialog::StyleConfigDialog(QWidget* parent, QString styleName):
  KDialogBase(parent, "StyleConfigDialog",
           true, /*modal*/
           styleName,
           KDialogBase::Default | KDialogBase::Ok | KDialogBase::Cancel,
           KDialogBase::Cancel)
{
  m_styleDirty = false;
  m_decoDirty = false;
  connect( this, SIGNAL( defaultClicked() ), this, SIGNAL( defaults() ));
  connect( this, SIGNAL( okClicked() ), this, SIGNAL( save() ));
  connect( this, SIGNAL( okClicked() ), this, SLOT( saveDeco() ));
}

void StyleConfigDialog::saveDeco()
{
   emit save(0);
}

bool StyleConfigDialog::isStyleDirty() const
{
  return m_styleDirty;
}

void StyleConfigDialog::setStyleDirty(bool dirty)
{
  m_styleDirty = dirty;
}

bool StyleConfigDialog::isDecoDirty() const
{
   return m_decoDirty;
}

void StyleConfigDialog::setDecoDirty()
{
   m_decoDirty = TRUE;
}

#include <styleconfdialog.moc>
