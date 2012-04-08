#ifndef __LIQUID_STYLE_H
#define __LIQUID_STYLE_H

#include <qcstring.h>
#include <qdir.h>
#include <qintdict.h>
#include <qprogressbar.h>
#include <qtimer.h>
#include <qwmatrix.h> 
#include <kpixmap.h>
#include <qwidget.h>
#include <kstyle.h>

// #define BITMAP_ITEMS 75 // Don't worry, they're demand loaded ;-)
#define LIQUID_MENU_CHANGE 667
#define MOSFET_BUTTON_CHANGE 661

class QColor;
class QHeader;
class QImage;
class QMenuBar;
class QPalette;
class QPoint;
class QPushButton;
class QRect;
class QScrollBar;
class QSize;
class QString;
class QTabBar;
class LiquidStyle;

enum BitmapData{RadioOn = 0, RadioOff, RadioOnHover, RadioOffHover, RadioOnPressed, RadioOffPressed,
		VSBSliderTop, VSBSliderMid, VSBSliderBtm,
		VSBSliderTopActive, VSBSliderMidActive, VSBSliderBtmActive,
		VSBSliderTopPressed, VSBSliderMidPressed, VSBSliderBtmPressed,
		VSBSliderTopBg, VSBSliderMidBg, VSBSliderMidBgI, VSBSliderBtmBg,
                HSBSliderTop, HSBSliderMid, HSBSliderBtm,
		HSBSliderTopActive, HSBSliderMidActive, HSBSliderBtmActive,
		HSBSliderTopPressed, HSBSliderMidPressed, HSBSliderBtmPressed,
		HSBSliderTopBg, HSBSliderMidBg, HSBSliderMidBgI, HSBSliderBtmBg,
		Tab, TabDown, TabFocus, belowTab, belowTabDown,
		ButtonShadow, CB, CBDown, CBTri, CBHover, CBDownHover, CBTriHover, CBPressed, CBDownPressed, CBTriPressed,
		HSlider, VSlider, VSliderAbove, HSliderInactive, VSliderInactive, VSliderAboveInactive,
		Progress, rectbutton, combo, comboShadow, progress2, TitleBar, BITMAP_ITEMS};

enum CustomColor{CustomCBOn = 0, CustomCBOff, CustomRadioOn, CustomRadioOff,
                 CustomTabOn, CustomTabOff, CustomSBSlider, CustomSBSliderHover, CustomSBSliderPressed, CustomSBGroove};

enum PulseType{PushButton, ComboBox, AppletHandle, Splitter, ToolButton};

enum MenuBackground{Standard=0, Plain, Stipples, Gradient};

enum InactiveButtonColor{Original=0, Background, Active, Custom};

enum myExpanderStyle{Apple=0, MS};

enum progressStyle{mac=0, liquid, baghira};

enum DotlineStyle{Line=0, Dots};

enum Tile{TileTopLeft = 0, TileTop, TileTopRight,
          TileLeft, TileMiddle, TileRight, TileBtmLeft, TileBtm, TileBtmRight};

enum myPosition{left = 0, center, right, full};

enum tabStyle {TrueTab = 0, Clever, Chooser};

enum Style {Jaguar = 0, Panther, Brushed, Tiger, Milk, NUMSTYLES };

class ButtonTile
    {
    public:
    ButtonTile()
        {
        for ( int i = 0;i < 9;++i )
            pixmaps[ i ] = 0;
        }
    ~ButtonTile()
        {
        for ( int i = 0;i < 9;++i )
            if ( pixmaps[ i ] ) delete pixmaps[ i ];
        }
    QPixmap* pixmap( Tile pos )
        {
        return ( pixmaps[ ( int ) pos ] );
        }
    void setPixmap( Tile pos, QPixmap *pix )
        {
        pixmaps[ ( int ) pos ] = pix;
        }
    protected:
            QPixmap *pixmaps[ 9 ];
    };


class OptionHandler : public QObject
{
   Q_OBJECT
public:
   OptionHandler( QObject *parent );
   void reloadSettings();
   inline static bool useShadowText()
   {
      return ( shadowText );
   }
   const QColor& textColor();
   const QColor& bgColor();
   const QColor& buttonColor();
   static int custCols[8];
		
   inline void reset()
   {
      menusProcessed = false;
   }

   inline static bool glossyMenus()
   {
      return glossyMenus_;
   }

   inline static bool drawMenuStripe()
   {
      return (drawMenuStripe_);
   }

   inline static QColor menuStripeColor()
   {
      return menuStripeColor_;
   }

   inline static QColor textColorHigh()
   {
      return fgColorHigh;
   }

   inline static QColor menuColorHigh()
   {
      return colorHigh;
   }

   inline static bool bevelHighlights()
   {
      return (bevelHighlights_);
   }
   inline static bool useBgStipple()
   {
      return ( bgStipple );
   }
   inline static Style style()
   {
      return (style_);
   }
   inline static int stippleContrast()
   {
      return ( contrast );
   }
   inline static bool useReverseBtnColor() 
   {
      return ( reverseBtnColor );
   }
   inline static bool useToolButtonFrame()
   {
      return ( tbFrame );
   }

   inline static bool useCustomColors()
   {
      return ( customWidgetColor );
   }
   inline static const QColor& customColor( int idx )
   {
      return ( customColors[ idx ] );
   }

   inline static bool DrawDotlines()
   {
      return ( drawDotlines );
   }
   inline static bool squeezeSbSlider()
   {
      return ( squeezesbslider );
   }
   inline static bool shadowSbSlider()
   {
      return (shadowsbslider);
   }
   inline static bool drawGroupBoxShadow()
   {
      return ( groupboxshadow );
   }
   inline static int ShadowDarkness()
   {
      return ( shadowDarkness );
   }
   inline static int InactiveButtonColorType()
   {
      return ( inactiveButtonColor );
   }
   inline static int MenuBackground()
   {
      return (menuBackground);
   }
   inline static int MenuOpacity()
   {
      return (menuOpacity);
   }
   inline static bool UseCustomMenuColor()
   {
      return (useCustomMenuColor);
   }
   inline static int DotlineStyle()
   {
      return (dotlineStyle);
   }
   inline static bool MenuColorButton()
   {
      return (menuColorButton);
   }
   inline static int ExpanderStyle()
   {
      return (expanderStyle);
   }
   inline static bool UseCustomExpanderColor()
   {
      return (useCustomExpanderColor);
   }
   inline static QColor CustomExpanderColor()
   {
      return (expanderColor);
   }
   inline static QColor DotlineColor()
   {
      return (dotlineColor);
   }
   inline QColor InactiveButtonColor()
   {
      switch (inactiveButtonColor)
      {
      case 0 : return (QColor(232,232,232));
      case 1 : return (bgColor());
      case 2 : return (buttonColor());
      case 3 : return (customInactiveButtonColor);
      }
   }
   inline static bool CenterTabs()
   {
      return centerTabs;
   }

   inline static bool SmoothListViewHeaders()
   {
      return smoothListViewHeaders;
   }

   inline static bool SmootherLVH()
   {
      return smootherLVH;
   }
		
   inline static bool IcyButtons()
   {
      return icyButtons;
   }

   inline static Style buttonStyle()
   {
      return _buttonStyle;
   }

   inline static int ProgressBar()
   {
      return progressBar;
   }

   inline static bool RemoveKickerBevel()
   {
      return removeKickerBevel;
   }

   inline static Style toolbuttonStyle()
   {
      return _toolbuttonStyle;
   }

   inline void setGroupShadowButton()
   {
      groupShadowButton = true;
   }
   inline void setNotGroupShadowButton()
   {
      groupShadowButton = false;
   }
   inline bool GroupShadowButton()
   {
      return groupShadowButton;
   }

   inline void setScrollerActive()
   {
      scrollerActive = true;
   }

   inline void setScrollerInactive()
   {
      scrollerActive = false;
   }

   inline bool ScrollerActive()
   {
      return scrollerActive;
   }

   inline static QColor Color()
   {
      return color;
   }

   inline static QColor Color2()
   {
      return color2;
   }
		
   inline static bool DrawProgressLabel()
   {
      return drawProgressLabel;
   }
		
   inline static tabStyle TabStyle()
   {
      return tabStyle_;
   }
		
   inline static QColor & BrushTint()
   {
      return brushTint;
   }
		
   inline static bool TintBrush()
   {
      return tintBrush;
   }
		
   inline static bool AnimateSlider()
   {
      return animateSlider;
   }
   inline static bool AnimateProgress()
   {
      return animateProgress;
   }
   inline static QColor CustomButtonColor()
   {
      return customButtonColor;
   }
   
   inline static bool AnimateButton()
   {
      return animateButton;
   }
   
   inline static QColor aDecoColor1()
   {
      return aDecoColor1_;
   }
   inline static QColor aDecoColor2()
   {
      return aDecoColor2_;
   }
   inline static QColor titleColor( int i )
   {
      return titleColor_[i];
   }
   inline static QColor titleButtonColor( int i )
   {
      return titleButtonColor_[i];
   }
		
   QMenuBar *currentMenu;
   QPixmap *indicatedMenuItemPix;
   QRect *indicatedMenuItemRect;
   QPixmap *lastIndicatedMenuItemPix;
   QRect *lastIndicatedMenuItemRect;
   bool mouseOverMenu;
   WId lastIndicatedMenuItemID;
   static unsigned long wmDesign;

		
public slots:
   inline void warningSlot()
   {
      qWarning("widget destroyed");
   }
		
protected:
   static QColor titleColor_[2];
   static QColor titleButtonColor_[3];

   // menu
   static int menuBackground;
   static int menuOpacity;
   static bool menuColorButton;
   static bool animateButton;
   static bool shadowText;
   static bool drawMenuStripe_;
   static QColor menuStripeColor_;
   static bool glossyMenus_;
   //color
   static QColor customButtonColor;
   //widgets
   static bool customWidgetColor;
   static QColor customColors[ 10 ];
   //menu
   static bool useCustomMenuColor;
   static QColor color, color2, colorHigh, fgColor, fgColorHigh;
   //incativeButton
   static int inactiveButtonColor;
   static QColor customInactiveButtonColor;

   //Design
   //stipples
   static bool bgStipple, bevelHighlights_;
   static Style style_;
   static int contrast;
   //shadows
   static bool groupboxshadow;
   static int shadowDarkness;
   //istviews;
   static bool drawDotlines, useCustomExpanderColor;
   static int expanderStyle;
   static QColor expanderColor, dotlineColor;
   static int dotlineStyle;
   //slider
   static bool squeezesbslider;
   static bool shadowsbslider;
   //toolbar
   static bool reverseBtnColor, tbFrame;
   //tweaks
   static bool centerTabs;
   static bool smoothListViewHeaders;
   static bool smootherLVH;
   static bool icyButtons;
   static Style _buttonStyle;
   bool menusProcessed;
   bool scrollerActive;
   int menuLevelCounter;
   bool groupShadowButton;
   static int progressBar;
   static bool removeKickerBevel;
   static Style _toolbuttonStyle;
   static bool drawProgressLabel;
   static tabStyle tabStyle_;
   
   QString colorStr, fgColorStr/*, panelColorStr*/;
   static QColor brushTint;
   static QColor aDecoColor1_;
   static QColor aDecoColor2_;
   static bool tintBrush;
   static bool animateSlider;
   static bool animateProgress;
};
	
class EventKiller : public QObject 
{
   Q_OBJECT
public:
   bool eventFilter( QObject *, QEvent *)
   {
      return TRUE;
   }
};
    
class BMShower : public QObject 
{
   Q_OBJECT
public:
   BMShower(QObject *parent);
   bool eventFilter( QObject *object, QEvent *event );
};
    
class ButtonFadeInfo 
{
public:
   ButtonFadeInfo()
   {
   timerId=0; index=0; fadeIn=TRUE;
   }
public:
   int timerId;
   int index;
   bool fadeIn;
};

class LiquidStyle : public KStyle
{
   Q_OBJECT
public:
   friend class OptionHandler;
   LiquidStyle();
   virtual ~LiquidStyle();
   bool isPlain() const;
   
   virtual void polish( QWidget *w );
//                 virtual void polishPopupMenu( QPopupMenu *w );
   virtual void unPolish( QWidget *w );
   virtual void polish( QPalette &p );
   virtual void unPolish( QApplication *a );
   int styleHint(StyleHint sh, const QWidget * w, const QStyleOption & opt, QStyleHintReturn * shr) const;
   void drawKStylePrimitive( KStylePrimitive kpe, QPainter* p, const QWidget* widget, const QRect &r, const QColorGroup &cg, SFlags flags = Style_Default, const QStyleOption& = QStyleOption::Default ) const;
   void drawPrimitive( PrimitiveElement pe, QPainter* p, const QRect &r, const QColorGroup &cg, SFlags flags = Style_Default, const QStyleOption& = QStyleOption::Default ) const;
   void drawControl( ControlElement element, QPainter *p, const QWidget *widget, const QRect &r, const QColorGroup &cg, SFlags flags = Style_Default, const QStyleOption& = QStyleOption::Default ) const;
//		void drawControlMask( ControlElement element, QPainter *p, const QWidget *widget, const QRect &r, const QStyleOption& = QStyleOption::Default ) const;
   void drawComplexControl( ComplexControl control, QPainter *p, const QWidget *widget, const QRect &r, const QColorGroup &cg, SFlags flags = Style_Default, SCFlags controls = SC_All, SCFlags active = SC_None, const QStyleOption& = QStyleOption::Default ) const;
//		void drawComplexControlMask( ComplexControl control, QPainter *p, const QWidget *widget, const QRect &r, const QStyleOption& = QStyleOption::Default ) const;

   int pixelMetric( PixelMetric m, const QWidget *widget = 0 ) const;

   QSize sizeFromContents( ContentsType contents, const QWidget *widget, const QSize &contentSize, const QStyleOption& opt ) const;
   QRect subRect( SubRect r, const QWidget *widget ) const;
   QRect querySubControlMetrics( ComplexControl control, const QWidget *widget, SubControl subcontrol, const QStyleOption &opt = QStyleOption::Default ) const;
   // Fix Qt3's wacky image positions
   QPixmap stylePixmap( StylePixmap stylepixmap, const QWidget *widget = 0, const QStyleOption& = QStyleOption::Default ) const;
   bool eventFilter( QObject *object, QEvent *event );
		
private slots:
   void updateProgressPos();
   void updateSliderPos();
   void progressBarDestroyed(QObject*);
   void fakeMouse();
	
protected:
   void clearImage( QImage &img ) const;
   ButtonTile* createButtonTile( const QColor &c, const QColor &bgColor, QImage *buttonImage, QImage *shadowImage, QImage *glowImage, QIntDict <ButtonTile>*buttonDict, QIntDict<ButtonTile>*shadowDict, QIntDict<ButtonTile>*glowDict, int w, int h, int xOff, int yOff, int centerW, int centerH, int shadowH, int glowWH, bool sunken) const;
   ButtonTile* createRoundFrameTile(QImage &img, const QColor &color, const QColor *bg, const QColor &baseColor ) const;
   QPixmap* createSliderEnd( const QColor &c, const QColor &bgColor, bool top/*, bool vertical*/ ) const;
   ButtonTile* separateTiles( QPixmap *pix, int xO, int yO, int w, int h, int sh, bool sunken) const;
   
   QPixmap* getPixmap( BitmapData item ) const;
   QPixmap* adjustHSV( QImage &img, const QColor &c, const QColor *bg = NULL ) const;
   QImage* adjustHSVImage( QImage &img, const QColor &c, const QColor *bg = NULL ) const;
   void adjustHSV( QPixmap &pix, const QColor &c ) const;
   QImage* tintBrush( const QImage &img, const QColor &c ) const;
   QPixmap* processEmbedded( const char *label, const QColor &c, bool blend = false, const QColor *bg = NULL ) const;
   void drawRoundButton( QPainter *p, const QColorGroup &cg, const QColor &c, /*const QColor &bg,*/ int x, int y, int w, int h, bool pushedDown = false, bool hover = false, int bgX = -1, int bgY = -1) const;
   void drawRectangularButton( QPainter *p, const QColorGroup &cg, const QColor &c, int x, int y, int w, int h, bool sunken = false, bool hover = false, bool isCombo = false, int position = full, bool isHtml = false) const;
   void drawCombo( QPainter *painter, const QColorGroup &cg, const QColor &c, int x, int y, int w, int h, bool sunken, bool hover, bool isCombo, int position = full, bool isHTML = false ) const;
   void drawEditFrame( QPainter *p, const QRect &r, const QColorGroup &cg, bool isHTML = false, bool focused = false, bool inverse = false/*, bool round = false*/ ) const;
   void drawRoundFrame( QPainter *p, const QRect &r, const QColorGroup &cg, bool focused, QPoint offset ) const;
   void drawHTMLCBBorder( const QPixmap &pix, const QColor &c ) const;
   bool isHTMLWidget( const QWidget *w ) const;


private:
   LiquidStyle( const LiquidStyle & );
   LiquidStyle& operator=( const LiquidStyle & );
   int getBrightness(unsigned int rgb) const;
   QImage fetchImage( const char *name );
   QColor mapFadeColor(QColor &color, int index) const;
   void fadeIn(QPushButton *button);
   void fadeOut(QPushButton *button);
   QColor brushedMetalColor(QColor &c) const;
   QColor originalBgColor;
   
   
   QBitmap *sbLeft, *sbRight, *sbUp, *sbDown, *checkmark;
   QImage *btnBorderImg, *btnShadowImg, *slider_top, *slider_btm, *slider_top_shd, *slider_btm_shd, *rectbutton, *combo, *comboShadow, *buttonGlow, *roundFrame;
   QPixmap *btnBlendPix, *menuPix, *groupShadow ;
   QPixmap plainBrush, gradBrush;
   QBrush pagerBrush, pagerHoverBrush;

   QPixmap *pixmaps[ BITMAP_ITEMS ];
   QPixmap sbBuffer;
   QScrollBar *currentScrollBar;
   QWMatrix lMatrix;
   QWMatrix rMatrix;
   QWMatrix iMatrix;
   QWMatrix mMatrix;
   QWMatrix m2Matrix;

   bool isKicker, isOOO, isHTMLButton, initialPaletteLoaded, inExitPolish;

   QHeader *currentHeader;
   QTabBar *currentTabBar;
   int headerHoverID;
   int headerSortID;
   QPoint btnOffset;
   QPoint cursorPos_;
   QWidget *currentTaskContainer;
   KPixmap activeToolbarPix;
   bool isTaskContainer, taskContainerHover;

   QPalette polishedPalette, tooltipPalette;
   unsigned int qtrcModificationTime;
   
   int progAnimShift;
   int prog2AnimShift;
   int sliderAnimShift;
   int hoveredTabIndex;
   bool mouseButtonPressed_;

   OptionHandler *optionHandler;

   QIntDict<ButtonTile>btnDict;
   QIntDict<ButtonFadeInfo>bfi;
   QIntDict<QRgb*>fadeColorMap;
   QIntDict<ButtonTile>btnShadowedDict;
   QIntDict<ButtonTile>buttonGlowDict;
   QIntDict<ButtonTile>comboDict;
   QIntDict<ButtonTile>comboShadowedDict;
   QIntDict<ButtonTile>tabDict;
   QIntDict<ButtonTile>inverseTabDict;
   QIntDict<ButtonTile>rectBtnDict;
   QIntDict<ButtonTile>progress2Dict;
   QMap<QWidget*, int> progAnimWidgets;

   QPixmap *tmpBtnPix;
   ButtonTile *inactiveRoundFrame;
   ButtonTile *activeRoundFrame;

   QWidget *highlightWidget;
   QScrollBar *activeScroller;
   EventKiller *eventKiller;
   BMShower *bmShower;
   static QPixmap popupBack;
   QWidget *paintWidget;
};
	
#endif
