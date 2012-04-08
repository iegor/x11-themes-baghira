#include <qcursor.h>
#include <kpixmapeffect.h> 
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include <fixx11h.h>

bool LiquidStyle::eventFilter( QObject *obj, QEvent *ev )
{
//   qWarning("%s, %s <- %s, %s: %d", obj->className(), obj->name(), obj->parent() ? obj->parent()->className() : "none", obj->parent()?obj->parent()->name():"none",ev->type());
   switch (ev->type())
   {
      case QEvent::None:
      case QEvent::Timer:
      {
         // first check if this is one of our fading pushbuttons - return elsewise
         if (!(optionHandler->AnimateButton() && ::qt_cast<QPushButton*>(obj)))
            return false;
         ButtonFadeInfo *bfi_ = bfi[((QPushButton*)obj)->winId()];
         if (!bfi_ || ((QTimerEvent*)ev)->timerId() != bfi_->timerId)
            return false;
         // ok, this is one of our fading buttons asking for repaint
         ((QPushButton*)obj)->repaint(false);
         int index = bfi_->fadeIn?bfi_->index + 1:bfi_->index - 1;
         // handle border cases
         if (index < 0) // fade-out run has finished, either stop fading stuff, or start fade-in
         {
            bfi_->fadeIn=true;
            if (((QPushButton*)obj)->isDefault())
            {
               bfi_->index=0;
            }
            else
            {
               obj->killTimer(bfi_->timerId);
               // qWarning("%u: stopped (%d)", ((QPushButton*)obj)->winId(), bfi_->timerId);
               bfi_->timerId=0;
               ((QPushButton*)obj)->repaint(false); // must be to clean out button
            }
         }
         else if (index > 9) // fade-in run has finished, either stop fading stuff, or start fade-out
         {
            if (((QPushButton*)obj)->isDefault())
            {
               bfi_->fadeIn=false;
               bfi_->index=9;
            }
            else
            {
               obj->killTimer(bfi_->timerId);
               // qWarning("%u: stopped (%d)", ((QPushButton*)obj)->winId(), bfi_->timerId);
               bfi_->timerId=0;
            }
         }
         else
            bfi_->index = index;
      }
      case QEvent::MouseMove:
         if (::qt_cast<QTabBar*>(obj))
         {
            QTabBar *tb = (QTabBar*)obj;
            QRect tmpRect;
            bool done = false;
            QMouseEvent *me = ( QMouseEvent*)ev;
            int oldTab = hoveredTabIndex;
            if (hoveredTabIndex == -1)
            {
               for (int i = 0; i < tb->count(); i++)
               {
                  if (tb->tabAt(i))
                  {
                     tmpRect = tb->tabAt(i)->rect();
                     if ( tmpRect.x() < me->x() && me->x() < tmpRect.right())
                     {
                        hoveredTabIndex = i;
                        break;
                     }
                  }
               }
            }
            else
            {
               if ((oldTab > 0) && tb->tabAt(oldTab-1))
               {
                  tmpRect = tb->tabAt(oldTab-1)->rect();
                  if ((tmpRect.x() < me->x()) && (me->x() < tmpRect.right()))
                  {
                     hoveredTabIndex = oldTab-1;
                     done = true;
                  }
               }
               if (!done && (oldTab < tb->count()-1) && tb->tabAt(oldTab+1))
               {
                  tmpRect = tb->tabAt(oldTab+1)->rect();
                  if ((tmpRect.x() < me->x()) && (me->x() < tmpRect.right()))
                  {
                     hoveredTabIndex = oldTab+1;
                  }
               }
            }
            if ( oldTab != hoveredTabIndex )
            {
               // reset old tab
               if ( oldTab != -1 )
                  if (tb->tabAt(oldTab)) tb->repaint( tb->tabAt(oldTab)->rect(), false );
               if ( hoveredTabIndex != -1 )
                  tb->repaint( tmpRect, false );
            }
            return false;
         }
         else if ( ::qt_cast<QHeader*>(obj) )
         {
            QHeader *hw = (QHeader*)obj;
            QMouseEvent * me = ( QMouseEvent * ) ev;
            int oldHeader = headerHoverID;
            headerHoverID = hw->orientation() == Qt::Horizontal ? hw->sectionAt( me->x() + hw->offset() ) : hw->sectionAt( me->y() + hw->offset() );
            if ( oldHeader != headerHoverID )
            {
               // reset old header
               if ( oldHeader != -1 )
                  hw->repaint(hw->sectionRect ( oldHeader ), false);
//                   hw->repaint( hw->sectionPos( oldHeader )-hw->offset(), 0, hw->sectionSize( oldHeader ), hw->height(), false );
               if ( headerHoverID != -1 )
                  hw->repaint(hw->sectionRect ( headerHoverID ), false);
//                   hw->repaint( hw->sectionPos( headerHoverID )-hw->offset(), 0, hw->sectionSize( headerHoverID ), hw->height(), false );
            }
            return false;
         }
         return false;
      case QEvent::Paint:
         if ( ::qt_cast<QToolButton*>(obj) )
         {
            QToolButton * btn = ( QToolButton * ) obj;
            if (!obj->inherits( "TaskContainer" ))
            {
               if (btn->backgroundOrigin() != QWidget::WindowOrigin)
                  btn->setBackgroundOrigin( QWidget::WindowOrigin );
            }
            else
            {
               isTaskContainer = true;
               if ( currentTaskContainer == btn )
                  taskContainerHover = true;
               else
                  taskContainerHover = false;
            }
            return false;
         }
         else if (::qt_cast<QMenuBar*>(obj))
         {
            QWidget *w = (QWidget *)obj;
            if(w->backgroundMode() != Qt::PaletteBackground)
            {
               w->setBackgroundMode(PaletteBackground);
               w->setBackgroundOrigin(QWidget::WindowOrigin);
            }
            return false;
         }
         else if ( ::qt_cast<QHeader*>(obj) )
         {
            QHeader *hw = (QHeader*)obj;
            if (hw->parentWidget() && ::qt_cast<QListView*>(hw->parentWidget()))
               headerSortID = ((QListView*)hw->parentWidget())->sortColumn();
            return false;
         }
         else if ( obj->inherits( "QDockWindow" ) )
         {
            if (KStyle::eventFilter( obj, ev ))
               return true;
            QWidget * w = ( QWidget * ) obj;
            if ( w->backgroundMode() != Qt::PaletteBackground )
            {
               w->setBackgroundMode( Qt::PaletteBackground );
               w->setBackgroundOrigin( QWidget::WindowOrigin );
            }
            return false;
         }
         else if ( obj->inherits( "QRadioButton" ) || obj->inherits( "QCheckBox" ) )
         {
            QButton * btn = ( QButton * ) obj;
            bool isStatusChild = btn->parent() && ( btn->parent() ->inherits( "QStatusBar" ) || btn->parent() ->inherits( "KonqFrameStatusBar" ) );
            bool isRadio = obj->inherits( "QRadioButton" );
            btn->erase();
            QPainter p;
            p.begin( btn );
            QFontMetrics fm = btn->fontMetrics();
            QSize lsz = fm.size( ShowPrefix, btn->text() );
            QSize sz = isRadio ?
            QSize( pixelMetric( PM_ExclusiveIndicatorWidth ), pixelMetric( PM_ExclusiveIndicatorHeight ) ) :
            QSize( pixelMetric( PM_IndicatorWidth ), pixelMetric( PM_IndicatorHeight ) );
            int x = 0;
            int y = isStatusChild ? 0 : ( btn->height() - lsz.height() + fm.height() - sz.height() ) / 2;
            SFlags flags = Style_Default;
            if ( btn->isEnabled() )
               flags |= Style_Enabled;
            if ( btn->hasFocus() )
               flags |= Style_HasFocus;
            if ( btn->isDown() )
               flags |= Style_Down;
            if ( highlightWidget == btn )
               flags |= Style_MouseOver;
            if ( btn->state() == QButton::On )
               flags |= Style_On;
            else if ( btn->state() == QButton::Off )
               flags |= Style_Off;
            if ( isRadio )
               drawControl( CE_RadioButton, &p, btn, QRect( x, y, sz.width(), sz.height() ), btn->colorGroup(), flags );
            else
               drawControl( CE_CheckBox, &p, btn, QRect( x, y, sz.width(), sz.height() ), btn->colorGroup(), flags );
            x = sz.width() + 6;
            y = 0;
            if ( btn->hasFocus() )
            {
               QColor tmp( btn->colorGroup().background().dark( 130 ) );
               drawItem( &p, QRect( sz.width() + 6 + 2, 1, btn->width() - ( sz.width() + 6 + 1 ), btn->height() ), AlignLeft | AlignVCenter | ShowPrefix, btn->colorGroup(), btn->isEnabled(), btn->pixmap(), btn->text(), -1, &tmp );
               drawItem( &p, QRect( sz.width() + 6 + 3, 2, btn->width() - ( sz.width() + 6 + 1 ), btn->height() ), AlignLeft | AlignVCenter | ShowPrefix, btn->colorGroup(), btn->isEnabled(), btn->pixmap(), btn->text(), -1, &tmp );
            }
            drawItem( &p, QRect( sz.width() + 6 + 1, 0, btn->width() - ( sz.width() + 6 + 1 ), btn->height() ), AlignLeft | AlignVCenter | ShowPrefix, btn->colorGroup(), btn->isEnabled(), btn->pixmap(), btn->text() );
            p.end();
            return true;
         }
         else if (obj->inherits("KColorGrid") && !(((QWidget*)obj)->width() && ((QWidget*)obj)->height()))
            return true;
         return false;
      case QEvent::KeyPress:
      case QEvent::KeyRelease:
         return false;
      case QEvent::Wheel:
         if ( ::qt_cast<QScrollView*>(obj) )
         {
            optionHandler->setScrollerActive();
            return false;
         }
         return false;
      case QEvent::Enter:
         if ( ::qt_cast<QToolButton*>(obj) )
         {
            bool isTask = obj->inherits( "TaskContainer" );
            QToolButton * btn = ( QToolButton * ) obj;
            if (isTask)
            {
               currentTaskContainer = btn;
               btn->repaint( false );
               return false;
            }
            else if ( btn->isEnabled() )
            {
               highlightWidget = btn;
               btn->repaint( false );
               return false;
            }
         }
         else if ( ::qt_cast<QPushButton*>(obj) || ::qt_cast<QComboBox*>(obj) || obj->inherits( "QSplitterHandle" ) || obj->inherits( "AppletHandleDrag" ) )
         {
            QWidget * btn = ( QWidget * ) obj;
            if ( btn->isEnabled() )
            {
               highlightWidget = btn;
               btn->repaint( false );
               if (optionHandler->AnimateButton() && ::qt_cast<QPushButton*>(obj))
                  fadeIn((QPushButton*)obj);
            }
            return false;
         }
         else if (::qt_cast<KToolBar*>(obj))
         {
            QObjectList *handlerList = obj->queryList( "QDockWindowHandle" );
            for (uint i= 0; i < handlerList->count(); i++)
               ((QWidget*)(handlerList->at(i)))->repaint(false);
            return false;
         }
         if ( ::qt_cast<QScrollBar*>(obj) )
         {
            QScrollBar * sb = ( QScrollBar * ) obj;
            if ( !sb->draggingSlider() )
            {
               if ( sb->isEnabled() )
               {
                  activeScroller = sb;
                  highlightWidget = sb;
                  sb->repaint( false );
               }
            }
            return false;
         }
         else if ( ::qt_cast<QScrollView*>(obj) )
         {
            QScrollView *sv = ( QScrollView * ) obj;
            if (sv->vScrollBarMode() == QScrollView::AlwaysOff && sv->hScrollBarMode() == QScrollView::AlwaysOff)
               return false;
            optionHandler->setScrollerActive();
            if (!(highlightWidget == sv->verticalScrollBar()))
               sv->verticalScrollBar()->repaint(false);
            if (!(highlightWidget == sv->horizontalScrollBar()))
               sv->horizontalScrollBar()->repaint(false);
            return false;
         }
         else if ( ::qt_cast<QHeader*>(obj) )
         {
            currentHeader = (QHeader*)obj;
            headerHoverID = -1;
            return false;
         }
         else if (::qt_cast<QTabBar*>(obj))
         {
            currentTabBar = (QTabBar*)obj;
            hoveredTabIndex = -1;
            return false;
         }
         else if ( obj->inherits( "KateView" ) || obj->inherits( "TEWidget") || obj->inherits("CHexViewWidget"))
         {
            QObjectList *scrollerList = obj->queryList( "QScrollBar" );
            optionHandler->setScrollerActive();
            for (uint i= 0; i < scrollerList->count(); i++)
               ((QWidget*)(scrollerList->at(i)))->repaint(false);
            return false;
         }
         else if (obj->inherits("KDockWidgetAbstractHeaderDrag"))
         {
            ((QWidget *)obj)->repaint();
            return false;
         }
         else if (obj->inherits( "KAnimWidget" ))
         {
            return true;
         }
         else if ( obj->inherits( "QLineEdit" ) && obj->parent() && ::qt_cast<QComboBox*>(obj->parent()))
         {
            QWidget * btn = ( QWidget * ) obj->parent();
            if ( btn->isEnabled() )
            {
               highlightWidget = btn;
               btn->repaint( false );
            }
            return false;
         }
         else if ( obj->inherits( "QRadioButton" ) || obj->inherits( "QCheckBox" ) )
         {
            QButton * btn = ( QButton * ) obj;
            if (btn->isEnabled())
            {
               bool isStatusChild = btn->parent() && ( btn->parent() ->inherits( "QStatusBar" ) || btn->parent() ->inherits( "KonqFrameStatusBar" ) );
               bool isRadio = obj->inherits( "QRadioButton" );
               highlightWidget = btn;
               QFontMetrics fm = btn->fontMetrics();
               QSize lsz = fm.size( ShowPrefix, btn->text() );
               QSize sz = isRadio ?
                           QSize( pixelMetric( PM_ExclusiveIndicatorWidth ), pixelMetric( PM_ExclusiveIndicatorHeight ) ) :
                           QSize( pixelMetric( PM_IndicatorWidth ), pixelMetric( PM_IndicatorHeight ) );
               int x = 0;
               int y = isStatusChild ? 0 : ( btn->height() - lsz.height() + fm.height() - sz.height() ) / 2;
               QPainter p;
               p.begin( btn );
               SFlags flags = Style_Default;
               if ( btn->isEnabled() )
                  flags |= Style_Enabled;
               if ( btn->hasFocus() )
                  flags |= Style_HasFocus;
               if ( btn->isDown() )
                  flags |= Style_Down;
               if ( highlightWidget == btn )
                  flags |= Style_MouseOver;
               if ( btn->state() == QButton::On )
                  flags |= Style_On;
               else if ( btn->state() == QButton::Off )
                  flags |= Style_Off;
               if ( isRadio )
                  drawControl( CE_RadioButton, &p, btn, QRect( x, y, sz.width(), sz.height() ), btn->colorGroup(), flags );
               else
                  drawControl( CE_CheckBox, &p, btn, QRect( x, y, sz.width(), sz.height() ), btn->colorGroup(), flags );
               p.end();
            }
            return false;
         }
         return false;
      case QEvent::Leave:
         if ( ::qt_cast<QToolButton*>(obj) )
         {
            QToolButton * btn = ( QToolButton * ) obj;
            if (obj->inherits( "TaskContainer" ))
            {
               currentTaskContainer = NULL;
               btn->repaint( false );
               return false;
            }
            else if ( btn == highlightWidget )
            {
               highlightWidget = NULL;
               btn->repaint( false );
               return false;
            }
         }
         else if ( ::qt_cast<QPushButton*>(obj) || ::qt_cast<QComboBox*>(obj) || obj->inherits( "QSplitterHandle" ) || obj->inherits( "AppletHandleDrag" ) )
         {
            QWidget * btn = ( QWidget * ) obj;
            if ( btn == highlightWidget )
            {
               highlightWidget = NULL;
               if (optionHandler->AnimateButton() && ::qt_cast<QPushButton*>(obj))
                  fadeOut((QPushButton*)obj);
               else
                  btn->repaint( false );
            }
            return false;
         }
         else if (::qt_cast<KToolBar*>(obj))
         {
            QObjectList *handlerList = obj->queryList( "QDockWindowHandle" );
            for (uint i= 0; i < handlerList->count(); i++)
               ((QWidget*)(handlerList->at(i)))->repaint(false);
            return false;
         }
         if ( ::qt_cast<QScrollBar*>(obj) )
         {
            QScrollBar * sb = ( QScrollBar * ) obj;
            if ( sb == highlightWidget && !sb->draggingSlider() )
            {
               activeScroller = NULL;
               highlightWidget = NULL;
               sb->repaint( false );
            }
            return false;
         }
         else if ( ::qt_cast<QScrollView*>(obj) )
         {
            QScrollView *sv = (QScrollView*)obj;
//            if (sv->clipRegion().boundingRect().contains(sv->mapFromGlobal(QCursor::pos())))
//               return false; // mouse is still in the scrollview
            if (sv->vScrollBarMode() == QScrollView::AlwaysOff && sv->hScrollBarMode() == QScrollView::AlwaysOff)
               return false; // no sliders available - don't mess up things
            optionHandler->setScrollerInactive();
            if (!(highlightWidget == sv->verticalScrollBar()))
               sv->verticalScrollBar()->repaint(false);
            if (!(highlightWidget == sv->horizontalScrollBar()))
               sv->horizontalScrollBar()->repaint(false);
            return false;
         }
         else if ( ::qt_cast<QHeader*>(obj) )
         {
            QHeader *hw = (QHeader*)obj;
            currentHeader = NULL;
            if ( headerHoverID != -1 )
               hw->repaint( hw->sectionPos( headerHoverID )-hw->offset(), 0, hw->sectionSize( headerHoverID ), hw->height(), false );
            headerHoverID = -1;
            return false;
         }
         else if (::qt_cast<QTabBar*>(obj))
         {
            QTabBar *tb = (QTabBar*)obj;
            currentTabBar = NULL;
            if ( hoveredTabIndex != -1 )
            {
               QTab *tmpTab = tb->tabAt(hoveredTabIndex);
               if (tmpTab) tb->repaint( tmpTab->rect(), false );
            }
            hoveredTabIndex = -1;
            return false;
         }
         else if ( obj->inherits( "KateView" ) || obj->inherits( "TEWidget") || obj->inherits("CHexViewWidget"))
         {
            QObjectList *scrollerList = obj->queryList( "QScrollBar" );
            optionHandler->setScrollerInactive();
            for (uint i= 0; i < scrollerList->count(); i++)
               ((QWidget*)(scrollerList->at(i)))->repaint(false);
            return false;
         }
         else if (obj->inherits("KDockWidgetAbstractHeaderDrag"))
         {
            ((QWidget *)obj)->repaint();
            return false;
         }
         else if (obj->inherits( "KAnimWidget" )){
               return true;
         }
         else if ( obj->inherits( "QLineEdit" ) && obj->parent() && ::qt_cast<QComboBox*>(obj->parent())) {
               QWidget *btn = (QWidget*)obj->parent();
               if ( btn == highlightWidget )
                  highlightWidget = NULL;
               btn->repaint( false );
               return false;
         }
         else if ( obj->inherits( "QRadioButton" ) || obj->inherits( "QCheckBox" ) )
         {
            QButton * btn = ( QButton * ) obj;
            if (btn == highlightWidget)
            {
               bool isRadio = obj->inherits( "QRadioButton" );
               bool isStatusChild = btn->parent() && ( btn->parent() ->inherits( "QStatusBar" ) || btn->parent() ->inherits( "KonqFrameStatusBar" ) );
               highlightWidget = NULL;
               QFontMetrics fm = btn->fontMetrics();
               QSize lsz = fm.size( ShowPrefix, btn->text() );
               QSize sz = isRadio ?
                           QSize( pixelMetric( PM_ExclusiveIndicatorWidth ), pixelMetric( PM_ExclusiveIndicatorHeight ) ) :
                           QSize( pixelMetric( PM_IndicatorWidth ), pixelMetric( PM_IndicatorHeight ) );
               int x = 0;
               int y = isStatusChild ? 0 : ( btn->height() - lsz.height() + fm.height() - sz.height() ) / 2;
               QPainter p;
               p.begin( btn );
               SFlags flags = Style_Default;
               if ( btn->isEnabled() )
                  flags |= Style_Enabled;
               if ( btn->hasFocus() )
                  flags |= Style_HasFocus;
               if ( btn->isDown() )
                  flags |= Style_Down;
               if ( highlightWidget == btn )
                  flags |= Style_MouseOver;
               if ( btn->state() == QButton::On )
                  flags |= Style_On;
               else if ( btn->state() == QButton::Off )
                  flags |= Style_Off;
               if ( isRadio )
                  drawControl( CE_RadioButton, &p, btn, QRect( x, y, sz.width(), sz.height() ), btn->colorGroup(), flags );
               else
                  drawControl( CE_CheckBox, &p, btn, QRect( x, y, sz.width(), sz.height() ), btn->colorGroup(), flags );
               p.end();
            }
            return false;
         }
         return false;
      case QEvent::MouseButtonPress:
//	 qWarning("class: %s, name: %s",obj->className(), obj->name());
      if ( optionHandler->style() > Panther && ::qt_cast<QToolBar*>(obj))
         {
            QMouseEvent *mev = (QMouseEvent *) ev;
            if (mev->button() != Qt::LeftButton)
               return false;
            mouseButtonPressed_ = true;
            cursorPos_ = mev->globalPos();
            QTimer::singleShot(50,this,SLOT(fakeMouse()));
            return true; // fire this event
         }
         if ( ::qt_cast<QScrollBar*>(obj) && ((QMouseEvent*)ev)->button() != Qt::RightButton)
         {
            QScrollBar * sb = ( QScrollBar * ) obj;
            activeScroller = NULL;
            sb->repaint( false );
            return false;
         }
         else if (obj->inherits( "KAnimWidget" ))
         {
            ((QFrame *)obj)->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
            return false;
         }
         return false;
      case QEvent::MouseButtonRelease:
         if ( mouseButtonPressed_ )
         {
            mouseButtonPressed_= false;
         }
         if ( ::qt_cast<QScrollBar*>(obj) )
         {
            QScrollBar * sb = ( QScrollBar * ) obj;
            QMouseEvent * me = ( QMouseEvent * ) ev;
            if ( sb == highlightWidget)
            {
               if ( sb->rect().contains( me->pos() ) )
               {
                  activeScroller = sb;
                  sb->repaint( false );
               }
               else
               {
                  // this assumes, scrollers are right/bottom to scrollview
                  if ( ((sb->orientation() == Vertical) && (me->pos().x() < 0)) || ((sb->orientation() == Horizontal) && (me->pos().y() < 0)) )
                  {
                     optionHandler->setScrollerActive();
                     sb->repaint( false );
                  }
               }
            }
            return false;
         }
         else if ( ::qt_cast<QHeader*>(obj) && obj->parent() && ::qt_cast<QListView*>(obj->parent()))
         {
            QMouseEvent * me = ( QMouseEvent * ) ev;
            headerSortID = ((QHeader*)obj)->sectionAt( me->x() + ((QHeader*)obj)->offset());
            return false;
         }
         else if (obj->inherits( "KAnimWidget" )){
               ((QFrame *)obj)->setFrameShape(QFrame::NoFrame);
               return false;
         }
         return false;
      case QEvent::FocusIn:
         if (optionHandler->AnimateButton() && ::qt_cast<QPushButton*>(obj))
         {
            fadeIn((QPushButton*)obj);
            return false;
         }
         if ( obj->inherits( "QLineEdit" ) )
         {
            QObject *daddy = obj->parent();
            if ( daddy && daddy->inherits( "QSpinWidget" ))
               ((QWidget*)daddy)->repaint(false);
            else
               ((QWidget*)obj)->repaint(false);
            return false;
         }
         return false;
      case QEvent::FocusOut:
         if (optionHandler->AnimateButton() && ::qt_cast<QPushButton*>(obj))
         {
               fadeOut((QPushButton*)obj);
               return false;
         }
         if ( obj->inherits( "QLineEdit" ) )
         {
            if ( obj->parent() && obj->parent()->inherits( "QSpinWidget" ))
               ((QWidget*)obj->parent())->repaint(false);
            else
               ((QWidget*)obj)->repaint(false);
            return false;
         }
         return false;
      case QEvent::Move:
         if ( !(::qt_cast<QToolBar*>(obj) || obj->inherits( "KActiveLabel" )))
            return false;
      case QEvent::Show:
         if (optionHandler->style() == Tiger && ::qt_cast<QToolBar*>(obj))
         {
            QToolBar *tb = (QToolBar*)obj;
            QPoint pt = tb->mapTo(tb->topLevelWidget(), QPoint(0,0));
            if (pt.y() == 0)
            {
               activeToolbarPix = QPixmap(32,tb->height());
               KPixmapEffect::gradient(activeToolbarPix, optionHandler->aDecoColor1(), optionHandler->aDecoColor2(), KPixmapEffect::VerticalGradient, 0);
               QPainter painter(&activeToolbarPix);
               painter.setPen( optionHandler->aDecoColor2().dark(130) );
               painter.drawLine ( 0, tb->height()-1, 31, tb->height()-1 );
               painter.end();
               tb->installEventFilter(eventKiller);
               if (tb->isActiveWindow())
               {
                  tb->setPaletteBackgroundColor(optionHandler->aDecoColor2());
                  tb->setPaletteBackgroundPixmap(activeToolbarPix);
               }
               else
               {
                  QPalette pal( tb->palette() );
                  pal.setColor( QColorGroup::Background, qApp->palette().color(QPalette::Active, QColorGroup::Background) );
                  pal.setBrush ( QColorGroup::Background, qApp->palette().brush(QPalette::Active, QColorGroup::Background) );
                  tb->setPalette( pal );
               }
               tb->removeEventFilter(eventKiller);
            }
            else
            {
               QPalette pal( tb->palette() );
               pal.setBrush ( QColorGroup::Background, qApp->palette().brush(QPalette::Active, QColorGroup::Background) );
               tb->installEventFilter(eventKiller);
               tb->setPalette( pal );
               tb->removeEventFilter(eventKiller);
            }
         }
         if (optionHandler->AnimateButton() && ::qt_cast<QPushButton*>(obj))
         {
            ButtonFadeInfo *bfi_ = bfi[((QPushButton*)obj)->winId()];
            if (bfi_)
               return false;
            bfi_ = new ButtonFadeInfo();
            bfi.insert(((QPushButton*)obj)->winId(),bfi_);
            if (((QPushButton*)obj)->isDefault())
               fadeIn((QPushButton*)obj);
            return false;
         }
         if (::qt_cast<QProgressBar*>(obj))
         {
            progAnimWidgets[(QWidget*)obj] = 0;
            return false;
         }
         if ( !qstrcmp(obj->name(), "kde toolbar widget") )
         {
            QWidget* lb = static_cast<QWidget*>(obj);
            if (lb->backgroundMode() == Qt::PaletteButton)
               lb->setBackgroundMode(Qt::PaletteBackground);
            lb->removeEventFilter(this);
            return false;
         }
         else if ( !obj->inherits( "KActiveLabel" ))
            return false;
      case QEvent::Resize:{
         QResizeEvent* rev = (QResizeEvent*) ev;
         if (::qt_cast<QPopupMenu*>(obj) && optionHandler->MenuBackground() == Gradient)
         {
            KPixmap tmpPix = QPixmap(rev->size().width(),32);
            ((QWidget *)obj)->setPaletteBackgroundPixmap(KPixmapEffect::unbalancedGradient(tmpPix, optionHandler->Color(), optionHandler->Color2(), KPixmapEffect::HorizontalGradient, 100, 100));
         }
         else if (optionHandler->style() == Tiger && ::qt_cast<QToolBar*>(obj) && rev->size().height() != rev->oldSize().height())
         {
            QToolBar *tb = (QToolBar*)obj;
            QPoint pt = tb->mapTo(tb->topLevelWidget(), QPoint(0,0));
            if (pt.y() == 0)
            {
               activeToolbarPix = QPixmap(32,rev->size().height());
               KPixmapEffect::gradient(activeToolbarPix, optionHandler->aDecoColor1(), optionHandler->aDecoColor2(), KPixmapEffect::VerticalGradient, 0);
               QPainter painter(&activeToolbarPix);
               painter.setPen( optionHandler->aDecoColor2().dark(130) );
               painter.drawLine ( 0, rev->size().height()-1, 31, rev->size().height()-1 );
               painter.end();
               tb->installEventFilter(eventKiller);
               if (tb->isActiveWindow())
               {
                  tb->setPaletteBackgroundColor(optionHandler->aDecoColor2());
                  tb->setPaletteBackgroundPixmap(activeToolbarPix);
               }
               else
               {
                  QPalette pal( tb->palette() );
                  pal.setColor( QColorGroup::Background, qApp->palette().color(QPalette::Active, QColorGroup::Background) );
                  pal.setBrush ( QColorGroup::Background, qApp->palette().brush(QPalette::Active, QColorGroup::Background) );
                  tb->setPalette( pal );
               }
               tb->removeEventFilter(eventKiller);
            }
            else
            {
               QPalette pal( tb->palette() );
               pal.setBrush ( QColorGroup::Background, qApp->palette().brush(QPalette::Active, QColorGroup::Background) );
               tb->installEventFilter(eventKiller);
               tb->setPalette( pal );
               tb->removeEventFilter(eventKiller);
            }
         }
         /*hmhmhmm... this may break qgroupbox children?!?*/
         else if (::qt_cast<QGroupBox*>(obj))
         {
            QGroupBox* gb = (QGroupBox*)(obj);
            if(gb->title().isEmpty()) return false;
            QResizeEvent * e = (QResizeEvent*)ev;
            int fm = gb->fontMetrics().height()+(gb->isCheckable()?4:1);
            gb->setFrameRect(QRect(gb->rect().x(), gb->rect().y()+fm, e->size().width(), e->size().height()-fm/*-gb->frameWidth()*/));
            if(gb->layout())
               gb->layout()->setGeometry(QRect(gb->rect().x(), gb->rect().y(), e->size().width(), e->size().height()));
            return true;
         }
         else if ( obj->inherits( "KActiveLabel" ))
         {
            QWidget * w = ( QWidget * ) obj;
            QPalette pal( w->palette() );
            QPixmap *tile = pal.brush( QPalette::Active, QColorGroup::Background ).pixmap();
            if ( tile )
            {
               QSize sz( ev->type() == QEvent::Resize ? rev->size() : w->size() );
               QPixmap pix( sz.width(), sz.height() );
               QPainter p;
               if (optionHandler->style() == Brushed)
               {
                  p.begin( &pix );
                  // calc brush offsets
                  QWidget *window = w->topLevelWidget();
                  int leftWidth;
                  int rightWidth;
                  if (window)
                  {
                     QPoint zero = w->mapTo(window, QPoint(0,0));
                     leftWidth = (window->width()-1024)/2 - zero.x();
                     rightWidth = pix.width() - (1024 + leftWidth);
                     if (leftWidth > 0)
                        p.drawTiledPixmap(0, 0, leftWidth, sz.height(), plainBrush);
                     p.drawTiledPixmap(leftWidth>0?leftWidth:0, 0,
                                       pix.width()-(leftWidth>0?leftWidth:0)-(rightWidth>0?rightWidth:0), sz.height(),
                                       gradBrush,leftWidth>0?0:(zero.x()-(window->width()-1024)/2));
                     if (rightWidth > 0)
                        p.drawTiledPixmap(pix.width()-rightWidth, 0, rightWidth, sz.height(), plainBrush);
                  }
                  else if (pix.width() > 1024)
                  {
                     leftWidth = (pix.width()-1024)/2;
                     rightWidth = pix.width() - leftWidth;
                     p.drawTiledPixmap(0, 0, leftWidth, sz.height(), plainBrush);
                     p.drawTiledPixmap(leftWidth, 0, 1024, sz.height(), gradBrush);
                     p.drawTiledPixmap(leftWidth + 1024, 0, rightWidth, sz.height(), plainBrush);
                  }
                  else
                     p.drawTiledPixmap(0, 0, pix.width(), sz.height(), gradBrush, (1024-pix.width())/2, 0);
                  p.end();
                  QBrush brush( pal.active().background(), pix );
                  pal.setBrush( QColorGroup::Base, brush );
                  w->setPalette( pal );
               }
               else
               {
                  p.begin(w);
                  p.drawTiledPixmap(QRect(0,0,sz.width(),sz.height()), *tile, w->backgroundOffset());
               }
            }
            return false;
         }
         else if ((optionHandler->style() == Brushed) && obj->isWidgetType() && ((QWidget*)obj)->isTopLevel() &&
                  !(::qt_cast<QPopupMenu*>(obj) || ::qt_cast<QMenuBar*>(obj) || ((QWidget*)obj)->isDesktop () || ::qt_cast<QListBox*>(obj) ||
                  obj->inherits("QTipLabel") || obj->inherits("AlphaWidget") || obj->inherits("ScrollWidget") ) && (ev->type() == QEvent::Show ||
                  rev->size().width() != rev->oldSize().width()))
         {
            QWidget* widget = (QWidget*)obj;
            // create new brushpix
         QPixmap tmpBrushPix(ev->type() == QEvent::Show?widget->width():rev->size().width(), plainBrush.height());
            QPainter lightMe(&tmpBrushPix);
            // calc brush offsets
            int tmpWidth;
            if ((tmpWidth = tmpBrushPix.width()) > 1024)
            {
               int leftWidth = (tmpWidth-1024)/2;
               int rightWidth = tmpWidth - leftWidth;
               lightMe.drawTiledPixmap(0, 0, leftWidth, plainBrush.height(), plainBrush,256-leftWidth,0);
               lightMe.drawPixmap(leftWidth, 0, gradBrush);
               lightMe.drawTiledPixmap(leftWidth + 1024, 0, rightWidth, plainBrush.height(), plainBrush);
            }
            else
               lightMe.drawPixmap(0, 0, gradBrush, (1024-tmpWidth)/2, 0, tmpWidth);
            lightMe.end();
            //update palette
            QPalette pal( qApp->palette());
            QBrush brush( pal.active().background(), tmpBrushPix );
            pal.setBrush ( QColorGroup::Background, brush );
            //reset apps palette
            qApp->installEventFilter(eventKiller); // pssst... don't let anyone know...
            qApp->setPalette(pal);
            pal = QPalette(widget->palette());
            pal.setBrush ( QColorGroup::Background, brush );
            widget->installEventFilter(eventKiller); // pssst... don't let anyone know...
            widget->setPalette(pal);
            widget->removeEventFilter(eventKiller); // ...that we just replaced the palette
            QObjectList *kids = widget->queryList("QWidget"); //TODO: implement a faster version, depending on ::qt_cast<QWidget*>()
            for (uint i= 0; i < kids->count(); i++)
            {
               QWidget *kid = (QWidget*)kids->at(i);
               if (kid && kid->paletteBackgroundPixmap() && !(kid->isTopLevel() || kid->inherits( "KonqIconViewWidget" ) || kid->inherits( "khtml") || kid->inherits( "KActiveLabel") || qstrcmp( kid->name(), "_B_ALI_HEADER" ) == 0 ||  (kid->parentWidget() && kid->parentWidget()->inherits("Thumbnail") || kid->parentWidget()->inherits("MarkListItem")) ))
               {
               //we can loose those kids anytime - especially if the user plays on konqueror...
                  if (::qt_cast<KToolBarButton*>(kid))
                     kid->setErasePixmap(tmpBrushPix);
                  else
                  {
                     pal = QPalette(kid->palette());
                     pal.setBrush ( QColorGroup::Background, brush );
                     kid->installEventFilter(eventKiller); // pssst... don't let anyone know...
                     kid->setPalette(pal);
                     kid->removeEventFilter(eventKiller); // ...that we just replaced the palette
                  }
               }
            }
            delete kids;
            qApp->removeEventFilter(eventKiller); // ...that we just replaced the palette
            return false;
         }
         return false;
      }
      case QEvent::Hide:
         if (optionHandler->AnimateButton() && ::qt_cast<QPushButton*>(obj))
         {
            ButtonFadeInfo *bfi_ = bfi[((QPushButton*)obj)->winId()];
            if (bfi_)
            {
               obj->killTimer(bfi_->timerId);
               // qWarning("%u: stopped (%d)", ((QPushButton*)obj)->winId(), bfi_->timerId);
               bfi_->index=0;
               bfi_->timerId=0;
            }
         }
      case QEvent::ApplicationPaletteChange:
         if ( obj->inherits( "QTipLabel" ) )
         {
            ( ( QWidget* ) obj ) ->setPalette( tooltipPalette );
            return true;
         }
         return false;
      case QEvent::PaletteChange:
         if (obj->inherits("QStatusBar"))
         {
            obj->installEventFilter(eventKiller);
            ((QWidget*)obj)->setPalette(qApp->palette());
            obj->removeEventFilter(eventKiller);
            return true;
         }
         if ( obj->inherits( "QTipLabel" ) )
         {
            ( ( QWidget* ) obj ) ->setPalette( tooltipPalette );
            return true;
         }
         if (obj->inherits("KCalcButton"))
         {
            QWidget *w = (QWidget*)obj;
            QPalette pal( w->palette() );
            pal.setBrush( QColorGroup::Background, qApp->palette().brush(QPalette::Active, QColorGroup::Background));
            w->installEventFilter(eventKiller);
            w->setPalette( pal );
            w->removeEventFilter(eventKiller);
            w->setBackgroundOrigin( QWidget::WindowOrigin );
            return true;
         }
         return false;
      case QEvent::WindowActivate:
         if (optionHandler->style() == Tiger && ::qt_cast<QToolBar*>(obj))
         {
            QToolBar *tb = (QToolBar*)obj;
            QPoint pt = tb->mapTo(tb->topLevelWidget(), QPoint(0,0));
            if (pt.y() == 0)
            {
               tb->installEventFilter(eventKiller);
               tb->setPaletteBackgroundColor(optionHandler->aDecoColor2());
               tb->setPaletteBackgroundPixmap(activeToolbarPix);
               tb->removeEventFilter(eventKiller);
            }
         }
         return false;
      case QEvent::WindowDeactivate:
         if (optionHandler->style() == Tiger && ::qt_cast<QToolBar*>(obj))
         {
            QToolBar *tb = (QToolBar*)obj;
            QPoint pt = tb->mapTo(tb->topLevelWidget(), QPoint(0,0));
            if (pt.y() == 0)
            {
               QPalette pal( tb->palette() );
               pal.setColor( QColorGroup::Background, qApp->palette().color(QPalette::Active, QColorGroup::Background) );
               pal.setBrush ( QColorGroup::Background, qApp->palette().brush(QPalette::Active, QColorGroup::Background) );
               tb->installEventFilter(eventKiller);
               tb->setPalette( pal );
               obj->removeEventFilter(eventKiller);
            }
         }
         return false;
      default:
         return false;
   }
}

void LiquidStyle::fakeMouse()
{
   if (mouseButtonPressed_) // delayed mousepress for move event
   {
      QCursor::setPos ( cursorPos_ );
      XTestFakeButtonEvent(qt_xdisplay(),1, false, 0);
      XTestFakeKeyEvent(qt_xdisplay(),XKeysymToKeycode(qt_xdisplay(), XK_Alt_L), true, 0);
      XTestFakeButtonEvent(qt_xdisplay(),1, true, 0);
      XTestFakeKeyEvent(qt_xdisplay(),XKeysymToKeycode(qt_xdisplay(), XK_Alt_L), false, 0);
      XFlush(qt_xdisplay());
   }
}

void LiquidStyle::fadeIn(QPushButton *button)
{
   ButtonFadeInfo *bfi_ = bfi[button->winId()];
   if (!bfi_)
   {
      bfi_ = new ButtonFadeInfo();
      bfi.insert(button->winId(),bfi_);
   }
   bfi_->fadeIn = true;
   if (bfi_->timerId == 0)
   {
      bfi_->index = 0;
      bfi_->timerId = button->startTimer(button->isDefault()?100:50);
      // qWarning("%u: started (%d)", button->winId(), bfi_->timerId);
   }
}

void LiquidStyle::fadeOut(QPushButton *button)
{
   ButtonFadeInfo *bfi_ = bfi[button->winId()];
   if (!bfi_)
   {
      bfi_ = new ButtonFadeInfo();
      bfi.insert(button->winId(),bfi_);
   }
   bfi_->fadeIn = false;
   if (bfi_->timerId == 0)
   {
      bfi_->index = 9;
      bfi_->timerId = button->startTimer(50);
      // qWarning("%u: started (%d)", button->winId(), bfi_->timerId);
   }
}

BMShower::BMShower(QObject *parent) : QObject(parent)
{
}

bool BMShower::eventFilter( QObject *obj, QEvent *ev )
{
   if (ev->type() == QEvent::Show && (obj->isWidgetType() && ((QWidget*)obj)->isTopLevel() &&
         !(::qt_cast<QPopupMenu*>(obj) || ::qt_cast<QMenuBar*>(obj) || ((QWidget*)obj)->isDesktop () || ::qt_cast<QListBox*>(obj) ||
         obj->inherits("QTipLabel") || obj->inherits("AlphaWidget") || obj->inherits("ScrollWidget") )))
   {
      QResizeEvent* rev = new QResizeEvent(((QWidget*)obj)->size(), QSize(0,0));
      parent()->eventFilter( obj, rev );
      obj->removeEventFilter(this);
      delete rev;
   }
   return false;
}
