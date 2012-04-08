
#include <kiconloader.h>
#include "listboxlink.h"
#include <qpainter.h>
#include <qbitmap.h>
#include "eject.xbm"
#include "lock.xbm"

static QBitmap eject = QBitmap( eject_width, eject_height, eject_bits, true );
static QBitmap locked = QBitmap( lock_width, lock_height, lock_bits, true );

ListBoxLink::ListBoxLink( const QString & icon, uint size, const QString & title, const QString & url): QListBoxPixmap(KGlobal::iconLoader()->loadIcon(icon, KIcon::Desktop, size), title),url_(url),icon_(icon)
{
}

ListBoxLink::ListBoxLink( const QPixmap & pixmap, const QString & title, const QString & url) : QListBoxPixmap(pixmap, title), url_(url), icon_(0)
{
}

ListBoxLink::ListBoxLink(ListBoxLink & link) : QListBoxPixmap(*link.pixmap(), link.text()), url_(link.URL()), icon_(link.icon())
{
}

ListBoxLink::~ListBoxLink()
{
}

void ListBoxLink::setURL(const QString & url)
{
   url_ = url;
}

void ListBoxLink::setIcon(const QString & icon)
{
   icon_ = icon;
}

int ListBoxLink::height ( const QListBox * lb ) const
{
   return (QListBoxPixmap::height ( lb ) + 4);
}

ListBoxDevice::ListBoxDevice(const QString & icon, uint size, const QString & title, const QString & url, const QString & name, const QString & mountPoint, bool mounted, bool ejectable, bool removable, int id) : ListBoxLink(icon, size, title, url), name_(name), mountPoint_(mountPoint), mounted_(mounted), ejectable_(ejectable), removable_(removable),id_(id)
{
   if (!eject.mask())
      eject.setMask(eject);
   if (!locked.mask())
      locked.setMask(locked);
}

ListBoxDevice::ListBoxDevice(const QPixmap & pixmap, const QString & title, const QString & url, const QString & name, const QString & mountPoint, bool mounted, bool ejectable, bool removable, int id) : ListBoxLink(pixmap, title, url), name_(name), mountPoint_(mountPoint), mounted_(mounted), ejectable_(ejectable), removable_(removable),id_(id)
{
   if (!eject.mask())
      eject.setMask(eject);
   if (!locked.mask())
      locked.setMask(locked);
}

int ListBoxDevice::width ( const QListBox * lb ) const
{
   if (ejectable_ && mounted_ && lb)
      return lb->width();
   return ListBoxLink::width(lb);
}

void ListBoxDevice::paint( QPainter * p )
{
   if ((ejectable_ || removable_) && mounted_)
   {
      p->save();
      ListBoxLink::paint(p);
      p->setBrush(isSelected()?listBox()->colorGroup().highlightedText():listBox()->colorGroup().text());
      p->drawPixmap(listBox()->width()-22, 11, ejectable_?eject:locked);
      p->restore();
   }
   else
      ListBoxLink::paint(p);
}
