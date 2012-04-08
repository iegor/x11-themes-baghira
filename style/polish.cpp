//#include <qdir.h>
#include "baghira.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <qapplication.h>
#include <qbrush.h>
#include <qcolor.h>
#include <qcombobox.h>
#include <qcursor.h>
#include <qdialog.h>
#include <qframe.h>
#include <qfile.h>
#include <qheader.h>
#include <qgroupbox.h>
//#include <qiconview.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlistbox.h>
#include <klistview.h>
#include <kmenubar.h>
#include <qpalette.h>
#include <qpainter.h>
#include <qpopupmenu.h>
#include <qpushbutton.h>
#include <qscrollbar.h>
#include <qscrollview.h>
#include <qsizepolicy.h> 
#include <qslider.h>
#include <qtabbar.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <kdeversion.h>
#include <kpixmapeffect.h>
#include <kglobalsettings.h>
#include <kwin.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include <fixx11h.h>
#define OPAQUE	0xffffffff
#define OPACITY	"_KDE_WM_WINDOW_OPACITY"
#define _FORCE_SCROLLBARS_ 0

#ifndef CLAMP
#define CLAMP(x,l,u) x < l ? l :\
x > u ? u :\
x
#endif


void LiquidStyle::polish( QPalette &pal )
{
   if ( inExitPolish )
      return ;
   // clear out all old colorized pixmaps
   int i;
   for ( i = 0; i < BITMAP_ITEMS; ++i )
   {
      if ( pixmaps[ i ] )
      {
         delete pixmaps[ i ];
         pixmaps[ i ] = NULL;
      }
   }
   // clear out all dicts
   btnDict.clear();
   btnShadowedDict.clear();
   // reset brushes
   pagerHoverBrush = QBrush();
   pagerBrush = QBrush();

   bool newPalette = false;
   struct stat buffer;
   if ( stat( QFile::encodeName( QDir::homeDirPath() + "/.qt/qtrc" ), &buffer ) == 0 )
   {
      unsigned int lastModTime = ( unsigned int ) buffer.st_mtime;
      if ( lastModTime > qtrcModificationTime )
      {
         qtrcModificationTime = lastModTime;
         newPalette = true;
      }
   }
   else if ( !initialPaletteLoaded )  // Hack, should always have qtrc in KDE
      newPalette = true;
   initialPaletteLoaded = true;

   if ( !newPalette )
   {
      pal = polishedPalette;
      return ;
   }

   originalBgColor = pal.color(QPalette::Active, QColorGroup::Background);
   if (optionHandler->style() == Brushed)
      pal.setColor(QColorGroup::Background, brushedMetalColor(optionHandler->BrushTint()));
   if (isOOO)
      pal.setColor( QColorGroup::Button, pal.active().background() );
   else if (optionHandler->custCols[0] != -1)
   {
      pal.setColor(QColorGroup::Background, QColor(optionHandler->custCols[0]));
      pal.setColor(QColorGroup::Button, QColor(optionHandler->custCols[1]));
      pal.setColor(QColorGroup::Base, QColor(optionHandler->custCols[2]));
      pal.setColor(QPalette::Active, QColorGroup::Text, QColor(optionHandler->custCols[3]));
      pal.setColor(QPalette::Active, QColorGroup::Foreground, QColor(optionHandler->custCols[3]));
      pal.setColor(QColorGroup::Highlight, QColor(optionHandler->custCols[4]));
      pal.setColor(QPalette::Active, QColorGroup::HighlightedText, QColor(optionHandler->custCols[5]));
      pal.setColor(QPalette::Active, QColorGroup::ButtonText, QColor(optionHandler->custCols[6]));
      pal.setColor(QColorGroup::Mid, QColor((qRed(optionHandler->custCols[2])+qRed(optionHandler->custCols[3]))/2,(qGreen(optionHandler->custCols[2])+qGreen(optionHandler->custCols[3]))/2,(qBlue(optionHandler->custCols[2])+qBlue(optionHandler->custCols[3]))/2));
      
      pal.setColor(QPalette::Inactive, QColorGroup::Text, QColor(optionHandler->custCols[3]));
      pal.setColor(QPalette::Inactive, QColorGroup::HighlightedText, QColor(optionHandler->custCols[5]));
      pal.setColor(QPalette::Inactive, QColorGroup::ButtonText, QColor(optionHandler->custCols[6]));
   }
   else
      pal.setColor( QColorGroup::Button, optionHandler->CustomButtonColor() );
#if 0
   if ( isKicker )
   {
      origPanelBrush = pal.brush( QPalette::Active, QColorGroup::Background );
      origPanelPalette = pal;
      QColor c( pal.active().background() );
      
      if ( optionHandler->usePanelCustomColor() && optionHandler->panelCustomColor().isValid() )
         c = optionHandler->panelCustomColor();
      
      kickerColor = c;

      pal.setColor( QColorGroup::Mid, optionHandler->CustomButtonColor().dark( 110 ) );
      pal.setColor( QColorGroup::Dark, optionHandler->CustomButtonColor().dark( 130 ) );
      pal.setColor( QColorGroup::Midlight, optionHandler->CustomButtonColor().light( 110 ) );
      pal.setColor( QColorGroup::Light, optionHandler->CustomButtonColor().light( 115 ) );
      pal.setColor( QColorGroup::Button, optionHandler->CustomButtonColor());

      if (optionHandler->style() == Brushed)
      {
         QBrush brush( c, gradBrush );
         pal.setBrush( QColorGroup::Background, brush );
      }
      else if ( optionHandler->usePanelStipple() )
      {
         QPixmap stipple( 64, 64 );
         stipple.fill( c.rgb() );
         QPainter p( &stipple );
         p.setPen( c.dark( 100 + (int)(optionHandler->stippleContrast()/4) ) );
         for ( int i = 1; i < 64; i += 4 )
         {
            p.drawLine( 0, i, 63, i );
            p.drawLine( 0, i+2, 63, i+2 );
         }
         p.setPen( c.dark( 100 + optionHandler->stippleContrast()) );
         for ( int i = 2; i < 63; i += 4 )
            p.drawLine( 0, i, 63, i );
         p.end();
         QBrush brush( c, stipple );
         pal.setBrush( QColorGroup::Background, brush );
      }
      else
         pal.setBrush( QColorGroup::Background, c );
   }
#endif
   /*else*/ if (optionHandler->style() == Brushed)
   {
      QColor c( pal.active().background() );
      QBrush brush( c, gradBrush );
      pal.setBrush( QColorGroup::Background, brush );
   }
   else if ( !isPlain() )
   {
      QColor c( pal.active().background() );
      QPixmap stipple( 64, 64 );
      stipple.fill( c.rgb() );
      QPainter p;
      p.begin( &stipple );
      p.setPen( c.dark( 100 + (int)(optionHandler->stippleContrast()/2) ) );
      for ( i = 1; i < 64; i += 4 )
      {
         p.drawLine( 0, i, 63, i );
         p.drawLine( 0, i+2, 63, i+2 );
      }
      p.setPen( c.dark( 100 + optionHandler->stippleContrast() ) );
      for ( i = 2; i < 63; i += 4 )
         p.drawLine( 0, i, 63, i );
      p.end();
      QBrush brush( c, stipple );
      pal.setBrush( QColorGroup::Background, brush );
   }
    
   if (optionHandler->bevelHighlights())
   {
      QColor c(pal.active().highlight());
      QColor c2(c.light(140));
      QPixmap pix2 = QPixmap(48,48);
      QPainter coolPainter(&pix2);
      KPixmap pix = QPixmap(48,24);
      KPixmapEffect::gradient(pix,c,c2,KPixmapEffect::VerticalGradient,0);
      coolPainter.drawPixmap(0,0,pix);
      KPixmapEffect::gradient(pix,c2,c,KPixmapEffect::VerticalGradient,0);
      coolPainter.drawPixmap(0,24,pix);
      QBrush brush(c, pix2);
      pal.setBrush(QColorGroup::Highlight, brush);
   }
   polishedPalette = pal;
   optionHandler->reset();
}

void LiquidStyle::unPolish( QApplication *app )
{
   popupBack.resize(0,0);
   QPalette pal( app->palette() );
   inExitPolish = true;
   
   if ( !isPlain() )
   {
      // reset any stipples
      if ( pal.brush( QPalette::Active, QColorGroup::Background ).pixmap() )
         pal.setBrush( QColorGroup::Background, pal.active().background() );
      if ( pal.brush( QPalette::Active, QColorGroup::Button ).pixmap() )
         pal.setBrush( QColorGroup::Button, pal.active().button() );
//             app->blockSignals( TRUE );
      app->setPalette( pal );
//             app->blockSignals( FALSE );
   }

   inExitPolish = false;
}

Atom baghira_deco_design = XInternAtom(qt_xdisplay(), "BAGHIRA_DECO_DESIGN", False);

void LiquidStyle::polish( QWidget *w )
{
//     bool testWidget = w->inherits("Splash");
//     int testInt = 0;
//     if (w->sizePolicy().horData() == QSizePolicy::Minimum || w->sizePolicy().horData() == QSizePolicy::MinimumExpanding)
// 	qWarning("%s, \"%s\"",w->className(), w->name());
//	w->installEventFilter(this);

    bool tmpBool = false;
    bool isViewport = qstrcmp( w->name(), "qt_viewport" ) == 0 ||
                      qstrcmp( w->name(), "qt_clipped_viewport" ) == 0;
    bool isViewportChild = w->parent() &&
                     ( ( qstrcmp( w->parent() ->name(), "qt_viewport" ) == 0 ) ||
                     ( qstrcmp( w->parent() ->name(), "qt_clipped_viewport" ) == 0 ) );

    if (w->isTopLevel())
    {
        if ( ::qt_cast<QPopupMenu*>(w)) // popup menu designs
        {
            switch (optionHandler->MenuBackground())
            {
            case Plain:
               w->setBackgroundMode( QWidget::PaletteBackground );
               if (optionHandler->UseCustomMenuColor())
                  w->setPaletteBackgroundColor(optionHandler->Color());
               else if (optionHandler->MenuColorButton())
                  w->setPaletteBackgroundColor(optionHandler->buttonColor());
               else
                  w->setPaletteBackgroundColor(optionHandler->bgColor());
                break;
            case Stipples:
            {
                QColor c;
                if (optionHandler->UseCustomMenuColor())
                    c = QColor( optionHandler->Color() );
                else if (optionHandler->MenuColorButton())
                    c = QColor( optionHandler->buttonColor() );
                else
                   c = QColor( originalBgColor );
                if (popupBack.isNull())
                {
                    popupBack = QPixmap(64,64);
                    QPixmap *stipple = &popupBack;
                    stipple->fill( c.rgb() );
                    QPainter p;
                    p.begin( stipple );
                    if (optionHandler->UseCustomMenuColor())
                    {
                        QColor c2 = QColor( optionHandler->Color2() );
                        p.setPen(QColor((3*c.red()+c2.red())/4,
                                        (3*c.green()+c2.green())/4,
                                        (3*c.blue()+c2.blue())/4));
                        for ( int i = 1; i < 64; i += 4 )
                        {
                            p.drawLine( 0, i, 63, i );
                            p.drawLine( 0, i+2, 63, i+2 );
                        }
                        p.setPen(c2);
                        for ( int i = 2; i < 63; i += 4 )
                        {
                            p.drawLine( 0, i, 63, i );
                        }
                    }
                    else
                    {
                        p.setPen( c.dark( 100 + (int)(optionHandler->stippleContrast()/4) ) );
                        for ( int i = 1; i < 64; i += 4 )
                        {
                            p.drawLine( 0, i, 63, i );
                            p.drawLine( 0, i+2, 63, i+2 );
                        }
                        p.setPen( c.dark( 100 + optionHandler->stippleContrast() ) );
                        for ( int i = 2; i < 63; i += 4 )
                        {
                            p.drawLine( 0, i, 63, i );
                        }
                    }
                    p.end();
                }
                w->setBackgroundMode( QWidget::NoBackground );
                break;
            }
            case Gradient:
            {
               w->setBackgroundMode( QWidget::PaletteBackground );
               KPixmap tmpPix = QPixmap(w->width(), 32);
               w->setPaletteBackgroundPixmap(KPixmapEffect::unbalancedGradient(tmpPix, optionHandler->Color(), optionHandler->Color2(),KPixmapEffect::HorizontalGradient, 100, 100, 16));
               w->installEventFilter( this );
               break;
            }
            default:
               w->setBackgroundMode( QWidget::PaletteBackground );
               break;
            }
            //set popup menu opacity for them as they bypass the windowmanager
            unsigned long opacity = optionHandler->MenuOpacity()/100.0*OPAQUE;
            XChangeProperty(qt_xdisplay(), w->winId(), XInternAtom(qt_xdisplay(), OPACITY, False), XA_CARDINAL, 32, PropModeReplace, (unsigned char *) &opacity, 1L);
            goto kstpolish;
        }
        else
        {
            if (optionHandler->wmDesign < 5) // set deco design X Property
            {
               unsigned char *data = 0;
               Atom actual;
               int format, result;
               unsigned long n, left;
               result = XGetWindowProperty(qt_xdisplay(), w->winId(), baghira_deco_design, 0L, 1L, False, XA_CARDINAL, &actual, &format, &n, &left, &data);
               if (result != Success || data == None) // not found set design
               {
            int tmp = optionHandler->wmDesign != Tiger ? optionHandler->wmDesign : Panther;
                  XChangeProperty(qt_xdisplay(), w->winId(), baghira_deco_design, XA_CARDINAL, 32, PropModeReplace, (unsigned char *) &(tmp), 1L);
               }
            }
           if ((optionHandler->style() == Brushed) && !( ::qt_cast<QPopupMenu*>(w) || ::qt_cast<QMenuBar*>(w) || w->isDesktop() || ::qt_cast<QListBox*>(w) || w->inherits("QTipLabel") || w->inherits("AlphaWidget") || w->inherits("ScrollWidget") || w->inherits("PlayerWidget")))
                // install resize event filter, allowing us to update the b/m gradient on horizontal resizes. some widgets must be excluded though
            {
                if (!isKicker)
                    w->installEventFilter( bmShower );
                w->installEventFilter( this );
            }
        }
    } // TopLevel Widgets

   if (w->inherits("KURLBar")/*ListBox*/)
   {
      w->setPaletteBackgroundColor(qApp->palette().color(QPalette::Active, QColorGroup::Base));
//	((QListBox*)w)->viewport()->setPaletteBackgroundColor(qApp->palette().color(QPalette::Active, QColorGroup::Base));
	goto kstpolish;
    }

        
    if (w->inherits("KColorGrid"))
    {
        // kiconedit doesn't check for the paintevent rect size, but tries to create a (possibly) NULL Pixmap (after palette change event e.g.)
        // so we catch it's paint event and destroy it if the size is NULL
        // remove this after kiconedit corrected its code
        w->installEventFilter( this );
        goto kstpolish;
    }
        
    if (w->inherits("BGMonitor") || w->inherits("mapWidget") || w->isA("MyButton") || (w->parentWidget() && w->parentWidget()->inherits("Thumbnail")))
        // don't touch these widgets at all - they don't like it
    {
        goto kstpolish;
    }
    


    if (qstrcmp( w->name(), "kde toolbar widget" ) == 0 )
        w->installEventFilter( this );
    
    if (::qt_cast<QButton*>(w))
    {
        w->setBackgroundOrigin(QWidget::WindowOrigin);
        if (::qt_cast<QPushButton*>(w))
        {
           if (w->inherits("KKeyButton"))
           {
              w->setBackgroundMode( QWidget::PaletteBackground);
              QPalette pal( w->palette() );
              pal.setBrush( QColorGroup::Background, qApp->palette().active().background() );
              w->setPalette( pal );
           }
           else if (w->inherits("KCalcButton"))
           {
              w->setBackgroundMode( QWidget::PaletteBackground);
              QPalette pal( w->palette() );
              pal.setBrush( QColorGroup::Background, qApp->palette().brush(QPalette::Active, QColorGroup::Background));
              w->setPalette( pal );
           }
            else
#if !KDE_IS_VERSION(3,4,3)
	    if (!(w->parent() && w->parent()->inherits("MiniBar")))
#endif
                w->setBackgroundMode( QWidget::NoBackground);
            w->installEventFilter( this );
            goto kstpolish;
        }
        else if (w->inherits( "QRadioButton" ) || w->inherits( "QCheckBox" ))
         {
            w->setBackgroundOrigin(QWidget::WindowOrigin);
            w->installEventFilter( this );
            goto kstpolish;
         }
    }
    
    if ( ::qt_cast<QToolBar*>(w))
    {
        if (optionHandler->useToolButtonFrame() && w->inherits("KToolBar"))
            ((QToolBar*)w)->boxLayout()->setSpacing( 0 );
        ((QToolBar*)w)->setMargin( 4 );
        w->setBackgroundMode(PaletteBackground);
        w->setBackgroundOrigin(QWidget::WindowOrigin);
      if (optionHandler->style() > Panther)
      {
         w->installEventFilter( this );
         if (optionHandler->wmDesign == Tiger)
         {
            XChangeProperty(qt_xdisplay(), w->topLevelWidget()->winId(), baghira_deco_design, XA_CARDINAL, 32, PropModeReplace, (unsigned char *) &(optionHandler->wmDesign), 1L);
         }
      }

        goto kstpolish ;
    }
        //if (testWidget) qWarning("testwidget stage %d",testInt++);
    
    if (tmpBool = ::qt_cast<QToolButton*>(w) ||  w->inherits( "QToolBarSeparator" ) ||  w->inherits( "KToolBarSeparator" ) )
    {
        if (tmpBool && w->parentWidget() && ::qt_cast<QToolBar*>(w->parentWidget()) )
        {
            w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        }
        w->setBackgroundMode( QWidget::PaletteBackground );
        w->setBackgroundOrigin(QWidget::WindowOrigin);
        w->installEventFilter( this );
        goto kstpolish;
    }
	
    if (::qt_cast<QComboBox*>(w))
    {
        w->setBackgroundMode(NoBackground);
        w->installEventFilter( this );
        w->setBackgroundOrigin(QWidget::WindowOrigin);
        goto kstpolish;
    }
    
    if (w->inherits("TaskBar"))
    {
        w->setBackgroundOrigin(QWidget::WidgetOrigin);
        goto kstpolish;
    }

    if (w->inherits("QSpinWidget") || w->inherits("KDockWidgetAbstractHeaderDrag")){
        w->installEventFilter( this );
        goto kstpolish;
    }
	
    if ( !isPlain () && (w->inherits( "KonqIconViewWidget" ) || w->inherits( "khtml")/* || w->inherits( "QtVisionView")*/))
    {
    // if you want custom backgrounds in konqueror, you have to reset pixmap, since Konq animation can't handle it
    // original liquid did this on khtml, too (at speed loss) so we'll see, if we get errors from this...
        QPalette pal( w->palette() );
        pal.setBrush( QColorGroup::Background, pal.active().background() );
        w->setPalette( pal );
    }

    if (::qt_cast<QSlider*>(w))
    {
        //avoid flicker
        w->setBackgroundMode( QWidget::NoBackground );
        goto kstpolish;
    }
    
    if ((::qt_cast<QScrollView*>(w) || w->inherits("KateView") || w->inherits("TEWidget") || w->inherits("CHexViewWidget")) && !(w->isA("KOAgenda")))
    {
       if (::qt_cast<KListView*>(w)  && optionHandler->custCols[7] != -1 && ((KListView*)w)->alternateBackground() == KGlobalSettings::alternateBackgroundColor())
       {
          ((KListView*)w)->setAlternateBackground(QColor(optionHandler->custCols[7]));
       }
#if _FORCE_SCROLLBARS_
        if (::qt_cast<QScrollView*>(w) && !w->inherits( "KActiveLabel" ))
        {
            ((QScrollView*)w)->setVScrollBarMode(QScrollView::AlwaysOn);
        }
#endif
        if (::qt_cast<QListBox*>(w) && w->isTopLevel())
            //set opacity for them as they bypass the windowmanager
        {
            uint opacity = 0.8*OPAQUE;
            XChangeProperty(qt_xdisplay(), w->winId(), XInternAtom(qt_xdisplay(), OPACITY, False), XA_CARDINAL, 32, PropModeReplace, (unsigned char *) &opacity, 1L);
        }
#if 0
        // doesn't work properly - best would be to have the items drawn via style()
        int height = ((QListBox*)w)->itemHeight();
        if (height >  0)
        {
            QPalette pal = w->palette();
            QColor c(pal.active().highlight());
            QColor c2(c.light(140));
            KPixmap pix = QPixmap(48,height);
            KPixmapEffect::gradient(pix,c2,c,KPixmapEffect::VerticalGradient,0);
            QBrush brush(c, pix);
            pal.setBrush(QColorGroup::Highlight, brush);
            w->setPalette(pal);
        }
#endif
        // listen to enter events - highlightning the scrollbars
        w->setMouseTracking( true );
        w->installEventFilter( this );
        goto kstpolish;
    }
    
    if (w->inherits("QStatusBar"))
    {
        w->setBackgroundOrigin(QWidget::WindowOrigin);
       if ( qstrcmp( w->name(), "KonquerorFrameStatusBar" ) == 0 )
            // konqueror resets the statusbars background to Qt::White as soon as you open a new tab - our eventfilter will block this
        {
            w->installEventFilter( this );
       }
        goto kstpolish;
    }
        
    if (w->inherits( "QLineEdit" ) || w->inherits("QSplitterHandle") || w->isA( "AppletHandleDrag" ) || w->inherits( "KMiniPagerButton" ))
    {
        w->setBackgroundOrigin(QWidget::WindowOrigin);
        w->installEventFilter( this );
        goto kstpolish;
    }
	
    if (w->inherits( "TaskContainer" ))
    {
        w->installEventFilter( this );
        goto kstpolish;
    }

    if (w->inherits( "KickerTip" ))
        // kickers new tooltip uses backgroundColor() instead of paletteBackgroundColor() - causing Qt::black
    {
      w->setPaletteBackgroundColor(qApp->palette().color(QPalette::Active, QColorGroup::Button));
      w->setBackgroundMode( QWidget::NoBackground );
      goto kstpolish;
    }

    if ( !isPlain() ) 
    {
        if ( !isViewport && w->parent() && qstrcmp( w->parent() ->name(), "proxyview" ) == 0 )
        {
            w->setBackgroundOrigin( QWidget::WindowOrigin );
            goto kstpolish ;
        }

        if ( w->inherits( "PanelButtonBase" ) )
            goto kstpolish ;
        if ( !isKicker&& !isViewport && !isViewportChild && !w->testWFlags( WType_Popup ) && !w->inherits( "KDesktop" ) && !w->inherits( "PasswordDlg" ))
        {
            if ( w->backgroundMode() == QWidget::PaletteBackground || w->backgroundMode() == QWidget::PaletteButton )
            {
                w->setBackgroundOrigin( QWidget::WindowOrigin );
            }
        }
    }
    if ( ::qt_cast<QPushButton*>(w) || ::qt_cast<QComboBox*>(w)) goto kstpolish;
    if ( ::qt_cast<QMenuBar*>(w))
    {
//             if (optionHandler->SupportAppleBar() && w->isTopLevel()){
// //                 QPalette pal(w->palette());
//                 qWarning("resetting erase pixmap");
//                 QColor c( w->palette().active().background() );
//                 QPixmap stipple( 64, 64 );
//                 stipple.fill( c.rgb() );
//                 QPainter p;
//                 p.begin( &stipple );
//                 p.setPen( c.dark( 100 + (int)(optionHandler->stippleContrast()/2) ) );
//                 for (int i = 1; i < 64; i += 4 ) {
//                     p.drawLine( 0, i, 63, i );
//                     p.drawLine( 0, i+2, 63, i+2 );
//                 }
//                 p.setPen( c.dark( 100 + optionHandler->stippleContrast()) );
//                         for ( int i = 2; i < 63; i += 4 ) {
//                                 p.drawLine( 0, i, 63, i );
//                                 }
//                 p.end();
// //                 QBrush brush( c, stipple );
// //                 pal.setBrush( QColorGroup::Background, brush );
// //                 w->setPalette( pal );
//                 w->setErasePixmap(stipple);
//             }
        w->setBackgroundMode( QWidget::PaletteBackground );
       if (optionHandler->UseCustomMenuColor())
       {
          QPalette pal( w->palette() );
          pal.setBrush( QColorGroup::Highlight, optionHandler->menuColorHigh() );
          pal.setBrush( QColorGroup::HighlightedText, optionHandler->textColorHigh() );
          w->setPalette( pal );
       }
        w->setBackgroundOrigin(QWidget::WindowOrigin);
        w->installEventFilter( this );
#if 0
        if (!::qt_cast<KMenuBar*>(w))
        {
           bool bold = w->font().bold();
           bool italic = w->font().italic();
           bool underline = w->font().underline();
           bool overline = w->font().overline();
           w->setFont(KGlobalSettings::menuFont());
           if (bold)
              (const_cast<QFont*>(&w->font()))->setBold(true);
           if (italic)
              (const_cast<QFont*>(&w->font()))->setItalic(true);
           if (underline)
              (const_cast<QFont*>(&w->font()))->setUnderline(true);
           if (overline)
              (const_cast<QFont*>(&w->font()))->setOverline(true);
        }
#endif
        goto kstpolish ;
    }
        
    if (::qt_cast<QFrame*>(w))
    {
        w->setBackgroundOrigin(QWidget::WindowOrigin);
        if ( ::qt_cast<QLabel*>(w))
        {
            if (w->parentWidget() && w->parentWidget()->inherits("Kontact::Splash"))
                goto kstpolish;
            else if ( !isPlain() && !(w->inherits( "QTipLabel" ) || w->inherits( "KSystemTray" )))
            {
                w->setBackgroundMode( QWidget::PaletteBackground );
            }
            else if ( w->inherits( "QTipLabel" ) )
            {
                w->setPalette( tooltipPalette );
                w->setBackgroundOrigin(QWidget::WidgetOrigin);
                w->installEventFilter( this );
            }
        }
        if (qstrcmp( w->name(), "backdrop" ) == 0) //kscd
        {
            ((QFrame *)w)->setFrameShape(QFrame::LineEditPanel);
            ((QFrame *)w)->setFrameShadow(QFrame::Sunken);
            goto kstpolish;
        }
        if (w->inherits( "KWinInternal::TabBox" ))
        {
            uint opacity = 0.8*OPAQUE;
            XChangeProperty(qt_xdisplay(), w->winId(), XInternAtom(qt_xdisplay(), OPACITY, False), XA_CARDINAL, 32, PropModeReplace, (unsigned char *) &opacity, 1L);
            goto kstpolish;
        }
        else if (w->inherits( "KAnimWidget" ))
        {
            ((QFrame *)w)->setFrameShape(QFrame::NoFrame);
            w->installEventFilter( this ); // the widget resets it's frameshape on mouseevents - looks crap!
            goto kstpolish ;
        }
                else if (((QFrame *)w)->frameShape() >= QFrame::Box && ((QFrame *)w)->frameShape() <= QFrame::WinPanel){
			((QFrame *)w)->setFrameShape(QFrame::StyledPanel);
                        goto kstpolish ;
		}
	}
        //if (testWidget) qWarning("testwidget stage %d",testInt++);
	
	
	 if ( w->parentWidget() && ( ( ::qt_cast<QListBox*>(w) && ::qt_cast<QPushButton*>(w->parentWidget())) || w->inherits( "KCompletionBox" ) ) ) {
		QListBox* listbox = (QListBox*) w;
		listbox->setBackgroundMode( NoBackground );
		w->installEventFilter( this );
		goto kstpolish;
	}
        //if (testWidget) qWarning("testwidget stage %d",testInt++);

	
	if (::qt_cast<QHeader*>(w)){
//                 if (::qt_cast<QListView*>(w->parentWidget()))
//                     headerSortID = ((QListView*)w->parentWidget())->sortColumn();
//    	if (w->parentWidget()) qWarning(w->parentWidget()->className());
		w->setBackgroundMode( NoBackground );
		w->installEventFilter( this );
		goto kstpolish;
	}
        //if (testWidget) qWarning("testwidget stage %d",testInt++);

	
	if (::qt_cast<QTabBar*>(w)){
//                qWarning("installing tab event");
		w->setMouseTracking( true );
		w->installEventFilter( this );
                w->setBackgroundOrigin(QWidget::WindowOrigin);
		goto kstpolish;
	}
        
   if (::qt_cast<QGroupBox*>(w) && ((QGroupBox*)w)->frameShape() == QFrame::GroupBoxPanel && !((QGroupBox*)w)->isFlat())
   {
      if ( optionHandler->drawGroupBoxShadow() )
      {
            w->setPaletteBackgroundPixmap ( *groupShadow );
         if (((QGroupBox*)w)->isCheckable())
         {
            QWidget *cb = (QWidget*)w->child("qt_groupbox_checkbox",0,false);
            cb->setPalette(w->parentWidget() ? w->parentWidget()->palette() : qApp->palette());
            QFont font( cb->font() );
            font.setBold( true );
            cb->setFont( font );
         }
      }
      w->setBackgroundOrigin(QWidget::WindowOrigin);
      w->installEventFilter(this);
   }
   //if (testWidget) qWarning("testwidget stage %d",testInt++);


	if (w->inherits("QProgressBar"))
	{
		w->setBackgroundMode( NoBackground );
               w->installEventFilter(this);
                connect(w, SIGNAL(destroyed(QObject*)), this, SLOT(progressBarDestroyed(QObject*)));
		goto kstpolish;
	}
	
	if ( w->inherits( "QDockWindow" ) ) {
		w->setBackgroundMode( QWidget::PaletteBackground );
		w->setBackgroundOrigin(QWidget::WindowOrigin);
		w->installEventFilter( this );
		goto kstpolish ;
	}

	if ( ::qt_cast<QScrollBar*>(w)) {
		QCursor tmpCursor(Qt::PointingHandCursor);
                w->setCursor(tmpCursor);
		w->installEventFilter( this );
		w->setBackgroundMode( QWidget::NoBackground );
		goto kstpolish ;
	}
        if ( w->inherits( "KdetvView")) {
            w->setBackgroundMode( QWidget::NoBackground );
        }
   //if (testWidget) qWarning("testwidget stage %d",testInt++);
   kstpolish:	KStyle::polish( w ); // nothing of all above? do default kde stuff
}



void LiquidStyle::unPolish( QWidget *w ) {

	bool isViewport;
	bool isViewportChild;
        
        // general - better than querying for each and ever widget if it got one...
        w->removeEventFilter( this );
        

	if (w->inherits("KColorGrid")){
            goto kstunpolish;
        }
        
        if ( w->inherits("QToolBar") || qstrcmp( w->name(), "kde toolbar widget" ) == 0 ) {
		w->setBackgroundOrigin( QWidget::WidgetOrigin );
// 		w->unsetPalette();
		goto kstunpolish;
		}
	if ( w->inherits( "QPopupMenu" ) ) {
		w->unsetPalette();
		w->setBackgroundMode( QWidget::PaletteButton );
		w->removeEventFilter( this );
                XDeleteProperty (qt_xdisplay(), w->winId(), XInternAtom(qt_xdisplay(), OPACITY, False));
		goto kstunpolish;
		}
        if (::qt_cast<QListBox*>(w) && w->isTopLevel()){
            XDeleteProperty (qt_xdisplay(), w->winId(), XInternAtom(qt_xdisplay(), OPACITY, False));
            goto kstunpolish;
        }
	if ( !isPlain () && w->inherits( "KonqIconViewWidget" ) ||
	        w->inherits( "KHTMLView" ) ) {
		w->unsetPalette();
		goto kstunpolish;
		}
	if (w->inherits("KateView") || w->inherits("TEWidget") || w->inherits("CHexViewWidget")){
//		w->setMouseTracking( false );
		goto kstunpolish;
	}

	if (w->isA( "QViewportWidget" ) || w->inherits( "QClipperWidget" ) )
		goto kstunpolish;

	if ( !isPlain() ) {
		if ( w->inherits( "KActiveLabel" ) || w->inherits( "QTipLabel" ) ) {
			w->unsetPalette();
			goto kstunpolish;
			}
		/*
		else if(w->inherits("KdmClock")){
		    ; // Check this!
		    return;
			}*/
		}
	else if ( w->inherits( "QMenuBar" ) ) {
		w->setBackgroundMode( QWidget::PaletteButton );
		if ( !isPlain() )
			w->setBackgroundOrigin( QWidget::WidgetOrigin );
		goto kstunpolish;
		}else if ( w->inherits( "QDockWindow" ) ) {
		w->setBackgroundMode( QWidget::PaletteButton );
		if ( !isPlain() )
			w->setBackgroundOrigin( QWidget::WidgetOrigin );
		goto kstunpolish;
		} else if ( w->inherits( "QComboBox" ) || w->inherits( "QPushButton" ) ) {
                    if (w->inherits( "QPushButton" ))
                    {
                        ButtonFadeInfo *bfi_ = bfi[w->winId()];
                        if (bfi_){
                        if (bfi_->timerId != 0)
                            w->killTimer(bfi_->timerId);
                        bfi.setAutoDelete(TRUE);
                        bfi.remove(w->winId());
                        }
                    }
		  w->setBackgroundMode( QWidget::PaletteButton );
		} else if ( w->inherits( "QScrollBar" ) ) {
//		if (!w->isEnabled()) w->setEnabled(true);
		w->setBackgroundMode( QWidget::PaletteBackground );
		goto kstunpolish;
		} else if ( w->inherits( "QHeader" ) ) {
		w->setMouseTracking( false );
		}

	if ( w->inherits( "QToolButton" ) ) {
// 		w->unsetPalette();
		w->setBackgroundMode( QWidget::PaletteButton );
		if ( !isPlain() )
			w->setBackgroundOrigin( QWidget::WidgetOrigin );
		goto kstunpolish;
		}

	isViewport = qstrcmp( w->name(), "qt_viewport" ) == 0 ||
	                  qstrcmp( w->name(), "qt_clipped_viewport" ) == 0;
	isViewportChild = w->parent() &&
	                       ( ( qstrcmp( w->parent() ->name(), "qt_viewport" ) == 0 ) ||
	                         ( qstrcmp( w->parent() ->name(), "qt_clipped_viewport" ) == 0 ) );

	if ( isViewportChild ) {
		if ( w->inherits( "QRadioButton" ) || w->inherits( "QComboBox" ) || w->inherits( "QPushButton" ) ) {
			if ( isHTMLWidget( w ) ) {
				w->setBackgroundMode( QWidget::PaletteBackground );
				goto kstunpolish;
				}
			}
		}

	if ( !isPlain() ) {
		if ( !isViewport && w->parent() && qstrcmp( w->parent() ->name(), "proxyview" ) == 0 ) {
			w->setBackgroundOrigin( QWidget::WidgetOrigin );
			goto kstunpolish;
			}
		if ( w->ownPalette() && !w->inherits( "QButton" ) && !w->inherits( "QComboBox" ) )
			goto kstunpolish;
		if ( w->inherits( "PanelButtonBase" ) )
			goto kstunpolish;

		if ( !isViewport && !isViewportChild && !w->testWFlags( WType_Popup ) &&
		        !w->inherits( "KDesktop" ) && !w->inherits( "PasswordDlg" ) ) {

			if ( w->backgroundOrigin() == QWidget::WindowOrigin )
				w->setBackgroundOrigin( QWidget::WidgetOrigin );
			goto kstunpolish;
			}
		}
kstunpolish:	KStyle::unPolish( w );
	}
