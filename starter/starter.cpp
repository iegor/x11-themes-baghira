
#include <qcombobox.h>
#include <qcursor.h>
#include <qdesktopwidget.h>
#include <kglobal.h>
#include <klocale.h>
#include <kconfig.h>
#include <kmessagebox.h>
#include <kapplication.h>
# include <kpopupmenu.h>
#include <qimage.h>
#include <qfile.h>
#include <qlabel.h>
#include <qradiobutton.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qtimer.h>
#include <kiconloader.h>
#include <dcopclient.h>
#include <kdebug.h>
#include <kkeybutton.h>
#include <kurlrequester.h>
#include "starter.h"
#include "starterconfig.h"
#include "starterhelp.h"
#include "menu.h"

#define _SIZE2VALUE_(_s_) (_s_ == 16) ? 0 :\
(_s_ == 22) ? 1 :\
(_s_ == 32) ? 2 :\
(_s_ == 48) ? 3 : 4

#define _VALUE2SIZE_(_v_) (_v_ == 0) ? 16 :\
(_v_ == 1) ? 22 :\
(_v_ == 2) ? 32 :\
(_v_ == 3) ? 48 : 64

starter::starter(const QString& configFile, Type type, int actions, QWidget *parent, const char *name)
: DCOPObject("StarterIface"), KPanelApplet(configFile, type, actions, parent, name)
{
   configPopup = new KPopupMenu(this);
   popupBlocked = false;
   mainView = new QLabel(this);

   configDialog = new StarterConfig(this);
   QRect desktop = QDesktopWidget().availableGeometry( configDialog );
   configDialog->move((desktop.width() - configDialog->width())/2, (desktop.height() - configDialog->height())/2);
   KConfig config("bStarter", false, false);
   config.setGroup("Settings");
   configDialog->buttonShortcut->setEnabled(false);
   configDialog->BaseURL->setURL(config.readEntry("BaseImage", "" ));
   configDialog->HoverURL->setURL(config.readEntry("HoverImage", "" ));
   configDialog->DownURL->setURL(config.readEntry("DownImage", "" ));
   configDialog->useKTTS->setChecked(config.readBoolEntry("useKTTS", false ));
   
   configDialog->customPopupSize->setChecked(config.readBoolEntry("CustomPopupSize", false ));
   configDialog->customDialogSize->setChecked(config.readBoolEntry("CustomDialogSize", false ));
   configDialog->customDialogPos->setChecked(config.readBoolEntry("CustomDialogPos", false ));
   configDialog->fixedDialogPos->setChecked(config.readBoolEntry("FixedDialogPos", false ));
   configDialog->dialogFollowMouse->setChecked(config.readBoolEntry("DialogFollowMouse", false ));
   configDialog->showDialogTitlebar->setChecked(config.readBoolEntry("ShowDialogTitlebar", false ));
   configDialog->popupW->setValue(config.readNumEntry("PopupW", 0 ));
   configDialog->popupH->setValue(config.readNumEntry("PopupH", 0 ));
   configDialog->dialogW->setValue(config.readNumEntry("DialogW", 0 ));
   configDialog->dialogH->setValue(config.readNumEntry("DialogH", 0 ));
   configDialog->dialogX->setValue(config.readNumEntry("DialogX", 0 ));
   configDialog->dialogY->setValue(config.readNumEntry("DialogY", 0 ));
   configDialog->favItemAmount->setValue(config.readNumEntry("FavItemAmount", 10 ));
   configDialog->dialogPanelPos->setCurrentItem(config.readNumEntry("DialogPanelPos", 1 ));
   configDialog->popupPanelPos->setCurrentItem(config.readNumEntry("PopupPanelPos", 2 ));
   _iconSize = config.readNumEntry("IconSize", 32 );
   configDialog->iconSize->setCurrentItem(_SIZE2VALUE_(_iconSize));
   
   startMenu = new StartMenu(_iconSize, this, Qt::WType_Popup);
   shortcutList = startMenu->shortcutList;
   configDialog->categoryList->insertStringList(startMenu->categories());
   connect (startMenu, SIGNAL(aboutToHide()), this, SLOT(unblockPopupDelayed()));
   connect(configDialog->useKTTS, SIGNAL(toggled( bool )), startMenu, SLOT(toggleKTTS(bool)));
   
   //---
   connect(configDialog->categoryList, SIGNAL(highlighted(int)), this, SLOT(activateShortcutButton(int)));
   connect(configDialog->buttonShortcut, SIGNAL(capturedShortcut (const KShortcut &)), this, SLOT(addShortcut(const KShortcut &)));
   connect(configDialog->categoryList, SIGNAL(highlighted ( const QString & )), this, SLOT(updateShortcutButton(const QString &)));
   connect(configDialog->buttonOk, SIGNAL(clicked()), this, SLOT(updateSettings()));
   StarterHelp *helpDialog = new StarterHelp(configDialog);
   connect(configDialog->buttonHelp, SIGNAL(clicked()), helpDialog, SLOT(show()));
   //-----
   configPopup->insertItem(i18n("Configure the Startmenu"), configDialog, SLOT(show()));
   configDialog->BaseURL->setFilter( "*.png" );
   configDialog->HoverURL->setFilter( "*.png" );
   configDialog->DownURL->setFilter( "*.png" );
   mainView->move(0,0);
   if (parent) move(parent->x(),parent->y());
   mainView->installEventFilter(this);
   reloadImages();
   mainView->setPixmap(pixmap);
   mainView->show();
}

void starter::activateShortcutButton(int i)
{
   configDialog->buttonShortcut->setEnabled(bool(i));
}

void starter::addShortcut(const KShortcut &cut)
{
   // in case of empty shortcut, remove the entry from the list and return
   if (!short(cut.keyCodeQt()))
   {
      ShortcutList::Iterator it;
      for ( it = shortcutList.begin(); it != shortcutList.end(); ++it )
         if (it.data() == configDialog->categoryList->currentText())
         {
            shortcutList.remove(it);
            break;
         }
      configDialog->buttonShortcut->setShortcut(KShortcut::null(), false);
      return;
   }
   // generate MyKey
   short state = 0;
   if (cut.seq(0).key(0).modFlags() & KKey::CTRL)
      state |= Qt::ControlButton;
   if (cut.seq(0).key(0).modFlags() & KKey::ALT)
      state |= Qt::AltButton;
   if (cut.seq(0).key(0).modFlags() & KKey::SHIFT)
      state |= Qt::ShiftButton;
   MyKey key(cut.seq(0).keyCodeQt(), state);
   // Test if this is a valid shotrcut, i.e. contains 'ctrl' or 'alt', returns iff not
   if (!(state & Qt::ControlButton || state & Qt::AltButton))
   {
      KMessageBox::sorry(this, i18n("<qt>To ensure usefull behaviour of the searchline, the shortcut <b>must contain</b> a metabutton, i.e. <b>'ctrl' and/or 'alt'</b></qt>"), i18n("Sorry, invalid Shortcut"));
      return;
   }
   // test if the cut was allready bound to another category and ask the user whta to do (return iff not rebind)
   ShortcutList::Iterator it = shortcutList.find(key);
   if ((it != shortcutList.end() && KMessageBox::questionYesNo(this, i18n("<qt>The selected shortcut is allready bound to the category \"%1\".<br>Do you want to <b>rebind</b> it?</qt>").arg(it.data()), i18n("Rebind Shortcut?")) == KMessageBox::No))
      return;
   // if rebind (it is not end and we did not return ;) remove the old shortcut
   if (it != shortcutList.end())
   {
      shortcutList.remove(it);
   }
   // test if another shortcut is bound to this category and remove it in case
   for ( it = shortcutList.begin(); it != shortcutList.end(); ++it )
      if (it.data() == configDialog->categoryList->currentText())
      {
         shortcutList.remove(it);
         break;
      }
   // add new shortcut/category map entry
   shortcutList[key] = configDialog->categoryList->currentText();
   // update UI
   configDialog->buttonShortcut->setShortcut(cut, false);
}

void starter::updateShortcutButton(const QString & category)
{
   ShortcutList::Iterator it;
   for ( it = shortcutList.begin(); it != shortcutList.end(); ++it )
      if (it.data() == category)
      {
         QKeyEvent qke( QEvent::KeyPress, it.key().key(), 0, it.key().modFlags());
         KKey kkey(&qke);
         KShortcut ksc(kkey);
         configDialog->buttonShortcut->setShortcut(ksc, false);
         return;
      }
   configDialog->buttonShortcut->setShortcut(KShortcut::null(), false);
}

starter::~starter()
{
}

void starter::updateSettings()
{
   startMenu->updateShortcuts(shortcutList);
   KConfig *config = new KConfig("bStarter", false, false);
   config->setGroup("Settings");
   config->writeEntry("BaseImage", configDialog->BaseURL->url());
   config->writeEntry("HoverImage", configDialog->HoverURL->url());
   config->writeEntry("DownImage", configDialog->DownURL->url());
   config->writeEntry("useKTTS", configDialog->useKTTS->isChecked());
   config->writeEntry("CustomPopupSize",  configDialog->customPopupSize->isChecked());
   config->writeEntry("CustomDialogSize", configDialog->customDialogSize->isChecked());
   config->writeEntry("CustomDialogPos", configDialog->customDialogPos->isChecked());
   config->writeEntry("FixedDialogPos", configDialog->fixedDialogPos->isChecked());
   config->writeEntry("DialogFollowMouse", configDialog->dialogFollowMouse->isChecked());
   config->writeEntry("ShowDialogTitlebar", configDialog->showDialogTitlebar->isChecked());
   config->writeEntry("PopupW", configDialog->popupW->value());
   config->writeEntry("PopupH", configDialog->popupH->value());
   config->writeEntry("DialogW", configDialog->dialogW->value());
   config->writeEntry("DialogH", configDialog->dialogH->value());
   config->writeEntry("DialogX", configDialog->dialogX->value());
   config->writeEntry("DialogY", configDialog->dialogY->value());
   config->writeEntry("FavItemAmount", configDialog->favItemAmount->value());
   config->writeEntry("DialogPanelPos", configDialog->dialogPanelPos->currentItem());
   config->writeEntry("PopupPanelPos", configDialog->popupPanelPos->currentItem());
   if (_iconSize != (_VALUE2SIZE_(configDialog->iconSize->currentItem())))
   {
      _iconSize = _VALUE2SIZE_(configDialog->iconSize->currentItem());
      config->writeEntry("IconSize", _iconSize);
      startMenu->reloadIcons(_iconSize);
   }
   startMenu->setFavItemAmount(configDialog->favItemAmount->value());
   config->setGroup("Shortcuts");
   QStringList cuts; QStringList cats;
   ShortcutList::Iterator it;
   for ( it = shortcutList.begin(); it != shortcutList.end(); ++it )
   {
      cuts.append(it.key().toString());
      cats.append(it.data());
   }
   config->writeEntry("Shortcuts", cuts, ',');
   config->writeEntry("Categories", cats, ',');
   reloadImages();
   delete config;
}

#define _VALID_(_url_) configDialog && !configDialog->_url_->url().isEmpty() && QFile::exists(configDialog->_url_->url())

void starter::reloadImages()
{
   KIconLoader* iLoader = KGlobal::iconLoader();
   QString pth;
   if (_VALID_(BaseURL))
      pth = configDialog->BaseURL->url();
   else
      pth = iLoader->iconPath("bStarter", KIcon::Small, true);
   if (pth)
      pixmap = QImage(pth);
   if (!pth || pixmap.isNull())
   {
      pixmap = QPixmap(22,22);
      pixmap.fill(Qt::black);
   }
   pth = QString();
   if (_VALID_(HoverURL))
      pth = configDialog->HoverURL->url();
   else
      pth = iLoader->iconPath("bStarter_hover", KIcon::Small, true);
   if (pth)
      hoverPixmap = QImage(pth);
   if (!pth || hoverPixmap.isNull())
   {
      hoverPixmap = QPixmap(22,22);
      hoverPixmap.fill(Qt::black);
   }
   pth = QString();
   if (_VALID_(DownURL))
      pth = configDialog->DownURL->url();
   else
      pth = iLoader->iconPath("bStarter_down", KIcon::Small, true);
   if (pth)
      downPixmap = QImage(pth);
   if (!pth || downPixmap.isNull())
   {
      downPixmap = QPixmap(22,22);
      downPixmap.fill(Qt::white);
   }
   int wd = pixmap.width();
   int ht = pixmap.height();
   if (wd < hoverPixmap.width()) wd = hoverPixmap.width();
   if (wd < downPixmap.width()) wd = downPixmap.width();
   if (ht < hoverPixmap.height()) ht = hoverPixmap.height();
   if (ht < downPixmap.height()) ht = downPixmap.height();
   mainView->setFixedSize(wd,ht);
   repaint();
}

void starter::resizeEvent ( QResizeEvent *rev )
{
    pixmap = pixmap.convertToImage().smoothScale(rev->size().height()*pixmap.width()/pixmap.height(),rev->size().height());
    downPixmap = downPixmap.convertToImage().smoothScale(rev->size().height()*downPixmap.width()/downPixmap.height(),rev->size().height());
    hoverPixmap = hoverPixmap.convertToImage().smoothScale(rev->size().height()*hoverPixmap.width()/hoverPixmap.height(),rev->size().height());
    mainView->setFixedSize(rev->size().height()*mainView->width()/mainView->height(),rev->size().height());
    mainView->setPixmap(pixmap);
    KPanelApplet::resizeEvent(rev);
}

void starter::configureMenu()
{
   KApplication::startServiceByDesktopName("kmenuedit", QStringList(), 0, 0, 0, "", true);
}

void starter::preferences()
{
   KApplication::startServiceByDesktopName("kmenuedit", QStringList(), 0, 0, 0, "", true);
}

int starter::widthForHeight(int height) const
{
    return mainView->width();
}

int starter::heightForWidth(int width) const
{
    return mainView->height();
}

void starter::unblockPopupDelayed()
{
   popupBlocked = true;
   QTimer::singleShot ( 50, this, SLOT(unblockPopup()) );
}

void starter::unblockPopup()
{
   popupBlocked = false;
   if (mainView->hasMouse())
      mainView->setPixmap(hoverPixmap);
   else
      mainView->setPixmap(startMenu->isShown() ? downPixmap : pixmap);
   mainView->repaint();
}

#define _MAX_(a,b) (a > b ? a : b)
void starter::popupMenu()
{
   if (popupBlocked)
      return;
   {
//       if (!isDialog_)
//          return;
      startMenu->setMinimumSize ( 0, 0 );
      startMenu->setMaximumSize ( 32767, 32767 );
      setActiveWindow();
      startMenu->setPanelPosition((StartMenu::PanelPosition)configDialog->popupPanelPos->currentItem());
      if (configDialog->customPopupSize->isChecked())
      {
         startMenu->resize(configDialog->popupW->value(),configDialog->popupH->value());
         startMenu->setFixedSize(startMenu->size());
//          startMenu->setFixedSize(_MAX_(configDialog->popupW->value(), startMenu->minimumWidth()), _MAX_(configDialog->popupH->value(), startMenu->minimumHeight()));
      }
      else
      {
         QRect desktop = QDesktopWidget().availableGeometry( startMenu );
         startMenu->resize(desktop.width()/5, 2*desktop.height()/3);
         startMenu->setFixedSize(startMenu->size());
//          startMenu->setFixedSize(_MAX_(startMenu->minimumWidth(), desktop.width()/5) ,_MAX_(startMenu->minimumHeight(), 2*desktop.height()/3)); // 1/5 screen width, 2/3 screen height
      }
      
      QPoint pt = mapToGlobal(pos());
      QRect desktop = QDesktopWidget().availableGeometry( startMenu );
      int x = pt.x();
      int y = pt.y();
      
      switch(position())
      {
      case pTop:
         y += height() + 1;
         if (x + startMenu->width() > desktop.right())
            x =  desktop.right() - startMenu->width();
         if (x < 0) x = 0;
         break;
      case pLeft:
         x += width() + 1;
         if (y + startMenu->height() > desktop.bottom())
            y =  desktop.bottom() - startMenu->height();
         if (y < 0) y = 0;
         break;
      case pBottom:
         y -= startMenu->height() - 1;
         if (x + startMenu->width() > desktop.right())
            x =  desktop.right() - startMenu->width();
         if (x < 0) x = 0;
         break;
      case pRight:
         x -= startMenu->width() - 1;
         if (y + startMenu->height() > desktop.bottom())
            y =  desktop.bottom() - startMenu->height();
         if (y < 0) y = 0;
      }
      pt = QPoint(x, y);
      
      startMenu->reparent(this, Qt::WType_Popup, pt, true);
   }
}

void starter::showMenu()
{
   startMenu->setMinimumSize ( 0, 0 );
   startMenu->setMaximumSize ( 32767, 32767 );
   startMenu->setPanelPosition((StartMenu::PanelPosition)configDialog->dialogPanelPos->currentItem());
   if (configDialog->customDialogSize->isChecked())
   {
      startMenu->resize(configDialog->dialogW->value(), configDialog->dialogH->value());
      startMenu->setFixedSize(startMenu->size());
//       startMenu->setFixedSize(_MAX_(configDialog->dialogW->value(), startMenu->minimumWidth()), _MAX_(startMenu->minimumHeight(), configDialog->dialogH->value()));
   }
   else
   {
      QRect desktop = QDesktopWidget().availableGeometry( startMenu );
//    setActiveWindow();
      startMenu->resize(desktop.width()/2, desktop.width()*9/32);
      startMenu->setFixedSize(startMenu->size());
//       startMenu->setFixedSize(_MAX_(startMenu->minimumWidth(), desktop.width()/2) , _MAX_(startMenu->minimumHeight(), desktop.width()*9/32)); // 16:9 window, width == 1/2 screen
   }
   QPoint pt;
   if (configDialog->customDialogPos->isChecked())
   {
      if (configDialog->dialogFollowMouse->isChecked())
      {
         QRect desktop = QDesktopWidget().availableGeometry( startMenu );
         int x,y;
         x = QCursor::pos().x() + startMenu->width()/2 < desktop.width() ? QCursor::pos().x() - startMenu->width()/2 : desktop.width() - startMenu->width();
         if (x < 0) x = 0;
         y = QCursor::pos().y() + startMenu->height()/2 < desktop.height() ? QCursor::pos().y() - startMenu->height()/2 : desktop.height() - startMenu->height();
         if (y < 0) y = 0;
         pt = QPoint(x, y);
      }
      else
         pt = QPoint(configDialog->dialogX->value(), configDialog->dialogY->value());
   }
   else
   {
      QRect desktop = QDesktopWidget().availableGeometry( startMenu );
      pt = QPoint((desktop.right() - startMenu->width())/2, (desktop.bottom() - startMenu->height())/2);
   }
   if (configDialog->showDialogTitlebar->isChecked())
      startMenu->reparent(this, Qt::WType_TopLevel, pt, true);
   else
      startMenu->reparent(this, Qt::WType_TopLevel | Qt::WStyle_Customize | Qt::WStyle_NoBorder, pt, true);
}

bool starter::eventFilter( QObject*, QEvent *e )
{
   switch (e->type())
   {
      case QEvent::Enter:
      {
         mainView->setPixmap(hoverPixmap);
         mainView->repaint();
         return TRUE;
      }
      case QEvent::Leave:
      {
         mainView->setPixmap(startMenu->isShown() ? downPixmap : pixmap);
         mainView->repaint();
         return TRUE;
      }
      case QEvent::MouseButtonPress:
      {
         if (((QMouseEvent*)e)->button() == Qt::RightButton)
         {
            configPopup->popup(((QMouseEvent*)e)->globalPos());
            return TRUE;
         }
         if (((QMouseEvent*)e)->button() == Qt::LeftButton)
         {
            mainView->setPixmap(downPixmap);
            mainView->repaint();
            popupMenu();
         }
         return TRUE;
      }
      case QEvent::MouseButtonRelease:
      {
         if (((QMouseEvent*)e)->button() != Qt::LeftButton)
               return FALSE;
         if (mainView->hasMouse())
            mainView->setPixmap(hoverPixmap);
         else
            mainView->setPixmap(startMenu->isShown() ? downPixmap : pixmap);
         mainView->repaint();
         return TRUE;
      }
      default:
         return FALSE;
   }
}

extern "C"
{
    KPanelApplet* init( QWidget *parent, const QString& configFile)
    {
       KGlobal::locale()->insertCatalogue("starter");
       return new starter(configFile, KPanelApplet::Normal,
                                 KPanelApplet::Preferences,
                                 parent, "baghirastarter");
    }
}
