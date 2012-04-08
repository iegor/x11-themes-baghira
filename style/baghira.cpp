#include "baghira.h"
#include "bitmaps.h"
#include "pixmaps.h"
//#include "../deco/baghiradecobuttons.h"

#include <qapplication.h>
#include <qbitmap.h>
#include <qbrush.h>
#include <qbutton.h>
#include <qcleanuphandler.h>
#include <qcolor.h>
#include <qcombobox.h>
//#include <qevent.h> 
#include <qfont.h>
#include <qframe.h>
#include <qgroupbox.h>
#include <qheader.h>
#include <qiconset.h>
#include <qimage.h>
#include <qlayout.h> 
#include <qlistbox.h>
#include <qlistview.h>
#include <qmenubar.h>
#include <qobject.h>
#include <qobjectlist.h>
#include <qpaintdevice.h>
#include <qpainter.h>
#include <qpalette.h>
#include <qpen.h>
#include <qpoint.h>
#include <qpopupmenu.h>
#include <qpushbutton.h>
//#include <qrangecontrol.h>
#include <qrect.h>
#include <qscrollbar.h>
#include <qscrollview.h>
#include <qsize.h>
#include <qslider.h>
#include <qstring.h>
#include <qstyleplugin.h>
#include <qtabbar.h>
#include <qtabwidget.h>
#include <qvariant.h>
#include <ktoolbar.h>
#include <ktoolbarbutton.h>
#include <kwordwrap.h>
#include "config.h"

#define PRINTDEVICE(p) qWarning("device is %s", (p->device()->devType() == QInternal::Widget) ?\
"Widget": (p->device()->devType() == QInternal::Pixmap) ?\
"Pixmap": (p->device()->devType() == QInternal::Printer) ?\
"Printer": (p->device()->devType() == QInternal::Picture) ?\
"Picture": (p->device()->devType() == QInternal::UndefinedDevice) ?\
"UndefinedDevice": "fuckdevice!" );

#define GCC_VERSION (__GNUC__ * 10000 \
+ __GNUC_MINOR__ * 100 \
+ __GNUC_PATCHLEVEL__)

static const int windowsItemFrame	= 1; // menu item frame width
static const int windowsSepHeight	= 2; // separator item height
static const int windowsItemHMargin	= 3; // menu item hor text margin
static const int windowsItemVMargin	= 1; // menu item ver text margin
static const int windowsArrowHMargin	= 6; // arrow horizontal margin
static const int windowsTabSpacing	= 12; // space between text and tab
static const int windowsCheckMarkHMargin	= 2; // horiz. margins of check mark
static const int windowsRightBorder	= 12; // right border on windows
static const int windowsCheckMarkWidth	= 12; // checkmarks width on windows

// options
int OptionHandler::menuBackground;
int OptionHandler::menuOpacity;
bool OptionHandler::menuColorButton;
bool OptionHandler::shadowText;
//color
QColor OptionHandler::customButtonColor, OptionHandler::menuStripeColor_;
//widgets
bool OptionHandler::customWidgetColor;
QColor OptionHandler::customColors[ 10 ];
//menu
bool OptionHandler::useCustomMenuColor;
QColor OptionHandler::color, OptionHandler::color2, OptionHandler::colorHigh, OptionHandler::fgColor, OptionHandler::fgColorHigh;
//Panel
// bool OptionHandler::panelCustom;
// QColor OptionHandler::customPanelColor;
//incativeButton
int OptionHandler::inactiveButtonColor;
QColor OptionHandler::customInactiveButtonColor;

//Design
//stipples
bool OptionHandler::bgStipple, /*OptionHandler::panelStipple,*/ OptionHandler::bevelHighlights_, OptionHandler::drawMenuStripe_, OptionHandler::glossyMenus_;
Style OptionHandler::style_;
int OptionHandler::contrast;
//shadows
bool OptionHandler::groupboxshadow;
int OptionHandler::shadowDarkness;
//istviews;
bool OptionHandler::drawDotlines, OptionHandler::useCustomExpanderColor;
int OptionHandler::expanderStyle;
QColor OptionHandler::expanderColor, OptionHandler::dotlineColor;
int OptionHandler::dotlineStyle;
//slider
bool OptionHandler::squeezesbslider;
bool OptionHandler::shadowsbslider;
bool OptionHandler::animateSlider;
bool OptionHandler::animateProgress;
//toolbar
bool OptionHandler::reverseBtnColor, OptionHandler::tbFrame;
//tweaks
bool OptionHandler::centerTabs;
bool OptionHandler::smoothListViewHeaders;
bool OptionHandler::smootherLVH;
bool OptionHandler::icyButtons;
Style OptionHandler::_buttonStyle;
int OptionHandler::progressBar;
bool OptionHandler::removeKickerBevel;
Style OptionHandler::_toolbuttonStyle;
bool OptionHandler::drawProgressLabel;
QColor OptionHandler::brushTint;
QColor OptionHandler::aDecoColor1_;
QColor OptionHandler::aDecoColor2_;
QColor OptionHandler::titleColor_[2];
QColor OptionHandler::titleButtonColor_[3];
bool OptionHandler::tintBrush;
tabStyle OptionHandler::tabStyle_;
unsigned long OptionHandler::wmDesign;
bool OptionHandler::animateButton;
int OptionHandler::custCols[8];

QPixmap LiquidStyle::popupBack;

//============

class KAccessToolBarButton : public KToolBarButton
{
// to get me access to the toolbbar
   friend class LiquidStyle;
public:
   KAccessToolBarButton( QWidget* parent=0, const char* name=0 )
      : KToolBarButton( parent, name ) {}
};


class LiquidStylePlugin : public QStylePlugin {
	public:
		LiquidStylePlugin() {
			;
			}
		~LiquidStylePlugin() {
			;
			}

		QStringList keys() const {
			return ( QStringList() << "Baghira" );
			}
		QStyle *create( const QString & );
	};

QStyle *LiquidStylePlugin::create( const QString &s ) {
	if ( s.lower() == "baghira" )
		return ( new LiquidStyle() );
	return ( 0 );
	}

Q_EXPORT_PLUGIN( LiquidStylePlugin )

static const int itemFrame = 1;
static const int itemHMargin = 3;
static const int itemVMargin = 1;
static const int arrowHMargin = 6;
static const int rightBorder = 12;


LiquidStyle::LiquidStyle() : KStyle( AllowMenuTransparency | FilledFrameWorkaround, ThreeButtonScrollBar )
{
   QIntDict<ButtonFadeInfo>bfi(31);// assuming 31 btns will be enough - a pointer list would be more accurate, but we need to lookup this every 50ms...
   QIntDict<ButtonTile>btnShadowedDict(223);// we need around 10/color, 3 colors/button assuming 10 different colors (e.g. kcalc)
   QIntDict<ButtonTile>buttonGlowDict(109);
   QIntDict<ButtonTile>buttonDict(331);
   headerHoverID = -1;
   headerSortID = -1;
	hoveredTabIndex = -1;
	currentTabBar = 0L;
   paintWidget = 0L;
	highlightWidget = 0L;
   currentTaskContainer = 0L;
	taskContainerHover = false;
	isTaskContainer = false;
	isHTMLButton = false;
	inExitPolish = false;
   inactiveRoundFrame = 0L;
   activeRoundFrame = 0L;
   eventKiller = new EventKiller;
   bmShower = 0L; // only if b/m

	btnDict.setAutoDelete( true );
	btnShadowedDict.setAutoDelete( true );
        bfi.setAutoDelete( true );
//	inverseTabDict.setAutoDelete( true );
//	tabDict.setAutoDelete( true );
	initialPaletteLoaded = false;
	qtrcModificationTime = 0;
	
	// animations kram!
	progAnimShift = 0;
	prog2AnimShift = 0;
	sliderAnimShift = 0;
	QTimer* timer = new QTimer( this );
	timer->start(50, false);
//       if (optionHandler->AnimateProgress())
         connect(timer, SIGNAL(timeout()), this, SLOT(updateProgressPos()));
	activeScroller = NULL;
	//--------------------------------------

	tooltipPalette = qApp->palette();
	tooltipPalette.setBrush( QColorGroup::Background, QColor( 255, 255, 220 ) );
	tooltipPalette.setBrush( QColorGroup::Foreground, Qt::black );

	rMatrix.rotate( 90.0 );
	lMatrix.rotate( 270.0 );
	iMatrix.rotate( 180.0 );
   mMatrix.setMatrix ( -1, 0, 0, 1, 0, 0);
   m2Matrix.setMatrix ( 1, 0, 0, -1, 0, 0);

   optionHandler = new OptionHandler( this );
   if (optionHandler->style() != Milk && optionHandler->AnimateSlider())
      connect(timer, SIGNAL(timeout()), this, SLOT(updateSliderPos()));
   if (optionHandler->style() == Brushed)
      bmShower = new BMShower(this);

   switch (optionHandler->buttonStyle())
   {
   case Jaguar:
      btnBorderImg = new QImage( uic_findImage("button-jaguar") );
      btnShadowImg = new QImage( uic_findImage( "button-shadow" ) );
      buttonGlow = new QImage( uic_findImage( "button-glow" ) );
      combo = new QImage( uic_findImage("combo-jaguar") );
      comboShadow = new QImage( uic_findImage( "combo-shadow" ) );
      if (optionHandler->toolbuttonStyle() == Brushed || !optionHandler->SmoothListViewHeaders())
         rectbutton = new QImage( uic_findImage( "rectbutton" ) );
      else  rectbutton = 0L;
      break;
   case Panther:
   case Tiger:
      btnBorderImg = new QImage( uic_findImage("button-base") );
      btnShadowImg = new QImage( uic_findImage( "button-shadow" ) );
      buttonGlow = new QImage( uic_findImage( "button-glow" ) );
      combo = new QImage( uic_findImage("combo") );
      comboShadow = new QImage( uic_findImage( "combo-shadow" ) );
      if (optionHandler->toolbuttonStyle() == Brushed || !optionHandler->SmoothListViewHeaders())
         rectbutton = new QImage( uic_findImage( "rectbutton" ) );
      else  rectbutton = 0L;
      break;
   case Brushed:
      btnBorderImg = 0L;
      btnShadowImg = 0L;
      buttonGlow = 0L;
      rectbutton = new QImage( uic_findImage( "rectbutton" ) );
      switch (optionHandler->toolbuttonStyle())
      {
      case Jaguar:
         combo = new QImage( uic_findImage("combo-jaguar") );
         comboShadow = new QImage( uic_findImage( "combo-shadow" ) );
         break;
      case Panther:
      case Tiger:
      case Brushed:
         combo = new QImage( uic_findImage("combo") );
         comboShadow = new QImage( uic_findImage( "combo-shadow" ) );
         break;
      case Milk:
         combo = new QImage( uic_findImage("combo-milk") );
         comboShadow = new QImage( uic_findImage( "combo-shadow" ) );
      default:
         break;
      }
      break;
   case Milk:
      btnBorderImg = new QImage( uic_findImage("button-milk") );
      btnShadowImg = 0L;
      buttonGlow = 0L;
      combo = new QImage( uic_findImage("combo-milk") );
      comboShadow = new QImage( uic_findImage( "combo-shadow" ) );
      if (optionHandler->toolbuttonStyle() == Brushed || !optionHandler->SmoothListViewHeaders())
         rectbutton = new QImage( uic_findImage( "rectbutton" ) );
      else  rectbutton = 0L;
   default:
      break;
   }
   
   if ( btnBorderImg && btnBorderImg->depth() < 32 )
		* btnBorderImg = btnBorderImg->convertDepth( 32 );
   
   if ( btnShadowImg && btnShadowImg->depth() < 32 )
      * btnShadowImg = btnShadowImg->convertDepth( 32 );
   
   if ( buttonGlow && buttonGlow->depth() < 32 )
      * buttonGlow = buttonGlow->convertDepth( 32 );
   
   if ( combo && combo->depth() < 32 )
      * combo = combo->convertDepth( 32 );
   
   if ( comboShadow && comboShadow->depth() < 32 )
      * comboShadow = comboShadow->convertDepth( 32 );
   
   if ( rectbutton && rectbutton->depth() < 32 )
      * rectbutton = rectbutton->convertDepth( 32 );

   if (optionHandler->style() == Milk)
   {
      slider_top = new QImage( uic_findImage( "sbslider_top-milk" ) );
      slider_btm = new QImage( uic_findImage( "sbslider_btm-milk" ) );
   }
   else
   {
      slider_top = new QImage( uic_findImage( "sbslider_top" ) );
      slider_btm = new QImage( uic_findImage( "sbslider_btm" ) );
   }
   
	if ( slider_top->depth() < 32 )
		* slider_top = slider_top->convertDepth( 32 );

	if ( slider_btm->depth() < 32 )
		* slider_btm = slider_btm->convertDepth( 32 );

   if (optionHandler->shadowSbSlider())
   {
      slider_top_shd = new QImage( uic_findImage( "sbslider_top_shd" ) );
      if ( slider_top_shd->depth() < 32 )
         * slider_top_shd = slider_top_shd->convertDepth( 32 );
   
      slider_btm_shd = new QImage( uic_findImage( "sbslider_btm_shd" ) );
      if ( slider_btm_shd->depth() < 32 )
         * slider_btm_shd = slider_btm_shd->convertDepth( 32 );
   }
   else
   {
      slider_top_shd = 0L;
      slider_btm_shd = 0L;
   }

	roundFrame = new QImage( uic_findImage( "roundFrame" ) );
   if ( roundFrame->depth() < 32 )
      * roundFrame = roundFrame->convertDepth( 32 );
	
	menuPix = NULL;
	tmpBtnPix = NULL;

	int i;
	for ( i = 0; i < BITMAP_ITEMS; ++i ) {
		pixmaps[ i ] = NULL;
		}

	sbLeft = new QBitmap( 6, 7, sbarrow_left_bits, true );
	sbLeft->setMask( *sbLeft );
	sbRight = new QBitmap( 6, 7, sbarrow_right_bits, true );
	sbRight->setMask( *sbRight );
	sbUp = new QBitmap( 7, 6, sbarrow_up_bits, true );
	sbUp->setMask( *sbUp );
	sbDown = new QBitmap( 7, 6, sbarrow_down_bits, true );
	sbDown->setMask( *sbDown );
        checkmark = new QBitmap( 6, 7, checkmark_bits, true );
	checkmark->setMask( *checkmark );

   isKicker = ( qstrcmp( qApp->argv() [ 0 ], "kicker" ) == 0 ) ||
	           ( qstrcmp( qApp->argv() [ 0 ], "appletproxy" ) == 0 );
   isOOO = !qstrcmp( qApp->argv() [ 0 ], "soffice.bin" );

	groupShadow = new QPixmap( 64, 64 );
   QColor c;
   optionHandler->custCols[0] != -1 ? c = QColor(optionHandler->custCols[0]) : c= QColor(qApp->palette().color(QPalette::Active, isKicker?QColorGroup::Button:QColorGroup::Background));

	groupShadow->fill( ( c.dark( 100 + optionHandler->ShadowDarkness() ) ).rgb() );
   if ( (optionHandler->style() != Brushed) && optionHandler->useBgStipple() ) {
		QPainter p;
		p.begin( groupShadow );
		p.setPen( c.dark( 100 + optionHandler->ShadowDarkness() + (int)(optionHandler->stippleContrast()/2) ) );
		for ( int i = 1; i < 64; i += 4 ) {
			p.drawLine( 0, i, 63, i );
			p.drawLine( 0, i+2, 63, i+2 );
		}
                p.setPen( c.dark( 100 + optionHandler->ShadowDarkness() + optionHandler->stippleContrast()) );
                for ( int i = 2; i < 63; i += 4 ) {
                        p.drawLine( 0, i, 63, i );
                        }
	}
        
   if (optionHandler->style() == Brushed)
   {
      plainBrush = optionHandler->TintBrush() ?
         *tintBrush(uic_findImage( "brushed-tile" ), optionHandler->BrushTint()) :
         uic_findImage( "brushed-tile" );
      gradBrush = optionHandler->TintBrush() ?
         *tintBrush(uic_findImage( "brushed-gradient" ), optionHandler->BrushTint()) :
         uic_findImage( "brushed-gradient" );
   }
}

LiquidStyle::~LiquidStyle() {
// 	btnDict.setAutoDelete(true);
	btnDict.clear();
// 	btnShadowedDict.setAutoDelete(true);
	btnShadowedDict.clear();
// 	buttonGlowDict.setAutoDelete(true);
	buttonGlowDict.clear();
// 	comboDict.setAutoDelete(true);
	comboDict.clear();
// 	comboShadowedDict.setAutoDelete(true);
	comboShadowedDict.clear();
//	if (! tabDict.isEmpty()) tabDict.clear();
//	if (! inverseTabDict.isEmpty()) inverseTabDict.clear();
// 	rectBtnDict.setAutoDelete(true);
	rectBtnDict.clear();
// 	progress2Dict.setAutoDelete(true);
	progress2Dict.clear();
//         bfi.setAutoDelete(true);
   bfi.clear();
//         fadeColorMap.setAutoDelete(true);
   fadeColorMap.clear();

	delete optionHandler;

	delete sbUp;
	delete sbDown;
	delete sbLeft;
	delete sbRight;
   delete checkmark;


	if ( btnBorderImg )
		delete btnBorderImg;
	if ( btnShadowImg )
		delete btnShadowImg;
   if (combo)
      delete combo;
   if (comboShadow)
      delete comboShadow;
   if (buttonGlow)
      delete buttonGlow;
   delete slider_top;
   delete slider_btm;
   if (slider_top_shd)
      delete slider_top_shd;
   if (slider_btm_shd)
      delete slider_btm_shd;
   if (rectbutton)
      delete rectbutton;
   delete roundFrame;
   
	if ( tmpBtnPix )
		delete tmpBtnPix;
	if ( menuPix )
		delete menuPix;

	int i;
	for ( i = 0; i < BITMAP_ITEMS; ++i ) {
		if ( pixmaps[ i ] )
			delete pixmaps[ i ];
		}
	if ( groupShadow )
		delete groupShadow;

	}
	
inline void LiquidStyle::updateProgressPos()
{
    progAnimShift = (progAnimShift + 1) % 20;
    prog2AnimShift = (prog2AnimShift + 1) % 38;
    //Update the registered progressbars.
    QMap<QWidget*, int>::iterator iter;
    for (iter = progAnimWidgets.begin(); iter != progAnimWidgets.end(); iter++)
    {
        if ( !::qt_cast<QProgressBar*>(iter.key()) ) 
            continue;
        (iter.key())->repaint(false);
    }
}

inline void LiquidStyle::updateSliderPos(){
	sliderAnimShift = (sliderAnimShift + 1) % 32;
        if (activeScroller)
            activeScroller->repaint(false);
}

void LiquidStyle::progressBarDestroyed(QObject* obj){
    progAnimWidgets.remove(static_cast<QWidget*>(obj));
}

bool LiquidStyle::isPlain() const {
	return ( !optionHandler->useBgStipple() );
	}


void LiquidStyle::drawPrimitive( PrimitiveElement pe, QPainter *p,
                                 const QRect &r, const QColorGroup &cg,
                                 SFlags flags, const QStyleOption &opt ) const
{
   bool down = flags & Style_Down;
   bool on = flags & Style_On;
   
   switch ( pe )
   {
   case PE_TabBarBase:
   {
      bool isDialog = false;
      if (p->device() && p->device()->devType() == QInternal::Widget)
      {
         QWidget *w = (QWidget*)p->device();
         if (w && ::qt_cast<QTabWidget*>(w) && w->children())
         {
            QObjectListIt it( *w->children() ); // iterate over the buttons
            QObject *obj;
            QTabBar *tabBar = 0L;
            while ( (obj = it.current()) != 0 )
            {
               ++it;
               if (::qt_cast<QTabBar*>(obj))
               {
                  tabBar = (QTabBar*)obj;
                  break;
               }
            }
            if (!tabBar || !tabBar->isVisible())
               break;
         }
         isDialog = optionHandler->TabStyle() == Clever && w->topLevelWidget()->inherits("QDialog");
      }
      if (isDialog || optionHandler->TabStyle() == Chooser)
      {
         int x = r.x(); int right = r.right(); int bottom = r.bottom(); int y2 = r.y() + 1;
         QColor bright( cg.background().dark( 110 ) );
         QColor mid( cg.background().dark( 130 ) );
         QColor dark( cg.background().dark( 150 ) );
         p->setPen( dark );
         p->drawLine( x + 6, y2, right - 6, y2 );
         p->drawPoint(x+3,y2+1);p->drawPoint(x+4,y2+1);
         p->drawPoint(x+2,y2+2);
         p->drawPoint(x+1,y2+3);
         p->drawPoint(right-3,y2+1);p->drawPoint(right-4,y2+1);
         p->drawPoint(right-2,y2+2);
         p->drawPoint(right-1,y2+3);
         p->drawLine(x,y2+5,x,bottom);
         p->drawLine(right,y2+5,right,bottom);
         
         p->setPen( mid );
         p->drawPoint(x+4, y2);p->drawPoint(x+5,y2);
         p->drawPoint(right-4, y2);p->drawPoint(right-5,y2);
         p->drawPoint(x+2, y2+1);
         p->drawPoint(right-2, y2+1);
         p->drawLine(x+5,y2 + 1, right - 5, y2 + 1);
         p->drawPoint(x+1,y2+2);
         p->drawPoint(right-1,y2+2);
         p->drawPoint(x+2,y2+3);
         p->drawPoint(right-2,y2+3);
         p->drawPoint(x,y2+4);p->drawPoint(x+1,y2+4);
         p->drawPoint(right,y2+4);p->drawPoint(right+1,y2+4);
         
         p->setPen(bright);
         p->drawPoint(x+3,y2);p->drawPoint(right-3,y2);
         p->drawLine(x+4,y2+2,right-4,y2+2);
         p->drawPoint(x,y2+3);p->drawPoint(x+3,y2+3);
         p->drawPoint(right,y2+3);p->drawPoint(right-3,y2+3);
         p->drawPoint(x+2,y2+4);p->drawPoint(right-2,y2+4);
         p->drawLine(x+1,y2+5,x+1,bottom);
         p->drawLine(right-1,y2+5,right-1,bottom);
         break;
      }
      else
      {
         drawCombo( p, cg, cg.button(), r.x()+1, r.y()+1, r.width()-2, r.height()+1, false, false, false, center);
         QPen oldPen = p->pen();
//          p->setClipping ( false );
         p->setPen( cg.button().dark( 158 ) );
         p->drawRect(r);
//          p->fillRect(r, Qt::red);
//          p->flush();
         p->setPen(oldPen);
      }
      break;
   }
   case PE_ButtonCommand:
   case PE_ButtonDefault:
   {
      bool sunken = on || down;
      bool hover = flags & Style_MouseOver;
      bool focused = flags & Style_HasFocus;
      QColor newColor = sunken ? cg.button().dark( 110 ) : hover || focused ? cg.button() : optionHandler->InactiveButtonColor();
      if (optionHandler->AnimateButton() && (sunken || hover || focused))
      {
         ButtonFadeInfo *bfi_ = /*(::qt_cast<QWidget*>(*/opt.widget()/*))*/ ? bfi[opt.widget()->winId()] : 0L;
         if (bfi_)
         {
            int index = bfi_->index;
            newColor = mapFadeColor(newColor, index);
         }
      }

      if (optionHandler->buttonStyle() == Brushed)
      {
         drawRectangularButton( p, cg, newColor, r.x(), r.y(), r.width(), r.height(), sunken, hover, false, full);
      }
      else if ( ( r.width() < 21 || r.height() < 25 ))
      {
         drawCombo( p, cg, newColor, r.x(), r.y(), r.width(), r.height()+2, sunken, hover, false, center, isHTMLButton );
         QPen oldpen(p->pen());
         p->setPen( newColor.dark(150));
         p->drawRect(r.x(), r.y(), r.width(), r.height());
         if (sunken)
         {
            p->setPen(newColor.dark(180));
            p->drawRect(r.x()+1, r.y()+1, r.width()-2, r.height()-2);
         }
         p->setPen(oldpen);
         break;
      }
      else if (optionHandler->buttonStyle() == Milk)
         drawCombo( p, cg, newColor, r.x(), r.y(), r.width(), r.height(), sunken, hover, false, full, isHTMLButton );
      else
      {
         drawRoundButton( p, cg, newColor, r.x(), r.y(), r.width(), r.height(), sunken, hover, btnOffset.x(), btnOffset.y() );
      }
      break;
   }
   case PE_ButtonBevel:
   case PE_ButtonDropDown:
   {
      bool sunken = on || down;
      bool hover = flags & Style_MouseOver;
      if (optionHandler->buttonStyle() == Brushed)
         drawRectangularButton( p, cg, sunken ? cg.button().dark( 110 ) : hover ? cg.button() : optionHandler->InactiveButtonColor(), r.x(), r.y(), r.width(), r.height(), sunken, hover, false, full);
      else
         drawCombo( p, cg, sunken ? cg.button().dark( 110 ) : hover ? cg.button() : optionHandler->InactiveButtonColor(), r.x(), r.y(), r.width(), r.height(), sunken, hover, false, full );
      break;
   }
   case PE_ButtonTool:
   {
      bool sunken = on || down;
      bool hover = flags & Style_MouseOver;
      if (optionHandler->buttonStyle() == Brushed)
         drawRectangularButton( p, cg, sunken ? cg.button().dark( 110 ) : hover ? cg.button() : optionHandler->InactiveButtonColor(), r.x(), r.y(), r.width(), r.height(), sunken, hover, false, full);
      else
         drawCombo( p, cg, sunken ? cg.button().dark( 110 ) : hover ? cg.button() : optionHandler->InactiveButtonColor(), r.x(), r.y(), r.width(), r.height(), sunken, hover, false, full );
      break;
   }
   case PE_HeaderSection:
   {
      bool sunken = on || down;
      bool hover = flags & Style_MouseOver;
      QPen oldpen(p->pen());
      if ( isTaskContainer ) // is kicker?
      {
         if (optionHandler->SmoothListViewHeaders())
         {
         drawCombo( p, cg, sunken ? cg.button() : taskContainerHover ? cg.button().light( 110 ) : cg.background(), r.x(), r.y(), r.width(), isKicker ? r.height() + 2 : r.height(), sunken, hover, false, optionHandler->SmootherLVH() ? full : center );
            if (isKicker && !optionHandler->SmootherLVH())
            {
            p->setPen(cg.background().dark(150));
            p->drawRect(r.x(), r.y(), r.width(), r.height());
               if (sunken)
               {
                  p->setPen(cg.background().dark(200));
                  p->drawRect(r.x()+1, r.y()+1, r.width()-2, r.height()-2);
               }
            }
         }
         else
         {
         drawRectangularButton( p, cg, sunken ? cg.button() : taskContainerHover ? cg.button().light( 110 ) : cg.background(), r.x(), r.y(), r.width(), r.height(), sunken, hover, false, center);
            p->setPen(cg.background().dark(150));
            p->drawLine(r.x(),r.y()+1,r.x(),r.height()-2);
         }
         const_cast<LiquidStyle*>( this ) ->isTaskContainer = false;
         const_cast<LiquidStyle*>( this ) ->taskContainerHover = false;
      }
      else
      {
         if ( (p->device() && p->device()->devType() == QInternal::Widget) && ::qt_cast<QHeader*>((QWidget*)p->device()) )
         {
            QHeader *header = (QHeader*)p->device();
            int id = header->orientation() == Qt::Horizontal ? header->sectionAt( r.x() + 2 + header->offset() ) : header->sectionAt( r.y() + 2 + header->offset() );
            hover = ((id != -1) && ((currentHeader == header && id == headerHoverID ) || id == headerSortID));
            if (header->mapToIndex(id) == header->count()-1 && (r.bottom() < header->rect().bottom() || r.right() < header->rect().right()))
            {
               bool hadClip = p->hasClipping();
               p->setClipping( false );
               if (header->orientation() == Qt::Horizontal)
                  p->fillRect(r.right()+1, r.y(), header->width()-r.right(), r.height(), cg.base());
               else
                  p->fillRect(r.x(), r.bottom()+1, r.width(), header->height()-r.bottom(), cg.base());
               p->setClipping( hadClip );
            }
         }
         if (optionHandler->SmoothListViewHeaders())
         {
            drawCombo( p, cg, sunken ? cg.button().dark( 110 ) : hover ? cg.button() : optionHandler->InactiveButtonColor(), r.x(), r.y(), r.width(), r.height(), sunken, hover, false, center);
         }
         else
            drawRectangularButton( p, cg, sunken ? cg.button().dark( 110 ) : hover ? cg.button() : optionHandler->InactiveButtonColor(), r.x(), r.y(), r.width(), r.height(), sunken, hover, false, center);
      }
      p->setPen(oldpen);
      break;
   }
   case PE_FocusRect:
   {
      p->drawWinFocusRect( r );
      break;
   }
   case PE_ScrollBarSlider:
   case PE_ScrollBarAddPage:
   case PE_ScrollBarSubPage:
   {
      QColor sbBgColor = optionHandler->useCustomColors() ?
                           optionHandler->customColor( CustomSBGroove ) :
                           qApp->palette().active().background();

      bool isActive = optionHandler->ScrollerActive();
      bool isHover = currentScrollBar->hasMouse();
      bool isPressed = currentScrollBar->draggingSlider();
      bool drawSlider = currentScrollBar->minValue() < currentScrollBar->maxValue();
      
      // horizontal slider
      if ( flags & Style_Horizontal )
      {
         int extent = currentScrollBar->height();
         QRect bgR( extent, 0, currentScrollBar->width() - extent * 3 + 1, extent );
         if ( sbBuffer.size() != currentScrollBar->size() )
            const_cast<LiquidStyle*>( this )->sbBuffer.resize( currentScrollBar->size() );
         QPainter painter;
         painter.begin( &sbBuffer );
         // pixmaps
         if (drawSlider)
         {
            painter.drawPixmap( bgR.x(), bgR.y() + 1, *getPixmap( HSBSliderBtmBg ) );
            painter.drawTiledPixmap( bgR.x() + 7, bgR.y() + 1, bgR.width() - 15, 13, *getPixmap( HSBSliderMidBg ) );
            painter.drawPixmap( bgR.right() - 8, bgR.y() + 1, *getPixmap( HSBSliderTopBg ) );
         }
         else
            painter.drawTiledPixmap( bgR.x(), bgR.y() + 1, bgR.width(), 13, *getPixmap( HSBSliderMidBg ) );
         
         // lines
         painter.setPen( sbBgColor.dark( 110 ) );
         painter.drawLine( bgR.x(), bgR.y(), bgR.right(), bgR.y());
         painter.drawLine( bgR.x(), bgR.bottom(), bgR.right(), bgR.bottom());

         if ( pe == PE_ScrollBarSlider && drawSlider)
         {
         int offset = isPressed * ( optionHandler->squeezeSbSlider() + optionHandler->shadowSbSlider()*2);
         int offset2 = optionHandler->shadowSbSlider()*2 + (isPressed && optionHandler->squeezeSbSlider());

         painter.drawPixmap( r.x() + offset, r.y() + 1,
               isPressed ? *getPixmap( HSBSliderBtmPressed ) :
               (isHover || isActive || isOOO) ? *getPixmap( HSBSliderBtmActive ) :
               *getPixmap( HSBSliderBtm));

         painter.drawPixmap( r.right() - 6 - offset2, r.y() + 1,
               isPressed ? *getPixmap( HSBSliderTopPressed ) :
               (isHover || isActive || isOOO) ? *getPixmap( HSBSliderTopActive ) :
               *getPixmap( HSBSliderTop ) );
         // mid part last, as i have to redefine clip section
         painter.setClipRect(r.x() + 7 + offset2, r.y() + 1, r.width() - 14 - 2*offset2, 13, QPainter::CoordPainter);
         painter.drawTiledPixmap( r.x() + 7 + offset2 + ((isHover && !isPressed) ? (sliderAnimShift - 32) : 0), r.y() + 1, r.width() - 14 - 2*offset2 - ((isHover && !isPressed) ?
               (sliderAnimShift - 32) : 0), 13, isPressed ? *getPixmap( HSBSliderMidPressed ) :
               (isHover || isActive || isOOO) ? *getPixmap( HSBSliderMidActive ) :
               *getPixmap( HSBSliderMid ), isPressed?r.x() : 0, 0);
         }

         painter.end();
      }

   // vertical slider
      else
      {
         int extent = currentScrollBar->width();
         QRect bgR( 0, extent, extent, currentScrollBar->height() - extent * 3 + 1 );
   
         if ( sbBuffer.size() != currentScrollBar->size() )
            const_cast<LiquidStyle*>( this ) -> sbBuffer.resize( currentScrollBar->size() );
         QPainter painter;
         painter.begin( &sbBuffer ); // set painter to pixelbuffer
         if (drawSlider)
         {
            // top slidergroove rounder
            painter.drawPixmap( bgR.x() + 1, bgR.y(), *getPixmap( VSBSliderTopBg ) );
            // the groove
            painter.drawTiledPixmap( bgR.x() + 1, bgR.y() + 8, 13, bgR.height() - 15, *getPixmap( VSBSliderMidBg ) );
            // bottom slidergroove rounder
            painter.drawPixmap( bgR.x() + 1, bgR.bottom() - 7, *getPixmap( VSBSliderBtmBg ) );
         }
         else
            painter.drawTiledPixmap( bgR.x() + 1, bgR.y(), 13, bgR.height(), *getPixmap( VSBSliderMidBg ) );
         
         painter.setPen( sbBgColor.dark( 110 ) );
         painter.drawLine( bgR.x(), bgR.y(), bgR.x(), bgR.bottom() );
         painter.drawLine( bgR.right(), bgR.y(), bgR.right(), bgR.bottom() );
   
         // here we are, now the slider
         if ( pe == PE_ScrollBarSlider && drawSlider )
         {
            int offset = isPressed * ( optionHandler->squeezeSbSlider() + optionHandler->shadowSbSlider()*2);
            int offset2 = (isPressed * optionHandler->squeezeSbSlider()) + optionHandler->shadowSbSlider()*2;
            
            painter.drawPixmap( r.x() + 1, r.y() + offset, isPressed ?
                  *getPixmap( VSBSliderTopPressed ) :
                  (isHover || isActive || isOOO) ? *getPixmap( VSBSliderTopActive ) :
                  *getPixmap( VSBSliderTop));
            painter.drawPixmap( r.x() + 1, r.bottom()  - 6 - offset2, isPressed ?
                  *getPixmap( VSBSliderBtmPressed ) :
                  (isHover || isActive || isOOO) ? *getPixmap( VSBSliderBtmActive ) :
                  *getPixmap( VSBSliderBtm ) );
            // mid part last, as i have to redefine clipping section
            painter.setClipRect(r.x() + 1, r.y() + 7+offset2, 13, r.height() - 14 - 2*offset2, QPainter::CoordPainter);
            painter.drawTiledPixmap( r.x() + 1, r.y() + 7 + offset2 - ((isHover && !isPressed) ? sliderAnimShift : 0),
                  13, r.height() - 14 - 2*offset2 + ((isHover && !isPressed) ?
                  sliderAnimShift : 0), isPressed ? *getPixmap( VSBSliderMidPressed ) :
                  (isHover || isActive || isOOO) ? *getPixmap( VSBSliderMidActive ) :
                  *getPixmap( VSBSliderMid ),0,isPressed?r.y():0 );
         }
         painter.end();
      }
      p->drawPixmap( r.x(), r.y(), sbBuffer, r.x(), r.y(), r.width(), r.height() );
      break;
   }
   case PE_ScrollBarAddLine:
   {
      QColor c( optionHandler->useCustomColors() ?
            optionHandler->customColor( CustomSBGroove ) :
            qApp->palette().active().background() );
      QColor pixColor = flags & Style_Down ? qApp->palette().active().button() : c;
      
      bool drawSlider = currentScrollBar->minValue() < currentScrollBar->maxValue();  
      
      if ( flags & Style_Horizontal )
      {
         p->drawTiledPixmap( r.x(), r.y() + 1, r.width(), 13, *getPixmap(drawSlider ? HSBSliderMidBgI : HSBSliderMidBg));
         p->setPen( c.dark( 110 ) );
         p->drawLine( r.x(), r.y(), r.right(), r.y());
         p->drawLine( r.x(), r.bottom(), r.right(), r.bottom());
      }
      else
      {
         p->drawTiledPixmap( r.x() + 1, r.y(), 13, r.height(), *getPixmap(drawSlider ? VSBSliderMidBgI : VSBSliderMidBg));
         p->setPen( c.dark( 110 ) );
         p->drawLine( r.x(), r.y(), r.x(), r.bottom());
         p->drawLine( r.right(), r.y(), r.right(), r.bottom());
      }
      if (drawSlider)
      {
         p->setPen( flags & Style_Down ? cg.button() : QColor(85,85,85) );
         p->setBrush( flags & Style_Down ? cg.button() : QColor(85,85,85) );
         p->drawPixmap( r.x() + 4, r.y() + 4, flags & Style_Horizontal ? *sbRight : *sbDown );
      }
      break;
   }
   case PE_ScrollBarSubLine:
   {
      QColor c( optionHandler->useCustomColors() ?
            optionHandler->customColor( CustomSBGroove ) :
                  qApp->palette().active().background() );
      QColor pixColor = flags & Style_Down ? qApp->palette().active().button() : c;
      bool top = flags & Style_Horizontal ? r.x() == 0 : r.y() == 0;

      bool drawSlider = currentScrollBar->minValue() < currentScrollBar->maxValue();

      if ( flags & Style_Horizontal ) 
      {
         if ( top )
         {
            p->drawTiledPixmap( r.x(), r.y() + 1, r.width(), 13, *getPixmap(drawSlider ? HSBSliderMidBgI : HSBSliderMidBg));
         }
         else
         {
            p->drawTiledPixmap( r.x(), r.y() + 1, r.width(), 13, *getPixmap(drawSlider ? HSBSliderMidBgI : HSBSliderMidBg));
         }
         p->setPen( c.dark( 110 ) );
         p->drawLine( r.x(), r.y(), r.right(), r.y());
         p->drawLine( r.x(), r.bottom(), r.right(), r.bottom());
      }
      else
      {
         if ( top )
         {
            p->drawTiledPixmap( r.x() + 1, r.y(), 13, r.height(), *getPixmap(drawSlider ? VSBSliderMidBgI : VSBSliderMidBg));
         }
         else
         {
            p->drawTiledPixmap( r.x() + 1, r.y(), 13, r.height(), *getPixmap(drawSlider ? VSBSliderMidBgI : VSBSliderMidBg));
         }
         p->setPen( c.dark( 110 ) );
         p->drawLine( r.x(), r.y(), r.x(), r.bottom());
         p->drawLine( r.right(), r.y(), r.right(), r.bottom());
      }
      if (drawSlider)
      {
         p->setPen( flags & Style_Down ? cg.button() : QColor(85,85,85) );
         p->setBrush( flags & Style_Down ? cg.button() : QColor(85,85,85) );
         if (currentScrollBar->minValue() < currentScrollBar->maxValue()) p->drawPixmap( r.x() + 4, r.y() + 4, flags & Style_Horizontal ? *sbLeft : *sbUp );
      }
      break;
   }
   case PE_Indicator:
   {
      bool hover = flags & Style_MouseOver;
      bool pressed = flags & Style_Down;
//       bool isMasked = p->device() && p->device() ->devType() == QInternal::Widget && ( ( QWidget * ) p->device() ) ->autoMask();
#if 0
      if ( isMasked )
      {
         if ( !( flags & Style_Off ) )
            p->drawPixmap( r.x(), r.y(),
                  hover ? *getPixmap( HTMLCBDownHover ) :
                  *getPixmap( HTMLCBDown ) );
         else
            p->drawPixmap( r.x(), r.y(), hover ?
                  *getPixmap( HTMLCBHover ) : *getPixmap( HTMLCB ) );
      }
      else
#endif
      {
         if ( flags & Style_On )
            p->drawPixmap( r.x(), r.y(),
                  pressed ? *getPixmap( CBDownPressed ) :
                  hover ? *getPixmap( CBDownHover ) :
                  *getPixmap( CBDown ) );
         else if ( flags & Style_Off )
            p->drawPixmap( r.x(), r.y(),
                  pressed ? *getPixmap( CBPressed ) :
                  hover ? *getPixmap( CBHover ) :
                  *getPixmap( CB ) );
         else //tristate
            p->drawPixmap( r.x(), r.y(),
                  pressed ? *getPixmap( CBTriPressed ) :
                  hover ? *getPixmap( CBTriHover ) :
                  *getPixmap( CBTri ) );
      }
      break;
   }
   case PE_IndicatorMask:
   {
      p->fillRect( r, Qt::color1 );
      break;
   }
   case PE_ExclusiveIndicator:
   {
      bool hover = flags & Style_MouseOver;
      bool pressed = flags & Style_Down;

         if ( on || down )
            p->drawPixmap( r.x(), r.y(),
                  pressed ? *getPixmap( RadioOnPressed ) :
                  hover ? *getPixmap( RadioOnHover ) :
                  *getPixmap( RadioOn ) );
         else
            p->drawPixmap( r.x(), r.y(),
                  pressed ? *getPixmap( RadioOffPressed ) :
                  hover ? *getPixmap( RadioOffHover ) :
                  *getPixmap( RadioOff ) );
      break;
   }
   case PE_ExclusiveIndicatorMask:
   {
      break;
   }
   case PE_Splitter:
   case PE_DockWindowResizeHandle:
   {
      QPen oldpen(p->pen());
      QBrush oldbrush(p->brush());
      if (highlightWidget == p->device())
      {
         if (flags & Style_Horizontal)
         { //vertical?
            p->setPen(cg.mid());
            p->drawLine(r.x(), r.y(), r.x(), r.bottom());
            p->drawLine(r.right(), r.y(), r.right(), r.bottom());
            p->setPen(cg.button().dark(140));
            p->setBrush(cg.button().dark(140));
            p->drawEllipse(r.x()+1,r.y() + (int)(r.height() / 2.5), 4, 4);
         }
         else
         {
            p->setPen(cg.mid());
            p->drawLine(r.x(), r.y(), r.right(), r.y());
            p->drawLine(r.x(), r.bottom(), r.right(), r.bottom());
            p->setPen(cg.button().dark(140));
            p->setBrush(cg.button().dark(140));
            p->drawEllipse(r.x()+r.width()/2,r.y() + 1, 4, 4);
         }

      }
      else
      {
         if (flags & Style_Horizontal)
         {
            p->setPen(cg.mid());
            p->drawLine(r.x(), r.y(), r.x(), r.bottom());
            p->drawLine(r.right(), r.y(), r.right(), r.bottom());
            p->setPen(cg.background().dark(130));
            p->setBrush(cg.background().dark(150));
            p->drawEllipse(r.x()+1,r.y() + (int)(r.height() / 2.5), 4, 4);
         }
         else
         {
            p->setPen(cg.mid());
            p->drawLine(r.x(), r.y(), r.right(), r.y());
            p->drawLine(r.x(), r.bottom(), r.right(), r.bottom());
            p->setPen(cg.background().dark(130));
            p->setBrush(cg.background().dark(150));
            p->drawEllipse(r.x()+r.width()/2,r.y() + 1, 4, 4);
         }
      }
      p->setPen(oldpen);
      p->setBrush(oldbrush);
      break;
   }
//    case PE_BoxFrame:
   case PE_PanelGroupBox:
   case PE_GroupBoxFrame:
   {
      QColor bgColor;
      QWidget* w = (p->device() && p->device()->devType() == QInternal::Widget) ?
            (QWidget*)(p->device()) : 0;
      QGroupBox* gb = ::qt_cast<QGroupBox*>(w);
      
      optionHandler->drawGroupBoxShadow() ?
            bgColor = QColor(cg.background().dark( 100 + optionHandler->ShadowDarkness() )) :
            bgColor = QColor(cg.background());
      QColor bright( bgColor.dark( 110 ) );
      QColor mid( bgColor.dark( 130 ) );
      QColor dark( bgColor.dark( 150 ) );

      int titleHeight = (gb && !gb->title().isEmpty() && pe == PE_PanelGroupBox) ?
         p->fontMetrics().height()+(gb->isCheckable()?4:1) : 0;

      int x = r.x(); int right = r.right(); int bottom = r.bottom();
      int y2 = r.y();

      /***
      ok, to get the sunken look somwhat efficient and less glitchy
      we reset the groupbox brush and only repaint the title
      (saves us from several is item in groupbox checks as well)
      only problem left: draw the parents brush on the title window aligned...
      ***/

      if (gb && titleHeight)
      {
         bool hadClip = p->hasClipping();
         QRegion oldClip = p->clipRegion();
         if (gb->parentWidget())
         {
            const QPixmap *pix = gb->parentWidget()->paletteBackgroundPixmap();
            
            if (optionHandler->drawGroupBoxShadow())
            {
               QPointArray pa1(3);
               pa1.setPoint(0, x,y2);
               pa1.setPoint(1, x+5,y2);
               pa1.setPoint(2, x,y2+5);
               QPointArray pa2(3);
               pa2.setPoint(0, right-5,y2);
               pa2.setPoint(1, right,y2);
               pa2.setPoint(2, right,y2+5);
               QPointArray pa3(3);
               pa3.setPoint(0, right,bottom-5);
               pa3.setPoint(1, right,bottom);
               pa3.setPoint(2, right-5,bottom);
               QPointArray pa4(3);
               pa4.setPoint(0, x+5,bottom);
               pa4.setPoint(1, x,bottom);
               pa4.setPoint(2, x,bottom-5);
                  
               QRegion reg2(0,0, r.right(), titleHeight);
               p->setClipRegion(QRegion(pa1)+QRegion(pa2)+QRegion(pa3)+QRegion(pa4)+reg2);
            }
            else
               p->setClipRect(0,0,r.right(),titleHeight);
            pix ? p->drawTiledPixmap( QRect( 0, 0, w->width(), w->height()), *pix, w->backgroundOffset()) : p->fillRect(QRect(0, 0, w->width(), w->height()),w->parentWidget()->backgroundBrush());
         }
         if (gb && !gb->isCheckable())
         {
            QString title = gb->title();
            QFont font( p->font() );
            font.setBold( true );
            p->setFont( font );
            drawItem( p, QRect(10,0,r.width(),titleHeight), ShowPrefix, cg, flags & Style_Enabled, 0, title, -1, &cg.text() );
         }
         p->setClipRegion(oldClip);
         p->setClipping(hadClip);
      }
      
      //outer rect
      // top
      p->setPen( dark );
      if (pe == PE_PanelGroupBox)
      {
         p->drawLine( x + 6, y2, right - 6, y2 );
         // left points
         p->drawPoint(x+3,y2+1);p->drawPoint(x+4,y2+1);
         p->drawPoint(x+2,y2+2);
         p->drawPoint(x+1,y2+3);
         // right points
         p->drawPoint(right-3,y2+1);p->drawPoint(right-4,y2+1);
         p->drawPoint(right-2,y2+2);
         p->drawPoint(right-1,y2+3);
         // vertcal outer lines
         p->drawLine(x,y2+5,x,bottom-5);
         p->drawLine(right,y2+5,right,bottom-5);
      }
      else
      {
         // vertcal outer lines
         p->drawLine(x,y2,x,bottom-5);
         p->drawLine(right,y2,right,bottom-5);
      }
      
      p->setPen( mid );
      if (pe == PE_PanelGroupBox)
      {
      // top stuff
         p->drawPoint(x+4, y2);p->drawPoint(x+5,y2);
         p->drawPoint(right-4, y2);p->drawPoint(right-5,y2);
         p->drawPoint(x+2, y2+1);
         p->drawPoint(right-2, y2+1);
         p->drawLine(x+5,y2 + 1, right - 5, y2 + 1);
         p->drawPoint(x+1,y2+2);
         p->drawPoint(right-1,y2+2);
         p->drawPoint(x+2,y2+3);
         p->drawPoint(right-2,y2+3);
         p->drawPoint(x,y2+4);p->drawPoint(x+1,y2+4);
         p->drawPoint(right,y2+4);p->drawPoint(right+1,y2+4);
      }
      // bottom stuff
      p->drawPoint(x,bottom - 4); p->drawPoint(x+1,bottom - 4);
      p->drawPoint(right,bottom - 4); p->drawPoint(right-1,bottom - 4);
      p->drawPoint(x+1,bottom - 3);
      p->drawPoint(right-1,bottom - 3);
      p->drawPoint(x+1,bottom - 2); p->drawPoint(x+2,bottom - 2);
      p->drawPoint(right-1,bottom - 2); p->drawPoint(right-2,bottom - 2);
      p->drawPoint(x+2, bottom - 1); p->drawPoint(x+3, bottom - 1);
      p->drawPoint(right-2, bottom - 1); p->drawPoint(right-3, bottom - 1);
      p->drawLine(x + 4, bottom,right - 4, bottom);
      
      p->setPen(bright);
      // top stuff
      if (pe == PE_PanelGroupBox)
      {
         p->drawPoint(x+3,y2);p->drawPoint(right-3,y2);
         p->drawLine(x+4,y2+2,right-4,y2+2);
         p->drawPoint(x,y2+3);p->drawPoint(x+3,y2+3);
         p->drawPoint(right,y2+3);p->drawPoint(right-3,y2+3);
         p->drawPoint(x+2,y2+4);p->drawPoint(right-2,y2+4);
         // vertical lines
         p->drawLine(x+1,y2+5,x+1,bottom-5);
         p->drawLine(right-1,y2+5,right-1,bottom-5);
      }
      else
      {
         // vertical lines
         p->drawLine(x+1,y2,x+1,bottom-5);
         p->drawLine(right-1,y2,right-1,bottom-5);
      }
      // bottom stuff
      p->drawPoint(x, bottom-3);p->drawPoint(right, bottom-3);
      p->drawPoint(x+3, bottom);p->drawPoint(right-3, bottom);

      break;
   }
   case PE_StatusBarSection:
   {
      break;
   }
   case PE_WindowFrame:
   {
      drawEditFrame( p, r, cg, false, false, true );
      break;
   }
   case PE_Panel:
   {
      bool sunken = flags & Style_Sunken;
      if (!(isKicker && optionHandler->RemoveKickerBevel()))
         sunken ? drawEditFrame( p, r, cg ) : drawEditFrame( p, r, cg, false, false, true );
      break;
   }
   case PE_PanelTabWidget:
   {
      // don't ask
      bool isDialog = optionHandler->TabStyle() == Clever &&
      (p->device() && p->device()->devType() == QInternal::Widget)&&
      ((QWidget*)p->device())->topLevelWidget()->inherits("QDialog");
      
      QColor bright( cg.background().dark( 110 ) );
      QColor mid( cg.background().dark( 130 ) );
      QColor dark( cg.background().dark( 150 ) );
      int x = r.x(); int right = r.right(); int bottom = r.bottom(); int y2 = r.y();
      if (isDialog || optionHandler->TabStyle() == Chooser)
      {
         //outer rect
         // top
         p->setPen( dark );
         // vertcal outer lines
         p->drawLine(x,y2,x,bottom-5);
         p->drawLine(right,y2,right,bottom-5);
         
         p->setPen( mid );
         // bottom stuff
         p->drawPoint(x,bottom - 4); p->drawPoint(x+1,bottom - 4);
         p->drawPoint(right,bottom - 4); p->drawPoint(right-1,bottom - 4);
         p->drawPoint(x+1,bottom - 3);
         p->drawPoint(right-1,bottom - 3);
         p->drawPoint(x+1,bottom - 2); p->drawPoint(x+2,bottom - 2);
         p->drawPoint(right-1,bottom - 2); p->drawPoint(right-2,bottom - 2);
         p->drawPoint(x+2, bottom - 1); p->drawPoint(x+3, bottom - 1);
         p->drawPoint(right-2, bottom - 1); p->drawPoint(right-3, bottom - 1);
         p->drawLine(x + 4, bottom,right - 4, bottom);
         
         p->setPen(bright);
         // top stuff
         // vertical lines
         p->drawLine(x+1,y2,x+1,bottom-5);
         p->drawLine(right-1,y2,right-1,bottom-5);
         
         // bottom stuff
         p->drawPoint(x, bottom-3);p->drawPoint(right, bottom-3);
         p->drawPoint(x+3, bottom);p->drawPoint(right-3, bottom);
         break;
      }
      else
      {
         p->setPen( mid );
         p->drawLine( x, y2, x, bottom-2 );
         p->drawLine( right, y2, right, bottom-2 );
         p->setPen( dark );
         p->drawLine( x, bottom-1, right, bottom-1 );
         p->drawLine( x, bottom, right, bottom );
         break;
      }
   }
   case PE_PanelLineEdit:
   {
      bool focused = flags & Style_HasFocus;
      bool isHTML = p->device() && p->device()->devType() == QInternal::Widget && isHTMLWidget((QWidget*)p->device());
      drawEditFrame( p, r, cg, isHTML, focused, false);
      break;
   }
   case PE_PanelPopup:
   {
      int x, y, w , h;
      r.rect( &x, &y, &w, &h );
      QColor c( optionHandler->UseCustomMenuColor()?
            optionHandler->Color():
            optionHandler->MenuColorButton()?
            optionHandler->buttonColor():
            optionHandler->bgColor() );
      p->setPen( c.dark( 140 ) );
      p->drawRect( x, y, w, h );
      p->setPen( c.light( 120 ) );
      p->drawRect( x + 1, y + 1, w - 2, h - 2 );
      break;
   }
   case PE_PanelMenuBar:
   {
      break;
   }
   case PE_PanelDockWindow:
   {
      break;
   }
   case PE_Separator:
   case PE_DockWindowSeparator:
   {
      break; //don't draw anything
      if (!(p->device() && p->device()->devType() == QInternal::Widget && ::qt_cast<QToolBar*>((QWidget*)p->device())))
      {
         if ( !( flags & Style_Horizontal ) ) 
         {
            p->setPen( cg.mid() );
            p->drawLine( 4, r.height() / 2, r.width() - 5, r.height() / 2 );
            p->setPen( cg.light() );
            p->drawLine( 4, r.height() / 2 + 1, r.width() - 5, r.height() / 2 + 1 );
         }
         else
         {
            p->setPen( cg.mid() );
            p->drawLine( r.width() / 2, 4, r.width() / 2, r.height() - 5 );
            p->setPen( cg.light() );
            p->drawLine( r.width() / 2 + 1, 4, r.width() / 2 + 1, r.height() - 5 );
         }
      }
      break;
   }
   case PE_SpinWidgetUp:
   case PE_SpinWidgetDown:
   {
      bool sunken = on || down;
      bool hover = flags & Style_MouseOver;
      int x = r.x() + ( r.width() - 7 ) / 2;
      int y = pe == PE_SpinWidgetDown ? r.y() + 1 : r.bottom() - 7;
      QPen oldPen( p->pen() );
      QBrush oldBrush( p->brush() );
      p->setPen( sunken ? cg.button() : hover ? cg.mid() : cg.text() );
      p->setBrush( sunken ? cg.button() : hover ? cg.mid() : cg.text() );
      p->drawPixmap( x, y, pe == PE_SpinWidgetUp ? *sbUp : *sbDown );
      p->setBrush( oldBrush );
      p->setPen( oldPen );
      break;
   }
   case PE_CheckMark:
   {
      QPen oldPen( p->pen() );
      p->setPen( flags & Style_On ? cg.highlightedText() : optionHandler->textColor() );
      p->drawPixmap(r.x() + r.width()/2 - 4,r.y() + r.height()/2 - 4,*checkmark);
      p->setPen( oldPen );
      break;
   }
   default:
   {
      if ( (pe >= PE_ArrowUp && pe <= PE_ArrowLeft ) || pe == PE_HeaderArrow)
      {
         QBrush oldBrush(p->brush());
         QColor tmpC = flags & Style_Enabled ? (flags & Style_Down || flags & Style_Sunken) && pe != PE_HeaderArrow ? cg.light() : cg.buttonText() : cg.mid();
         if (tmpC == Qt::black) tmpC = QColor(85,85,85);
         p->setBrush(tmpC);
         int x = r.width() > 7 ? r.x() + r.width()/2 - 4 : r.right() - 7;
         int y = r.height() > 7 ? r.y() + r.height()/2 - 4 : r.bottom() - 7;

         switch ( pe )
         {
         case PE_ArrowRight:
         {
            p->drawPixmap(x,y,*sbRight);
            break;
         }
         case PE_ArrowUp:
         {
            p->drawPixmap(x,y,*sbUp);
            break;
         }
         case PE_ArrowDown:
         {
            p->drawPixmap(x,y,*sbDown);
            break;
         }
         case PE_HeaderArrow:
         {
            if (flags & Style_Up)
               p->drawPixmap(x,y,*sbUp);
            else
               p->drawPixmap(x,y,*sbDown);
            break;
         }
         default:
         {
            p->drawPixmap(x,y,*sbLeft);
            break;
         }
      }
      p->setBrush( oldBrush );
   }
   else
      KStyle::drawPrimitive( pe, p, r, cg, flags, opt );
   }
   } // general switch
}

void LiquidStyle::drawKStylePrimitive( KStylePrimitive kpe, QPainter* p,
                                       const QWidget* widget, const QRect &r,
                                       const QColorGroup &cg, SFlags flags,
                                       const QStyleOption &opt ) const 
{
   switch ( kpe )
   {
   case KPE_ToolBarHandle:
   {
      if (widget && widget->parentWidget() && widget->parentWidget()->hasMouse())
      {
         int x, y, w, h;
         r.rect( &x, &y, &w, &h );
         w = (int)((w-3)/2);
         QColor c( (optionHandler->style() == Brushed) ? cg.background() : cg.background().dark(120) );
         p->setPen(c);
         p->setBrush(c);
         for (int i = 0; i < h-3; i += 5 )
            p->drawEllipse(x+w,y+i,3,3);
      }
      break;
   }
   case KPE_GeneralHandle:
   {
      if (!widget)
         break;
      if ( widget->inherits( "AppletHandleDrag" ) )
      {
         int x, y, w, h;
         r.rect( &x, &y, &w, &h );
         w = (int)((w-3)/2);
         QColor c( widget->hasMouse() ? cg.button() : cg.background().dark(110) );
         p->setPen(c);
         p->setBrush(c);
         for (int i = 0; i < h-3; i += 5 )
            p->drawEllipse(x+w,y+i,3,3);
         break;
      }
      else if (widget->hasMouse())
      {
         if (optionHandler->style() == Brushed)
         {
            drawCombo( p, cg, cg.button(), r.x(), r.y(), r.width(), r.height()+2, false, false, false, center);
         }
         else
         {
            p->fillRect(r,cg.button());
            p->setPen(cg.button().dark( 100 + optionHandler->stippleContrast() ));
            int x=r.x();
            int y=r.y();
            int w=r.width();
            int h=r.height();
            for (int i = y;i < y+h; i += 4 )
            {
               p->drawLine( x, i, x+w, i );
               p->drawLine( x, i + 1, x+w, i + 1 );
            }
         }
      }
      else if (optionHandler->style() == Brushed && widget->parentWidget()) // needs alignment
      {
         p->drawTiledPixmap(r, *widget->paletteBackgroundPixmap(), widget->parentWidget()->backgroundOffset());
      }
      break;
   }
   case KPE_SliderGroove:
   {
      int x, y, w, h;
      r.rect( &x, &y, &w, &h );
      if (qstrcmp( qApp->argv() [ 0 ], "kmix" ) == 0) x = x+3;
      if ( ( ( const QSlider * ) widget ) ->orientation() == Qt::Horizontal )
      {
         int x2 = x + w - 1;
         y = y + ( h - 5 ) / 2;
         p->setPen( cg.background().dark( 135 ) );
         p->drawLine( x + 1, y, x2 - 1, y ); //1
         p->drawLine( x, y + 4, x2, y + 4 ); //5
         p->setPen( cg.background().dark( 150 ) );
         p->drawLine( x, y + 1, x2, y + 1 ); //2
         p->setPen( cg.background().dark( 115 ) );
         p->drawLine( x, y + 2, x2, y + 2 ); //3
         p->drawLine( x, y + 3, x2, y + 3 ); //4
         p->setPen( cg.background().light( 130 ) );
         p->drawLine( x + 1, y + 5, x2 - 1, y + 5 ); //6
      }
      else
      {
         int y2 = y + h - 1;
         x = x + ( w - 5 ) / 2;
         p->setPen( cg.background().dark( 135 ) );
         p->drawLine( x, y + 1, x, y2 - 1 );
         p->drawLine( x + 4, y, x + 4, y2 );
         p->setPen( cg.background().dark( 150 ) );
         p->drawLine( x + 1, y, x + 1, y2 );
         p->setPen( cg.background().dark( 115 ) );
         p->drawLine( x + 2, y, x + 2, y2 );
         p->drawLine( x + 3, y, x + 3, y2 );
         p->setPen( cg.background().light( 130 ) );
         p->drawLine( x + 5, y + 1, x + 5, y2 - 1 );
      }
   break;
   }
   case KPE_SliderHandle:
   {
      if (!widget)
      {
         p->drawPixmap( r.x(), r.y(), *getPixmap( flags & Style_Enabled ? RadioOffHover : RadioOff));
         break;
      }
      if ((( const QSlider * ) widget ) ->orientation() == Qt::Horizontal)
         (widget && widget->height() < 17) ? p->drawPixmap( r.x(), r.y(), *getPixmap( flags & Style_Enabled ? RadioOffHover : RadioOff)) : p->drawPixmap( r.x(), (optionHandler->style() == Milk) ? r.y() : r.y()+2, *getPixmap( flags & Style_Enabled ? HSlider : HSliderInactive));
      else
      {
         bool right = (( const QSlider * ) widget )->tickmarks() == QSlider::NoMarks || (( const QSlider * ) widget )->tickmarks() == QSlider::Right;
         (widget && widget->width() < 17) ? p->drawPixmap( r.x()+2, r.y(), *getPixmap( flags & Style_Enabled ? RadioOffHover : RadioOff)) : p->drawPixmap( (optionHandler->style() == Milk) ? r.x() : right ? r.x()+5 : r.x()-3, r.y(), *getPixmap( flags & Style_Enabled ? (right ? VSlider : VSliderAbove) : (right ? VSliderInactive : VSliderAboveInactive)));
      }
      break;
   }
   case KPE_ListViewExpander:
   {
      p->fillRect(r,Qt::green);
    // 				int radius = ( r.width() - 4 ) / 2;
    // 				int centerx = r.x() + r.width() / 2;
    // 				int centery = r.y() + r.height() / 2;
    // 
    // 				int red, green, blue;
    // 				red = ( cg.dark().red() >> 1 ) + ( cg.base().red() >> 1 );
    // 				green = ( cg.dark().green() >> 1 ) + ( cg.base().green() >> 1 );
    // 				blue = ( cg.dark().blue() >> 1 ) + ( cg.base().blue() >> 1 );
    // 				QColor blendColor( red, green, blue );
    // 
    // 				p->setPen( cg.dark() );
    // 				p->drawLine( r.x() + 1, r.y(), r.right() - 1, r.y() );
    // 				p->drawLine( r.x() + 1, r.bottom(), r.right() - 1, r.bottom() );
    // 				p->drawLine( r.x(), r.y() + 1, r.x(), r.bottom() - 1 );
    // 				p->drawLine( r.right(), r.y() + 1, r.right(), r.bottom() - 1 );
    // 				p->setPen( blendColor );
    // 				p->drawPoint( r.x(), r.y() );
    // 				p->drawPoint( r.right(), r.y() );
    // 				p->drawPoint( r.x(), r.bottom() );
    // 				p->drawPoint( r.right(), r.bottom() );
    // 				p->setPen( cg.light() );
    // 				p->drawRect( r.x() + 1, r.y() + 1, r.width() - 2, r.height() - 2 );
    // 				p->fillRect( r.x() + 2, r.y() + 2, r.width() - 4, r.height() - 4,
    // 				             cg.background() );
    // 				p->setPen( cg.text() );
    // 				p->drawLine( centerx - radius, centery, centerx + radius, centery );
    // 				if ( flags & Style_On )
    // 					p->drawLine( centerx, centery - radius, centerx, centery + radius );
      break;
   }
   default:
      KStyle::drawKStylePrimitive( kpe, p, widget, r, cg, flags, opt );
   }
}

void LiquidStyle::drawControl( ControlElement element, QPainter *p,
                               const QWidget *widget, const QRect &r,
                               const QColorGroup &cg, SFlags flags,
                               const QStyleOption &opt ) const
{
   switch ( element )
   {
   case CE_ProgressBarGroove:
   {
      p->setPen(cg.background().dark(160));
      p->drawRect(r);
      drawCombo( p, cg, cg.background(), r.x()+1, r.y()+1, r.width()-2, r.height()+1, false, false, false, center );
      break;
   }
   case CE_ProgressBarContents:
   {
//	qWarning("%s - %s,%s : %s,%s",qApp->argv()[0],widget->className(),widget->name(),widget->parentWidget()->className(),widget->parentWidget()->name());
      const QProgressBar *progress = ( const QProgressBar * ) widget;
      QRect contentsR( subRect( SR_ProgressBarContents, widget ) );
      double val = progress->progress();
      bool reverse = QApplication::reverseLayout();
      val = val / progress->totalSteps();
      if ( val > 0.0 )
      {
         int w = QMIN( contentsR.width(), ( int ) (val * contentsR.width() ) );
         if ( w > 1 )
         {
            int x = contentsR.x();
            int y = contentsR.y();
            QRect progressRect;
            if (reverse)
               progressRect = QRect(x+(contentsR.width()-w), y, w, contentsR.height()+1);
            else
               progressRect = QRect(x, y, w, contentsR.height()+1);
            if (optionHandler->ProgressBar() == liquid)
            {
               int x2 = x + w - 1;
               int y2 = contentsR.bottom();
               //Clip to the old rectangle
               p->setClipRect(progressRect, QPainter::CoordPainter);
               //Expand
               progressRect.setLeft(progressRect.x() - progAnimShift);
               p->setPen(cg.button().dark(120));
               p->drawLine(x, y, x2, y);
               p->drawLine(x, y, x, y2);
               p->drawLine(x+1, y2+1, x2-1, y2+1);
               p->setPen(cg.button().dark(110));
               p->drawLine(x2, y, x2, y2);
               p->drawLine(x, y2, x2, y2);
               QPixmap * pix = getPixmap( Progress );
               p->drawTiledPixmap(/*x+1, y+1, w-2, contentsR.height()-2*/progressRect, *pix);
            }
            else if (optionHandler->ProgressBar() == baghira)
            {
               drawCombo( p, cg, cg.button(), r.x()-1, r.y()-1, w, contentsR.height()+5, false, false, false, center);
            }
            else
            {
               ButtonTile *progress2Tile = progress2Dict.find( cg.button().rgb() );
               if ( !progress2Tile )
               {
                  QPixmap *pix = getPixmap(progress2);
                  progress2Tile = separateTiles( pix, 0, 15, 38, 1, 0, true);
                  if ( !progress2Tile )
                  {
                     qWarning( "Progress tile is NULL!" );
                     return ;
                  }
                  else
                  {
                     LiquidStyle *ptr = const_cast<LiquidStyle*>( this );
                     ptr->progress2Dict.insert( cg.button().rgb(), progress2Tile );
                  }
               }
               //Clip to the old rectangle
               p->setClipRect(progressRect, QPainter::CoordPainter);
               //Expand
               progressRect.setLeft(progressRect.x() - 38 + prog2AnimShift);
               int h; /*x, y, w;*/
               progressRect.rect(&x, &y, &w, &h);
               bool tooSmall = h < 32;
               int mid = h/2;
               p->drawTiledPixmap(x,tooSmall ? mid + 1 : progressRect.bottom()-15,w,16, *progress2Tile->pixmap(TileBtm));
               p->drawTiledPixmap(x,tooSmall ? mid : y+15,w,tooSmall ? 1 : h-31, *progress2Tile->pixmap(TileMiddle));
               p->drawTiledPixmap(x,tooSmall ? mid - 15 : y,w,15, *progress2Tile->pixmap(TileTop));
            }
         }
      }
      break;
   }/*
   case CE_ToolButtonLabel:
   {
      const QToolButton *toolbutton = (const QToolButton *) widget;
      QRect rect = r;
      Qt::ArrowType arrowType = opt.isDefault()
         ? Qt::DownArrow : opt.arrowType();
      
      int shiftX = 0;
      int shiftY = 0;
      if (flags & (Style_Down | Style_On)) {
         shiftX = pixelMetric(PM_ButtonShiftHorizontal, widget);
         shiftY = pixelMetric(PM_ButtonShiftVertical, widget);
      }
      
      if (!opt.isDefault())
      {
         PrimitiveElement pe;
         switch (arrowType)
         {
         case Qt::LeftArrow:  pe = PE_ArrowLeft;  break;
         case Qt::RightArrow: pe = PE_ArrowRight; break;
         case Qt::UpArrow:    pe = PE_ArrowUp;    break;
         default:
         case Qt::DownArrow:  pe = PE_ArrowDown;  break;
         }
         
         rect.moveBy(shiftX, shiftY);
         drawPrimitive(pe, p, rect, cg, flags, opt);
      }
      else
      {
         QColor btext = Qt::black;//(flags & (Style_Down | Style_On)) ? qApp->palette().color(QPalette::Active,QColorGroup::ButtonText) : toolbutton->paletteForegroundColor();
         
         if (toolbutton->iconSet().isNull() &&
               ! toolbutton->text().isNull() &&
               ! toolbutton->usesTextLabel()) {
                  int alignment = AlignCenter | ShowPrefix;
                  if (!styleHint(SH_UnderlineAccelerator, widget, QStyleOption::Default, 0))
                     alignment |= NoAccel;
                  
                  rect.moveBy(shiftX, shiftY);
                  drawItem(p, rect, alignment, cg,
                           flags & Style_Enabled, 0, toolbutton->text(),
                           toolbutton->text().length(), &btext);
               } else {
                  QPixmap pm;
                  QIconSet::Size size =
                     toolbutton->usesBigPixmap() ? QIconSet::Large : QIconSet::Small;
                  QIconSet::State state =
                     toolbutton->isOn() ? QIconSet::On : QIconSet::Off;
                  QIconSet::Mode mode;
                  if (! toolbutton->isEnabled())
                     mode = QIconSet::Disabled;
                  else if (flags & (Style_Down | Style_On) ||
                           (flags & Style_Raised) && (flags & Style_AutoRaise))
                     mode = QIconSet::Active;
                  else
                     mode = QIconSet::Normal;
                  pm = toolbutton->iconSet().pixmap( size, mode, state );
                  
                  if ( toolbutton->usesTextLabel() )
                  {
                     p->setFont( toolbutton->font() );
                     QRect pr = rect, tr = rect;
                     int alignment = ShowPrefix;
                     if (!styleHint(SH_UnderlineAccelerator, widget, QStyleOption::Default, 0))
                        alignment |= NoAccel;
                     
                     if ( toolbutton->textPosition() == QToolButton::Under )
                     {
                        int fh = p->fontMetrics().height();
                        pr.addCoords( 0, 1, 0, -fh-3 );
                        tr.addCoords( 0, pr.bottom(), 0, -3 );
                        pr.moveBy(shiftX, shiftY);
                        drawItem( p, pr, AlignCenter, cg, TRUE, &pm, QString::null );
                        alignment |= AlignCenter;
                     }
                     else
                     {
                        pr.setWidth( pm.width() + 8 );
                        tr.addCoords( pr.right(), 0, 0, 0 );
                        pr.moveBy(shiftX, shiftY);
                        
                        drawItem( p, pr, AlignCenter, cg, TRUE, &pm, QString::null );
                        alignment |= AlignLeft | AlignVCenter;
                     }
                     
                     tr.moveBy(shiftX, shiftY);
                     drawItem( p, tr, alignment, cg,
                              flags & Style_Enabled, 0, toolbutton->textLabel(),
                              toolbutton->textLabel().length(), &btext);
                  }
                  else
                  {
                     rect.moveBy(shiftX, shiftY);
                     drawItem( p, rect, AlignCenter, cg, TRUE, &pm, QString::null );
                  }
               }
      }
      
      break;
   }*/

/*
   case CE_HeaderLabel:
   {
      QRect rect = r;
      const QHeader* header = (const QHeader *) widget;
      int section = opt.headerSection();
      
      QIconSet* icon = header->iconSet( section );
      if ( icon ) {
         QPixmap pixmap = icon->pixmap( QIconSet::Small,
                                        flags & Style_Enabled ?
                                        QIconSet::Normal : QIconSet::Disabled );
         int pixw = pixmap.width();
         int pixh = pixmap.height();
		// "pixh - 1" because of tricky integer division
         
         QRect pixRect = rect;
         pixRect.setY( rect.center().y() - (pixh - 1) / 2 );
         drawItem ( p, pixRect, AlignVCenter, cg, flags & Style_Enabled,
                    &pixmap, QString::null );
         rect.setLeft( rect.left() + pixw + 2 );
      }
      drawItem ( p, rect, AlignVCenter, cg, flags & Style_Enabled,
                 0, header->label( section ), -1, (isTaskContainer && (flags & Style_MouseOver)) ? &(qApp->palette().color(QPalette::Active,QColorGroup::ButtonText)) :  &(cg.buttonText()) );
      break;
   }*/
   case CE_ProgressBarLabel:
   {
      if (optionHandler->DrawProgressLabel())
      {
         QRect contentsR( subRect( SR_ProgressBarContents, widget ) );
         if ( !contentsR.isValid() )
            return ;
         QFont font( p->font() );
         font.setBold( true );
         p->setFont( font );
         p->setPen( widget->colorGroup().buttonText() );
         p->setClipRect(r, QPainter::CoordPainter);
	 p->drawText( contentsR, AlignCenter, ( ( const QProgressBar * ) widget ) ->progressString() );
      }
      break;
   }
   case CE_TabBarTab:
   { // new style with tiled pixmaps
	   if ( !widget || !widget->parentWidget(true) )
		   break;
	   const QTabBar *tabBar = ( const QTabBar * ) widget;
	   bool selected = flags & Style_Selected;
	   bool hover = currentTabBar && currentTabBar == tabBar && tabBar->indexOf(opt.tab()->identifier()) == hoveredTabIndex;

	   myPosition pos;
	   if (tabBar->count() == 1)
		   pos = full;
	   else if ((tabBar->indexOf(opt.tab()->identifier()) == 0))
		   pos = left;
	   else if (tabBar->indexOf(opt.tab()->identifier()) == tabBar->count() - 1)
		   pos = right;
	   else 
		   pos = center;

	   QColor c;
	   if (hover || selected)
	   {
		   if (optionHandler->useCustomColors())
			   c = optionHandler->customColor(CustomTabOn);
		   else
			   c = optionHandler->buttonColor();
	   }
	   else
	   {
		   if (optionHandler->useCustomColors())
			   c = optionHandler->customColor(CustomTabOff);
		   else
			   c = optionHandler->InactiveButtonColor();
	   }

	   bool isDialog = optionHandler->TabStyle() == Clever && widget->topLevelWidget()->inherits("QDialog");
	   if (isDialog || optionHandler->TabStyle() == Chooser)
		   drawCombo( p, cg, c, r.x(), r.y(), r.width(), r.height(), false, false, false, pos );
	   else
	   {
		   LiquidStyle *ptr = const_cast<LiquidStyle*>( this );
		   bool above = tabBar->shape() == QTabBar::RoundedAbove || tabBar->shape() == QTabBar::TriangularAbove;
		   QPixmap *pix;
		   if (above)
			   pix = selected ? getPixmap( TabDown ) : getPixmap( Tab );
		   else
			   pix = selected ? getPixmap( belowTabDown ) : getPixmap( belowTab );
		   ButtonTile *tabTile = above ? tabDict.find( c.rgb() ) : inverseTabDict.find(c.rgb());
		   if ( !tabTile )
			   tabTile = separateTiles( pix, 9, 15, 67, 1, 0, false);
		   if ( !tabTile )
		   {
			   qWarning( "Tab tile is NULL!" );
			   return ;
		   }
		   else
			   above ? ptr->tabDict.insert( c.rgb(), tabTile ) : ptr->inverseTabDict.insert( c.rgb(), tabTile );

		   int offset;
		   selected ? offset = 0 : offset = 2;

		   if ( r.width() > 18 )
		   {
			   p->drawTiledPixmap( r.x() + 9, r.y() + offset, r.width() - 18, 15, *tabTile->pixmap( TileTop ) );
			   p->drawTiledPixmap( r.x() + 9, r.height() - 11, r.width() - 18, 11, *tabTile->pixmap( TileBtm ) );
		   }
		   if ( r.height() + offset > 26 )
		   {
			   p->drawTiledPixmap( r.x() + 0, r.y() + 15 + offset, 9, r.height() - 26 - offset, *tabTile->pixmap( TileLeft ) );
			   p->drawTiledPixmap( r.right() - 8, r.y() + 15 + offset, 9, r.height() - 26 - offset, *tabTile->pixmap( TileRight ) );
		   }
		   if ( r.width() > 18 && r.height() + offset > 22 )
			   p->drawTiledPixmap( r.x() + 9, r.y() + 15 + offset, r.width() - 18, r.height() - 26 - offset, *tabTile->pixmap( TileMiddle ) );
		   p->drawPixmap( r.x() + 0, r.y() + offset, *tabTile->pixmap( TileTopLeft ) );
		   p->drawPixmap( r.right() - 8, r.y() + offset, *tabTile->pixmap( TileTopRight ) );
		   p->drawPixmap( r.x() + 0, r.height() - 11, *tabTile->pixmap( TileBtmLeft ) );
		   p->drawPixmap( r.right() - 8, r.height() - 11, *tabTile->pixmap( TileBtmRight ) );

		   p->setPen( optionHandler->buttonColor());
		   bool drawPoint = optionHandler->CenterTabs() || pos == center || pos == right;
		   if ( selected )
		   {
			   if (drawPoint) p->drawPoint(r.x(), above ? r.bottom() : r.y());
			   p->drawPoint(r.right(), above ? r.bottom() : r.y());
		   }
		   else
		   {
			   p->drawLine( r.x() + drawPoint ? 0 : 1, above ? r.bottom() : r.y(), r.right(), above ? r.bottom() : r.y() );
			   p->setPen( optionHandler->buttonColor().dark( 158 ));
			   p->drawLine( r.x() + drawPoint ? 0 : 1, above ? r.bottom()-1 : r.y()+1, r.right(), above ? r.bottom()-1 : r.y() + 1 );
		   }
	   }
	   break;
   }
   case CE_TabBarLabel:
   {
	   if ( opt.isDefault() )
		   return ;
	   QTab *t = opt.tab();
	   const QTabBar * tb = ( const QTabBar * ) widget;
	   if ( ( tb->currentTab() != t->identifier() ))
	   {
		   if (optionHandler->TabStyle() == TrueTab ||
				   (optionHandler->TabStyle() == Clever && !(widget->topLevelWidget()->inherits("QDialog"))))
		   {
			   drawItem( p, QRect( r.x(), r.y() + 1, r.width(), r.height() ), AlignCenter | ShowPrefix, cg, flags & Style_Enabled, 0, t->text(), -1/*, &cg.text()*/ );
			   break;
		   }
		   drawItem( p, QRect( r.x(), r.y(), r.width(), r.height() ), AlignCenter | ShowPrefix, cg, flags & Style_Enabled, 0, t->text(), -1/*, &cg.text()*/ );
		   break;
	   }
	   drawItem( p, QRect( r.x(), r.y(), r.width(), r.height() ), AlignCenter | ShowPrefix, cg, flags & Style_Enabled, 0, t->text(), -1/*, &cg.text()*/ );
	   break;
   }
   case CE_PushButton:
   {
	   //       if (_htmlWidgetWId == widget->winId())
	   //          return;
	   QPushButton *btn = ( QPushButton* ) widget;
	   const_cast<LiquidStyle*>( this ) ->isHTMLButton = isHTMLWidget( btn );
	   ButtonFadeInfo *bfi_ = bfi[btn->winId()];
	   if ( widget == highlightWidget || (bfi_ && !bfi_->fadeIn) )
		   flags |= Style_MouseOver;
	   if (btn->isDefault())
		   flags |= Style_HasFocus;
	   // Qt messes this up with WindowOrigin, so we do it ourselves :P
	   if ( !isPlain() && !btn->autoMask() )
		   const_cast<LiquidStyle*>( this ) ->btnOffset = btn->backgroundOffset();
	   const_cast<LiquidStyle*>( this ) ->paintWidget = const_cast<QWidget*>( widget );
	   drawPrimitive( PE_ButtonDefault, p, r, cg, flags, QStyleOption( btn ));
	   const_cast<LiquidStyle*>( this ) ->paintWidget = 0L;
	   const_cast<LiquidStyle*>( this ) ->isHTMLButton = false;
	   break;
   }
   case CE_PushButtonLabel:
   {
	   //       if (_htmlWidgetWId == widget->winId())
	   //          return;
	   const QPushButton *btn = ( const QPushButton* ) widget;
	   int x, y, w, h;
	   r.rect( &x, &y, &w, &h );

	   if ( btn->isOn() || btn->isDown() )
		   flags |= Style_Sunken;

	   int xoff = x;
	   if ( btn->iconSet() && !btn->iconSet() ->isNull() )
	   {
		   QIconSet::Mode mode = btn->isEnabled() ? QIconSet::Normal : QIconSet::Disabled;
		   if ( mode == QIconSet::Normal && btn->hasFocus() )
			   mode = QIconSet::Active;

		   QIconSet::State state = QIconSet::Off;
		   if ( btn->isToggleButton() && btn->isOn() )
			   state = QIconSet::On;

		   QPixmap pix( btn->iconSet() ->pixmap( QIconSet::Small, mode, state ) );
		   xoff += (x<6?8:2);
		   if (btn->text().isEmpty() && !btn->pixmap())
			   p->drawPixmap( x + w/2 - pix.width()/2, y + h / 2 - pix.height() / 2, pix );
		   else
			   p->drawPixmap( x<6?6:x, y + ( h - pix.height() ) / 2, pix );
	   }

	   if ( btn->isMenuButton() )
	   {
		   drawPrimitive( PE_ArrowDown, p, QRect( w-14, (int)(h/2), 7, 7 ) , cg, flags, opt );
		   w -= 14;
	   }
	   QColor tmp;

	   tmp = btn->isDown() && !optionHandler->IcyButtons() ? btn->colorGroup().highlightedText() : btn->colorGroup().buttonText();
	   drawItem( p, QRect( xoff, optionHandler->style() == Milk ? y-1 : y, w, h ), AlignCenter | ShowPrefix, cg, btn->isEnabled(), btn->pixmap(), btn->text(), -1, &tmp );
	   break;
   }
   case CE_MenuBarItem:
   {
	   QMenuItem *mi = opt.menuItem();
	   bool active = flags & Style_Active;
	   bool focused = flags & Style_HasFocus;

	   if ( active && focused )
	   {
		   optionHandler->glossyMenus() ?
			   drawCombo( p, cg, cg.highlight(), r.x(), r.y()-1, r.width(), r.height() + 5, false, false, false, center ) :
			   p->fillRect(r,cg.highlight());
	   }

	   //       QColor tmpColor( cg.background().dark( 120 ) );
	   if ( optionHandler->useShadowText() && !( active && focused ))
	   {
		   p->setPen(cg.background().dark( 120 ));
		   p->drawText(r, AlignCenter | AlignVCenter | ShowPrefix | DontClip | SingleLine, mi->text());
	   }
	   //          drawItem( p, QRect( r.x() + 1, r.y() + 1, r.width(), r.height() ), AlignCenter | AlignVCenter | ShowPrefix | DontClip | SingleLine, cg, flags & Style_Enabled, mi->pixmap(), mi->text(), -1, &tmpColor );

	   //       drawItem( p, r, AlignCenter | AlignVCenter | ShowPrefix | DontClip | SingleLine, cg, flags & Style_Enabled, mi->pixmap(), mi->text(), -1, active && focused ? &(cg.highlightedText()) : &(cg.text()) );
	   p->setPen(active && focused ? cg.highlightedText() : cg.text());
	   p->drawText(r, AlignCenter | AlignVCenter | ShowPrefix | DontClip | SingleLine, mi->text());
	   break;
   }
   case CE_PopupMenuItem:
   {
	   int x,y,w, h;
	   r.rect( &x, &y, &w, &h );

	   const QPopupMenu *popupmenu = ( const QPopupMenu * ) widget;
	   QMenuItem *mi = opt.menuItem();

	   int tab = opt.tabWidth();
	   int maxpmw = opt.maxIconWidth();
	   bool dis = mi && !mi->isEnabled();
	   bool checkable = popupmenu->isCheckable();
	   bool act = flags & Style_Active;
	   bool enabled = flags & Style_Enabled;
	   int checkcol = maxpmw;

	   QColor menuBgColor(  optionHandler->UseCustomMenuColor() ? optionHandler->Color() : optionHandler->bgColor() );

	   if ( checkable )
		   checkcol = QMAX( maxpmw, 20 );

	   if ( mi && mi->isSeparator() )
	   {
		   p->setPen( menuBgColor.dark( 130 ) );
		   p->drawLine( x + 1, y, x + w - 1, y );
		   p->setPen( menuBgColor.light( 120 ) );
		   p->drawLine( x+1, y + 1, x + w - 1 , y + 1 );
		   return ;
	   }
	   if ( act && mi && enabled)
	   {
		   optionHandler->glossyMenus() ?
			   drawCombo( p, cg, cg.highlight(), r.x(), r.y(), r.width(), r.height() + 2, false/*sunken*/, false/*hover*/, false, center ) :
			   p->fillRect(x,y,w,h,cg.highlight());
	   }
	   else
	   {
		   if (popupBack.isNull())
			   p->fillRect( x, y, w, h, popupmenu->backgroundBrush() );
		   else
			   p->drawTiledPixmap( x, y, w, h, popupBack );

		   if (act)
		   {
			   p->setPen(cg.highlight());
			   p->drawRect(x,y,w,h);
		   }
	   }
	   if ( !mi )
		   return ;

	   // Menu contents drawing code based on Qt's styles. Qt is property of
	   // TrollTech: www.trolltech.com. Used with permission.
	   int xpos = x;
	   QRect vrect = visualRect( QRect( xpos, y, checkcol, h ), r );
	   int xvis = vrect.x();

	   // icon
	   if (optionHandler->drawMenuStripe())
		   p->fillRect(x,y,h,h, optionHandler->menuStripeColor());
	   if ( mi->iconSet() )
	   {
		   QIconSet::Mode mode = dis ? QIconSet::Disabled : QIconSet::Normal;
		   if ( act && !dis )
			   mode = QIconSet::Active;
		   QPixmap pixmap;
		   if ( checkable && mi->isChecked() )
			   pixmap = mi->iconSet() ->pixmap( QIconSet::Small, mode, QIconSet::On );
		   else
			   pixmap = mi->iconSet() ->pixmap( QIconSet::Small, mode );
		   int pixw = pixmap.width();
		   int pixh = pixmap.height();
		   QRect pmr( 0, 0, pixw, pixh );
		   pmr.moveCenter( vrect.center() );
		   p->setPen( cg.text() );
		   p->drawPixmap( pmr.topLeft(), pixmap );
	   }
	   else if ( checkable )
	   {
		   if ( mi->isChecked() )
		   {
			   int xp = xpos + windowsItemFrame;

			   SFlags cflags = Style_Default;
			   if ( !dis )
				   cflags |= Style_Enabled;
			   if ( act )
				   cflags |= Style_On;
			   drawPrimitive( PE_CheckMark, p, visualRect( QRect( xp, y + windowsItemFrame, checkcol - 2 * windowsItemFrame, h - 2 * windowsItemFrame ), r ), cg, cflags );
		   }
	   }
	   QColor discol;
	   if ( dis )
	   {
		   discol = cg.text();
		   p->setPen( discol );
	   }

	   int xm = windowsItemFrame + checkcol + windowsItemHMargin;
	   xpos += xm;

	   vrect = visualRect( QRect( xpos, y + windowsItemVMargin, w - xm - tab + 1, h - 2 * windowsItemVMargin ), r );
	   xvis = vrect.x();
	   if ( mi->custom() )
	   {
		   p->save();
		   if ( dis && !act )
		   {
			   p->setPen( cg.light() );
			   mi->custom() ->paint( p, cg, act, !dis, xvis + 1, y + windowsItemVMargin + 1, w - xm - tab + 1, h - 2 * windowsItemVMargin );
			   p->setPen( discol );
		   }
		   mi->custom() ->paint( p, cg, act, !dis, xvis, y + windowsItemVMargin, w - xm - tab + 1, h - 2 * windowsItemVMargin );
		   p->restore();
	   }

	   // Text
	   QString s = mi->text();
	   if ( !s.isNull() )
	   {
		   QColor textColor, shadowColor;
		   shadowColor = optionHandler->bgColor().dark( 130 );

		   if ( dis )
			   textColor = shadowColor.light( 115 );
		   else if (/*!optionHandler->glossyMenus() && */act && mi)
			   textColor = cg.highlightedText();
		   else if ( optionHandler->UseCustomMenuColor())
			   textColor = optionHandler->textColor();
		   else
			   textColor = cg.text();

		   int t = s.find( '\t' );
		   int text_flags = AlignVCenter | ShowPrefix | DontClip | SingleLine;
		   text_flags |= ( QApplication::reverseLayout() ? AlignRight : AlignLeft );
		   if ( t >= 0 )
		   {                         // draw tab text
			   int xp;
			   xp = x + w - tab - 20 - windowsItemHMargin - windowsItemFrame + 1;
			   QString tabStr( s.right( s.length() - t - 1 ) );
			   if ( !tabStr.isEmpty() )
			   {
				   if ( optionHandler->useShadowText() )
				   {
					   p->setPen( shadowColor );
					   p->drawText( xp + 1, y + windowsItemVMargin + 1, w - xm - tab + 1, h - 2 * windowsItemVMargin, text_flags, tabStr );
				   }
				   p->setPen( textColor );
				   p->drawText( xp, y + windowsItemVMargin, w - xm - tab + 1, h - 2 * windowsItemVMargin, text_flags, tabStr );
			   }
			   s = s.left( t );
		   }
		   if ( optionHandler->useShadowText() && !(act && mi) )
		   {
			   p->setPen( shadowColor );
			   p->drawText( xvis + 1, y + windowsItemVMargin + 1, w - xm - tab + 1, h - 2 * windowsItemVMargin, text_flags, s, t );
		   }
		   p->setPen( textColor );
		   p->drawText( xvis, y + windowsItemVMargin, w - xm - tab + 1, h - 2 * windowsItemVMargin, text_flags, s, t );
	   }
	   else if ( mi->pixmap() )
		   // draw pixmap
	   {
		   QPixmap * pixmap = mi->pixmap();
		   if ( pixmap->depth() == 1 )
			   p->setBackgroundMode( OpaqueMode );
		   p->drawPixmap( xvis, y + windowsItemFrame, *pixmap );
		   if ( pixmap->depth() == 1 )
			   p->setBackgroundMode( TransparentMode );
	   }
	   // Child menu
	   if ( mi->popup() )
	   {
		   int dim = (int)((float)( h - 2 * windowsItemFrame ) / 2);
		   PrimitiveElement arrow;
		   arrow = ( QApplication::reverseLayout() ? PE_ArrowLeft : PE_ArrowRight );
		   xpos = x + w - windowsArrowHMargin - windowsItemFrame/* - dim*/-8;
		   vrect = visualRect( QRect( xpos, y + h / 2 - dim / 2, /*dim*/8, dim ), r );
		   if ( act )
		   {
			   if ( !dis )
				   discol = cg.highlightedText();
			   QColorGroup g2( discol, cg.highlight(), white, white, dis ? discol : white, discol, white );
			   drawPrimitive( arrow, p, vrect, g2, Style_Enabled );
		   }
		   else
		   {
			   if ( !dis )
				   discol = optionHandler->textColor();
			   QColorGroup g2( discol, cg.background(), white, white, dis ? discol : white, discol, white );
			   drawPrimitive( arrow, p, vrect, g2, mi->isEnabled() ? Style_Enabled : Style_Default );
		   }
	   }
	   break;
   }
   /*
      case CE_ToolBoxTab:
      {
      break;
      }*/
   default:
   KStyle::drawControl( element, p, widget, r, cg, flags, opt );
   }
}


// void LiquidStyle::drawControlMask( ControlElement element, QPainter *p, const QWidget *widget, const QRect &r, const QStyleOption& opt ) const {
// 	switch ( element ) {
// 			case CE_PushButton: {
// 				p->fillRect( r, Qt::color1 );
// 				break;
// 				}
// 			default:
// 			KStyle::drawControlMask( element, p, widget, r, opt );
// 			break;
// 		}
// 	}

// void LiquidStyle::drawComplexControlMask( ComplexControl control, QPainter *p, const QWidget *widget, const QRect &r, const QStyleOption &opt ) const {
// // 	switch ( control ) {
// // 			case CC_ComboBox: {
// // 				drawControlMask( CE_PushButton, p, widget, r, opt );
// // 				break;
// // 				}
// // 			default:
// 			KStyle::drawComplexControlMask( control, p, widget, r, opt );
// //		}
// 	}

void LiquidStyle::drawComplexControl( ComplexControl control, QPainter *p, const QWidget *widget, const QRect &r, const QColorGroup &cg, SFlags flags, SCFlags controls, SCFlags active, const QStyleOption &opt ) const
{
   switch ( control )
   {
   case CC_ComboBox:
   {
      bool sunken = (active & SC_ComboBoxArrow);// || (flags & Style_Active) || (flags & Style_Sunken) || (active & SC_ComboBoxListBoxPopup);
      bool hovered = (widget == highlightWidget) || (flags & Style_HasFocus);
      //       bool isHtml = isHTMLWidget( widget );
      const_cast<LiquidStyle*>( this ) ->paintWidget = const_cast<QWidget*>( widget );
      QPen oldPen( p->pen() );
      if ( controls & SC_ComboBoxArrow )
      {
         if ( controls & SC_ComboBoxEditField && ( ( const QComboBox * ) widget ) ->editable())
         { // editable?
#if 0
						if ( optionHandler->buttonStyle() == Brushed )
						{ // rectangular?
							drawRectangularButton( p, cg, sunken ? cg.button().light( 110 ) : cg.button(), r.x(), r.y(), r.width(), r.height(), sunken, hovered, true, full/*, isHtml*/);
							p->fillRect( r.x() + 6, r.y() +2, r.width()-32, r.height() - 7, cg.background());
							p->setPen( oldPen );
							return ;
						}
						else
#endif
            { // round frame
               drawRoundFrame( p, r, cg, (flags & Style_HasFocus) || sunken , widget->backgroundOffset());
               p->setPen(cg.mid());
               p->drawPixmap(r.right() - 16, r.height()/2 - 2, *sbDown);
               p->setPen( oldPen );
               //                return;
            }
#if 0
						else
						{ // default!
							drawCombo( p, cg, active == SC_ComboBoxArrow ? cg.button().light( 110 ) : cg.button(), r.x(), r.y(), r.width(), r.height(), sunken, hovered, true, full );
							p->fillRect( r.x() + 6, r.y() +2, r.width()-32, r.height() - 7, cg.background());
							p->setPen( oldPen );
							return;
						}
#endif
         }
         else
         { // end editable - default handling
            if ( optionHandler->buttonStyle() == Brushed )
            { // retangular?
               drawRectangularButton( p, cg, sunken ? cg.button().light( 110 ) : cg.button(), r.x(), r.y(), r.width(), r.height(), sunken, hovered, true, full, isHTMLWidget( widget ));
               //                p->setPen( oldPen );
               //                return ;
            }
            else
            { //default plain good old combobox... ;)
               drawCombo( p, cg, active == SC_ComboBoxArrow ? cg.button().light( 110 ) : cg.button(), r.x(), r.y(), r.width(), r.height(), sunken, hovered, true, full, isHTMLWidget( widget ) );
                     //                p->setPen( oldPen );
                     //                return;
            }
         }
      }
      const_cast<LiquidStyle*>( this ) ->paintWidget = 0L;
      break;
   }
   case CC_ToolButton:
   {
      const QToolButton *toolbutton = ( const QToolButton * ) widget;
      const_cast<LiquidStyle*>( this ) ->paintWidget = const_cast<QWidget*>( widget );
      const KAccessToolBarButton *toolbutton2 = 0L;
      QRect button, menuarea;
      button = querySubControlMetrics( control, widget, SC_ToolButton, opt );
      menuarea = querySubControlMetrics( control, widget, SC_ToolButtonMenu, opt );
      if (::qt_cast<KToolBarButton*>(toolbutton))
      {
               toolbutton2 = (const KAccessToolBarButton*)toolbutton;
#if GCC_VERSION < 40000
               if (toolbutton2->iconTextMode() == 3)
                     button.setBottom(button.bottom() - p->fontMetrics().height() - 1);
#endif
      }
      else if (toolbutton->usesTextLabel() && toolbutton->textPosition() == QToolButton::BelowIcon)
               button.setBottom(button.bottom() - p->fontMetrics().height());
      menuarea.setX(menuarea.x() - 1);
      SFlags bflags = flags, mflags = flags;

      if ( active & SC_ToolButton )
               bflags |= Style_Down;
      if ( active & SC_ToolButtonMenu )
               mflags |= Style_Down;

      if ( widget == highlightWidget )
               bflags |= Style_MouseOver;
      if ( controls & SC_ToolButton )
      {
         bool sunken = mflags & ( Style_Down /*| Style_On*/ );
         bool hover = bflags & (Style_MouseOver | Style_On );

         if ( !optionHandler->useToolButtonFrame())// || !(toolbutton->parentWidget() &&  ( ::qt_cast<QToolBar*>(toolbutton->parentWidget()) || toolbutton->parentWidget()->inherits("KonqFrameTabs"))))
         {
            if ((!(hover || sunken) || (toolbutton->usesTextLabel() && toolbutton->textPosition() == QToolButton::BelowIcon)) && toolbutton->parentWidget() &&                 !::qt_cast<QToolBar*>(toolbutton->parent()) &&                 toolbutton->parentWidget()->paletteBackgroundPixmap() &&                !toolbutton->parentWidget()->paletteBackgroundPixmap()->isNull())
            {
               QPixmap pixmap = *(toolbutton->parentWidget()->paletteBackgroundPixmap());
               if (hover || sunken)
                        p->drawTiledPixmap( r.x(), button.bottom()+1, r.width(), p->fontMetrics().height(), pixmap, toolbutton->x(), toolbutton->y()+button.height() );
               else
                        p->drawTiledPixmap( r, pixmap, toolbutton->pos() );
            }
            if (hover || sunken)
            {
               const_cast<LiquidStyle*>( this ) ->paintWidget = const_cast<QWidget*>( widget );
               if (optionHandler->toolbuttonStyle() != Brushed)
                  drawCombo( p, cg, sunken ? cg.button().light( 110 ) :
                           (hover ||  (cg.button() != qApp->palette().active().button())) ? cg.button() :
                           optionHandler->InactiveButtonColor(), button.x(), button.y(), button.width(),
                           button.height(), sunken, hover, false, full );
               else
                  drawRectangularButton( p, cg, sunken ? cg.button().light( 110 ) : cg.button(),
                           button.x(), button.y(), button.width(), button.height(),
                           sunken, hover, false);
               const_cast<LiquidStyle*>( this ) ->paintWidget = 0L;
            }
         }
         else
         {
            myPosition pos = full;
            KToolBar *bar = NULL;

            if (toolbutton2 && toolbutton2->parentWidget() && ::qt_cast<KToolBar*>(toolbutton2->parentWidget()) )
            { //position calculation - would be nice if ktoolbarbutton provided this as flag
               bar = (KToolBar*)toolbutton2->parentWidget();
               int buttonId = toolbutton2->id();
               int buttonIndex = bar->itemIndex(buttonId);

               // ok, this is a very hackish heck of a hack...
               // kbookmarkbar (e.g. used by konqueror) accesses the iterator of the toolbar
               // this confuses the iterator of the toolbar a bit, as it is used from the bookmark stuff and my function at the same time (seems, as if kbookmarkbar acts on shown toolbars)
               // unfortunately, there's no way to figure out if it does for this specific toolbar
               // so i check for the toolbars name, knowing this will work for current konqueror - but may fail on further versions
               // even worse - the stuff is hardcoded (assuming first item to be left aligned, last one right and the rest centered)
               // this is crap at all - so if the best thing would be to patch qtoolbutton to provide conjuncted buttons - i'll ask them (maybe qt4)
               if (bar && ( qstrcmp( bar->name(), "bookmarkToolBar" ) == 0 ))
               {
                        return;
                        buttonIndex == 0 ?
                              pos = left :
                              buttonIndex == bar->count() - 1 ?
                              pos = right :
                              pos = center;
               }
               else if (bar->orientation() == Qt::Horizontal)
               {
                  if (buttonIndex == 0)
                  {
                     KToolBarButton *tmpButton = bar->getButton(bar->idAt(buttonIndex+1));
                     (tmpButton && tmpButton->x() == toolbutton2->x()+toolbutton2->width()) ? pos = left : pos = full;
                  }
                  else if (buttonIndex == bar->count() - 1)
                  {
                     KToolBarButton *tmpButton = NULL;
                     tmpButton = bar->getButton(bar->idAt(buttonIndex-1));
                     (tmpButton && tmpButton->x() + tmpButton->width() == toolbutton2->x()) ? pos = right : pos = full;
                  }
                  else
                  {
                     KToolBarButton *tmpButton1 = bar->getButton(bar->idAt(buttonIndex-1));
                     KToolBarButton *tmpButton2 = bar->getButton(bar->idAt(buttonIndex+1));
                     tmpButton1 && tmpButton1->x() + tmpButton1->width() == toolbutton2->x() ?
                              pos = center : pos = left;
                     if (!(tmpButton2 && tmpButton2->x() == toolbutton2->x()+toolbutton2->width()))
                        pos == left ?  pos = full :  pos = right;
                  }
               }
               else
               {
                  if (buttonIndex == 0)
                  {
                     KToolBarButton *tmpButton = bar->getButton(bar->idAt(buttonIndex+1));
                     (tmpButton && tmpButton->y() == toolbutton2->y()+toolbutton2->height()) ?
                              pos = left : pos = full;
                  }
                  else if (buttonIndex == bar->count() - 1)
                  {
                     KToolBarButton *tmpButton = NULL;
                     tmpButton = bar->getButton(bar->idAt(buttonIndex-1));
                     (tmpButton && tmpButton->y() + tmpButton->height() == toolbutton2->y()) ?
                              pos = right : pos = full;
                  }
                  else
                  {
                     KToolBarButton *tmpButton1 = bar->getButton(bar->idAt(buttonIndex-1));
                     KToolBarButton *tmpButton2 = bar->getButton(bar->idAt(buttonIndex+1));
                     tmpButton1 && tmpButton1->y() + tmpButton1->height() == toolbutton2->y() ?
                              pos = center : pos = left;
                     if (!(tmpButton2 && tmpButton2->y() == toolbutton2->y()+toolbutton2->height()))
                              pos == left ?  pos = full :  pos = right;
                  }
               }
            }

            if (optionHandler->toolbuttonStyle() != Brushed)
            {
               (bar && bar->orientation() == Qt::Vertical) ?
                     drawCombo( p, cg, sunken ? cg.button().light( 110 ) : (hover ||  (cg.button() != qApp->palette().active().button())) ? cg.button() : optionHandler->InactiveButtonColor(), button.x(), (pos == full || pos == left) ? button.y() : button.y() - 7, button.width(), (pos == full) ? button.height()+2 : (pos == center) ? button.height()+16 : button.height()+9, sunken, hover, false, center ) :
                     drawCombo( p, cg, sunken ? cg.button().light( 110 ) : (hover ||  (cg.button() != qApp->palette().active().button())) ? cg.button() : optionHandler->InactiveButtonColor(), button.x(), button.y(), button.width(), button.height()/*+2*/, sunken, hover, false, pos );
            }
            else
            {
               (bar && bar->orientation() == Qt::Vertical) ?
                     drawRectangularButton( p, cg, sunken ? cg.button().light( 110 ) : (hover ||  (cg.button() != qApp->palette().active().button())) ? cg.button() : optionHandler->InactiveButtonColor(), button.x(), (pos == full || pos == left) ? button.y() : button.y()-4, button.width(), (pos == full) ? button.height() : (pos == center) ? button.height()+8 : button.height()+4, sunken, hover, false) :
                     drawRectangularButton( p, cg, sunken ? cg.button().light( 110 ) : (hover ||  (cg.button() != qApp->palette().active().button())) ? cg.button() : optionHandler->InactiveButtonColor(), button.x(), button.y(), button.width(), button.height(), sunken, hover, false, pos);
            }
         }
      }
      // Draw a toolbutton menu indicator if required
      if ( controls & SC_ToolButtonMenu )
      {
               if ( mflags & ( Style_Down | Style_On | Style_Raised ) )
                     drawPrimitive( PE_ButtonDropDown, p, menuarea, cg, mflags, opt );
               drawPrimitive( PE_ArrowDown, p, menuarea, cg, mflags, opt );
      }
#if 0
      if ( toolbutton->hasFocus() && !toolbutton->focusProxy() )
      {
               QRect fr = toolbutton->rect();
               fr.addCoords( 3, 3, -3, -3 );
               drawPrimitive( PE_FocusRect, p, fr, cg );
      }
#endif
      const_cast<LiquidStyle*>( this ) ->paintWidget = 0L;
      break;
   }
		case CC_ScrollBar:
			{
				const_cast<LiquidStyle*>( this ) ->currentScrollBar = ( QScrollBar * ) widget;
				KStyle::drawComplexControl( control, p, widget, r, cg, flags, controls, active, opt );
				break;
			}
		case CC_SpinWidget: {
					    const QSpinWidget * sw = (const QSpinWidget *) widget;
					    SFlags flags;
					    PrimitiveElement pe;

					    if ( controls & SC_SpinWidgetFrame )
						    drawEditFrame( p, r, cg, false, sw->hasFocus());

					    if ( controls & SC_SpinWidgetUp ) {
						    flags = Style_Default | Style_Enabled;
						    if (active == SC_SpinWidgetUp ) {
							    flags |= Style_On;
							    flags |= Style_Sunken;
						    } else
							    flags |= Style_Raised;
						    pe = PE_SpinWidgetUp;

						    QRect re = sw->upRect();
						    QColorGroup ucg = sw->isUpEnabled() ? cg : sw->palette().disabled();
						    p->fillRect(re, ucg.base());
						    drawPrimitive(pe, p, re, ucg, flags);
					    }

					    if ( controls & SC_SpinWidgetDown ) {
						    flags = Style_Default | Style_Enabled;
						    if (active == SC_SpinWidgetDown ) {
							    flags |= Style_On;
							    flags |= Style_Sunken;
						    } else
							    flags |= Style_Raised;
						    pe = PE_SpinWidgetDown;

						    QRect re = sw->downRect();
						    QColorGroup dcg = sw->isDownEnabled() ? cg : sw->palette().disabled();
						    p->fillRect(re, dcg.base());
						    drawPrimitive(pe, p, re, dcg, flags);
					    }
					    break; }

					    // SLIDER - KDE code for double buffering
					    // -------------------------------------------------------------------
		case CC_Slider: {
					const QSlider* slider = (const QSlider*)widget;
					QRect groove = querySubControlMetrics(CC_Slider, widget, SC_SliderGroove, opt);
					QRect handle = querySubControlMetrics(CC_Slider, widget, SC_SliderHandle, opt);

					// Double-buffer slider for no flicker
					QPixmap pix(widget->size());
					QPainter p2;
					p2.begin(&pix);
					if ( slider->parentWidget() &&
							(slider->parentWidget()->paletteBackgroundPixmap() && !slider->parentWidget()->paletteBackgroundPixmap()->isNull() )) {
						QPixmap pixmap = *(slider->parentWidget()->backgroundPixmap());
						if (QWidget *topDaddy = slider->topLevelWidget()){
							QPoint zero = QPoint(0,0);
#if 0
							zero = slider->mapTo(topDaddy, zero);
							int zxmpw = zero.x()%pixmap.width();
							int zymph;
#endif
							// probably due to argb mismatches in drawpixmap, this fails on kompmgr startups
							p2.drawTiledPixmap(r, pixmap, slider->mapTo(topDaddy, zero));
#if 0
							for (int dx = 0; dx < pix.width(); dx += pixmap.width())
							{
								zymph = zero.y()%pixmap.height();
								for (int dy = 0; dy < pix.height(); dy += pixmap.height())
								{
									copyBlt (&pix, dx, dy, &pixmap,
											zxmpw, zymph,
											pix.width()-dx>pixmap.width()-zxmpw?pixmap.width()-zxmpw:pix.width()-dx, pix.height()-dy>pixmap.height()-zymph?pixmap.height()-zymph:pix.height()-dy);
									zymph=0;
								}
								zxmpw=0;
							}
#endif
						} else
						{
							// probably due to argb mismatches in drawpixmap, this fails on kompmgr startups
							p2.drawTiledPixmap(r, pixmap, slider->pos());
#if 0
							for (int dx = 0; dx < pix.width(); dx += pixmap.width())
								for (int dy = 0; dy < pix.height(); dy += pixmap.height())
									copyBlt (&pix, dx, dy, &pixmap, 0, 0,
											pix.width()-dx>pixmap.width()?pixmap.width():pix.width()-dx, pix.height()-dy>pixmap.height()?pixmap.height():pix.height()-dy);
#endif
						}
						//                                 p2.begin(&pix);
					} else{
						//                                      p2.begin(&pix);
						pix.fill(cg.background());
					}

					// Draw slider groove
					if ((controls & SC_SliderGroove) && groove.isValid()) {
						drawKStylePrimitive( KPE_SliderGroove, &p2, widget, groove, cg, flags, opt );

						// Draw the focus rect around the groove
						/*				if (slider->hasFocus())
										drawPrimitive(PE_FocusRect, &p2, groove, cg);*/
					}

					// Draw the tickmarks
					if (controls & SC_SliderTickmarks)
						QCommonStyle::drawComplexControl(control, &p2, widget,
								r, cg, flags, SC_SliderTickmarks, active, opt);

					// Draw the slider handle
					if ((controls & SC_SliderHandle) && handle.isValid()) {
						if (active == SC_SliderHandle)
							flags |= Style_Active;
						drawKStylePrimitive( KPE_SliderHandle, &p2, widget, handle, cg, flags, opt );
					}

					p2.end();
					bitBlt((QWidget*)widget, r.x(), r.y(), &pix);
					break;
				}

#ifndef QT_NO_LISTVIEW
		case CC_ListView: {
					  if ( controls & SC_ListView ) {
						  QCommonStyle::drawComplexControl( control, p, widget, r, cg, flags, controls, active, opt );
					  }
					  if ( controls & ( SC_ListViewBranch | SC_ListViewExpand ) ) {
						  if ( opt.isDefault() )
							  break;

						  QListViewItem *item = opt.listViewItem();
						  QListViewItem *child = item->firstChild();
						  QListView* v = item->listView();
						  bool drawDotlines = optionHandler->DrawDotlines();
						  static QBitmap *verticalLine = 0, *horizontalLine = 0;
						  static QCleanupHandler<QBitmap> qlv_cleanup_bitmap;
						  int dotoffset = 0;

						  if (drawDotlines && (optionHandler->DotlineStyle() == Dots)){
							  if ( !verticalLine ) {
								  // make 128*1 and 1*128 bitmaps that can be used for
								  // drawing the right sort of lines.
								  verticalLine = new QBitmap( 1, 128, TRUE );
								  horizontalLine = new QBitmap( 128, 1, TRUE );
								  QPointArray a( 64 );
								  QPainter p;
								  p.begin( verticalLine );
								  int i;
								  for( i=0; i<64; i++ )
									  a.setPoint( i, 0, i*2+1 );
								  p.setPen( optionHandler->DotlineColor() );
								  p.drawPoints( a );
								  p.end();
								  QApplication::flushX();
								  verticalLine->setMask( *verticalLine );
								  p.begin( horizontalLine );
								  for( i=0; i<64; i++ )
									  a.setPoint( i, i*2+1, 0 );
								  p.setPen( optionHandler->DotlineColor() );
								  p.drawPoints( a );
								  p.end();
								  QApplication::flushX();
								  horizontalLine->setMask( *horizontalLine );
								  qlv_cleanup_bitmap.add( &verticalLine );
								  qlv_cleanup_bitmap.add( &horizontalLine );
							  }
						  }


						  int y = r.y();
						  int c;
						  QPointArray dotlines;
						  if ( drawDotlines && active == SC_All && controls == SC_ListViewExpand ) {
							  c = 2;
							  dotlines.resize( 2 );
							  dotlines[ 0 ] = QPoint( r.right(), r.top() );
							  dotlines[ 1 ] = QPoint( r.right(), r.bottom() );
						  }
						  else {
							  int linetop = 0, linebot = 0;
							  // each branch needs at most two lines, ie. four end points
							  dotoffset = (item->itemPos() + item->height() - y) %2;
							  dotlines.resize( item->childCount() * 4 );
							  c = 0;

							  // skip the stuff above the exposed rectangle
							  while ( child && y + child->height() <= 0 ) {
								  y += child->totalHeight();
								  child = child->nextSibling();
							  }

							  int bx = r.width() / 2;

							  // paint stuff in the magical area
							  while ( child && y < r.height() ) {
								  int lh;
								  if ( !item->multiLinesEnabled() )
									  lh = child ? child->height() : 0;
								  else
									  lh = p->fontMetrics().height() + 2 * v->itemMargin();
								  lh = QMAX( lh, QApplication::globalStrut().height() );
								  if ( lh % 2 > 0 )
									  lh++;
								  linebot = y + lh / 2;
								  if ( ( child->isExpandable() || child->childCount() ) && ( child->height() > 0 ) ) {
									  if ( optionHandler->ExpanderStyle() == MS ) { // f****** M$ +/- style :-(
										  // needs a box
										  p->setPen( optionHandler->DotlineColor() );
										  p->drawRect( bx - 4, linebot - 4, 9, 9 );
										  // plus or minus
										  p->setPen( optionHandler->UseCustomExpanderColor() ? optionHandler->CustomExpanderColor() : cg.text() );
										  p->drawLine( bx - 2, linebot, bx + 2, linebot );
										  if ( !child->isOpen() )
											  p->drawLine( bx, linebot - 2, bx, linebot + 2 );
									  } else { // ultracool triangles ;-)
										  QPointArray a;
										  p->setPen( optionHandler->UseCustomExpanderColor() ? optionHandler->CustomExpanderColor() : cg.text() );
										  if ( child->isOpen() ) {
											  a.setPoints( 3, bx - 4, linebot - 2,
													  bx, linebot + 2,
													  bx + 4, linebot - 2 );
										  } //DownArrow
										  else {
											  a.setPoints( 3, bx - 2, linebot - 4,
													  bx + 2, linebot,
													  bx - 2, linebot + 4 );
										  } //RightArrow
										  p->setBrush( optionHandler->UseCustomExpanderColor() ? optionHandler->CustomExpanderColor() : cg.text() );
										  p->drawPolygon( a );

										  p->setBrush( NoBrush );
									  }
									  // dotlinery
									  if ( drawDotlines ) {
										  dotlines[ c++ ] = QPoint( bx, linetop );
										  dotlines[ c++ ] = QPoint( bx, linebot - (/* optionHandler->plusMinus() ? 4 : */5 ) );
										  dotlines[ c++ ] = QPoint( bx + 5, linebot );
										  dotlines[ c++ ] = QPoint( r.width(), linebot );
										  linetop = linebot + 5;
									  }
								  } else {
									  // just dotlinery
									  if ( drawDotlines ) {
										  dotlines[ c++ ] = QPoint( bx + 2, linebot );
										  dotlines[ c++ ] = QPoint( r.width(), linebot );
									  }
								  }

								  y += child->totalHeight();
								  child = child->nextSibling();
							  }

							  // Expand line height to edge of rectangle if there's a
							  // child, and it's visible
							  if ( child && ( child->height() > 0 ) ) {
								  linebot = r.height();
							  }

							  if ( drawDotlines ) {
								  if ( linetop < linebot ) {
									  dotlines[ c++ ] = QPoint( bx, linetop );
									  dotlines[ c++ ] = QPoint( bx, linebot );
								  }
							  }
						  }

						  if ( drawDotlines && optionHandler->DotlineStyle() == Line) {
							  int line; // index into dotlines
							  p->setPen( optionHandler->DotlineColor() );

							  if ( controls & SC_ListViewBranch )
								  for ( line = 0; line < c; line += 2 ) {
									  p->drawLine( dotlines[ line ].x(), dotlines[ line ].y(),
											  dotlines[ line + 1 ].x(), dotlines[ line + 1 ].y() );
								  }

						  }
						  else if (drawDotlines && (optionHandler->DotlineStyle() == Dots)) {
							  int line; // index into dotlines
							  if ( controls & SC_ListViewBranch ){
								  p->setPen( optionHandler->DotlineColor() );
								  for( line = 0; line < c; line += 2 ) {
									  // assumptions here: lines are horizontal or vertical.
									  // lines always start with the numerically lowest
									  // coordinate.

									  // point ... relevant coordinate of current point
									  // end ..... same coordinate of the end of the current line
									  // other ... the other coordinate of the current point/line
									  if ( dotlines[line].y() == dotlines[line+1].y() ) {
										  int end = dotlines[line+1].x();
										  int point = dotlines[line].x();
										  int other = dotlines[line].y();
										  while( point < end ) {
											  int i = 128;
											  if ( i+point > end )
												  i = end-point;
											  p->drawPixmap( point, other, *horizontalLine, 0, 0, i, 1 );
											  point += i;
										  }
									  }
									  else {
										  int end = dotlines[line+1].y();
										  int point = dotlines[line].y();
										  int other = dotlines[line].x();
										  int pixmapoffset = ((point & 1) != dotoffset ) ? 1 : 0;
										  while( point < end ) {
											  int i = 128;
											  if ( i+point > end )
												  i = end-point;
											  p->drawPixmap( other, point, *verticalLine, 0, pixmapoffset, 1, i );
											  point += i;
										  }
									  }
								  }
							  }
						  }
					  }

					  break;
				  }
#endif // QT_NO_LISTVIEW

#ifndef QT_NO_TITLEBAR
		case CC_TitleBar:
				  {
					  switch (optionHandler->style())
					  {
						  case Jaguar:
							  if (!widget->isActiveWindow()) p->drawTiledPixmap(r, *getPixmap( TitleBar ));
							  p->setPen(optionHandler->titleColor(1).dark(180));
							  p->drawLine(r.left(), r.bottom(), r.right(), r.bottom());
							  break;
						  case Panther:
						  case Tiger:
						  case Milk:
							  {
								  if (widget->isActiveWindow()) p->drawTiledPixmap(r, *getPixmap( TitleBar ));
								  p->setPen(optionHandler->titleColor(1).dark(180));
								  p->drawLine(r.left(), r.bottom(), r.right(), r.bottom());
								  break;
							  }
						  case Brushed:
							  break;
					  }
					  if ( controls & SC_TitleBarLabel )
					  {
						  QRect ir = visualRect( querySubControlMetrics( CC_TitleBar, widget, SC_TitleBarLabel ), widget );
						  p->setPen( cg.text() );
						  p->drawText(ir.x()+2, ir.y(), ir.width()-2, ir.height(), AlignCenter | AlignVCenter | SingleLine, widget->caption() );
					  }

					  QRect ir;
					  bool down = FALSE;
					  QPixmap pm;

					  if ( controls & SC_TitleBarCloseButton )
					  {
						  ir = visualRect( querySubControlMetrics( CC_TitleBar, widget, SC_TitleBarCloseButton ), widget );
						  down = active & SC_TitleBarCloseButton;
						  if (optionHandler->buttonStyle() == Brushed)
							  drawRectangularButton( p, cg, down ? optionHandler->titleButtonColor(0).dark(120) : optionHandler->titleButtonColor(0), ir.x(), ir.y(), ir.width(), ir.height(), down, false, false, full);
						  else
							  drawCombo( p, cg, down ? optionHandler->titleButtonColor(0).dark(120) : optionHandler->titleButtonColor(0), ir.x(), ir.y(), ir.width(), ir.height()+2, down, false, false, full );
					  }
					  if ( controls & SC_TitleBarSysMenu )
					  {
						  ir = visualRect( querySubControlMetrics( CC_TitleBar, widget, SC_TitleBarSysMenu ), widget );
						  down = active & SC_TitleBarSysMenu;
						  if (optionHandler->buttonStyle() == Brushed)
							  drawRectangularButton( p, cg, down ? cg.button().dark(120) : cg.button(), ir.x(), ir.y(), ir.width(), ir.height(), down, false, false, full);
						  else
							  drawCombo( p, cg, down ? cg.button().dark(120) : cg.button(), ir.x(), ir.y(), ir.width(), ir.height()+2, down, false, false, full );
					  }

					  if ( controls & SC_TitleBarMaxButton )
					  {
						  ir = visualRect( querySubControlMetrics( CC_TitleBar, widget, SC_TitleBarMaxButton ), widget );
						  down = active & SC_TitleBarMaxButton;
						  if (optionHandler->buttonStyle() == Brushed)
							  drawRectangularButton( p, cg, down ? optionHandler->titleButtonColor(2).dark(120) : optionHandler->titleButtonColor(2), ir.x(), ir.y(), ir.width(), ir.height(), down, false, false, full);
						  else
							  drawCombo( p, cg, down ? optionHandler->titleButtonColor(2).dark(120) : optionHandler->titleButtonColor(2), ir.x(), ir.y(), ir.width(), ir.height()+2, down, false, false, full );
					  }

					  if ( controls & SC_TitleBarMinButton )
					  {
						  ir = visualRect( querySubControlMetrics( CC_TitleBar, widget, SC_TitleBarMinButton ), widget );
						  down = active & SC_TitleBarMinButton;
						  if (optionHandler->buttonStyle() == Brushed)
							  drawRectangularButton( p, cg, down ? optionHandler->titleButtonColor(1).dark(120) : optionHandler->titleButtonColor(1), ir.x(), ir.y(), ir.width(), ir.height(), down, false, false, full);
						  else
							  drawCombo( p, cg, down ? optionHandler->titleButtonColor(1).dark(120) : optionHandler->titleButtonColor(1), ir.x(), ir.y(), ir.width(), ir.height()+2, down, false, false, full );
					  }
					  if ( controls & SC_TitleBarNormalButton )
					  {
						  ir = visualRect( querySubControlMetrics( CC_TitleBar, widget, SC_TitleBarNormalButton ), widget );
						  down = active & SC_TitleBarNormalButton;
						  if (optionHandler->buttonStyle() == Brushed)
							  drawRectangularButton( p, cg, down ? optionHandler->titleButtonColor(1).dark(120) : optionHandler->titleButtonColor(1), ir.x(), ir.y(), ir.width(), ir.height(), down, false, false, full);
						  else
							  drawCombo( p, cg, down ? optionHandler->titleButtonColor(1).dark(120) : optionHandler->titleButtonColor(1), ir.x(), ir.y(), ir.width(), ir.height()+2, down, false, false, full );
					  }

					  if ( controls & SC_TitleBarShadeButton )
					  {
						  ir = visualRect( querySubControlMetrics( CC_TitleBar, widget, SC_TitleBarShadeButton ), widget );
						  down = active & SC_TitleBarShadeButton;
						  if (optionHandler->buttonStyle() == Brushed)
							  drawRectangularButton( p, cg, down ? cg.button().dark(120) : cg.button(), ir.x(), ir.y(), ir.width(), ir.height(), down, false, false, full);
						  else
							  drawCombo( p, cg, down ? cg.button().dark(120) : cg.button(), ir.x(), ir.y(), ir.width(), ir.height()+2, down, false, false, full );
					  }

					  if ( controls & SC_TitleBarUnshadeButton )
					  {
						  ir = visualRect( querySubControlMetrics( CC_TitleBar, widget, SC_TitleBarUnshadeButton ), widget );
						  down = active & SC_TitleBarUnshadeButton;
						  if (optionHandler->buttonStyle() == Brushed)
							  drawRectangularButton( p, cg, down ? cg.button().dark(120) : cg.button(), ir.x(), ir.y(), ir.width(), ir.height(), down, false, false, full);
						  else
							  drawCombo( p, cg, down ? cg.button().dark(120) : cg.button(), ir.x(), ir.y(), ir.width(), ir.height()+2, down, false, false, full );
					  }
					  break;
				  }
#endif //QT_NO_TITLEBAR

		default:
				  KStyle::drawComplexControl( control, p, widget,
						  r, cg, flags, controls, active, opt );
				  break;
	}
}

int LiquidStyle::styleHint(StyleHint sh, const QWidget * w, const QStyleOption & opt, QStyleHintReturn * shr) const
{
	switch (sh)
	{
		case QStyle::SH_GUIStyle:
			//       if (w && ::qt_cast<QMenuBar*>(w))
			//          return WindowsStyle;
			return MacStyle;
		case QStyle::SH_TabBar_Alignment:
			if (optionHandler->CenterTabs() && !QApplication::reverseLayout())
				return AlignHCenter;
			else
				return KStyle::styleHint(sh, w, opt, shr);
		default:
			return KStyle::styleHint(sh, w, opt, shr);
	}
}

QRect LiquidStyle::subRect( SubRect r, const QWidget *widget ) const {
	if (r == SR_ComboBoxFocusRect){
		return querySubControlMetrics( CC_ComboBox, widget, SC_ComboBoxEditField );
	}
	return ( KStyle::subRect( r, widget ) );
}

int LiquidStyle::pixelMetric( PixelMetric m, const QWidget *widget ) const
{
	switch ( m )
	{
		case PM_ButtonMargin:
			return ( 5 );
		case PM_ButtonDefaultIndicator:
			return ( 0 );
		case PM_ExclusiveIndicatorWidth:
		case PM_ExclusiveIndicatorHeight:
			return ( 16 );
		case PM_IndicatorWidth:
		case PM_IndicatorHeight:
			return ( 16 );
		case PM_ScrollBarExtent:
			return ( 15 );
		case PM_ScrollBarSliderMin:
			if (isOOO) return 10; return ( 40 );
		case PM_SplitterWidth:
			return 6;
		case PM_SliderControlThickness:
			return(15);
		case PM_SliderThickness:
			return ( 22 );
			/*          if (widget && (widget->width() < widget->height() && widget->width() < 17) || (widget->width() > widget->height() && widget->height() < 17))
				    return (16);*/
		case PM_SliderLength:
			return ( optionHandler->style() == Milk ? 15 : 13 );
		case PM_TabBarTabOverlap:
			return 0;
		case PM_DefaultFrameWidth:
			{
				/*         
					   if (widget && ::qt_cast<QFrame*>(widget) && ((QFrame*)widget)->frameShape() == QFrame::LineEditPanel)
					   return 6;*/
				if (widget && widget->parentWidget() && ::qt_cast<QTabWidget*>(widget->parentWidget()) && widget->parentWidget()->parentWidget() && (widget->parentWidget()->parentWidget()->inherits("KonqMainWindow") || widget->parentWidget()->parentWidget()->inherits("KonqFrameContainer"))) //for the konqueror statusbar offset, thanks konqueror team... ... ... f***f***f***!!!
					return 0;
				return 2;
			}
      case PM_TabBarBaseOverlap:
      case PM_TabBarBaseHeight:
      {
         if (!widget || !::qt_cast<QTabWidget*>(widget) || !widget->children())
            return 0;
         QObjectListIt it( *widget->children() ); // iterate over the buttons
         QObject *obj;
         QTabBar *tabBar = 0L;
         while ( (obj = it.current()) != 0 )
         {
            ++it;
            if (::qt_cast<QTabBar*>(obj))
            {
               tabBar = (QTabBar*)obj;
               break;
            }
         }
         if (!tabBar || !tabBar->isVisible())
            return 0;
         if (optionHandler->TabStyle() == Chooser || (optionHandler->TabStyle() == Clever && widget && widget->topLevelWidget()->inherits("QDialog")))
            return 12;
         if (m == PM_TabBarBaseOverlap)
            return 2;
         return 7;
      }
		case PM_TabBarTabHSpace:
			if (optionHandler->TabStyle() == Chooser || (optionHandler->TabStyle() == Clever && widget && widget->topLevelWidget()->inherits("QDialog"))) return 18;
			return 24;
		case PM_TabBarTabVSpace:
			{
				if (optionHandler->TabStyle() == Chooser || (optionHandler->TabStyle() == Clever && widget && widget->topLevelWidget()->inherits("QDialog"))) return 6;
				return 10;
			}
		case PM_TabBarTabShiftHorizontal:
			return 0;
		case PM_TabBarTabShiftVertical:
			if (optionHandler->TabStyle() == Chooser || (optionHandler->TabStyle() == Clever && widget && widget->topLevelWidget()->inherits("QDialog"))) return 0;
			return 2;
		case PM_ButtonShiftHorizontal:
			return 0;
		case PM_ButtonShiftVertical:
			return 0;
		default:
			return KStyle::pixelMetric( m, widget );
	}
}

QSize LiquidStyle::sizeFromContents( ContentsType contents,
		const QWidget *widget,
		const QSize &contentSize,
		const QStyleOption& opt ) const {
	switch ( contents ) {
		case CT_PushButton: {
					    // this is a little funky - we give values not based on pixelMetric
					    // because we want a custom width/height and we can only give one
					    // value in pixelMetric (used in sizeHint). Odd but works well
					    const QPushButton * button = ( const QPushButton* ) widget;

					    int w = contentSize.width() + 26 > 80 ? contentSize.width() + 26 : 80;
					    int h = contentSize.height() > 24 ? contentSize.height() : 25;

					    if ( button->text().isEmpty() )
						    return ( QSize( contentSize.width() + 2 * pixelMetric( PM_ButtonMargin, widget ), h ) );

					    return ( QSize( w, h ) );
				    }

		case CT_ToolButton: {
					    if (widget->parentWidget() && ::qt_cast<QTabWidget*>(widget->parentWidget())){
						    return QSize(contentSize.width() + 6, contentSize.height() + 5);
					    }
					    else{
						    int w = contentSize.width() + 6;
						    int sh = contentSize.height() + 5;
						    int sw = (int)(1.3*sh);
						    if (w < sw)
							    return QSize(sw, sh);
						    else
							    return QSize(w, sh);
					    }
				    }

		case CT_Slider: {
					int h = contentSize.height() > 24 ? contentSize.height() : 24;
					return ( QSize( contentSize.width(), h ) );
				}

		case CT_ComboBox: {
					  // 				QSize sz = KStyle::sizeFromContents( contents, widget, contentSize, opt );
					  // 				return ( QSize( sz.width()+3, sz.height() > 25 ? sz.height() : 25 ) );
					  if (  ::qt_cast<const QComboBox *>(widget) && ((const QComboBox *)widget)->editable())
						  return ( QSize( contentSize.width()+27, contentSize.height() + 4 > 22 ? contentSize.height()  + 4 : 26 ) );
					  else
						  return ( QSize( contentSize.width()+27, contentSize.height() + 2 > 24 ? contentSize.height() + 2 : 24 ) );
				  }
		case CT_PopupMenuItem: {
					       if ( ! widget || opt.isDefault() )
						       break;

					       const QPopupMenu *popup = ( const QPopupMenu * ) widget;
					       bool checkable = popup->isCheckable();
					       QMenuItem *mi = opt.menuItem();
					       int maxpmw = opt.maxIconWidth();
					       int w = contentSize.width();
					       int h = contentSize.height();

					       if ( mi->custom() ) {
						       w = mi->custom() ->sizeHint().width();
						       h = mi->custom() ->sizeHint().height();
						       if ( !mi->custom() ->fullSpan() )
							       h += 2 * windowsItemVMargin + 2 * windowsItemFrame;
					       } else if ( mi->widget() ) {}
					       else if ( mi->isSeparator() ) {
						       w = 10;
						       h = windowsSepHeight;
					       } else {
						       if ( mi->pixmap() )
							       h = QMAX( h, mi->pixmap() ->height() + 2 * windowsItemFrame );
						       else if ( ! mi->text().isNull() )
							       h = QMAX( h, popup->fontMetrics().height() + 2 * windowsItemVMargin +
									       2 * windowsItemFrame );
						       if ( mi->iconSet() != 0 )
							       h = QMAX( h, mi->iconSet() ->pixmap( QIconSet::Small,
										       QIconSet::Normal ).height() +
									       2 * windowsItemFrame );
					       }
					       if ( !mi->text().isNull() && mi->text().find( '\t' ) >= 0 )
						       w += windowsTabSpacing;
					       else if ( mi->popup() )
						       w += 2 * windowsArrowHMargin;
					       if ( checkable && maxpmw < 20 )
						       w += 20 - maxpmw;
					       if ( maxpmw )
						       w += maxpmw + 6;
					       if ( checkable || maxpmw > 0 )
						       w += windowsCheckMarkHMargin;
					       w += 20;
					       return ( QSize( w, h ) );
				       }
		default:
				       break;
	}
	return KStyle::sizeFromContents( contents, widget, contentSize, opt );
}


QPixmap LiquidStyle::stylePixmap( StylePixmap stylepixmap,
		const QWidget* widget,
		const QStyleOption& opt ) const {
	return KStyle::stylePixmap( stylepixmap, widget, opt );
}

#include "eventfilter.cpp"

QRect LiquidStyle::querySubControlMetrics( ComplexControl control, const QWidget *widget, SubControl subcontrol, const QStyleOption &opt ) const
{
	if ( control == CC_ComboBox && subcontrol == SC_ComboBoxEditField )
		return QRect( 8, 3, widget->width() - 32, widget->height() - 7 );
	if (control == CC_TitleBar)
	{
		switch (subcontrol)
		{
			case SC_TitleBarSysMenu:
				return QRect( widget->width() - 24, 1, 16, 16 );
			case SC_TitleBarMinButton:
				return QRect( 26, 1, 16, 16 );
			case SC_TitleBarMaxButton:
				return QRect( 47, 1, 16, 16 );
			case SC_TitleBarCloseButton:
				return QRect( 5, 1, 16, 16 );
			case SC_TitleBarLabel:
				return QRect( 68, 1, widget->width() - 107, 16 );
			case SC_TitleBarNormalButton:
				return QRect( 26, 1, 16, 16 );
			case SC_TitleBarShadeButton:
				return QRect( widget->width() - 29, 1, 24, 16 );
			case SC_TitleBarUnshadeButton:
				return QRect( widget->width() - 29, 1, 24, 16 );
			default:
				return ( KStyle::querySubControlMetrics( control, widget, subcontrol, opt ) );
		}
	}
	else
		return ( KStyle::querySubControlMetrics( control, widget, subcontrol, opt ) );
}

QImage LiquidStyle::fetchImage( const char *name ){
	return uic_findImage( name );
}

QColor LiquidStyle::mapFadeColor(QColor &color, int index) const
{
	QRgb **rgb = fadeColorMap.find(color.rgb());
	if (rgb)
		return QColor((*rgb)[index]);
	// no color map found, create one and return the queried value
	rgb = new QRgb*;
	*rgb = new QRgb[10];
	int iRed = optionHandler->InactiveButtonColor().red();
	int iGreen = optionHandler->InactiveButtonColor().green();
	int iBlue = optionHandler->InactiveButtonColor().blue();
	int cRed = color.red();
	int cGreen = color.green();
	int cBlue = color.blue();
	for (int i = 10; i < 20; i++)
	{
		(*rgb)[i-10] = qRgb( iRed + (cRed-iRed)*(i)/20, iGreen + (cGreen-iGreen)*(i)/20, iBlue + (cBlue-iBlue)*(i)/20);
	}
	LiquidStyle *ptr = const_cast<LiquidStyle*>( this );
	ptr->fadeColorMap.insert(color.rgb(), rgb);
	return QColor((*rgb)[index]);
}

QPixmap* LiquidStyle::processEmbedded( const char *label, const QColor &c, bool blend, const QColor *bg ) const {
	QImage img( uic_findImage( label ) );
	img.detach();
	if ( img.isNull() ) { // shouldn't happen, been tested
		qWarning( "Invalid embedded label %s", label );
		return ( NULL );
	}
	return ( adjustHSV( img, c, bg ) );
}

QPixmap* LiquidStyle::getPixmap( BitmapData item ) const
{
	if ( pixmaps[ item ] )
		return ( pixmaps[ item ] );
	QColor bgColor( optionHandler->InactiveButtonColor() );
	QColor btnColor( isOOO ? optionHandler->CustomButtonColor() : qApp->palette().active().button() );
	QColor btnHoverColor( isOOO ? optionHandler->CustomButtonColor().light(120) : btnColor.light( 120 ) );
	QColor sbGrooveColor( optionHandler->useCustomColors() ?
			optionHandler->customColor( CustomSBGroove ) : bgColor );
	QColor sbSliderColor( optionHandler->useCustomColors() ?
			optionHandler->customColor( CustomSBSlider ) : bgColor );
	QColor sbSliderHoverColor( optionHandler->useCustomColors() ?
			optionHandler->customColor( CustomSBSliderHover ) : isOOO?optionHandler->CustomButtonColor():btnColor );
	QColor sbSliderPressedColor( optionHandler->useCustomColors() ?
			optionHandler->customColor( CustomSBSliderPressed ) : btnColor.dark(110) );
	LiquidStyle *ptr = const_cast<LiquidStyle*>( this );
	switch ( item )
	{
		case RadioOn:
			ptr->pixmaps[ RadioOn ] = processEmbedded( (optionHandler->style() == Milk) ?  "radio_down-milk" : "radio_down", optionHandler->useCustomColors() ? optionHandler->customColor( CustomRadioOn ) : btnColor, true );
			break;
		case RadioOff:
			ptr->pixmaps[ RadioOff ] = processEmbedded( (optionHandler->style() == Milk) ?  "radio-milk" : "radio", optionHandler->useCustomColors() ? optionHandler->customColor( CustomRadioOff ) : bgColor, true );
			break;
		case RadioOnHover:
			ptr->pixmaps[ RadioOnHover ] = processEmbedded( (optionHandler->style() == Milk) ?  "radio_down-milk" : "radio_down", optionHandler->useCustomColors() ? optionHandler->customColor( CustomRadioOn ).light( 110 ) : btnHoverColor, true );
			break;
		case RadioOffHover:
			ptr->pixmaps[ RadioOffHover ] = processEmbedded( (optionHandler->style() == Milk) ?  "radio-milk" : "radio", optionHandler->useCustomColors() ? optionHandler->customColor( CustomRadioOn ) : btnColor, true );
		case RadioOnPressed:
			ptr->pixmaps[ RadioOnPressed ] = processEmbedded( (optionHandler->style() == Milk) ?  "radio_down-milk" : "radio_down", optionHandler->useCustomColors() ? optionHandler->customColor( CustomRadioOn ).dark( 120 ) : btnHoverColor.dark( 120 ), true );
			break;
		case RadioOffPressed:
			ptr->pixmaps[ RadioOffPressed ] = processEmbedded( (optionHandler->style() == Milk) ?  "radio-milk" : "radio", optionHandler->useCustomColors() ? optionHandler->customColor( CustomRadioOn ).dark( 120 ) : btnColor.dark( 120 ), true );
			break;
		case Tab:
			ptr->pixmaps[ Tab ] = processEmbedded( (optionHandler->style() == Jaguar) ? "tab-jaguar" : (optionHandler->style() == Milk) ? "tab-milk" : "tab", optionHandler->useCustomColors() ? optionHandler->customColor( CustomTabOff ) : optionHandler->InactiveButtonColor(), true );
			break;
		case belowTab:
			ptr->pixmaps[ belowTab ] = processEmbedded( (optionHandler->style() == Jaguar) ? "tab-jaguar" : (optionHandler->style() == Milk) ? "tab-milk" : "tab", optionHandler->useCustomColors() ? optionHandler->customColor( CustomTabOff ) : optionHandler->InactiveButtonColor(), true );
			*ptr->pixmaps[ belowTab ] = ptr->pixmaps[ belowTab ] ->xForm( iMatrix );
			break;
		case TabDown:
			ptr->pixmaps[ TabDown ] = processEmbedded( (optionHandler->style() == Jaguar) ? "tab-jaguar" : (optionHandler->style() == Milk) ? "tab-milk" : "tab", optionHandler->useCustomColors() ? optionHandler->customColor( CustomTabOn ) : btnColor, true );
			break;
		case belowTabDown:
			ptr->pixmaps[ belowTabDown ] = processEmbedded( (optionHandler->style() == Jaguar) ? "tab-jaguar" : (optionHandler->style() == Milk) ? "tab-milk" : "tab", optionHandler->useCustomColors() ? optionHandler->customColor( CustomTabOn ) : btnColor, true );
			*ptr->pixmaps[ belowTabDown ] = ptr->pixmaps[ belowTabDown ] ->xForm( iMatrix );
			break;
		case TabFocus:
			ptr->pixmaps[ TabFocus ] = processEmbedded( (optionHandler->style() == Jaguar) ? "tab-jaguar" : (optionHandler->style() == Milk) ? "tab-milk" : "tab", optionHandler->useCustomColors() ? optionHandler->customColor( CustomTabOn ).light( 120 ) : btnHoverColor, true );
			break;
		case CB:
			ptr->pixmaps[ CB ] = processEmbedded( (optionHandler->style() == Milk) ? "checkbox-milk" : "checkbox", optionHandler->useCustomColors() ? optionHandler->customColor( CustomCBOff ) : bgColor, true );
			break;
		case CBDown:
			ptr->pixmaps[ CBDown ] = processEmbedded( (optionHandler->style() == Milk) ? "checkboxdown-milk" : "checkboxdown", optionHandler->useCustomColors() ? optionHandler->customColor( CustomCBOn ) : btnColor, true );
			break;
		case CBDownHover:
			ptr->pixmaps[ CBDownHover ] = processEmbedded( (optionHandler->style() == Milk) ? "checkboxdown-milk" : "checkboxdown", optionHandler->useCustomColors() ? optionHandler->customColor( CustomCBOn ).light( 110 ) : btnHoverColor, true );
			break;
		case CBTri:
			ptr->pixmaps[ CBTri ] = processEmbedded( (optionHandler->style() == Milk) ? "checkboxdown-milk" : "checkboxdown", optionHandler->useCustomColors() ? optionHandler->customColor( CustomCBOn ) : btnColor, true );
			*ptr->pixmaps[ CBTri ] = ptr->pixmaps[ CBTri ] ->xForm( mMatrix );
			break;
		case CBTriHover:
			ptr->pixmaps[ CBTriHover ] = processEmbedded( (optionHandler->style() == Milk) ? "checkboxdown-milk" : "checkboxdown", optionHandler->useCustomColors() ? optionHandler->customColor( CustomCBOn ).light( 110 ) : btnHoverColor, true );
			*ptr->pixmaps[ CBTriHover ] = ptr->pixmaps[ CBTriHover ] ->xForm( mMatrix );
			break;
		case CBHover:
			ptr->pixmaps[ CBHover ] = processEmbedded( (optionHandler->style() == Milk) ? "checkbox-milk" : "checkbox", optionHandler->useCustomColors() ? optionHandler->customColor( CustomCBOn ) : btnColor, true );
			break;
		case CBDownPressed:
			ptr->pixmaps[ CBDownPressed ] = processEmbedded( (optionHandler->style() == Milk) ? "checkboxdown-milk" : "checkboxdown", optionHandler->useCustomColors() ? optionHandler->customColor( CustomCBOn ).dark( 120 ) : btnHoverColor.dark( 120 ), true );
			break;
		case CBTriPressed:
			ptr->pixmaps[ CBTriPressed ] = processEmbedded( (optionHandler->style() == Milk) ? "checkboxdown-milk" : "checkboxdown", optionHandler->useCustomColors() ? optionHandler->customColor( CustomCBOn ).dark( 120 ) : btnHoverColor.dark( 120 ), true );
			*ptr->pixmaps[ CBTriPressed ] = ptr->pixmaps[ CBTriPressed ] ->xForm( mMatrix );
			break;
		case CBPressed:
			ptr->pixmaps[ CBPressed ] = processEmbedded( (optionHandler->style() == Milk) ? "checkbox-milk" : "checkbox", optionHandler->useCustomColors() ? optionHandler->customColor( CustomCBOn ).dark( 120 ) : btnColor.dark( 120 ), true );
			break;
		case HSlider:
			ptr->pixmaps[ HSlider ] = processEmbedded( (optionHandler->style() == Milk) ? "sliderarrow-milk" : "sliderarrow", btnColor, true );
			break;
		case VSlider:
			ptr->pixmaps[ VSlider ] = processEmbedded( (optionHandler->style() == Milk) ? "sliderarrow-milk" : "sliderarrow", btnColor, true );
			*ptr->pixmaps[ VSlider ] = ptr->pixmaps[ VSlider ] ->xForm( lMatrix );
			break;
		case VSliderAbove:
			ptr->pixmaps[ VSliderAbove ] = processEmbedded( (optionHandler->style() == Milk) ? "sliderarrow-milk" : "sliderarrow", btnColor, true );
			*ptr->pixmaps[ VSliderAbove ] = ptr->pixmaps[ VSliderAbove ] ->xForm( rMatrix );
			break;
		case HSliderInactive:
			ptr->pixmaps[ HSliderInactive ] = processEmbedded( (optionHandler->style() == Milk) ? "sliderarrow-milk" : "sliderarrow", bgColor, true );
			break;
		case VSliderInactive:
			ptr->pixmaps[ VSliderInactive ] = processEmbedded( (optionHandler->style() == Milk) ? "sliderarrow-milk" : "sliderarrow", bgColor, true );
			*ptr->pixmaps[ VSliderInactive ] = ptr->pixmaps[ VSliderInactive ] ->xForm( lMatrix );
			break;
		case VSliderAboveInactive:
			ptr->pixmaps[ VSliderAboveInactive ] = processEmbedded( (optionHandler->style() == Milk) ? "sliderarrow-milk" : "sliderarrow", bgColor, true );
			*ptr->pixmaps[ VSliderAboveInactive ] = ptr->pixmaps[ VSliderAboveInactive ] ->xForm( rMatrix );
			break;
		case VSBSliderTop:
			ptr->pixmaps[ VSBSliderTop ] = optionHandler->shadowSbSlider() ? createSliderEnd( sbSliderColor, sbGrooveColor.dark( 120 ), true) : processEmbedded( (optionHandler->style() == Milk) ? "sbslider_top-milk" : "sbslider_top", sbSliderColor, true, &sbGrooveColor );
			break;
		case VSBSliderBtm:
			ptr->pixmaps[ VSBSliderBtm ] = optionHandler->shadowSbSlider() ? createSliderEnd( sbSliderColor, sbGrooveColor.dark( 120 ), false) : processEmbedded( (optionHandler->style() == Milk) ? "sbslider_btm-milk" : "sbslider_btm", sbSliderColor, true, &sbGrooveColor );
			break;
		case VSBSliderMid:
			ptr->pixmaps[ VSBSliderMid ] = processEmbedded( (optionHandler->style() == Milk) ? "sbslider_mid-milk" : "sbIslider_mid", sbSliderColor, &sbGrooveColor );
			break;
		case VSBSliderTopActive:
			ptr->pixmaps[ VSBSliderTopActive ] = optionHandler->shadowSbSlider() ? createSliderEnd( sbSliderHoverColor, sbGrooveColor.dark( 120 ), true) : processEmbedded( (optionHandler->style() == Milk) ? "sbslider_top-milk" : "sbslider_top", sbSliderHoverColor, true, &sbGrooveColor );
			break;
		case VSBSliderBtmActive:
			ptr->pixmaps[ VSBSliderBtmActive ] = optionHandler->shadowSbSlider() ? createSliderEnd( sbSliderHoverColor, sbGrooveColor.dark( 120 ), false) : processEmbedded( (optionHandler->style() == Milk) ? "sbslider_btm-milk" : "sbslider_btm", sbSliderHoverColor, true, &sbGrooveColor );
			break;
		case VSBSliderMidActive:
			ptr->pixmaps[ VSBSliderMidActive ] = processEmbedded( (optionHandler->style() == Milk) ? "sbslider_mid-milk" : "sbslider_mid", sbSliderHoverColor, false, &sbGrooveColor );
			break;
		case VSBSliderTopPressed:
			ptr->pixmaps[ VSBSliderTopPressed ] = processEmbedded( (optionHandler->style() == Milk) ? "sbslider_top-milk" : "sbslider_top", sbSliderPressedColor, true, &sbGrooveColor );
			*ptr->pixmaps[ VSBSliderTopPressed ] = ptr->pixmaps[ VSBSliderTopPressed ] ->xForm( mMatrix );
			break;
		case VSBSliderBtmPressed:
			ptr->pixmaps[ VSBSliderBtmPressed ] = processEmbedded( (optionHandler->style() == Milk) ? "sbslider_btm-milk" : "sbslider_btm", sbSliderPressedColor, true, &sbGrooveColor );
			*ptr->pixmaps[ VSBSliderBtmPressed ] = ptr->pixmaps[ VSBSliderBtmPressed ] ->xForm( mMatrix );
			break;
		case VSBSliderMidPressed:
			ptr->pixmaps[ VSBSliderMidPressed ] = processEmbedded( (optionHandler->style() == Milk) ? "sbslider_mid-milk" : "sbslider_mid", sbSliderPressedColor, false, &sbGrooveColor );
			if (optionHandler->style() == Milk)
				*ptr->pixmaps[ VSBSliderMidPressed ] = ptr->pixmaps[ VSBSliderMidPressed ] ->xForm( iMatrix );
			break;

		case HSBSliderTop:
			ptr->pixmaps[ HSBSliderTop ] = optionHandler->shadowSbSlider() ? createSliderEnd( sbSliderColor, sbGrooveColor.dark( 120 ), true) : processEmbedded( (optionHandler->style() == Milk) ? "sbslider_top-milk" : "sbslider_top", sbSliderColor, true, &sbGrooveColor );
			*ptr->pixmaps[ HSBSliderTop ] = ptr->pixmaps[ HSBSliderTop ]->xForm( rMatrix );
			break;
		case HSBSliderBtm:
			ptr->pixmaps[ HSBSliderBtm ] = optionHandler->shadowSbSlider() ? createSliderEnd( sbSliderColor, sbGrooveColor.dark( 120 ), false) : processEmbedded( (optionHandler->style() == Milk) ? "sbslider_btm-milk" : "sbslider_btm", sbSliderColor, true, &sbGrooveColor );
			*ptr->pixmaps[ HSBSliderBtm ] = ptr->pixmaps[ HSBSliderBtm ] ->xForm( rMatrix );
			break;
		case HSBSliderMid:
			ptr->pixmaps[ HSBSliderMid ] = processEmbedded( (optionHandler->style() == Milk) ? "sbslider_mid-milk" : "sbIslider_mid", sbSliderColor, false, &sbGrooveColor );
			*ptr->pixmaps[ HSBSliderMid ] = ptr->pixmaps[ HSBSliderMid ]->xForm( rMatrix );
			break;
		case HSBSliderTopActive:
			ptr->pixmaps[ HSBSliderTopActive ] = optionHandler->shadowSbSlider() ? createSliderEnd( sbSliderHoverColor, sbGrooveColor.dark( 120 ), true) : processEmbedded( (optionHandler->style() == Milk) ? "sbslider_top-milk" : "sbslider_top", sbSliderHoverColor, true, &sbGrooveColor );
			*ptr->pixmaps[ HSBSliderTopActive ] = ptr->pixmaps[ HSBSliderTopActive ] ->xForm( rMatrix );
			break;
		case HSBSliderBtmActive:
			ptr->pixmaps[ HSBSliderBtmActive ] = optionHandler->shadowSbSlider() ? createSliderEnd( sbSliderHoverColor, sbGrooveColor.dark( 120 ), false) : processEmbedded( (optionHandler->style() == Milk) ? "sbslider_btm-milk" : "sbslider_btm", sbSliderHoverColor, true, &sbGrooveColor );
			*ptr->pixmaps[ HSBSliderBtmActive ] = ptr->pixmaps[ HSBSliderBtmActive ] ->xForm( rMatrix );
			break;
		case HSBSliderMidActive:
			ptr->pixmaps[ HSBSliderMidActive ] = processEmbedded( (optionHandler->style() == Milk) ? "sbslider_mid-milk" : "sbslider_mid", sbSliderHoverColor, false, &sbGrooveColor );
			*ptr->pixmaps[ HSBSliderMidActive ] = ptr->pixmaps[ HSBSliderMidActive ] ->xForm( rMatrix );
			break;
		case HSBSliderTopPressed:
			ptr->pixmaps[ HSBSliderTopPressed ] = processEmbedded( (optionHandler->style() == Milk) ? "sbslider_top-milk" : "sbslider_top", sbSliderPressedColor, true, &sbGrooveColor );
			*ptr->pixmaps[ HSBSliderTopPressed ] = ptr->pixmaps[ HSBSliderTopPressed ] ->xForm( mMatrix );
			*ptr->pixmaps[ HSBSliderTopPressed ] = ptr->pixmaps[ HSBSliderTopPressed ] ->xForm( rMatrix );
			break;
		case HSBSliderBtmPressed:
			ptr->pixmaps[ HSBSliderBtmPressed ] = processEmbedded( (optionHandler->style() == Milk) ? "sbslider_btm-milk" : "sbslider_btm", sbSliderPressedColor, true, &sbGrooveColor );
			*ptr->pixmaps[ HSBSliderBtmPressed ] = ptr->pixmaps[ HSBSliderBtmPressed ] ->xForm( mMatrix );
			*ptr->pixmaps[ HSBSliderBtmPressed ] = ptr->pixmaps[ HSBSliderBtmPressed ] ->xForm( rMatrix );
			break;
		case HSBSliderMidPressed:
			ptr->pixmaps[ HSBSliderMidPressed ] = processEmbedded( (optionHandler->style() == Milk) ? "sbslider_mid-milk" : "sbslider_mid", sbSliderPressedColor, false, &sbGrooveColor );
			if (optionHandler->style() == Milk)
				*ptr->pixmaps[ HSBSliderMidPressed ] = ptr->pixmaps[ HSBSliderMidPressed ] ->xForm( lMatrix );
			else
				*ptr->pixmaps[ HSBSliderMidPressed ] = ptr->pixmaps[ HSBSliderMidPressed ] ->xForm( rMatrix );
			break;
		case VSBSliderTopBg:
			ptr->pixmaps[ VSBSliderTopBg ] = processEmbedded( (optionHandler->style() == Milk) ? "sbgroove_top-milk" : "sbgroove_top", sbGrooveColor, true, &sbGrooveColor );
			break;
		case VSBSliderBtmBg:
			ptr->pixmaps[ VSBSliderBtmBg ] = processEmbedded( (optionHandler->style() == Milk) ? "sbgroove_btm-milk" : "sbgroove_btm", sbGrooveColor, true, &sbGrooveColor );
			break;
		case VSBSliderMidBg:
			ptr->pixmaps[ VSBSliderMidBg ] = processEmbedded( (optionHandler->style() == Milk) ? "sbgroove_mid-milk" : "sbgroove_mid", sbGrooveColor, false, &sbGrooveColor );
			break;
		case VSBSliderMidBgI:
			ptr->pixmaps[ VSBSliderMidBgI ] = processEmbedded( (optionHandler->style() == Milk) ? "sb_subadd-milk" : "sb_subadd", sbGrooveColor, false, &sbGrooveColor );
			break;
		case HSBSliderTopBg:
			ptr->pixmaps[ HSBSliderTopBg ] = processEmbedded( (optionHandler->style() == Milk) ? "sbgroove_top-milk" : "sbgroove_top", sbGrooveColor, true, &sbGrooveColor );
			*ptr->pixmaps[ HSBSliderTopBg ] = ptr->pixmaps[ HSBSliderTopBg ] ->xForm( rMatrix );
			break;
		case HSBSliderBtmBg:
			ptr->pixmaps[ HSBSliderBtmBg ] = processEmbedded( (optionHandler->style() == Milk) ? "sbgroove_btm-milk" : "sbgroove_btm", sbGrooveColor, true, &sbGrooveColor );
			*ptr->pixmaps[ HSBSliderBtmBg ] = ptr->pixmaps[ HSBSliderBtmBg ] ->xForm( rMatrix );
			break;
		case HSBSliderMidBg:
			ptr->pixmaps[ HSBSliderMidBg ] = processEmbedded( (optionHandler->style() == Milk) ? "sbgroove_mid-milk" : "sbgroove_mid", sbGrooveColor, false, &sbGrooveColor );
			*ptr->pixmaps[ HSBSliderMidBg ] = ptr->pixmaps[ HSBSliderMidBg ] ->xForm( rMatrix );
			break;
		case HSBSliderMidBgI:
			ptr->pixmaps[ HSBSliderMidBgI ] = processEmbedded( (optionHandler->style() == Milk) ? "sb_subadd-milk" : "sb_subadd", sbGrooveColor, false, &sbGrooveColor );
			*ptr->pixmaps[ HSBSliderMidBgI ] = ptr->pixmaps[ HSBSliderMidBgI ] ->xForm( rMatrix );
			break;

		case Progress:
      ptr->pixmaps[ Progress ] = processEmbedded( "progress", btnColor );
      break;
   case progress2:
      ptr->pixmaps[ progress2 ] = processEmbedded( "progress2", btnColor );
      break;
   
   case TitleBar:
   {
      if (optionHandler->style() == Jaguar)
      {
         ptr->pixmaps[ TitleBar ] = new QPixmap(32,22);
         ptr->pixmaps[ TitleBar ]->fill( optionHandler->titleColor(0) );
         QPainter p;
         p.begin( ptr->pixmaps[ TitleBar ] );
         p.setPen( QColor(
               (optionHandler->titleColor(0).red()+optionHandler->titleColor(1).red())/2,
               (optionHandler->titleColor(0).green()+optionHandler->titleColor(1).green())/2,
               (optionHandler->titleColor(0).blue()+optionHandler->titleColor(1).blue())/2 ));
         for ( int i = 1; i < 22; i += 4 )
         {
            p.drawLine( 0, i, 31, i );
            p.drawLine( 0, i+2, 31, i+2 );
         }
         p.setPen( optionHandler->titleColor(1));
         for ( int i = 2; i < 22; i += 4 )
            p.drawLine( 0, i, 31, i );
         p.end();
      }
      else
         ptr->pixmaps[ TitleBar ] = &KPixmapEffect::gradient (*((KPixmap*)(new QPixmap(32,22))), optionHandler->titleColor(0), optionHandler->titleColor(1), KPixmapEffect::VerticalGradient, 22);
      break;
   }
   default:
      break;
   }
   return ( pixmaps[ item ] );
}


#include "baghira.moc"
