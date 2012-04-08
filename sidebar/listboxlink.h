
#ifndef LISTBOXLINK_H
#define LISTBOXLINK_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qlistbox.h>

class KListBox;
class QPainter;
class QPixmap;
class QStringList;
class KURL;

class ListBoxLink : public QListBoxPixmap
{
friend class DnDListBox;

public:
   ListBoxLink(const QString & icon, uint size, const QString & title, const QString & url);
   ListBoxLink(const QPixmap & pixmap, const QString & title, const QString & url);
   ListBoxLink(ListBoxLink & link);
   ~ListBoxLink();
   QString & URL(){return url_;}
   void setURL(const QString & url);
   QString & icon(){return icon_;}
   void setIcon(const QString & icon);
   int height ( const QListBox * lb ) const;

private:
   QString url_; //TODO: make this KURL?!
   QString icon_;
};

class ListBoxDevice : public ListBoxLink
{
   friend class MediaListBox;
public:
   ListBoxDevice(const QString & icon, uint size, const QString & title, const QString & url, const QString & name, const QString & mountPoint, bool mounted, bool ejectable = FALSE, bool removable = FALSE, int id = 0);
   ListBoxDevice(const QPixmap & pixmap, const QString & title, const QString & url, const QString & name, const QString & mountPoint, bool mounted, bool ejectable = FALSE, bool removable = FALSE, int id = 0);
   ~ListBoxDevice(){};
   QString & name(){return name_;}
   bool mounted(){return mounted_;}
   bool ejectable(){return ejectable_;}
   bool removable(){return removable_;}
   QString & mountPoint(){return mountPoint_;}
   int id(){return id_;}
   int width ( const QListBox * lb ) const;

protected:
   void paint( QPainter * p );

private:
   QString name_;
   QString mountPoint_;
   bool mounted_;
   bool ejectable_;
   bool removable_;
   int id_;
};

#endif
