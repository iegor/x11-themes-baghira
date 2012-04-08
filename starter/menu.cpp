#include <qcheckbox.h>
#include <qcursor.h>
#include <qdatetime.h>
#include <qdir.h>
#include <kpopupmenu.h>
#include <qobjectlist.h>
#include <qpainter.h>
#include <qptrlist.h>
#include <qstrlist.h>
#include <qstringlist.h>
#include <qvbox.h>
#include <dcopclient.h>
#include <kapplication.h>
#include <kconfig.h>
#include <kcombobox.h>
#include <kdeversion.h>
#include <kurl.h>
#include <kfileitem.h>
#include <kicondialog.h>
#include <kiconloader.h>
#include <kkeybutton.h>
#include <klistbox.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kpixmapeffect.h>
#include <kprocess.h>
#include <kurifilter.h>
#include <kurl.h>
#include <krun.h>
#include <kservice.h>
#include <kshortcut.h>
#include <ksqueezedtextlabel.h>
#include <kstandarddirs.h>
#include <ksycocaentry.h>
#include <ktextedit.h>
#include <kurlrequester.h>
#include <kwin.h>
//WARNING: THIS IS NOT PORTABLE!
// #include <X11/Xlib.h>
// #include <X11/Xatom.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include <fixx11h.h>
#include <stdlib.h>
// TO HERE --------
//#include "kdrawer.h"
#include "baghiralinkdrag.h"
#include "menu.h"
#include "config.h"
#include "help.h"
#include "linkconfig.h"
#define OPAQUE	0xffffffff
#define OPACITY	"_KDE_WM_WINDOW_OPACITY"

#define _BIGSIZE_(_s_) ((_s_ == 16) ? 22 :\
(_s_ == 22) ? 32 :\
(_s_ == 32) ? 48 :\
(_s_ == 48) ? 64 :\
(_s_ == 64) ? 128 :\
(int)(_s_*1.4))

//TODO: sort functions alphabetically, split files by classes... refactoring sucks ;)

static QColor commentColor;
static QColor infoColor;
static KConfig *config;
static bool useKTTS;


QString spell(const QString text)
{
   QString result;
   for (uint i = 0; i < text.length(); i++)
   {
      result += " "; result += text[i];
   }
   return result;
}

/*
  Internal class to get access to protected QBoxLayout-members
*/
class MyVBoxLayout : public QVBoxLayout
{
   friend class AppList;
public:
   MyVBoxLayout( QLayout * parentLayout, int spacing = -1, const char * name = 0 )
      : QVBoxLayout( parentLayout, spacing, name ) {}
};

StartMenuButton::StartMenuButton( int size, QString icon, QString title, QString command, Orientation orientation, QWidget* parent, const char * name) : QWidget(parent, name), m_title(title), m_command(command), m_icon(icon), m_orientation(orientation), _moving(false)
{
   setBackgroundOrigin(QWidget::ParentOrigin);
   int bigSize = _BIGSIZE_(size);
   m_pix = KGlobal::iconLoader()->loadIcon(icon, KIcon::Desktop, size);
   m_hoverPix = KGlobal::iconLoader()->loadIcon(icon, KIcon::Desktop, bigSize);
   m_pixmapLabel = new QLabel(this, name);
   m_pixmapLabel->setPixmap(m_pix);
   m_pixmapLabel->setBackgroundOrigin(QWidget::AncestorOrigin);
   QBoxLayout* layout;
   if (orientation == Horizontal)
   {
      m_titleLabel = new QLabel("<qt><b>" + title + "</b></qt>", this, name);
      m_titleLabel->setBackgroundOrigin(QWidget::AncestorOrigin);
      m_titleLabel->setTextFormat( Qt::RichText );
      m_titleLabel->setAlignment ( Qt::AlignLeft | Qt::AlignVCenter );
      m_pixmapLabel->setFixedSize(bigSize+2,bigSize+2);
      m_pixmapLabel->setAlignment ( Qt::AlignCenter );
      layout = new QHBoxLayout ( this );
      layout->addSpacing ( 5 );
      layout->addWidget(m_pixmapLabel,0,Qt::AlignCenter);
      layout->addSpacing ( 2 );
      layout->addWidget(m_titleLabel,1);
      layout->addSpacing ( 5 );
   }
   else if (orientation == Vertical)
   {
      m_titleLabel = new QLabel("<qt><b>" + title + "</b></qt>", this, name);
      m_titleLabel->setBackgroundOrigin(QWidget::AncestorOrigin);
      m_titleLabel->setTextFormat( Qt::RichText );
      m_titleLabel->setAlignment ( Qt::AlignHCenter | Qt::AlignTop );
      m_pixmapLabel->setFixedSize(bigSize+2,bigSize+2);
      m_pixmapLabel->setAlignment ( Qt::AlignCenter );
      layout = new QVBoxLayout ( this );
      layout->addSpacing ( 5 );
      layout->addWidget(m_pixmapLabel,0,Qt::AlignCenter);
      layout->addSpacing ( 2 );
      layout->addWidget(m_titleLabel,1);
      layout->addSpacing ( 5 );
   }
   else
   {
      setFixedSize(bigSize+2,bigSize+2);
      m_pixmapLabel->setAlignment ( Qt::AlignCenter );
      m_pixmapLabel->setFixedSize(bigSize+2,bigSize+2);
   }
   setCursor(Qt::PointingHandCursor);
}

void StartMenuButton::reloadIcon(int size)
{
   int bigSize = _BIGSIZE_(size);
   if (m_orientation == Status)
      setFixedSize(bigSize+2,bigSize+2);
   m_pixmapLabel->setFixedSize(bigSize+2,bigSize+2);
   m_pix = KGlobal::iconLoader()->loadIcon(m_icon, KIcon::Desktop, size);
   m_hoverPix = KGlobal::iconLoader()->loadIcon(m_icon, KIcon::Desktop, bigSize);
   m_pixmapLabel->setPixmap(m_pix);
}

void StartMenuButton::smartMove(QPoint &pt)
{
}

void StartMenuButton::smartMove(int x, int y)
{
   if (!dynamic_cast<Panel*>(parentWidget()))
      return;
   StartMenuButton *bt = 0L;
   QObjectList *kids = const_cast<QObjectList*>(parentWidget()->children());
   QRect dRect(QPoint(x,y), size());
   QRect bRect;
   if (kids && !kids->isEmpty())
   {
      QObject *o;
      for ( o = kids->first(); o; o = kids->next() )
         if ((o != this) && (bt = (dynamic_cast<StartMenuButton*>(o))))
         {
            bRect = QRect(bt->pos(), bt->size());
            if (dRect.intersects(bRect))
               break;
            bt = 0L;
         }
   }
   if (!bt)
   {
      move(dRect.topLeft());
      int dst;
      if (((Panel*)parentWidget())->orientation() == Qt::Horizontal)
      {
         dst = pos().x() + width() - parentWidget()->width();
         if (dst > 0)
            emit updateSize(1);
         else if (pos().x() < 0)
            emit updateSize(-1);
      }
      else
      {
         dst = pos().y() + height() - parentWidget()->height();
         if (dst > 0)
            emit updateSize(1);
         else if (pos().y() < 0)
            emit updateSize(-1);
      }
      ((Panel*)parentWidget())->ensureVisible(dRect);
   }
   else if (
            // left of center of left
            (dRect.right() > bRect.right() && dRect.x() < bRect.right() - bRect.width() / 2) ||
            // right of center of right
            (dRect.x() < bRect.x() && dRect.right() > bRect.x() + bRect.width() / 2) ||
            // up of center of upper
            (dRect.bottom() > bRect.bottom() && dRect.y() < bRect.bottom() - bRect.height() / 2) ||
            // below center of lower
            (dRect.y() < bRect.y() && dRect.bottom() > bRect.y() + bRect.height() / 2)
           )
   {
      QPoint bPt = bt->pos();
      bt->move(pos());
      move(bPt);
   }
}

void StartMenuButton::mouseReleaseEvent ( QMouseEvent * mre)
{
   if (mre->state() & Qt::LeftButton)
   {
      if (_moving) { _moving = false; return; }
      emit pressed(m_command);
      return;
   }
   if (mre->state() & Qt::RightButton)
   {
      if (!dynamic_cast<Panel*>(parentWidget()))
         return;
      ((Panel*)parentWidget())->linkConfigDialog->setCaption ( i18n("Configure Link") );
      ((Panel*)parentWidget())->linkConfigDialog->title->setText(m_title);
      ((Panel*)parentWidget())->linkConfigDialog->command->setText(m_command);
      ((Panel*)parentWidget())->linkConfigDialog->icon->setIcon(m_icon);
      disconnect(((Panel*)parentWidget())->linkConfigDialog->buttonOk, SIGNAL(clicked()), 0, 0);
      connect(((Panel*)parentWidget())->linkConfigDialog->buttonOk, SIGNAL(clicked()), ((Panel*)parentWidget())->linkConfigDialog, SLOT(accept()));
      connect(((Panel*)parentWidget())->linkConfigDialog->buttonOk, SIGNAL(clicked()), this, SLOT(edit()));
      ((Panel*)parentWidget())->linkConfigDialog->exec();
   }
}

void StartMenuButton::edit()
{
   if (!dynamic_cast<Panel*>(parentWidget()))
      return;
   m_command = ((Panel*)parentWidget())->linkConfigDialog->command->text();
   if (m_command.isEmpty())
   {
      deleteLater();
      return;
   }
   m_icon = ((Panel*)parentWidget())->linkConfigDialog->icon->icon();
   m_title = ((Panel*)parentWidget())->linkConfigDialog->title->text();
   int bigSize = _BIGSIZE_(((Panel*)parentWidget())->_size);
   m_pix = KGlobal::iconLoader()->loadIcon(m_icon, KIcon::Desktop, ((Panel*)parentWidget())->_size);
   m_hoverPix = KGlobal::iconLoader()->loadIcon(m_icon, KIcon::Desktop, bigSize);
   m_pixmapLabel->setPixmap(m_pix);
   if (m_orientation == Horizontal || m_orientation == Vertical)
      m_titleLabel->setText("<qt><b>" + m_title + "</b></qt>");
   ((Panel*)parentWidget())->linkConfigDialog->close();
}

void Panel::addIcon()
{
   if (!linkConfigDialog->command->text().isEmpty())
      addIcon ( linkConfigDialog->icon->icon(), linkConfigDialog->title->text(), linkConfigDialog->command->text(), iconAddPosition );
}

void StartMenuButton::mouseMoveEvent ( QMouseEvent * mme )
{
   if (!dynamic_cast<Panel*>(parentWidget()))
      return;
   if (mme->state() & Qt::LeftButton)
   {
      _moving = true;
      QPoint pt = mapToParent(mme->pos());
      if (!(mme->state() & Qt::ShiftButton))
      {
         if (pt.y() < -5 || pt.y() > parentWidget()->height() + 5 || pt.x() < -5 || pt.x() > parentWidget()->width() + 5)
         {
            BaghiraLinkDrag *d = new BaghiraLinkDrag( m_title, m_command, m_icon, -1, parentWidget() );
            d->setPixmap(m_hoverPix, QPoint(m_hoverPix.width()/2, m_hoverPix.height()/2));
            d->drag();
            if ((mme->state() & Qt::ControlButton) || BaghiraLinkDrag::accepted())
               return;
            ((Panel*)parentWidget())->poof();
            // do NOT delete d.
            return;
         }
      }
      if (((Panel*)parentWidget())->orientation() == Qt::Horizontal)
         smartMove(pt.x() - width()/2, pos().y());
      else
         smartMove(pos().x(), pt.y() - height()/2);
   }
   else
      _moving = false;
}


void StartMenuButton::enterEvent( QEvent * )
{
   if (m_orientation == Status) emit hovered(m_title);
   m_pixmapLabel->setPixmap(m_hoverPix);
}

void StartMenuButton::leaveEvent( QEvent * )
{
   if (m_orientation == Status) emit unhovered();
   m_pixmapLabel->setPixmap(m_pix);
}


StartMenuEntry::StartMenuEntry(KService * service, QString relPath, int size, bool newbie, QWidget * parent) : QWidget(parent)
{
   groupPath = relPath;
   forNewbie = newbie;
   if (config) // might be first use ever...
   {
      QString tmpString = config->readEntry(service->desktopEntryName());
      if (tmpString != QString::null)
      {
         usage = tmpString.section ( ' ', 0, 0 ).toUInt();
         lastUse = QDate::fromString(tmpString.section ( ' ', 1, 1 ), Qt::ISODate);
         // ranking is naiv but hopefully usefull for the beginning: often usage increases rank, time to the last usage decreases. "8" is just a "random" offset - the idea is that apps that have been used within the last week should have a higher rank than apps that don't appear in the list - setting these to - infinity isn't a good idea as well, as they might be brand new
         rank = 8 + usage - lastUse.daysTo(QDate::currentDate());
      }
      else
      {
         usage = 0;
         rank = 0; // neutral rank
      }
   }
   else
   {
      qWarning("no valid config!");
      usage = 0;
      rank = 0; // neutral rank
   }
   int bigSize = _BIGSIZE_(size);
   isCurrent = false;
   m_service = service;
   exec = m_service->exec();
   display = false;
   m_pix = m_service->pixmap( KIcon::Desktop, size );
   m_hoverPix = m_service->pixmap( KIcon::Desktop, bigSize );
   m_titleLabel = new QLabel("<qt><h3>" + m_service->name() + "</h3></qt>", this);
   m_titleLabel->setTextFormat( Qt::RichText );
   m_commentLabel = new QLabel(m_service->comment(), this);
   m_commentLabel->setPaletteForegroundColor(commentColor);
   m_commentLabel->setTextFormat( Qt::RichText );
   m_pixmapLabel = new QLabel(this);
   m_pixmapLabel->setFixedSize ( bigSize+2, bigSize+2 );
   m_pixmapLabel->setAlignment(Qt::AlignCenter);
   m_pixmapLabel->setPixmap(m_pix);
   QVBoxLayout* spacer = new QVBoxLayout ( this );
   spacer->addSpacing ( 1 );
   QHBoxLayout* layout = new QHBoxLayout ( spacer );
   layout->addWidget(m_pixmapLabel);
   layout->addSpacing ( 2 );
   QVBoxLayout* textLayout = new QVBoxLayout ( layout );
   layout->setStretchFactor ( textLayout, 1 );
   textLayout->addWidget(m_titleLabel);
   textLayout->addWidget(m_commentLabel);
   layout->addStretch();
   spacer->addSpacing ( 1 );
   setCursor(Qt::PointingHandCursor);
}

void StartMenuEntry::reloadIcon(int size)
{
   int bigSize = _BIGSIZE_(size);
   m_pixmapLabel->setFixedSize(bigSize+2,bigSize+2);
   m_pix = m_service->pixmap( KIcon::Desktop, size );
   m_hoverPix = m_service->pixmap( KIcon::Desktop, bigSize );
   m_pixmapLabel->setPixmap(m_pix);
}

StartMenuEntry::~StartMenuEntry()
{
//   if (m_service) delete m_service; m_service = 0L;
}

void StartMenuEntry::saveStats()
{
   if (usage > 0)
      config->writeEntry(m_service->desktopEntryName(), QString::number(usage) +  " " + lastUse.toString(Qt::ISODate));
}

bool StartMenuEntry::operator==( const StartMenuEntry& se ) const
{
   return rank == se.rank;
}
bool StartMenuEntry::operator!=( const StartMenuEntry& se ) const
{
   return rank != se.rank;
}
bool StartMenuEntry::operator<( const StartMenuEntry& se ) const
{
   /*
   if (rank == se.rank)
      return m_titleLabel->text() < se.title();
   else
      */
      return rank > se.rank; // to have descending sort order (we could also use a negative ranking...)
}
bool StartMenuEntry::operator>( const StartMenuEntry& se ) const
{
   /*
   if (rank == se.rank)
      return m_titleLabel->text() > se.title();
   else
   */
      return rank < se.rank; // to have descending sort order (we could also use a negative ranking...)
}
bool StartMenuEntry::operator==( const double& d ) const
{
   return rank == d;
}
bool StartMenuEntry::operator!=( const double& d ) const
{
   return rank != d;
}
bool StartMenuEntry::operator<( const double& d ) const
{
   return rank > d; // to have descending sort order (we could also use a negative ranking...)
}
bool StartMenuEntry::operator>( const double& d ) const
{
   return rank < d; // to have descending sort order (we could also use a negative ranking...)
}

void StartMenuEntry::mouseReleaseEvent ( QMouseEvent * e )
//TODO: do funny stuff with other buttons (e.g. provide edit dialog)
{
   if (e->button() == Qt::LeftButton)
   {
      m_pixmapLabel->setPixmap(m_pix);
      execute();
      if (!(e->state() & Qt::ControlButton))
         emit pressed();
   }
   if (e->button() == Qt::RightButton)
   {
      emit popup(this);
   }
}

void StartMenuEntry::mouseMoveEvent ( QMouseEvent * mme )
{
   if (mme->state() & Qt::LeftButton && (mme->pos().y() < 0 || mme->pos().y() > height() || mme->pos().x() < 0 || mme->pos().x() > width()))
   {
      BaghiraLinkDrag *d = new BaghiraLinkDrag( m_service->name(), m_service->exec(), m_service->icon(), -1, parentWidget() );
      d->setPixmap(m_hoverPix, QPoint(m_hoverPix.width()/2, m_hoverPix.height()/2));
      d->dragCopy();
      // do NOT delete d.
   }
}

void StartMenuEntry::keyPressEvent ( QKeyEvent * e )
{
   switch (e->key())
   {
   case Qt::Key_Escape:
      emit closeMenu();
      break;
   case Qt::Key_Return:
   case Qt::Key_Enter:
      execute();
      if (!(e->state() & Qt::ControlButton))
         emit pressed();
      break;
   case Qt::Key_Down:
      emit appDown();
      break;
   case Qt::Key_Up:
      emit appUp();
      break;
   case Qt::Key_Home: // ->searchline + select
   case Qt::Key_Left:
      emit appLeft();
      break;
   default:
      break;
   }
}

QString StartMenuEntry::title()
{
   return m_titleLabel->text();
}

void StartMenuEntry::focusInEvent ( QFocusEvent * )
{
   isCurrent = true;
   emit hovered("[ " + groupPath + " ]    " + exec);
   if (useKTTS)
   {
      QString text = i18n("for TTS output, telling which item is focussed (keyboard) and than reads the comment", "%1 focussed. %2").arg(m_titleLabel->text()).arg(m_commentLabel->text());
      emit sayText(text);
   }
   setPaletteBackgroundColor(KGlobalSettings::highlightColor());
   m_commentLabel->setPaletteBackgroundColor(KGlobalSettings::highlightColor());
   setPaletteForegroundColor(KGlobalSettings::highlightedTextColor());
   m_commentLabel->setPaletteForegroundColor(KGlobalSettings::highlightedTextColor());
}

void StartMenuEntry::focusOutEvent ( QFocusEvent * )
{
   emit unhovered();
   isCurrent = false;
   setPaletteBackgroundColor(KGlobalSettings::baseColor());
   m_commentLabel->setPaletteBackgroundColor(KGlobalSettings::baseColor());
   if (hasMouse())
   {
      setPaletteForegroundColor(KGlobalSettings::highlightColor());
      m_commentLabel->setPaletteForegroundColor(KGlobalSettings::highlightColor());
   }
   else
   {
      setPaletteForegroundColor(KGlobalSettings::textColor());
      m_commentLabel->setPaletteForegroundColor(commentColor);
   }
}

void StartMenuEntry::enterEvent( QEvent * )
{
   emit hovered("[ " + groupPath + " ]    " + exec);
   if (useKTTS)
   {
      QString text = i18n("for TTS output, telling which item is hovered (mouse) and than reads the comment", "%1 hovered. %2").arg(m_titleLabel->text()).arg(m_commentLabel->text());
      emit sayText(text);
   }
   if (!isCurrent)
   {
      setPaletteForegroundColor(KGlobalSettings::highlightColor());
      m_commentLabel->setPaletteForegroundColor(KGlobalSettings::highlightColor());
   }
   m_pixmapLabel->setPixmap(m_hoverPix);
}

void StartMenuEntry::leaveEvent( QEvent * )
{
   emit unhovered();
   if (!isCurrent)
   {
      setPaletteForegroundColor(KGlobalSettings::textColor());
      m_commentLabel->setPaletteForegroundColor(commentColor);
   }
   m_pixmapLabel->setPixmap(m_pix);
}

void StartMenuEntry::execute()
{
   usage++;
   lastUse = QDate::currentDate();
   rank = 8 + usage;
   KApplication::startServiceByDesktopPath(m_service->desktopEntryPath(), QStringList(), 0, 0, 0, "", true);
   emit executed();
}

Panel::Panel(int size, QWidget * parent, const char * name) : QWidget(parent, name), _size(size), _draggedMe(false), _count(0), _orientation(Qt::Horizontal), _poof(0), _poofIndex(0), _poofAnimPix(0), _poofPix(0)
{
   linkConfigDialog = new LinkConfig();
   setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
//    setPaletteBackgroundColor(KGlobalSettings::highlightColor());
   setAcceptDrops(true);
   config->setGroup("Panel");
   QStringList commands = config->readListEntry("Commands", '§');
   QStringList icons = config->readListEntry("Icons", '§');
   QStringList offsets = config->readListEntry("Offsets", '§');
   QStringList titles = config->readListEntry("Titles", '§');
   QStringList::Iterator it1 = commands.begin();
   QStringList::Iterator it2 = icons.begin();
   QStringList::Iterator it3 = offsets.begin();
   QStringList::Iterator it4 = titles.begin();
   while (it1 != commands.end() &&
          it2 != icons.end() &&
          it3 != offsets.end() &&
          it4 != titles.end())
   {
      addIcon(*it2, *it4, *it1, QPoint((*it3).toInt(),0));
      ++it1;
      ++it2;
      ++it3;
      ++it4;
   }
}

void Panel::save(KConfig *config)
{
   config->setGroup("Panel");
   QObjectList *kids = const_cast<QObjectList*>(children());
   StartMenuButton *bt = 0;
   if (kids && !kids->isEmpty())
   {
      QStringList icons;
      QStringList titles;
      QStringList commands;
      QStringList offsets;
      QObject *o;
      for ( o = kids->first(); o; o = kids->next() )
         if (bt = (dynamic_cast<StartMenuButton*>(o)))
         {
            icons.append(bt->icon());
            titles.append(bt->title());
            commands.append(bt->command());
            _orientation == Qt::Horizontal ?
               offsets.append(QString::number(bt->x())) :
               offsets.append(QString::number(bt->y()));
         }
      config->writeEntry("Commands", commands, '§');
      config->writeEntry("Icons", icons, '§');
      config->writeEntry("Offsets", offsets, '§');
      config->writeEntry("Titles", titles, '§');
   }
}

void Panel::reloadIcons( int size)
{
   _size = size;
   if (_orientation == Qt::Horizontal)
   {
      setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
      setFixedHeight(_BIGSIZE_(_size)+4);
      setMaximumWidth(32767);
   }
   else
   {
      setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
      setFixedWidth(_BIGSIZE_(_size)+4);
      setMaximumHeight(32767);
   }
   QObjectList *kids = const_cast<QObjectList*>(children());
   StartMenuButton *bt = 0;
   if (kids && !kids->isEmpty())
   {
      QObject *o;
      for ( o = kids->first(); o; o = kids->next() )
         if (bt = (dynamic_cast<StartMenuButton*>(o)))
         {
            bt->reloadIcon(size);
         }
   }
}

void Panel::ensureVisible(QRect & rect)
{
   int dx = 0; int dy = 0;
   if (rect.width() > clipRegion().boundingRect().width())
      dx = (rect.width() - clipRegion().boundingRect().width())/2 - rect.x();
   else if (rect.right() > clipRegion().boundingRect().right())
      dx = clipRegion().boundingRect().right() - rect.right();
   else if (rect.x() < clipRegion().boundingRect().x())
      dx = clipRegion().boundingRect().x() - rect.x();
   if (rect.height() > clipRegion().boundingRect().height())
      dy = (rect.height() - clipRegion().boundingRect().height())/2 - rect.y();
   else if (rect.bottom() > clipRegion().boundingRect().bottom())
      dy = clipRegion().boundingRect().bottom() - rect.bottom();
   else if (rect.y() < clipRegion().boundingRect().y())
      dy = clipRegion().boundingRect().y() - rect.y();
   scroll(dx, dy);
}

void Panel::updateSize(int dst)
{
   if (dst > 0)
   {
      if (_orientation == Qt::Horizontal)
      {
         resize(width()+dst, height());
      }
      else
      {
         resize(width(), height()+dst);
      }
   }
   else // more complex: resize and reposition all children, so first one is on (0,0)
   {
      QObjectList *kids = const_cast<QObjectList*>(children());
      if (kids && !kids->isEmpty())
      {
         QObject *o;
         StartMenuButton *bt;
         if (_orientation == Qt::Horizontal)
         {
            resize(width()-dst, height());
            for ( o = kids->first(); o; o = kids->next() )
               if (bt = (dynamic_cast<StartMenuButton*>(o)))
                  bt->move(QPoint(bt->x()-dst, bt->y()));
         }
         else
         {
            resize(width(), height()-dst);
            for ( o = kids->first(); o; o = kids->next() )
               if (bt = (dynamic_cast<StartMenuButton*>(o)))
                  bt->move(QPoint(bt->x(), bt->y()-dst));
         }
      }
   }
}

void Panel::wheelEvent ( QWheelEvent * we )
{
   if (_orientation == Qt::Vertical)
   {
      if (we->delta() > 0)
      {
         if (childrenRect().y() < clipRegion().boundingRect().y())
         {
            if (childrenRect().y() + we->delta() < clipRegion().boundingRect().y())
               scroll ( 0, we->delta() );
            else
               scroll ( 0, clipRegion().boundingRect().y() - childrenRect().y() );
         }
      }
      else
      {
         if (childrenRect().bottom() > clipRegion().boundingRect().bottom())
         {
            if (childrenRect().bottom() + we->delta() > clipRegion().boundingRect().bottom())
               scroll ( 0, we->delta() );
            else
               scroll ( 0, clipRegion().boundingRect().bottom() - childrenRect().bottom() );
         }
      }
   }
   else
   {
      if (we->delta() > 0)
      {
         if (childrenRect().x() < clipRegion().boundingRect().x())
         {
            if (childrenRect().x() + we->delta() < clipRegion().boundingRect().x())
               scroll ( we->delta(), 0 );
            else
               scroll ( clipRegion().boundingRect().x() - childrenRect().x(), 0 );
         }
      }
      else
      {
         if (childrenRect().right() > clipRegion().boundingRect().right())
         {
            if (childrenRect().right() + we->delta() > clipRegion().boundingRect().right())
               scroll ( we->delta(), 0 );
            else
               scroll ( clipRegion().boundingRect().right() - childrenRect().right(), 0 );
         }
      }
   }
}

void Panel::resizeEvent ( QResizeEvent * e)
{
   if (_orientation == Qt::Horizontal)
   {
      if (e->size().height() != e->oldSize().height())
      {
         int h = e->size().height();
         KPixmap bgPix = QPixmap(32, h);
         KPixmap bgPix1 = QPixmap(32, h/2);
         KPixmap bgPix2 = QPixmap(32, h - bgPix1.height());
         QColor color = palette().color(QPalette::Active, QColorGroup::Background);
         KPixmapEffect::gradient( bgPix1, color.light(130), color.dark(105), KPixmapEffect::VerticalGradient, 0);
         KPixmapEffect::gradient( bgPix2, color.dark(120), color.light(110), KPixmapEffect::VerticalGradient, 0);
         QPainter p(&bgPix);
         p.drawPixmap(0,0,bgPix1);
         p.drawPixmap(0,bgPix1.height(),bgPix2);
         p.end();
         setPaletteBackgroundPixmap( bgPix );
      }
   }
   else if (_orientation == Qt::Vertical)
   {
      if (e->size().width() != e->oldSize().width())
      {
         int w = e->size().width();
         KPixmap bgPix = QPixmap(w, 32);
         KPixmap bgPix1 = QPixmap(w/2, 32);
         KPixmap bgPix2 = QPixmap(w - bgPix1.width(), 32);
         QColor color = palette().color(QPalette::Active, QColorGroup::Background);
         KPixmapEffect::gradient( bgPix1, color.light(110), color.dark(120), KPixmapEffect::HorizontalGradient, 0);
         KPixmapEffect::gradient( bgPix2, color.dark(105), color.light(130), KPixmapEffect::HorizontalGradient, 0);
         QPainter p(&bgPix);
         p.drawPixmap(0,0,bgPix1);
         p.drawPixmap(bgPix1.width(),0,bgPix2);
         p.end();
         setPaletteBackgroundPixmap( bgPix );
      }
   }
   QWidget::resizeEvent( e );
}

void Panel::poof()
{
   /*if (_draggedMe)
   {
      _draggedMe = false;
      return;
   }*/
   QObjectList *kids = const_cast<QObjectList*>(children());
   if (kids && !kids->isEmpty())
   {
      QObject *o;
      StartMenuButton *bt = 0L;
      for ( o = kids->first(); o; o = kids->next() )
      {
         bt = 0L;
         if ((bt = (dynamic_cast<StartMenuButton*>(o))) && bt->isMoving())
         {
            bt->hide(); bt->deleteLater(); bt = 0L;
            --_count;
         }
      }
      _poofIndex = 0;
      _poofPix = new QPixmap(locateLocal("data", "baghira/poof.png"), "png");
      _poofAnimPix = new QPixmap(_poofPix->width(), _poofPix->width());
      if (!_poof)
         _poof = new QWidget(0,0, Qt::WType_TopLevel | Qt::WStyle_NoBorder | Qt::WStyle_StaysOnTop | Qt::WX11BypassWM);
#if KDE_IS_VERSION(3,3,91) //3.4 beta
      KWin::setShadowSize(_poof->winId(), 0);
#endif
      _poof->setFixedSize(_poofPix->width(), _poofPix->width());
      int x = QCursor::pos().x() - _poof->width()/2;
      int y = QCursor::pos().y() - _poof->height()/2;
      QPixmap bgPix = QPixmap::grabWindow( qt_xrootwin(), x, y, _poofPix->width(), _poofPix->width());
      _poof->move(x,y);
      _poof->show();
      _poof->setBackgroundOrigin(QWidget::WidgetOrigin);
      _poof->setPaletteBackgroundPixmap( bgPix );
      runPoof();
   }
}

void Panel::runPoof()
{
   if (_poofIndex > 4)
   {
      _poof->hide();
      delete _poofPix;
      _poofPix = 0L;
//       delete _poof;
//       _poof = 0L;
      delete _poofAnimPix;
      _poofAnimPix = 0L;
      _poofIndex = 0;
      return;
   }
   _poof->erase();
   bitBlt(_poof, 0 ,0, _poofPix, 0, _poofIndex * _poofPix->width(), _poofPix->width(), _poofPix->width(), Qt::AndROP);
   ++_poofIndex;
   QTimer::singleShot ( 70, this, SLOT(runPoof()) ); // around 15 fps
}

void Panel::mouseReleaseEvent ( QMouseEvent * mre )
{
   if (mre->state() & Qt::RightButton)
   {
      iconAddPosition = mre->pos();
      linkConfigDialog->setCaption ( i18n("New Link") );
      linkConfigDialog->title->clear();
      linkConfigDialog->command->clear();
      linkConfigDialog->icon->resetIcon();
      disconnect(linkConfigDialog->buttonOk, SIGNAL(clicked()), 0, 0);
      connect(linkConfigDialog->buttonOk, SIGNAL(clicked()), linkConfigDialog, SLOT(accept()));
      connect(linkConfigDialog->buttonOk, SIGNAL(clicked()), this, SLOT(addIcon()));
      linkConfigDialog->exec();
   }
}

void Panel::dragEnterEvent ( QDragEnterEvent *dee )
{
   if (BaghiraLinkDrag::canDecode(dee))
   {
      QObjectList *kids = const_cast<QObjectList*>(children());
      if (kids && !kids->isEmpty())
      {
         QObject *o;
         StartMenuButton *bt = 0L;
         for ( o = kids->first(); o; o = kids->next() )
            if ((bt = (dynamic_cast<StartMenuButton*>(o))) && bt->isMoving())
               // this is just some icon the user drags around ad that accidently left the panel
            {
               BaghiraLinkDrag::setAccepted();
                  // as we cannot access Qts dragmanager and qdragobject doesn't provide a function to cancel the drag, we just emit a virtual escape key...
               _draggedMe = true; // ensure to please not poof ;P
               XTestFakeKeyEvent(qt_xdisplay(),XKeysymToKeycode(qt_xdisplay(), XK_Escape), true, 0);
               XTestFakeKeyEvent(qt_xdisplay(),XKeysymToKeycode(qt_xdisplay(), XK_Escape), false, 0);
               XFlush(qt_xdisplay());
               repositionIcon(bt, mapFromGlobal(dee->pos()));
               return;
            }
      }
      dee->accept(TRUE);
   }
   else if (QUriDrag::canDecode(dee) || QTextDrag::canDecode(dee))
      dee->accept(TRUE);
}

void Panel::dropEvent ( QDropEvent *de )
{
   QStrList list;
   QString title;
   QString command;
   QString icon;
   int index;
   if ( BaghiraLinkDrag::decode(de, &title, &command, &icon, &index) )
   {
      addIcon ( icon, title, command, QPoint(de->pos().x() - (_BIGSIZE_(_size)/2), de->pos().y() - (_BIGSIZE_(_size)/2)));
      BaghiraLinkDrag::setAccepted();
   }
   else if ( QUriDrag::decode(de, list) )
   {
      char *uri;
      KURL url;
      for ( uri = list.first(); uri; uri = list.next() )
      {
         url = KURL(uri);
         if (url.protocol() == "http")
            addIcon ( "html", url.host()+(url.path()=="/"?QString(""):url.path()), uri, de->pos() );
         else
         {
            KFileItem item = KFileItem(KFileItem::Unknown, KFileItem::Unknown, url, true);
            addIcon ( item.iconName(), url.fileName().isEmpty()?url.prettyURL():url.fileName(), uri, de->pos() );
         }
      }
   }
   else if (QTextDrag::decode(de, command))
   {
      KURL url(command);
      if (url.isValid())
      {
         if (url.protocol() == "http")
            addIcon ( "html", url.host()+(url.path()=="/"?QString(""):url.path()), command, de->pos() );
         else
         {
            KFileItem item = KFileItem(KFileItem::Unknown, KFileItem::Unknown, url, true);
            addIcon ( item.iconName(), url.fileName().isEmpty()?url.prettyURL():url.fileName(), command, de->pos() );
         }
      }
      else if (command.contains('@'))
      {
         command.replace(" ","");
         addIcon ( "kmail", command, "mailto:"+command, de->pos() );
      }
      else if (command.contains("'at'")) //last chance for anti-spam addy
      {
         command.replace(" ","");
         command.replace("'at'","@");
         addIcon ( "kmail", command, "mailto:"+command, de->pos() );
      }
      else // ok, just take this as siple unknown command and hope the user knows, what she's ;) doing...
      {
         StartMenuButton *bt = addIcon ( command, command, command, de->pos() );
      }
   }
}

void Panel::repositionIcon(StartMenuButton *button, QPoint pt)
{
   QObjectList *kids = const_cast<QObjectList*>(children());
   if (!kids || kids->isEmpty()) // the easy one ;)
      return;
   if (kids->count() == 1) // button's for certain the only child ;)
   {
      if (_orientation == Qt::Horizontal)
         button->move(pt.x(),0);
      else
         button->move(0,pt.y());
      return;
   }
   QObject *o;
   StartMenuButton *bt = 0L;
   int xy = 0;
   if (_orientation == Qt::Horizontal)
   {
      if (pt.x() < 0) // append horizontally
      {
         for ( o = kids->first(); o; o = kids->next() )
            if ((bt = dynamic_cast<StartMenuButton*>(o)) && bt != button && bt->x() + bt->width() > xy )
               xy = bt->x() + bt->width();
         button->move(xy,0);
      }
      else // inject horizontally
      {
         // first find possible icon under the position
         for ( o = kids->first(); o; o = kids->next() )
         {
            if ((bt = dynamic_cast<StartMenuButton*>(o)) && QRect(bt->pos(), bt->size()).contains(pt))
               break;
            else
               bt = 0l;
         }
         if (bt) // found? - decide whether to insert left or right
         {
            if (bt->x() + bt->width()/2 > pt.x()) // move to old icon place
            {
               button->move(bt->x(), 0);
            }
            else // move to right
            {
               button->move(bt->x() + bt->width(), 0);
            }
            // adjust right icons
            for ( o = kids->first(); o; o = kids->next() )
               if ((bt = dynamic_cast<StartMenuButton*>(o)) && bt->x() >= button->x() && bt != button )
               {
                  bt->move(bt->x() + button->width(), 0);
               }
         }
         else // no collision, just move there
            button->move(pt.x(), 0);
      }
   }
   else
   {
      if (pt.y() < 0) // append vertically
      {
         for ( o = kids->first(); o; o = kids->next() )
            if ((bt = dynamic_cast<StartMenuButton*>(o)) && bt != button && bt->y() + bt->height() > xy )
               xy = bt->y() + bt->height();
         button->move(0, xy);
      }
      else // inject vertically
      {
         // first find possible icon under the position
         for ( o = kids->first(); o; o = kids->next() )
         {
            if ((bt = dynamic_cast<StartMenuButton*>(o)) && QRect(bt->pos(), bt->size()).contains(pt))
               break;
            else
               bt = 0l;
         }
         if (bt) // found? - decide whether to insert up or down
         {
            if (bt->y() + bt->height()/2 > pt.y()) // move to old icon place
               button->move(0, bt->y());
            else // move to right
               button->move(0, bt->y() + bt->height());
            // adjust lower icons
            for ( o = kids->first(); o; o = kids->next() )
               if ((bt = dynamic_cast<StartMenuButton*>(o)) && bt->y() >= button->y() && bt != button )
                  bt->move(0, bt->y() + button->height());
         }
         else // no collision, just move there
            button->move(0, pt.y());
      }
   }
}

StartMenuButton* Panel::addIcon ( QString icon, QString title, QString command, QPoint pt )
{
   StartMenuButton *tmpButton = new StartMenuButton(_size, icon, title, command, StartMenuButton::Status, this);
   // reposition icon
   repositionIcon(tmpButton, pt);
   // connections
   connect (tmpButton, SIGNAL(hovered(const QString &)), this, SIGNAL(message(const QString &)));
   connect (tmpButton, SIGNAL(unhovered()), this, SIGNAL(clearStatus()));
   connect (tmpButton, SIGNAL(updateSize(int)), this, SLOT(updateSize(int)));
   connect (tmpButton, SIGNAL(pressed(const QString &)), parent(), SLOT(execute(const QString &)));
   connect (tmpButton, SIGNAL(pressed(const QString &)), parent(), SLOT(close()));
   // done
   // inc counter
   _count++;
   tmpButton->show();
   return tmpButton;
}

void Panel::setOrientation ( Orientation ori )
{
   if (_orientation == ori)
      return;
   _orientation = ori;
   QObjectList *kids = const_cast<QObjectList*>(children());
   if (!kids || kids->isEmpty())
      return;
   QObject *o;
   StartMenuButton *bt = 0L;
   for ( o = kids->first(); o; o = kids->next() )
      if (bt = (dynamic_cast<StartMenuButton*>(o)))
         bt->move(QPoint(bt->pos().y(), bt->pos().x()));
}

AppList::AppList(int size, QWidget * parent) : QScrollView(parent), _size(size)
{
   popupBlocked_ = false;
   enableClipper( true );
   setFrameStyle(QFrame::LineEditPanel | QFrame::Sunken );
   configDialog_ = new ConfigDialog;
   helpDialog_ = new HelpDialog;
   connect (((QObject*)configDialog_->buttonHelp), SIGNAL(clicked()), ((QObject*)helpDialog_), SLOT(exec()));
   connect (((QObject*)configDialog_->buttonCancel), SIGNAL(clicked()), this, SLOT(unblockPopup()));
   m_widget = new QFrame(viewport());
   m_widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
   m_widget->setPaletteBackgroundColor(KGlobalSettings::baseColor());
   addChild(m_widget,0,0);
   setResizePolicy(QScrollView::AutoOneFit);
   m_widget->show();
   infoLayout = new QVBoxLayout(m_widget);
   infoLabel = new QLabel(m_widget);
   infoLabel->setPaletteBackgroundColor(infoColor);
   infoLabel->setTextFormat( Qt::RichText );
   infoLayout->addWidget(infoLabel);
   m_VLayout = new QVBoxLayout(infoLayout);
   m_iconLoader = KGlobal::iconLoader();
   m_popup = new KPopupMenu(this);
   m_popup->insertItem(i18n("Edit Entry"), this, SLOT(editDialog()));
   m_popup->insertItem(i18n("Remove Entry"), this, SLOT(removeEntry()));
   m_popup->insertSeparator();
   m_popup->insertItem(i18n("Add Entry"), this, SLOT(addDialog()));
   init();
}

void AppList::mouseReleaseEvent ( QMouseEvent * e )
{
   if (e->button() == Qt::RightButton)
      addDialog();
}

void AppList::windowActivationChange ( bool oldActive )
{
   if (isActiveWindow() && entryList.current())
      entryList.current()->setFocus();
   QScrollView::windowActivationChange ( oldActive );
}

void AppList::reloadIcons( int size)
{
   _size = size;
   QPtrListIterator<StartMenuEntry> it(entryList);
   StartMenuEntry *runner;
   while( (runner = it.current()) != 0 )
   {
      ++it;
      runner->reloadIcon(size);
   }
}

void AppList::addDialog()
{
   disconnect (((QObject*)configDialog_->buttonOk), SIGNAL(clicked()), this, 0);
   connect (((QObject*)configDialog_->buttonOk), SIGNAL(clicked()), this, SLOT(addEntry()));
   configDialog_->appName->clear();
   configDialog_->category->clearEdit();
   configDialog_->command->clear();
   configDialog_->genericName->clear();
   configDialog_->iconButton->resetIcon();
   configDialog_->keywords->clear();
   configDialog_->startupFeedback->setChecked( true );
   configDialog_->showInSystray->setChecked( false );
   configDialog_->description->clear();
   configDialog_->startInTerminal->setChecked( false );
   configDialog_->terminalSettings->clear();
   configDialog_->startAsUser->setChecked( false );
   configDialog_->username->clear();
   configDialog_->workingDir->clear();
   configDialog_->shortcut->setShortcut(KShortcut::null(), false);

   configDialog_->setCaption ( i18n("New Entry") );
   ((QWidget*)(configDialog_->extendedGroup))->hide();
   configDialog_->buttonDetail->setDown ( false );
   configDialog_->adjustSize();
   configDialog_->show();
}

void AppList::addEntry()
{
   QString fullpath = configDialog_->category->currentText();
   while (fullpath[0] == '/') // remove leading "/"'s
   {
      fullpath.remove(0,1);
   }
   fullpath = KService::newServicePath(true, fullpath + configDialog_->appName->text());
//qWarning("%s",fullpath.ascii());
   writeEntry(fullpath);
   KService *s = new KService(fullpath);
   QStringList list(KServiceGroup::group(configDialog_->category->currentText())->caption());
   StartMenuEntry *sme = addApp(s, list, configDialog_->category->currentText());
   sme->rank = 0xFFFFFF; // hype rank to max, i.e. keep this entry on top of everything - for this session or until first use
   sme->show();
   sort();
}

void AppList::removeEntry()
{
   popupBlocked_ = true;
   if (KMessageBox::questionYesNo (this, i18n("<qt>Are you sure you want to remove<br> %1</qt>").arg(handledEntry->title()), i18n("Remove ALI entry")) == KMessageBox::Yes)
   {
   qWarning("gonna delete!");
      writeEntry(handledEntry->m_service->locateLocal(), true);
      handledEntry->hide();
      entryList.removeRef(handledEntry);
      delete handledEntry;
      handledEntry = 0L;
   }
   popupBlocked_ = false;
}

void AppList::editDialog()
{
   if (!handledEntry)
      return;
   popupBlocked_ = true;
   disconnect (((QObject*)configDialog_->buttonOk), SIGNAL(clicked()), this, 0);
   connect (((QObject*)configDialog_->buttonOk), SIGNAL(clicked()), this, SLOT(editEntry()));
   configDialog_->appName->setText(handledEntry->m_service->name());
   configDialog_->category->setCurrentItem (handledEntry->groupPath, false);
   configDialog_->showInSystray->setChecked( handledEntry->m_service->exec().contains("ksystraycmd ") );
   if (!configDialog_->showInSystray->isChecked())
      configDialog_->command->setURL(handledEntry->m_service->exec());
   else
      configDialog_->command->setURL(handledEntry->m_service->exec().right(handledEntry->m_service->exec().length() - (handledEntry->m_service->exec().findRev("ksystraycmd ") + 12)));
   configDialog_->genericName->setText(handledEntry->m_service->genericName());
   configDialog_->iconButton->setIcon(handledEntry->m_service->icon ());
   configDialog_->keywords->setText(handledEntry->m_service->keywords().join(","));
   configDialog_->startupFeedback->setChecked( handledEntry->m_service->property ( "StartupNotify" ).toBool());
   configDialog_->description->setText(handledEntry->m_service->comment());
   configDialog_->startInTerminal->setChecked( handledEntry->m_service->terminal() );
   configDialog_->terminalSettings->setText(handledEntry->m_service->terminalOptions());
   configDialog_->startAsUser->setChecked( handledEntry->m_service->substituteUid() );
   configDialog_->username->setText(handledEntry->m_service->username());
   configDialog_->workingDir->setURL(handledEntry->m_service->path());
//    KKeyButton* shortcut;

   configDialog_->setCaption ( i18n("Edit Entry") );
   ((QWidget*)(configDialog_->extendedGroup))->hide();
   configDialog_->buttonDetail->setDown ( false );
   configDialog_->adjustSize();
   configDialog_->show();
}

void AppList::editEntry()
{
   QString fullpath;
   if (handledEntry->groupPath == configDialog_->category->currentText()) // group not changed
      fullpath = handledEntry->m_service->locateLocal(); // find a local replacement path
   else // more complex: remove (del/shadow) entry from old group and create one in the new group
   {
      writeEntry(handledEntry->m_service->locateLocal(), true); //hide from old group
      fullpath = configDialog_->category->currentText();
      while (fullpath[0] == '/') // remove leading "/"'s
         fullpath.remove(0,1);
      fullpath = KService::newServicePath(true, fullpath + configDialog_->appName->text()); // find new path
   }
   writeEntry(fullpath);
   handledEntry->hide();
   QDate lu = handledEntry->lastUse;
   uint u = handledEntry->usage;
   entryList.removeRef(handledEntry);
   delete handledEntry;
   handledEntry = 0L;
   KService *s = new KService(fullpath);
   QStringList list(KServiceGroup::group(configDialog_->category->currentText())->caption());
   StartMenuEntry *sme = addApp(s, list, configDialog_->category->currentText());
   sme->lastUse = lu;
   sme->usage = u;
   sme->rank = 8 + u - lu.daysTo(QDate::currentDate());
   if (!config) config = new KConfig("bStarter", false, false);
   config->setGroup("Statistics");
   sme->saveStats();
   delete config;
   config = 0L;
   sme->show();
   sort();
   popupBlocked_ = false;
}

void AppList::writeEntry(QString path, bool hidden )
{
   KConfig *config = new KConfig(path);
   config->setDesktopGroup();
   
   if (!configDialog_->description->text().isEmpty())
      config->writeEntry("Comment", configDialog_->description->text());
   if (!configDialog_->command->url().isEmpty())
      if (configDialog_->showInSystray->isChecked())
         config->writeEntry("Exec", "ksystraycmd " + configDialog_->command->url());
      else
         config->writeEntry("Exec", configDialog_->command->url());
   if (!configDialog_->genericName->text().isEmpty())
      config->writeEntry("GenericName", configDialog_->genericName->text());
   if (hidden)
      config->writeEntry("Hidden", true);
   else if (config->readBoolEntry("Hidden", false))
      config->writeEntry("Hidden", false);
   if (!configDialog_->iconButton->icon().isNull())
      config->writeEntry("Icon", configDialog_->iconButton->icon());
   if (!configDialog_->keywords->text().isEmpty())
      config->writeEntry("Keywords", QStringList::split(',', configDialog_->keywords->text()));
   if (!configDialog_->appName->text().isEmpty())
      config->writeEntry("Name", configDialog_->appName->text());
   if (!configDialog_->workingDir->url().isEmpty())
      config->writeEntry("Path", configDialog_->workingDir->url());
   if (configDialog_->startupFeedback->isChecked())
      config->writeEntry("StartupNotify", true);
   if (configDialog_->startInTerminal->isChecked())
   {
      config->writeEntry("Terminal", 1);
      if (!configDialog_->terminalSettings->text().isEmpty())
         config->writeEntry("TerminalOptions", configDialog_->terminalSettings->text());
   }
   config->writeEntry("Type", "Application");
   if (configDialog_->startAsUser->isChecked())
   {
      config->writeEntry("X-KDE-SubstituteUID", true);
      if (!configDialog_->username->text().isEmpty())
         config->writeEntry("X-KDE-Username", configDialog_->username->text());
   }
   
   delete config;
   
//    configDialog_->category->setCurrentItem (handledEntry->m_service->categories().join("/"), true);
}

void AppList::popup(StartMenuEntry* entry)
{
   if (popupBlocked_ && configDialog_->isShown())
      return;
   handledEntry = entry;
   m_popup->popup(QCursor::pos());
}

void AppList::sort()
{
   // first clean the layout (i assume that QLayout::remove() will search from beginning so removing over the current order will be quite fast)
   QPtrListIterator<StartMenuEntry> it(entryList);
   StartMenuEntry *runner;
   while( (runner = it.current()) != 0 )
   {
      ++it;
      m_VLayout->remove ( runner );
   }
   ((MyVBoxLayout*)m_VLayout)->deleteAllItems(); // get rid of the stretch
   // then sort the list
   entryList.sort();
   it.toFirst();
   // now rebuild the layout from new list
   while( (runner = it.current()) != 0 )
   {
      ++it;
      m_VLayout->addWidget ( runner );
   }
   m_VLayout->addStretch(1); // add final stretch
}

void AppList::init()
{
   m_root = KServiceGroup::group(QString::null);
   
   if (!m_root || !m_root->isValid())
   {
      qWarning("ROOT NOT FOUND");
      return;
   }
   favItemAmount = config->readNumEntry("FavItemAmount", 10);
   neewbieApps << "konqueror" << "kmail" << "kppp";
   if (newbie = config->readBoolEntry("firstUse", true)) // '/' is certainly not unescaped part of a filename ;)
   {
      infoLabel->setText ( i18n("<qt><b>First Session Applications</b></qt>") );
//       delete config;
//       config = 0L;
   }
   else
   {
      infoLabel->setText ( i18n("<qt><b>Favorite Applications</b><br></qt>") );
   }
   config->setGroup("Statistics");
   QStringList captions, paths;
   insertGroup(m_root, captions, paths);
   paths.sort();
   configDialog_->category->insertStringList(paths);
   configDialog_->category->completionObject()->setCompletionMode( KGlobalSettings::CompletionPopupAuto );
   configDialog_->category->completionObject()->insertItems(paths);
//    if (config) { delete config; config = 0L; }
   sort();
   reset();
}

void AppList::save(KConfig *config)
{
   config->setGroup("Statistics");
   QPtrListIterator<StartMenuEntry> it(entryList);
   StartMenuEntry *runner;
   while( (runner = it.current()) != 0 )
   {
      ++it;
      runner->saveStats();
   }
}

void AppList::insertGroup(KServiceGroup *g, QStringList & captions, QStringList & paths)
{
   KServiceGroup::List list = g->entries(true, true, false, false);
   
   if (list.isEmpty())
   {
      return;
   }
   else
   {
      captions.append(g->caption());
      if (!categories.contains(g->caption()))
         categories.append(g->caption());
      if (!paths.contains(g->relPath()))
         paths.append(g->relPath());
      for( KServiceGroup::List::ConstIterator it = list.begin(); it != list.end(); it++)
      {
         KSycocaEntry *p = (*it);
         if (p->isType(KST_KService))
         {
            KService *s = static_cast<KService *>(p);
            if ((s->name().at(0) == '.'))
               continue;
            if (s->type() == "Application")
            {
               addApp(s, captions, g->relPath());
            }
         }
         else if (p->isType(KST_KServiceGroup))
         {
            KServiceGroup *g2 = static_cast<KServiceGroup *>(p);
            if ((g2->name().at(0) == '.'))
               continue;
            insertGroup(g2,captions, paths);
         }
      }
      captions.remove(g->caption());
   }
}

StartMenuEntry* AppList::addApp(KService * s, QStringList & captions, QString relPath)
{
   StartMenuEntry * tmp = new StartMenuEntry(s, relPath, _size, neewbieApps.find(s->desktopEntryName()) != neewbieApps.end(), m_widget);
   connect (tmp, SIGNAL(appUp()), this, SLOT(appUp()));
   connect (tmp, SIGNAL(appDown()), this, SLOT(appDown()));
   connect (tmp, SIGNAL(appLeft()), this, SLOT(appLeft()));
   connect (tmp, SIGNAL(hovered(const QString&)), this, SIGNAL(message(const QString&)));
   if (useKTTS) connect (tmp, SIGNAL(sayText(const QString&)), this, SIGNAL(sayText(const QString&)));
   connect (tmp, SIGNAL(unhovered()), this, SIGNAL(clearStatus()));
   connect (tmp, SIGNAL(pressed()), parent(), SLOT(close()));
   connect (tmp, SIGNAL(popup(StartMenuEntry*)), this, SLOT(popup(StartMenuEntry*)));
   connect (tmp, SIGNAL(closeMenu()), parent(), SLOT(close()));
   connect (tmp, SIGNAL(executed()), this, SLOT(sort()));
   m_VLayout->addWidget(tmp);
   tmp->hide();
   entryList.append(tmp);
   KeyWordList::Iterator it;
   if (!(s->name().isNull() || s->name().isEmpty()))
   {
      it = m_keywordList.insert(s->name(), StartMenuEntryList(), false);
      it.data().append( tmp );
   }
   QStringList kw;
#if 0
   kw = s->categories(); // THIS IS ****IMPORTANT***** kicker will crash on init if you try to grep through the pointers!
   if (!kw.isEmpty())
   {
      QStringList::Iterator key;
      for ( key = kw.begin(); key != kw.end(); ++key )
      {
         if (!((*key).isNull() || (*key).isEmpty()))
         {
            it = m_keywordList.insJediKnightert(*key, StartMenuEntryList(), false);
            it.data().append( tmp );
         }
      }
   }
#endif
   kw = s->keywords(); // THIS IS ****IMPORTANT***** kicker will crash on init if you try to grep through the pointers!
   if (!kw.isEmpty())
   {
      for ( QStringList::Iterator key = kw.begin(); key != kw.end(); ++key )
      {
         if (!((*key).isNull() || (*key).isEmpty()))
         {
            it = m_keywordList.insert(*key, StartMenuEntryList(), false);
            it.data().append( tmp );
         }
      }
   }
   // group captions (so Games/IDSoftware/Quake3 will appear in Games and IDSoftware)
   if (!captions.isEmpty())
   {
      for ( QStringList::Iterator key = captions.begin(); key != captions.end(); ++key )
      {
         if (!((*key).isNull() || (*key).isEmpty()))
         {
            it = m_groupList.insert(*key, StartMenuEntryList(), false);
            it.data().append( tmp );
         }
      }
   }
   return tmp;
}

void AppList::finish()
{
   entryList.last(); entryList.next();
   m_VLayout->addStretch(1);
   categories.sort();
}

void AppList::appDown()
{
   StartMenuEntry *save;
   if (entryList.current() == 0L/*entryList.getLast()*/)
   {
      entryList.first();
      save = 0L;
   }
   else
   {
      save = entryList.current();
      entryList.next();
   }
   for ( StartMenuEntry *runner = entryList.current(); runner; runner = entryList.next() )
      if (runner->isShown())
      {
         if (save) save->clearFocus();
         runner->setFocus();
         QPoint pt(0,runner->height());
         pt = runner->mapToParent(pt);
         ensureVisible ( pt.x(), pt.y());
         return;
      }
//    if (currentEntry == entryList.end())
//       currentEntry = entryList.begin(); // keep this somewhere valid
}

void AppList::appUp()
{
   if (entryList.current() == entryList.getFirst())
   {
      entryList.current()->clearFocus();
      entryList.last(); entryList.next(); // we jump out
      emit looseKey();
      return;
   }
   StartMenuEntry *save = entryList.current();
   StartMenuEntry *runner;
   if (entryList.current())
      runner = entryList.prev();
   else
      runner = entryList.last();
   for ( ; runner != entryList.getFirst(); runner =  entryList.prev())
      if (runner->isShown())
      {
         if (save) save->clearFocus();
         runner->setFocus();
         QPoint pt(0,0);
         pt = runner->mapToParent(pt);
         ensureVisible ( pt.x(), pt.y());
         return;
      }
   if (runner == entryList.getFirst())
   {
      if (save) save->clearFocus();
      if (runner->isShown())
      {
         runner->setFocus();
         QPoint pt(0,0);
         pt = runner->mapToParent(pt);
         ensureVisible ( pt.x(), pt.y());
      }
      else
      {
         entryList.last(); entryList.next();// we jump out
         emit looseKey();
      }
   }
}
void AppList::appLeft()
{
   if (entryList.current())
   {
      entryList.current()->clearFocus();
      entryList.last(); entryList.next(); // we jump out
   }
   emit looseKey();
}

void AppList::showCategory(const QString & string)
{
   infoLabel->setText(string);
   infoLabel->show();
   StartMenuEntry *it2;
   KeyWordList::Iterator it;
   for ( it = m_groupList.begin(); it != m_groupList.end(); ++it )
   {
      if (it.key() == string)
      {
         for ( it2 = it.data().first(); it2; it2 = it.data().next())
            it2->display = true;
      }
      else
      {
         for ( it2 = it.data().first(); it2; it2 = it.data().next())
            it2->display = it2->display || false;
      }
   }
   uint visibleItems = 0;
   for ( it2 = entryList.first(); it2; it2 = entryList.next())
   {
      if (it2->display)
      {
         visibleItems++;
         it2->show();
      }
      else
         it2->hide();
      it2->display = false;
   }
   if (useKTTS && visibleItems == 0)
   {
      QString text = i18n("for TTS output, informs the user that no entries are in the currently selected group", "Warning! No Applications in group %1").arg(string);
      emit sayText(text);
   }
}

void AppList::search(const QString & string)
{
   StartMenuEntry *it2;
   if (string == QString::null || string == "") // empty line - remove all and exit
   {
      infoLabel->setText ( i18n("<qt><b>Favorite Applications</b><br></qt>") );
      infoLabel->show();
      int i = 0;
      for ( it2 = entryList.first(); it2; it2 = entryList.next() )
      {
         i < favItemAmount ? it2->show() : it2->hide();
         i++;
      }
      return;
   }
   infoLabel->hide();
   // ok, we need a 2pass search: 1st to figure out which to show, 2nd to show or hide things
   // the implementation also prevents us from spending time on showing/hiding things and some onscreen flicker, O(k*n(k)*n)
   KeyWordList::Iterator it;
   //keywords
   for ( it = m_keywordList.begin(); it != m_keywordList.end(); ++it )
   {
      if (it.key().contains(string, false) > 0)
      {
         for ( it2 = it.data().first(); it2; it2 = it.data().next())
            it2->display = true;
      }
      else
      {
         for ( it2 = it.data().first(); it2; it2 = it.data().next())
            it2->display = it2->display || false;
      }
   }
   //groups
   for ( it = m_groupList.begin(); it != m_groupList.end(); ++it )
   {
      if (it.key().contains(string, false) > 0)
      {
         for ( it2 = it.data().first(); it2; it2 = it.data().next())
            it2->display = true;
      }
      else
      {
         for ( it2 = it.data().first(); it2; it2 = it.data().next())
            it2->display = it2->display || false;
      }
   }
   //items
   uint visibleItems = 0;
   for ( it2 = entryList.first(); it2; it2 = entryList.next())
   {
      if (it2->display && visibleItems < 50) //limit this to a healthy size
      {
         visibleItems++;
         it2->show();
      }
      else
         it2->hide();
      it2->display = false;
   }
   if (useKTTS && visibleItems == 0)
   {
      QString text = i18n("for TTS output, no entries match the current search text", "Warning! No more Applications left. The entered Text is %1").arg(spell(string));
      emit sayText(text);
   }
}

void AppList::clear()
{
   for ( StartMenuEntry *it = entryList.first(); it; it = entryList.next())
      it->hide();
}

void AppList::reset()
{
   newbie ?
      infoLabel->setText ( i18n("<qt><b>First Session Applications</b></qt>") ) :
      infoLabel->setText ( i18n("<qt><b>Favorite Applications</b><br></qt>") );
   infoLabel->show();
   if (newbie)
   {
      for ( StartMenuEntry *it = entryList.first(); it; it = entryList.next())
      {
         it->forNewbie ? it->show() : it->hide();
      }
   }
   else
   {
      uint i = 0;
      for ( StartMenuEntry *it = entryList.first(); it; it = entryList.next())
      {
         i < favItemAmount ? it->show() : it->hide();
         i++;
      }
   }
   setContentsPos(0, 0);
}

StartMenu::StartMenu( int size, QWidget * parent, WFlags f ) : QWidget(parent, "StartMenu", f), _size(size), inMove(false)
{
   m_panelPos = StartMenu::Nowhere;
   panelLayout = new QGridLayout ( this, 3, 3 );
   config = new KConfig("bStarter", true, false);
   config->setGroup("Shell");
   history = config->readListEntry("History");
   config->setGroup("Settings");
   if (useKTTS = config->readBoolEntry("useKTTS", false))
      m_spokenText = 0;
   _filterData = new KURIFilterData();
   int r,g,b,r2,g2,b2;
   KGlobalSettings::baseColor().getRgb(&r,&g,&b);
   KGlobalSettings::textColor().getRgb(&r2,&g2,&b2);
   commentColor.setRgb((r+r2)/2,(g+g2)/2,(b+b2)/2);
   infoColor.setRgb((3*r+r2)/4,(3*g+g2)/4,(3*b+b2)/4);
   currentHistoryItem = history.end();
   header = new QWidget(this, "_B_ALI_HEADER");
   header->installEventFilter(this);
   QHBoxLayout *headerLayout = new QHBoxLayout(header, 5, 3);
   userButton = new StartMenuButton(_size, "folder_home", QString(getenv("USER")), "~", StartMenuButton::Status, header, "_B_ALI_HEADER");
   headerLayout->addWidget(userButton);
   header->setPaletteBackgroundColor(KGlobalSettings::highlightColor());
   connect (userButton, SIGNAL(pressed(const QString &)), this, SLOT(execute(const QString &)));
   connect (userButton, SIGNAL(pressed(const QString &)), this, SLOT(close()));
   QBoxLayout *mainLayout = new QVBoxLayout();
   panelLayout->addLayout(mainLayout, 1, 1);
   mainLayout->addWidget(header);
   mainLayout->addSpacing ( 3 );
   QBoxLayout *centerLayout = new QHBoxLayout( mainLayout );
   centerLayout->addSpacing ( 3 );

   appList = new AppList(_size, this);
   appList->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
   appList->finish();

   searchLine = new SearchLine(header);
   categoryCombo = new QComboBox(header, "_B_ALI_HEADER");
   headerLayout->addWidget(categoryCombo);
   categoryCombo->setLineEdit ( searchLine );
   categoryCombo->insertStringList(appList->categories);
   categoryCombo->setEditable ( true );
   categoryCombo->setInsertionPolicy(QComboBox::NoInsertion);
   searchLine->installEventFilter(this);
   searchLine->setCompletionMode(KGlobalSettings::CompletionAuto);
   config->setGroup("Shortcuts");
   // read shortcuts...
   QStringList cuts = config->readListEntry ("Shortcuts", ',');
   QStringList cats = config->readListEntry ("Categories", ',');
   QStringList::Iterator it;
   QStringList::Iterator it2 = cats.begin();
   for ( it = cuts.begin(); it != cuts.end(); ++it )
   {
      if (it2 == cats.end()) break;
      shortcutList.insert(MyKey((*it)),(*it2));
      it2++;
   }
   //---
   QString PATH(getenv("PATH"));
   int n = PATH.contains(':', false);
   QStringList list;
   for (int i = 0; i < n; i++)
   {
      QDir execDir(PATH.section(':', i, i));
      list = execDir.entryList(QDir::Files | QDir::Executable, QDir::Name | QDir::IgnoreCase );
      searchLine->completionObject()->insertItems(list);
   }

   connect (categoryCombo, SIGNAL(activated( const QString &)), appList, SLOT(showCategory(const QString &)));
   connect (searchLine, SIGNAL(typedTextChanged(const QString &)), appList, SLOT(search(const QString &)));
   connect (searchLine, SIGNAL(textChanged(const QString &)), this, SLOT(endHistory()));
   connect (appList, SIGNAL(looseKey()), searchLine, SLOT(setFocus()));
   connect (appList, SIGNAL(looseKey()), searchLine, SLOT(selectAll() ));
   connect (kapp, SIGNAL(shutDown()), this, SLOT(save() ));
   if (useKTTS) connect (appList, SIGNAL(sayText(const QString&)), this, SLOT(sayText(const QString&) ));

   centerLayout->addWidget(appList,10);
   centerLayout->addSpacing ( 3 );
   
   m_panel = new Panel(_size, this, "_B_ALI_HEADER");
//    m_panel->installEventFilter(this);
   m_panel->setFixedHeight(_BIGSIZE_(_size)+4);
   
   statusBar = new KSqueezedTextLabel(this);
   QFont tmpFnt = statusBar->font();
   tmpFnt.setBold(true);
   statusBar->setFont(tmpFnt);
   connect (appList, SIGNAL(message(const QString&)), this, SLOT(message(const QString&) ));
   connect (appList, SIGNAL(clearStatus()), this, SLOT(clearStatus() ));
   connect (m_panel, SIGNAL(message(const QString&)), this, SLOT(centerMessage(const QString&) ));
   connect (m_panel, SIGNAL(clearStatus()), this, SLOT(clearStatus() ));
   
   mainLayout->addSpacing ( 3 );
   mainLayout->addWidget ( statusBar );
   panelLayout->addWidget ( m_panel, 2, 1 );
   m_panel->hide();
   
   if (config) { delete config; config = 0L; }
//    KDrawer *drawer = new KDrawer(this, KDrawer::Bottom);
}

void StartMenu::save()
{
   config = new KConfig("bStarter", false, false);
   config->setGroup("Shell");
   QStringList lst;
   for ( QStringList::Iterator it = history.begin(); it != history.end(); ++it )
      lst.prepend(*it);
   config->writeEntry("History", lst);
   config->setGroup("Settings");
   config->writeEntry("firstUse", false);
   appList->save(config);
   m_panel->save(config);
   delete config; config = 0L;
}
#if 0
extern int kicker_screen_number;

void StartMenu::slotLock()
{
   QCString appname( "kdesktop" );
//    if ( kicker_screen_number )
//       appname.sprintf("kdesktop-screen-%d", kicker_screen_number);
   kapp->dcopClient()->send(appname, "KScreensaverIface", "lock()", "");
}
#endif

void StartMenu::reloadIcons( int size)
{
   _size = size;
   m_panel->reloadIcons(size);
   appList->reloadIcons(size);
}

void StartMenu::setCategory(const QString & category)
{
   categoryCombo->setCurrentItem ( categoryCombo->listBox()->index(categoryCombo->listBox()->findItem(category, Qt::ExactMatch)));
   //emit categoryCombo->activated ( category ); // dunno if i'll need that once, needs protected access
   appList->showCategory(category);
}

void StartMenu::updateShortcuts(ShortcutList & list)
{
   shortcutList = list;
}

void StartMenu::toggleKTTS(bool on)
{
   useKTTS = on;
   if (useKTTS)
   {
      m_spokenText = 0;
      QPtrListIterator<StartMenuEntry> it(appList->entryList);
      StartMenuEntry *runner;
      while( (runner = it.current()) != 0 )
      {
         ++it;
         connect (runner, SIGNAL(sayText(const QString&)), this, SIGNAL(sayText(const QString&)));
      }
      connect (appList, SIGNAL(sayText(const QString&)), this, SLOT(sayText(const QString&) ));
   }
   else
   {
      QPtrListIterator<StartMenuEntry> it(appList->entryList);
      StartMenuEntry *runner;
      while( (runner = it.current()) != 0 )
      {
         ++it;
         disconnect (runner, SIGNAL(sayText(const QString&)), this, SIGNAL(sayText(const QString&)));
      }
      disconnect (appList, SIGNAL(sayText(const QString&)), this, SLOT(sayText(const QString&) ));
   }
}

SearchLine::SearchLine( QWidget * parent ) : KLineEdit(parent){blocked = false;};

void SearchLine::makeCompletion (const QString & string)
{
   if (blocked) {blocked = false; return;}
   emit typedTextChanged(string);
   KLineEdit::makeCompletion (string);
}

StartMenu::~StartMenu()
{
//    appList->save();
}

void StartMenu::sayText(const QString &text)
{
   // strip tags
   QString cleanText;
   bool copy = true;
   for (uint i = 0; i < text.length(); i++)
   {
      if (!copy && text[i] == '>') // end tag, set copy true and move on
      {
         copy = true;
         continue;
      }
      if (copy && text[i] == '<') // tag start, set copy false and move on
      {
         copy = false;
         continue;
      }
      if (copy) //copy char ;)
         cleanText += text[i];
   }
   // done
   QByteArray data1;
   QDataStream arg1(data1, IO_WriteOnly);
   arg1 << m_spokenText; // stop what we messaged before (if)
   if (!kapp->dcopClient()->send("kttsd", "kspeech", "stopText(uint)", data1))
      qDebug("there was some error using DCOP.");
   
   QByteArray data, replyData;
   QCString replyType;
   QDataStream arg(data, IO_WriteOnly);
   arg << cleanText << ""; // ask for the full list
   if (!kapp->dcopClient()->call("kttsd", "kspeech", "sayText(QString, QString)", data, replyType, replyData))
      qDebug("there was some error using DCOP.");
   else
   {
      QDataStream reply(replyData, IO_ReadOnly);
      if (replyType == "uint")
         reply >> m_spokenText;
      else
         qWarning("properties() returned an unexpected type of reply (%s)!",QString(replyType).ascii());
   }
}

void StartMenu::show()
{
   m_panel->setBackgroundOrigin(QWidget::WidgetOrigin);
   statusBar->setBackgroundOrigin(QWidget::ParentOrigin);
   searchLine->setText(i18n("Type to search or enter a command"));
   searchLine->selectAll();
   searchLine->setFocus();
//    KWin::setOpacity(winId(), 80);
   QWidget::show();
}

void StartMenu::hide()
{
   emit aboutToHide();
   searchLine->clear();
   appList->reset();
   QWidget::hide();
}

void StartMenu::message(const QString &text)
{
   statusBar->setAlignment ( Qt::AlignAuto | Qt::AlignVCenter );
   statusBar->setText(text);
}

void StartMenu::centerMessage(const QString &text)
{
   statusBar->setAlignment ( Qt::AlignCenter );
   statusBar->setText(text);
}

void StartMenu::clearStatus()
{
   statusBar->clear();
}

void StartMenu::setPanelPosition(PanelPosition p)
{
   if (p == m_panelPos)
      return;
   panelLayout->remove(m_panel);
   switch (p)
   {
   case StartMenu::South:
      m_panel->show();
      if (m_panelPos == StartMenu::Nowhere || m_panelPos == StartMenu::West || m_panelPos == StartMenu::East )
      {
//          m_panel->set2SizePolicies(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
         m_panel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
         m_panel->setOrientation ( Qt::Horizontal );
         m_panel->setFixedHeight(_BIGSIZE_(_size)+4);
         m_panel->setMaximumWidth(32767);
      }
      panelLayout->addWidget(m_panel, 2, 1);
      break;
   case StartMenu::West:
      m_panel->show();
      if (m_panelPos == StartMenu::Nowhere || m_panelPos == StartMenu::South || m_panelPos == StartMenu::North )
      {
//          m_panel->set2SizePolicies(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
         m_panel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
         m_panel->setOrientation ( Qt::Vertical );
         m_panel->setFixedWidth(_BIGSIZE_(_size)+4);
         m_panel->setMaximumHeight(32767);
      }
      panelLayout->addWidget(m_panel, 1, 0);
      break;
   case StartMenu::East:
      m_panel->show();
      if (m_panelPos == StartMenu::Nowhere || m_panelPos == StartMenu::South || m_panelPos == StartMenu::North )
      {
//          m_panel->set2SizePolicies(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
         m_panel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
         m_panel->setOrientation ( Qt::Vertical );
         m_panel->setFixedWidth(_BIGSIZE_(_size)+4);
         m_panel->setMaximumHeight(32767);
      }
      panelLayout->addWidget(m_panel, 1, 2);
      break;
   case StartMenu::North:
      m_panel->show();
      if (m_panelPos == StartMenu::Nowhere || m_panelPos == StartMenu::West || m_panelPos == StartMenu::East )
      {
//          m_panel->set2SizePolicies(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
         m_panel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
         m_panel->setOrientation ( Qt::Horizontal );
         m_panel->setFixedHeight(_BIGSIZE_(_size)+4);
         m_panel->setMaximumWidth(32767);
      }
      panelLayout->addWidget(m_panel, 0, 1);
      break;
   default:
      m_panel->hide();
//       break;
   }
   m_panelPos = p;
}

void StartMenu::execute(const QString& command)
// adapted from kicker run applet - far more convenient ;)
{
   history.remove(command);
   history.append(command); // all - the list is not stored and the user may want to easily correct mistypes of LOOOOOOOOOOOOOOOOOOOONG commands with a lot of "\ "... ;P
   QString exec;
   
   kapp->propagateSessionManager();
   
   _filterData->setData( command.stripWhiteSpace() );
   QStringList filters;
   filters << "kurisearchfilter" << "kshorturifilter";
   KURIFilter::self()->filterURI( *(_filterData), filters );
   
   QString cmd = (_filterData->uri().isLocalFile() ? _filterData->uri().path():_filterData->uri().url());
   
    // Nothing interesting. Quit!
   if ( cmd.isEmpty() )
   {
      return;
   }
   else if (cmd == "logout")
   {
      close();
      kapp->requestShutDown();
   }
   else
   {
      switch( _filterData->uriType() )
      {
      case KURIFilterData::LOCAL_FILE:
      case KURIFilterData::LOCAL_DIR:
      case KURIFilterData::NET_PROTOCOL:
      case KURIFilterData::HELP:
         {
            (void) new KRun( _filterData->uri() );
            return;
         }
      case KURIFilterData::EXECUTABLE:
      case KURIFilterData::SHELL:
         {
            exec = cmd;
            if( _filterData->hasArgsAndOptions() )
               cmd += _filterData->argsAndOptions();
            break;
         }
      case KURIFilterData::UNKNOWN:
      case KURIFilterData::ERROR:
      default:
         return;
      }
   }
   KRun::runCommand( cmd, exec, "" );
   return;
}


void StartMenu::endHistory()
{
   currentHistoryItem = history.end();
}

void StartMenu::search(const QString & string)
{
   disconnect (searchLine, SIGNAL(textChanged ( const QString & )), this, SLOT(search(const QString &)));
   appList->search(string);
}

bool StartMenu::eventFilter ( QObject * o, QEvent * e )
{
   if (o == header)
   {
      if (e->type() == QEvent::MouseButtonPress && ((QMouseEvent*)e)->button() ==  Qt::LeftButton)
      {
         inMove = true;
         movePoint = ((QMouseEvent*)e)->pos();
         header->grabMouse(Qt::SizeAllCursor);
         return true;
      }
      else if (e->type() == QEvent::MouseButtonRelease && ((QMouseEvent*)e)->button() ==  Qt::LeftButton)
      {
         inMove = false;
         header->releaseMouse();
         return true;
      }
      else if (e->type() == QEvent::MouseMove && inMove)
      {
         move(((QMouseEvent*)e)->globalPos() - movePoint);
         return true;
      }
      else if (e->type() == QEvent::Resize && ((QResizeEvent*)e)->size().height() != ((QResizeEvent*)e)->oldSize().height())
      {
         int height = ((QResizeEvent*)e)->size().height();
         KPixmap bgPix = QPixmap(32, height);
         KPixmap bgPix1 = QPixmap(32, height/2);
         KPixmap bgPix2 = QPixmap(32, height - bgPix1.height());
         QColor buttonColor = ((QWidget*)o)->palette().color(QPalette::Active, QColorGroup::Button);
         KPixmapEffect::gradient( bgPix1, buttonColor.light(130), buttonColor, KPixmapEffect::VerticalGradient, 0);
         KPixmapEffect::gradient( bgPix2, buttonColor.dark(120), buttonColor.light(110), KPixmapEffect::VerticalGradient, 0);
         QPainter p(&bgPix);
         p.drawPixmap(0,0,bgPix1);
         p.drawPixmap(0,bgPix1.height(),bgPix2);
         p.end();
         ((QWidget*)o)->setPaletteBackgroundPixmap( bgPix );
      }
      return false;
   }
   if (o != searchLine)
      return false;
   QLineEdit *le = (QLineEdit *)o;
   if (e->type() == QEvent::KeyPress)
   {
      QKeyEvent *ke = (QKeyEvent*)e;
      if (ke->state() & Qt::ControlButton || ke->state() & Qt::AltButton)
         // might be shortcut, set category
      {
         if (ke->state() == Qt::ControlButton)
         {
            if (ke->key() == Qt::Key_Up)
            {
               if (categoryCombo->currentItem() > 0)
               {
                  categoryCombo->setCurrentItem ( categoryCombo->currentItem() - 1 );
                  appList->showCategory(categoryCombo->currentText());
               }
               return true;
            }
            if (ke->key() == Qt::Key_Down)
            {
               if (categoryCombo->currentItem() < categoryCombo->count())
               {
                  categoryCombo->setCurrentItem ( categoryCombo->currentItem() + 1 );
                  appList->showCategory(categoryCombo->currentText());
               }
               return true;
            }
         }
         if (ke->key() == Qt::Key_Shift || ke->key() == Qt::Key_Control || ke->key() == Qt::Key_Alt)
            return false;
         ShortcutList::Iterator it;
         for ( it = shortcutList.begin(); it != shortcutList.end(); ++it )
         {
            if (it.key().modFlags() == ke->state() && it.key().key() == ke->key())
               setCategory( it.data() );
         }
         return true; //fire event to prevent lienedit action like ctrl+d -> del etc.
      }
      switch(ke->key())
      {
      case Qt::Key_Return:
      case Qt::Key_Enter:
      {
         execute(le->text());
         if (!(ke->state() & Qt::ControlButton))
            close(); //bye
         return true; //don't do anything else
      }
      case Qt::Key_Backspace:
      case Qt::Key_Delete:
      {
         connect (le, SIGNAL(textChanged ( const QString & )), this, SLOT(search(const QString &)));
         break;
      }
      case Qt::Key_Down:
      {
         if (history.isEmpty() || currentHistoryItem == history.end())
         {
            appList->appDown();
         }
         else // navigate history
         {
            le->blockSignals(true);
            le->setText(*currentHistoryItem);
            le->blockSignals(false);
            currentHistoryItem++;
            if (currentHistoryItem == history.end())
               le->selectAll();
         }
         return true; //don't scroll the categories
      }
      case Qt::Key_Up:
      {
         if (!(history.isEmpty() || currentHistoryItem == history.begin()))
         {
            currentHistoryItem--;
            le->blockSignals(true);
            le->setText(*currentHistoryItem);
            le->blockSignals(false);
         }
         return true; //don't scroll the categories
      }
      case Qt::Key_Escape:
      {
         close(); //bye
         break;
      }
      default:
         break;
      }
      return false;
   }
   else if (isVisible() && useKTTS && e->type() == QEvent::FocusIn)
   {
      QString text = i18n("TTS output", "The searchline has now the focus.");
      sayText(text);
   }
   return false;
}
