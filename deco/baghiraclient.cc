//////////////////////////////////////////////////////////////////////////////
// baghiraclient.h
// -------------------
// Baghira window decoration for KDE
// -------------------
// Copyright (c) 2004, 2005 Thomas Lübking
// Please see the header file for copyright and license information.
//////////////////////////////////////////////////////////////////////////////

// #include <kconfig.h>
#include <kglobal.h>
#include <kglobalsettings.h>
#include <kimageeffect.h>
#include <klocale.h>
#include <kpixmapeffect.h>
#include <kwin.h>
#include <kwinmodule.h>
#include <qapplication.h>
#include <qbitmap.h>
#include <qcursor.h>
#include <qdir.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpainter.h>
#include <qtimer.h>
#include <qtooltip.h>
#include <qsettings.h>
#include <stdio.h>

#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/shape.h>
#include "baghiraclient.h" 
#include "pixmaps.h"
#include "masks.h"
#include "config.h"

#define COLOR_SPACE(R,G,B) \
    if ( R < 0 ) R = 0; else if ( R > 255 ) R = 255; \
    if ( G < 0 ) G = 0; else if ( G > 255 ) G = 255; \
    if ( B < 0 ) B = 0; else if ( B > 255 ) B = 255;

#define SATURATION_COLOR(R,G,B) \
    grey = (299 * R + 587 * G + 114 * B) / 1000; \
    delta = 255 - grey; \
    grey = (grey *(10 - 5)) / 10; \
    iGrey = 255 - grey;\
    destR = (iGrey * (srcR - delta) + grey * R) / 255; \
    destG = (iGrey * (srcG - delta) + grey * G) / 255; \
    destB = (iGrey * (srcB - delta) + grey * B) / 255;
    
#define CLAMP(x,l,u) x < l ? l :\
		     x > u ? u :\
		     x

#define SATURATION_COLOR2(S,R,G,B,cR,cG,cB) \
   int max = 255+0.65*(100-S); \
   destR = CLAMP((cR + R - 128), 0, max); \
   destG = CLAMP((cG + G - 128), 0, max); \
   destB = CLAMP((cB + B - 128), 0, max); \
   destR = (S*destR + (100-S)*R)/100; \
   destG = (S*destG + (100-S)*G)/100; \
   destB = (S*destB + (100-S)*B)/100;

using namespace Baghira;

static const int BUTTONSIZE = 15;
static const int MENUBUTTONWIDTH = 20;
static const int DECOSIZE = 8;
int TITLESIZE;// = 26;
static const int TITLESIZESMALL = 18;
static const int MARGIN = 6;

//////////////////////////////////////////////////////////////////////////////
// BaghiraFactory Class                                                      //
//////////////////////////////////////////////////////////////////////////////

bool BaghiraFactory::initialized_ = false;
Baghira::TitlebarType BaghiraFactory::effect_[5][2];
Qt::AlignmentFlags BaghiraFactory::titlealign_;
QColor BaghiraFactory::colors_[5][ ColorTypeCount ][ 2 ];
KPixmap BaghiraFactory::pix_[5][ PixmapTypeCount ][ 2 ][ 2 ];
int BaghiraFactory::bordersize_[5] = {0,0,6,0,0};//{BorderTiny, BorderTiny, BorderVeryHuge};
QPixmap BaghiraFactory::ButtonPixmap_[5][WindowStateAmount][ButtonTypeAmount][ButtonStateAmount][2];
QColor BaghiraFactory::brushedMetalColor;
QColor BaghiraFactory::ButtonColor_[ButtonTypeAmount+1];
QPixmap BaghiraFactory::nostalgia_[5][3][2];
int BaghiraFactory::contrast_;
int BaghiraFactory::_3DImpact_[5];
int BaghiraFactory::LineImpact_[5];
bool BaghiraFactory::bgStipple_;
bool BaghiraFactory::comicFrame_;
bool BaghiraFactory::addAutoSpacing_;
bool BaghiraFactory::maxResizable_;
bool BaghiraFactory::shape_ul[5];
bool BaghiraFactory::shape_ur[5];
bool BaghiraFactory::shape_ll[5];
bool BaghiraFactory::shape_lr[5];
bool BaghiraFactory::allowEasyClosing_;
bool BaghiraFactory::resizeGrip_;
uint BaghiraFactory::defaultMode_;
bool BaghiraFactory::drawIcon_[5];
bool BaghiraFactory::tintBrush_;
bool BaghiraFactory::showTitle_;
QColor BaghiraFactory::brushTint;
int BaghiraFactory::minimumTitleHeight_;
ButtonStyle BaghiraFactory::buttonStyle_[5];

QBitmap BaghiraFactory::helpMask;
QBitmap BaghiraFactory::jaguarDownMask;
QBitmap BaghiraFactory::jaguarMenuDownMask;
QBitmap BaghiraFactory::jaguarMenuMask;
QBitmap BaghiraFactory::jaguarMask;
QBitmap BaghiraFactory::milkMenuDownMask;
QBitmap BaghiraFactory::milkMenuMask;
QBitmap BaghiraFactory::milkMask;
QBitmap BaghiraFactory::pantherMenuMask;
QBitmap BaghiraFactory::pantherMask;
QBitmap BaghiraFactory::milkDownMask;
QBitmap BaghiraFactory::arrowUp;
QBitmap BaghiraFactory::arrowDown;
DeMaximizer BaghiraFactory::deMaximizer_;
bool BaghiraFactory::fullSpec_;
bool BaghiraFactory::noModalDeco_;
bool BaghiraFactory::delAppname_;

Atom baghira_deco_design = XInternAtom(qt_xdisplay(), "BAGHIRA_DECO_DESIGN", False);

extern "C" KDecorationFactory* create_factory() {
	return new Baghira::BaghiraFactory();
}

DeMaximizer::DeMaximizer() : QWidget(0, "deMaximizer", Qt::WType_TopLevel | Qt::WStyle_NoBorder | Qt::WX11BypassWM )
{
    client_ = 0L;
    setFixedSize(16,16);
    setMouseTracking(TRUE);
    setAutoMask(TRUE);
    move(QApplication::desktop()->availableGeometry().right()-17,QApplication::desktop()->availableGeometry().top()+2);
    setBackgroundMode( NoBackground );
//     setBackgroundMode (Qt::X11ParentRelative);
//     setPaletteBackgroundColor(Qt::green);
}

void DeMaximizer::mouseReleaseEvent( QMouseEvent *)
{
    hasMouse()?setState(1):setState(0);
    if (client_) client_->demaxButtonPressed();
}

void DeMaximizer::mousePressEvent ( QMouseEvent *)
{
    setState(2);
}

void DeMaximizer::enterEvent ( QEvent *)
{
    setState(1);
}

void DeMaximizer::leaveEvent ( QEvent *)
{
    setState(0);
}

void DeMaximizer::setState(int i)
{
    erase();
    QPainter p(this);
    p.drawPixmap(0,0,pixmap[i]);
    p.end();
}

void DeMaximizer::setPixmap(int i, QImage &image )
{
    pixmap[i] = QPixmap(image);
}


void DeMaximizer::setClient(BaghiraClient *client)
{
   if (client_)
   {
      QObject::disconnect ( client_, SIGNAL(hide()), this, SLOT(hide()) );
      client_ = 0L;
   }
   if (client)
   {
      client_ = client;
      connect ( client_, SIGNAL(hide()), SLOT(hide()) );
      setState(0);
   }
//     repaint();
}

#if KDE_IS_VERSION(3,3,91)
//////////////////////////////////////////////////////////////////////////////
// ResizeHandle()
// ---------------
// Constructor
ResizeHandle::ResizeHandle(BaghiraClient * parent) : QWidget(parent->widget(), 0, 0)
{
    if (!parent->widget())
        return;
    client = parent;
    setCursor(QCursor(Qt::SizeFDiagCursor));
    setFixedSize(16,16);
    updateLook(FALSE);
    WId root, daddy = 0;
    WId *kids = 0L;
    uint numKids = 0;
    XQueryTree(qt_xdisplay(), parent->windowId(), &root, &daddy, &kids, &numKids);
    if (daddy)
        XReparentWindow( qt_xdisplay(), winId(), daddy, 0, 0 );
    else return;
    move(client->width()-2*BaghiraFactory::borderSize(parent->currentStyle)-16,client->height()-client->titleheight_-16);
    shape();
    parent->widget()->installEventFilter(this);
    raise();
    show();
}

void ResizeHandle::updateLook(bool rp)
{
    pix = QPixmap(16,16);
    QPainter p(&pix);
   p.fillRect(pix.rect(), BaghiraFactory::effect(client->currentStyle, client->isActive()) == Baghira::Brushed ? BaghiraFactory::BrushedMetalColor().light(110):paletteBackgroundColor());
   p.setPen( BaghiraFactory::effect(client->currentStyle, client->isActive()) == Baghira::Brushed ? Qt::black:paletteForegroundColor() );
    p.drawLine(0,16,16,0);
    p.drawLine(4,16,16,4);
    p.drawLine(8,16,16,8);
    p.drawLine(12,16,16,12);
    p.end();
    setPaletteBackgroundPixmap(pix);
    if (rp) repaint();
}

void ResizeHandle::shape()
{
    XRectangle* xrects = new XRectangle[16];
    for (int i = 0; i < 16; i++)
    {
        xrects[ i ].x = 15-i;
        xrects[ i ].y = i;
        xrects[ i ].width = i+1;
        xrects[ i ].height = 1;
    }
    XShapeCombineRectangles( qt_xdisplay(), winId(), ShapeBounding, 0, 0,
                                xrects, 16, ShapeSet, 0 );
    delete[] xrects;
}

bool ResizeHandle::eventFilter(QObject *obj, QEvent *e)
{
    if ( obj != parent() )
        return false;
    if ( e->type() == QEvent::Resize)
    {
       if (client->maximizeMode() == BaghiraClient::MaximizeFull)
         move(client->width() - 16, client->height() - client->titleheight_ - 16);
       else
          move(client->width() - 2*BaghiraFactory::borderSize(client->currentStyle) - 16, client->height() - client->titleheight_ - BaghiraFactory::borderSize(client->currentStyle) - 16);
//         shape();
        return FALSE;
    }
   return FALSE;
}

void ResizeHandle::mousePressEvent ( QMouseEvent * )
{
    client->performWindowOperation(/*Unrestricted*/KDecoration::ResizeOp);
}
void ResizeHandle::mouseReleaseEvent ( QMouseEvent * )
{
    client->performWindowOperation(KDecoration::NoOp);
    repaint(TRUE);
}
void ResizeHandle::paintEvent ( QPaintEvent * )
{
    QPainter p(this);
    p.drawPixmap(0,0,pix);
}

#endif

//////////////////////////////////////////////////////////////////////////////
// BaghiraFactory()
// ---------------
// Constructor

BaghiraFactory::BaghiraFactory() {
    helpMask = QBitmap( 14, 15, help_bits, true );
    jaguarDownMask = QBitmap( 14, 15, jaguar2_bits, true );
    jaguarMenuDownMask = QBitmap( 20, 15, jaguar_menu_down_bits, true );
    jaguarMenuMask = QBitmap( 20, 15, jaguar_menu_bits, true );
    jaguarMask = QBitmap( 14, 15, jaguar_bits, true );
    milkMenuDownMask = QBitmap( 20, 15, milk_menu_down_bits, true );
    milkMenuMask = QBitmap( 20, 15, milk_menu_bits, true );
    milkMask = QBitmap( 14, 15, milk_bits, true );
    pantherMenuMask = QBitmap( 20, 15, panther_menu_bits, true );
    pantherMask = QBitmap( 14, 15, panther_bits, true );
    milkDownMask = QBitmap( 14, 15, milk_pressed_bits, true );
   arrowUp = QBitmap( 8, 4, arrowUp_bits, true );
   arrowUp.setMask( arrowUp );
   arrowDown = QBitmap( 8, 4, arrowDown_bits, true );
   arrowDown.setMask( arrowDown );
    readConfig();
    createPixmaps();
    initialized_ = true;
}

//////////////////////////////////////////////////////////////////////////////
// ~BaghiraFactory()
// ----------------
// Destructor

BaghiraFactory::~BaghiraFactory()
{
   deMaximizer_.setClient(NULL);
   initialized_ = false;
}

//////////////////////////////////////////////////////////////////////////////
// createDecoration()
// ------------------
// Create the decoration

KDecoration* BaghiraFactory::createDecoration( KDecorationBridge* b ) {
	return new BaghiraClient( b, this );
}

//////////////////////////////////////////////////////////////////////////////
// reset()
// -------
// Reset the handler. Returns true if decorations need to be remade

QString BaghiraFactory::indexedString(QString s, int i){
   QString result;
   result.setNum(i+1);
   result.prepend("_");
   result.prepend(s);
//   qWarning("%s",result.ascii());
   return result;
}

bool BaghiraFactory::reset( unsigned long changed ) {
	initialized_ = false;
	changed |= readConfig();
	if ( changed & ( SettingColors | SettingDecoration | SettingBorder ) ) {
		createPixmaps();
	}
	initialized_ = true;

	if ( changed & ( SettingColors | SettingDecoration | SettingFont |
	                 SettingButtons | SettingBorder ) ) {
		return true;
	} else {
		resetDecorations( changed );
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////////
// readConfig()
// ------------
// Read in the configuration file

unsigned long BaghiraFactory::readConfig() {
	
        QSettings config;
        config.beginGroup("/baghira/Style");
//         KConfig config( "baghirarc" );
//         config.setGroup( "Style" );
	bgStipple_ = config.readBoolEntry( "Design_StippleBackground", false);
	contrast_ = config.readNumEntry( "Design_StippleContrast", 30 );
	tintBrush_ = config.readBoolEntry( "Colors_TintBrushedMetal", false );
	if (tintBrush_)
		brushTint.setRgb( config.readNumEntry( "Colors_BrushTint"));
   
   QColor oldcolor;
   unsigned long changed = 0;
   oldcolor = colors_[3][ Baghira::ColorTitleBar ][ false ];
   colors_[3][ Baghira::ColorTitleBar ][ false ] = qApp->palette().active().background();
   if ( oldcolor != colors_[3][ Baghira::ColorTitleBar ][ false ] )
      changed |= SettingColors;
   
   oldcolor = colors_[3][ Baghira::ColorTitleBlend ][ false ];
   colors_[3][ Baghira::ColorTitleBlend ][ false ] = colors_[3][ Baghira::ColorTitleBar ][ false ].dark(100 + config.readNumEntry( "Design_StippleContrast", 3));
   if ( oldcolor != colors_[3][ Baghira::ColorTitleBlend ][ false ] )
      changed |= SettingColors;
	
   config.endGroup();
   config.beginGroup( "/baghira/Deco" );


        // common settings
        
   bool oldNoModalDeco = noModalDeco_;
   noModalDeco_ = config.readBoolEntry( "NoModalDeco", false );
   if (oldNoModalDeco != noModalDeco_)
      changed |= SettingDecoration;
   
   bool oldDelAppname = delAppname_;
   delAppname_ = config.readBoolEntry( "RemoveAppname", true );
   if (oldDelAppname != delAppname_)
      changed |= SettingDecoration;
   
   int oldMinTH = minimumTitleHeight_;
	minimumTitleHeight_ = (ButtonStyle) config.readNumEntry( "minimumTitleHeight", 18 );
	if ( oldMinTH != minimumTitleHeight_ )
	{
		QFontMetrics fm(options()->font(true, false));
		TITLESIZE = QMAX(minimumTitleHeight_, fm.height());
		changed |= SettingFont;
		changed |= SettingDecoration;
	}
        
	bool allowEasyClosing__ = allowEasyClosing_;
	allowEasyClosing_ = config.readBoolEntry( "allowEasyClosing", false );
	if ( allowEasyClosing__ != allowEasyClosing_ )
		changed |= SettingDecoration;
                
	bool resizeGrip__ = resizeGrip_;
	resizeGrip_ = config.readBoolEntry( "ResizeGrip", false );
	if ( resizeGrip__ != resizeGrip_ )
		changed |= SettingDecoration;
                
	bool defaultMode__ = defaultMode_;
	defaultMode_ = config.readNumEntry( "defaultMode", 1 );
	if ( defaultMode__ != defaultMode_ )
		changed |= SettingDecoration;
                
        Qt::AlignmentFlags oldalign = titlealign_;
	QString value = config.readEntry( "TitleAlignment", "AlignHCenter" );
        showTitle_ = TRUE;
        if ( value == "AlignLeft" )
		titlealign_ = Qt::AlignLeft;
	else if ( value == "AlignHCenter" )
		titlealign_ = Qt::AlignHCenter;
	else if ( value == "AlignRight" )
		titlealign_ = Qt::AlignRight;
        else if ( value == "noTitle" )
        {
                showTitle_ = FALSE;
                titlealign_ = Qt::AlignAuto;
        }
	if ( oldalign != titlealign_ )
		changed |= SettingFont;
	
        bool oldComic = comicFrame_;
	comicFrame_ = config.readBoolEntry( "DrawComicFrame", false );
	if ( oldComic != comicFrame_ )
		changed |= SettingBorder; changed |= SettingDecoration;

       	bool oldAutoSpacing = addAutoSpacing_;
	addAutoSpacing_ = config.readBoolEntry( "AddAutoSpacing", true );
	if ( oldAutoSpacing != addAutoSpacing_ )
		changed |= SettingDecoration;

       	bool oldMaxResizable = maxResizable_;
	maxResizable_ = config.readBoolEntry( "MaxResizable", true );
	if ( oldMaxResizable != maxResizable_ )
		changed |= SettingDecoration;
                
        bool oldSpec = fullSpec_;
	fullSpec_ = config.readBoolEntry( "FullSpec", false );
	if ( oldSpec != fullSpec_ )
		changed |= SettingDecoration;
                        
        // custom settings
	
   for (int i = 0; i < 5; i++){
      
      int oldbordersize = bordersize_[i];
      bordersize_[i] = config.readNumEntry( indexedString("BorderSize",i), i == 2 ? 6 : 0 );
      if ( oldbordersize != bordersize_[i] )
            changed |= SettingBorder;
        
	ButtonStyle oldButtons = buttonStyle_[i];
	buttonStyle_[i] = (ButtonStyle) config.readNumEntry( indexedString("ButtonStyle",i), i == 0 ? 1 : 0 );
	if ( oldButtons != buttonStyle_[i] )
		changed |= SettingDecoration;
		

	bool oldShUL = shape_ul[i];
	shape_ul[i] = config.readBoolEntry( indexedString("ShapeUL",i), true );
	if ( oldShUL != shape_ul[i] ){
		changed |= SettingBorder; changed |= SettingDecoration;
		}

	bool oldShUR = shape_ur[i];
	shape_ur[i] = config.readBoolEntry( indexedString("ShapeUR",i), true );
	if ( oldShUR != shape_ur[i] ){
		changed |= SettingBorder; changed |= SettingDecoration;
		}
		
	bool oldShLL = shape_ll[i];
	shape_ll[i] = config.readBoolEntry( indexedString("ShapeLL",i), i == 2 );
	if ( oldShLL != shape_ll[i] ){
		changed |= SettingBorder; changed |= SettingDecoration;
		}
		
	bool oldShLR = shape_lr[i];
	shape_lr[i] = config.readBoolEntry( indexedString("ShapeLR",i), i == 2 );
	if ( oldShLR != shape_lr[i] ){
		changed |= SettingBorder; changed |= SettingDecoration;
		}

		
	bool oldDrawIcon_ = drawIcon_[i];
	drawIcon_[i] = config.readBoolEntry( indexedString("drawIcon",i), true );
	if ( oldDrawIcon_ = drawIcon_[i] )
		changed |= SettingFont;
		changed |= SettingDecoration;
		
   if (i != 3)
   {
      Baghira::TitlebarType oldeffect = effect_[i][1];
      effect_[i][1] = ( Baghira::TitlebarType ) config.readNumEntry( indexedString("TitleEffect",i), i == 0 ? Baghira::Stipples : (i == 1 || i == 4) ? Baghira::Gradient : Baghira::Brushed);
   //        qWarning("%d",effect_[i]);
      if ( oldeffect != effect_[i][1] )
         changed |= SettingDecoration;
   
      oldeffect = effect_[i][0];
      effect_[i][0] = ( Baghira::TitlebarType ) config.readNumEntry( indexedString("inactiveTitleEffect",i), i == 0 ? Baghira::Stipples : (i == 1 || i == 4) ? Baghira::Gradient : Baghira::Brushed);
   //        qWarning("%d",effect_[i]);
      if ( oldeffect != effect_[i][0] )
         changed |= SettingDecoration;
         
      int oldLineImpact = LineImpact_[i];
      LineImpact_[i] = config.readNumEntry( indexedString("LineImpact",i), i == 2 ? 0 : 40 );
      if ( oldLineImpact != LineImpact_[i] )
         changed |= SettingDecoration;
      
      oldcolor = colors_[i][ Baghira::ColorTitleBar ][ false ];
      colors_[i][ Baghira::ColorTitleBar ][ false ].setRgb((unsigned int)config.readNumEntry( indexedString("inactiveColor1",i), i == 0 ? QColor(204,214,230).rgb() : i == 1 ? QColor(246,242,246).rgb() : i == 3 ? QColor(200,200,200).rgb() : QColor(230,230,230).rgb()));
      if ( oldcolor != colors_[i][ Baghira::ColorTitleBar ][ false ] )
         changed |= SettingColors;
      
      oldcolor = colors_[i][ Baghira::ColorTitleBlend ][ false ];
      colors_[i][ Baghira::ColorTitleBlend ][ false ].setRgb((unsigned int)config.readNumEntry( indexedString("inactiveColor2",i), i == 0 ? QColor(194,196,211).rgb() : i == 1 ? QColor(238,238,238).rgb() : i == 3 ? QColor(150,150,150).rgb() : QColor(250,250,250).rgb()));
      if ( oldcolor != colors_[i][ Baghira::ColorTitleBlend ][ false ] )
         changed |= SettingColors;
   }
   else
   {
      effect_[3][1] = Baghira::Gradient;
      effect_[3][0] = Baghira::FlatStipples;
      LineImpact_[3] = 0;
   }
		
	int old3DImpact = _3DImpact_[i];
	_3DImpact_[i] = config.readNumEntry( indexedString("3DImpact",i), 20 );
	if ( old3DImpact != _3DImpact_[i] )
		changed |= SettingDecoration;

   oldcolor = colors_[i][ Baghira::ColorTitleBar ][ true ];
      colors_[i][ Baghira::ColorTitleBar ][ true ].setRgb((unsigned int)config.readNumEntry( indexedString("activeColor1",i), i == 0 ? QColor(255,255,255).rgb() : i == 1 ? QColor(238,238,238).rgb() : i == 2 ? QColor(202,202,202).rgb() : i == 3 ? QColor(238,238,238).rgb() : QColor(250,250,250).rgb()));
	if ( oldcolor != colors_[i][ Baghira::ColorTitleBar ][ true ] )
		changed |= SettingColors;
                
   oldcolor = colors_[i][ Baghira::ColorTitleBlend ][ true ];
      colors_[i][ Baghira::ColorTitleBlend ][ true ].setRgb((unsigned int)config.readNumEntry( indexedString("activeColor2",i), i == 0 ? QColor(238,234,238).rgb() : i == 1 ? QColor(205,202,205).rgb() : i == 2 ? QColor(150,150,150).rgb() : i == 3 ? QColor(211,208,211).rgb() : QColor(230,230,230).rgb()));
	if ( oldcolor != colors_[i][ Baghira::ColorTitleBlend ][ true ] )
		changed |= SettingColors;
                
        }
	
	oldcolor = ButtonColor_[MinButton];
   ButtonColor_[MinButton].setRgb((unsigned int)config.readNumEntry( "MinButtonColor", QColor(230,155,40).rgb() ));
	if ( oldcolor != ButtonColor_[MinButton] )
		changed |= SettingColors;
	
	oldcolor = ButtonColor_[MaxButton];
   ButtonColor_[MaxButton].setRgb((unsigned int)config.readNumEntry( "MaxButtonColor", QColor(121,180,54).rgb() ));
	if ( oldcolor != ButtonColor_[MaxButton] )
		changed |= SettingColors;
	
	oldcolor = ButtonColor_[CloseButton];
   ButtonColor_[CloseButton].setRgb((unsigned int)config.readNumEntry( "CloseButtonColor", QColor(200,85,70).rgb() ));
	if ( oldcolor != ButtonColor_[CloseButton] )
		changed |= SettingColors;
	
	oldcolor = ButtonColor_[StickyButton];
   ButtonColor_[StickyButton].setRgb((unsigned int)config.readNumEntry( "StickyButtonColor", QColor(74,140,242).rgb() ));
	if ( oldcolor != ButtonColor_[StickyButton] )
		changed |= SettingColors;
	
	oldcolor = ButtonColor_[AboveBelowButton];
   ButtonColor_[AboveBelowButton].setRgb((unsigned int)config.readNumEntry( "AboveButtonColor", QColor(74,140,242).rgb() ));
	if ( oldcolor != ButtonColor_[AboveBelowButton] )
		changed |= SettingColors;
	oldcolor = ButtonColor_[ShadeButton];
	ButtonColor_[ShadeButton].setRgb((unsigned int)config.readNumEntry( "ShadeButtonColor", QColor(74,140,242).rgb() ));
	if ( oldcolor != ButtonColor_[ShadeButton] )
		changed |= SettingColors;
	
	oldcolor = ButtonColor_[MenuButton];
	ButtonColor_[MenuButton].setRgb((unsigned int)config.readNumEntry( "MenuButtonColor", QColor(74,140,242).rgb() ));
	if ( oldcolor != ButtonColor_[MenuButton] )
		changed |= SettingColors;
	
	oldcolor = ButtonColor_[HelpButton];
	ButtonColor_[HelpButton].setRgb((unsigned int)config.readNumEntry( "HelpButtonColor", QColor(0,0,0).rgb() ));
	if ( oldcolor != ButtonColor_[HelpButton] )
		changed |= SettingColors;
	
	oldcolor = ButtonColor_[InactiveButton];
	ButtonColor_[InactiveButton].setRgb((unsigned int)config.readNumEntry( "InactiveButtonColor", QColor(255,255,255).rgb() ));
	if ( oldcolor != ButtonColor_[InactiveButton] )
		changed |= SettingColors;
               

	config.endGroup();
        return changed;
}

QImage* BaghiraFactory::tintGlossy( const QImage &src, const QColor &c ) const
{
    QImage *dest = new QImage( src.width(), src.height(), 32);
    dest->setAlphaBuffer( true );
    unsigned int *data = ( unsigned int * ) src.bits();
    unsigned int *destData = ( unsigned int* ) dest->bits();
    int total = src.width() * src.height();
    int u3rd = src.width()*4;
    int red, green, blue;
    int destR, destG, destB, alpha;
    int srcR = c.red();
    int srcG = c.green();
    int srcB = c.blue();
    int hue, s, v;
    c.getHsv( &hue, &s, &v );
    int sq = CLAMP((int)((45.0/128.0)*s+55),0,100);
    // float srcPercent, destPercent;
    for ( int current = 0 ; current < total ; ++current ) {
        alpha = qAlpha( data[ current ] );
	if (alpha < 230){
		destData[ current ] =  data[ current ];
		continue; //do not handle translucent parts to not affect blending
	}
    	red = qRed( data[ current ] );
    	green = qGreen( data[ current ] );
        blue = qBlue( data[ current ] );
	if ( current > u3rd || red < 223 ){
            SATURATION_COLOR2(sq, red, green, blue, srcR, srcG, srcB)
        } else {
            SATURATION_COLOR2(sq, red, green, blue, 255, 255, 255);
        }
	// force back to valid colorspace !
	COLOR_SPACE(destR, destG, destB);
	destData[ current ] = qRgba( destR, destG, destB, alpha );
    }
    return ( dest );
}

QImage* BaghiraFactory::tintBrush( const QImage &img, const QColor &c ) const
{

//     if ( img.depth() != 32 )
//     	img = img.convertDepth( 32 );
    QImage *dest = new QImage( img.width(), img.height(), 32);
    unsigned int *data = ( unsigned int * ) img.bits();
    unsigned int *destData = ( unsigned int* ) dest->bits();
    int total = img.width() * img.height();
    int current;
    int delta;
    int red, green, blue, grey, iGrey;
    int destR, destG, destB, alpha;
 
    int srcR = c.red();
    int srcG = c.green();
    int srcB = c.blue();

    SATURATION_COLOR(180, 180, 180);
    COLOR_SPACE(destR, destG, destB);
    brushedMetalColor = QColor(destR, destG, destB);
    
    // float srcPercent, destPercent;
    for ( current = 0 ; current < total ; ++current ) {
        alpha = qAlpha( data[ current ] );
        blue = qBlue( data[ current ] );
    	red = qRed( data[ current ] );
    	green = qGreen( data[ current ] );
	SATURATION_COLOR(red, green, blue);
	// force back to valid colorspace !
	COLOR_SPACE(destR, destG, destB);
	destData[ current ] = qRgba( destR, destG, destB, alpha );
    }
    return ( dest );
}

void BaghiraFactory::createOS8Buttons(int style){
	QPainter p;
	QColor initColor;
	for ( int active = 0; active <= 1; ++active ){
		for (int t = 0; t < 3; t++){
			initColor = QColor(color(style, Baghira::ColorTitleBar, active ).dark(100 + t*15));
			nostalgia_[style][t][active] = QPixmap(BUTTONSIZE, BUTTONSIZE);
			p.begin(&nostalgia_[style][t][active]);
			p.setPen(color(style, Baghira::ColorTitleBar, active ));
			p.drawPoint(0,BUTTONSIZE-1);
			p.drawPoint(BUTTONSIZE-1,0);
			p.setPen(initColor);
			// dark color
			// outer relief ===============================
			p.drawLine(0,0,0,BUTTONSIZE-2);
			p.drawLine(0,0,BUTTONSIZE-2,0);
			// =============================================
			// inner relief ================================
			if (t < 2){
				p.drawLine(3,BUTTONSIZE-3,BUTTONSIZE-3,BUTTONSIZE-3);
				p.drawLine(BUTTONSIZE-3,3,BUTTONSIZE-3,BUTTONSIZE-3);
			}
			else{
				p.drawLine(2,2,2,BUTTONSIZE-4);
				p.drawLine(2,2,BUTTONSIZE-4,2);
			}
			// ========================================
			for (int i = 0; i < BUTTONSIZE - 6; i++){
				if (i == BUTTONSIZE - 7){
					p.drawPoint(2, BUTTONSIZE - 3);
					p.drawPoint(BUTTONSIZE - 3, 2);
				}
				for (int j = 0; j < i+1; j++){
					p.setPen(initColor.light(100 + (i + j)*3));
					p.drawPoint(i+3,j+3);
					if (i != j) p.drawPoint(j+3,i+3);
				}
			}
			// Color is now Bright ;-)
			// inner relief ===============================
			if (t < 2){
				p.drawLine(2,2,2,BUTTONSIZE-4);
				p.drawLine(2,2,BUTTONSIZE-4,2);
			}
			else{
				p.drawLine(3,BUTTONSIZE-3,BUTTONSIZE-3,BUTTONSIZE-3);
				p.drawLine(BUTTONSIZE-3,3,BUTTONSIZE-3,BUTTONSIZE-3);
			}
			// ================================
			// outer relief ======================================
			p.drawLine(1,BUTTONSIZE-1,BUTTONSIZE-1,BUTTONSIZE-1);
			p.drawLine(BUTTONSIZE-1,1,BUTTONSIZE-1,BUTTONSIZE-1);
			//==================================
			// black frame
			p.setPen(Qt::black);
			p.drawRect(1,1,BUTTONSIZE-2,BUTTONSIZE-2);
			p.end();
		}
	}
}

QPixmap &BaghiraFactory::ButtonPixmap( Baghira::WindowState wState,
                                       Baghira::ButtonType type, Baghira::ButtonState state, int style, bool small ) {
    return ButtonPixmap_[style][ wState ][ type ][ state ][small];
}

QImage* BaghiraFactory::DecoImage( QPixmap *pix, int x, int y, int width, int height ) {
	QPixmap tmpPix( width, height );
	bitBlt ( &tmpPix, 0, 0, pix, x, y, width, height );
	QImage *returnImage = new QImage( tmpPix.convertToImage() );
	return returnImage;
}

///////////////////////////////////////////////////////////////////////////
// createGradient()
// ----------------
// Create a gradient

void BaghiraFactory::createGradient(KPixmap &pix, const QColor &light, const QColor &dark, Baghira::TitlebarType effect)
{
   if (pix.width() == 0) return;
   
   QPainter p;
   switch( effect ){
   case Baghira::Stipples:
      KPixmapEffect::unbalancedGradient(pix, light, light.dark(110), KPixmapEffect::VerticalGradient, 0);
      p.begin(&pix);
      p.setPen(dark);
      for( int i = 0; i < pix.height(); i+=4 ){
         p.drawLine(0,i,pix.width(),i);
         p.drawLine(0,i+1,pix.width(),i+1);
      }
      p.end();
      break;
   case Baghira::FlatStipples:
      {
         p.begin(&pix);
         p.fillRect(pix.rect(),light);
         QColor mid;
         mid.setRgb((light.red()+2*dark.red())/3,(light.green()+2*dark.green())/3,(light.blue()+2*dark.blue())/3);
         p.setPen(mid);
         for( int i = pix.height() - 1; i > 0; i-=4 )
         {
            p.drawLine(0,i,pix.width(),i);
            p.drawLine(0,i-2,pix.width(),i-2);
         }
         p.setPen(dark);
         for( int i = pix.height() - 2; i > 0; i-=4 )
         {
            p.drawLine(0,i,pix.width(),i);
         }
         p.end();
         break;
      }
   case Baghira::Nostalgia:
      p.begin(&pix);
      p.fillRect(pix.rect(),light);
      p.setPen(light.light(120));
      for (int i = (pix.height() - 1) / 4; i < (pix.height() - 1) / 4 * 3 + 1; i +=2)
         p.drawLine(0,i,pix.width()-1,i);
      p.setPen(light.dark(120));
      for (int i = (pix.height() - 1) / 4; i < (pix.height() - 1) / 4 * 3 + 1; i +=2)
         p.drawLine(0,i+1,pix.width()-1,i+1);
      p.end();
      break;
   case Baghira::Gradient:
   default:
      KPixmapEffect::gradient(pix, light, dark, KPixmapEffect::VerticalGradient, 0);
      break;
   }
}

static bool blend( const QImage & upper, const QImage & lower, QImage & output)
// adopted from kimageeffect::blend - what is not endian safe...
{
   if
   (
   upper.width()  > lower.width()  ||
   upper.height() > lower.height() ||
   upper.depth() != 32             ||
   lower.depth() != 32
   )
      return false;

   output = lower.copy();
   
   register uchar *i, *o;
   register int a;
   register int col;
   register int w = upper.width();
   int row(upper.height() - 1);

   do
   {
      i = upper.scanLine(row);
      o = output.scanLine(row);
      
      col = w << 2;

      --col;
      
      do 
      {
#ifdef WORDS_BIGENDIAN
         while (!(a = i[col-3]) && (col != 3))
#else
 	 while (!(a = i[col]) && (col != 3))
#endif
         {
            --col; --col; --col; --col;
         }
#ifndef WORDS_BIGENDIAN
         --col;
#endif
         o[col] += ((i[col] - o[col]) * a) >> 8;
         
         --col;
         o[col] += ((i[col] - o[col]) * a) >> 8;

         --col;
         o[col] += ((i[col] - o[col]) * a) >> 8;
         
#ifdef WORDS_BIGENDIAN
         --col;
#endif

      } while (col--);
   } while (row--);
   return true;
}

//////////////////////////////////////////////////////////////////////////////
// createPixmaps()
// ---------------
// Create all our pixmaps
void BaghiraFactory::createPixmaps() {
//	QPainter painter;
	QColorGroup group;
	QPainter icyAPainter;
	QImage tmpResult;
	QImage tmpResult2;
	QImage tmpDeco;
	QImage buttonImage;
	QImage btnIcons[MenuButton];
   btnIcons[AboveBelowButton] = QImage();
	// load icon images - useless for nostalgia style, but i assume they'll be used at least once ;)
	btnIcons[MinButton] = uic_findImage( "icon_minimize" );
	btnIcons[MaxButton] = uic_findImage( "icon_maximize" );
	btnIcons[CloseButton] = uic_findImage( "icon_close" );
	btnIcons[StickyButton] = uic_findImage( "icon_sticky" );
// 	btnIcons[AboveBelowButton] = uic_findImage( "icon_above" );
	btnIcons[ShadeButton] = uic_findImage( "icon_shade" );
	
	int B_3 = 0;
	int B_6 = 0;
//	int bt, bs;
	int xoff = 0;
	int yoff = 1;
        bool brushedMode;
	
	for (int style = 0; style < 5; style++){
        for ( int active = 0; active <= 1; ++active ) {
		for ( int small = 0; small <= 1; ++small ) {
			KPixmap &tbar = pix_[style][ TitleBar ][ active ][ small ];
                        KPixmap &tcenter = pix_[style][ TitleCenter ][ active ][ small ];
                        brushedMode = effect_[style][active] == Baghira::Brushed;
			
			// create gradients and fills
			//	    if (effect_ == Baghira::Gradient || effect_ == Baghira::Stipples /*|| effect_ == Baghira::Glossy * /
   		if (effect_[style][active] < Baghira::Glossy || effect_[style][active] == Baghira::FlatStipples){
				// resize pixmap
				tbar.resize( 100, small ? TITLESIZESMALL : TITLESIZE );
				createGradient( tbar, color(style, Baghira::ColorTitleBar, active ), color(style, Baghira::ColorTitleBlend, active ), effect_[style][active] );
			}
			else if (brushedMode){
                                QPixmap brushTile;
				QPixmap brushGradient;
				if (tintBrush_){
                                    brushTile = QPixmap(*tintBrush(uic_findImage( "brushed-tile" ), brushTint));
				    brushGradient = QPixmap(*tintBrush(uic_findImage( "brushed-gradient" ), brushTint));
                                }
                                else{
                                    brushTile = uic_findImage( "brushed-tile" );
				    brushGradient = uic_findImage( "brushed-gradient" );
                                    brushedMetalColor = QColor(180,180,180);
                                }
				// resize pixmap
				tbar.resize( 256, small ? TITLESIZESMALL : TITLESIZE );
				tcenter.resize( 1024, small ? TITLESIZESMALL : TITLESIZE );
				QPainter painter(&tbar);
				painter.drawTiledPixmap( 0,0, tbar.width(), tbar.height(), brushTile );
				painter.end();
				painter.begin(&tcenter);
				painter.drawTiledPixmap( 0,0, tcenter.width(), tcenter.height(), brushGradient );
			}
			else{
				tbar.resize( 32, small ? TITLESIZESMALL : TITLESIZE);
				KPixmap tmp = QPixmap(*tintGlossy(uic_findImage( "deco_glossy" ), color(style, Baghira::ColorTitleBar, active )));
				if (tbar.height() > 18){
					for (int i = 8; i < tbar.height() - 10; i++){
						bitBlt(&tbar, 0, i, &tmp, 0, 8, 32, 1);
					}
				}
				bitBlt(&tbar, 0, tbar.height() - 11, &tmp, 0, 9, 32, 10);
				bitBlt(&tbar, 0, 0, &tmp, 0, 0, 32, 8);
			}
			
			switch ( buttonStyle_[style] ) {
				
			// Load Jaguar Buttons =========================================================================

			case Jaguar:

				tmpDeco = *DecoImage( &tbar, 0, ( tbar.height() - BUTTONSIZE ) / 2, BUTTONSIZE, BUTTONSIZE );
				
				// unpressed buttons
				buttonImage = uic_findImage( "button_jaguar");
				
				for (int bt = MinButton; bt < MenuButton; bt++){
					for (int bs = ButtonInactive; bs < Pressed; bs++){
						tmpResult2 = *tintGlossy( buttonImage, bs == ButtonInactive ?
										     ButtonColor_[InactiveButton] :
										     ButtonColor_[bt] );
						/*brushedMode ? blend( tmpResult2, tmpDeco, tmpResult ) : */blend( tmpResult2, tmpDeco, tmpResult );
                                                if (bs > ButtonActive) blend( btnIcons[bt], tmpResult, tmpResult );
						ButtonPixmap_[style][ active ][ bt ][ bs ][small] = QPixmap( tmpResult );
					}
				}
				
				// pressed buttons // as jaguar buttons shift
				buttonImage = uic_findImage( "button_jaguar_down");
				
				for (int bt = MinButton; bt < MenuButton; bt++){
						tmpResult2 = *tintGlossy( buttonImage, ButtonColor_[bt].dark(120));
						/*brushedMode ? blend( tmpResult2, tmpDeco, tmpResult ) : */blend( tmpResult2, tmpDeco, tmpResult );
						KImageEffect::blendOnLower( xoff, yoff, btnIcons[bt], tmpResult); // must shift as well
						ButtonPixmap_[style][ active ][ bt ][ Pressed ][small] = QPixmap( tmpResult );
				}
				
				
				// HelpButton
				buttonImage = uic_findImage( "icon_help");
				for (int bs = ButtonInactive; bs < ButtonStateAmount; bs++){
					tmpResult2 = *tintGlossy( buttonImage, bs == ButtonInactive ?
									ButtonColor_[InactiveButton] :
									bs == Hovered ?
									ButtonColor_[HelpButton].light(120) :
									bs == Pressed ?
									ButtonColor_[HelpButton].dark(120) :
									ButtonColor_[HelpButton] );
					/*brushedMode ? blend( tmpResult2, tmpDeco, tmpResult ) : */blend( tmpResult2, tmpDeco, tmpResult );
					ButtonPixmap_[style][ active ][ HelpButton ][ bs ][small] = QPixmap( tmpResult );
				}
				
				tmpDeco = *DecoImage( &tbar, 0, ( tbar.height() - BUTTONSIZE ) / 2, MENUBUTTONWIDTH, BUTTONSIZE );
				
				// MenuButton
				buttonImage = uic_findImage( "button_jaguar_menu");
				for (int bs = ButtonInactive; bs < Pressed; bs++){
					tmpResult2 = *tintGlossy( buttonImage, bs < Hovered ?
									ButtonColor_[InactiveButton] :
									ButtonColor_[MenuButton] );
					/*brushedMode ? blend( tmpResult2, tmpDeco, tmpResult ) : */blend( tmpResult2, tmpDeco, tmpResult );
					ButtonPixmap_[style][ active ][ MenuButton ][ bs ][small] = QPixmap( tmpResult );
				}

				

				// pressed MenuButton // as jaguar buttons shift
				buttonImage = uic_findImage( "button_jaguar_menu_down");
				tmpResult2 = *tintGlossy( buttonImage, ButtonColor_[MenuButton].dark(120));
				/*brushedMode ? blend( tmpResult2, tmpDeco, tmpResult ) : */blend( tmpResult2, tmpDeco, tmpResult );
				ButtonPixmap_[style][ active ][ MenuButton ][ Pressed ][small] = QPixmap( tmpResult );

				
				break;
				
			// Load Milk Buttons =========================================================================

			case Milk:
				
				tmpDeco = *DecoImage( &tbar, 0, ( tbar.height() - BUTTONSIZE ) / 2, BUTTONSIZE, BUTTONSIZE );
				
				// unpressed buttons
				buttonImage = uic_findImage( "button_milk");
				
				
				for (int bt = MinButton; bt < MenuButton; bt++){
					for (int bs = ButtonInactive; bs < Pressed; bs++){
						/*brushedMode ? blend( buttonImage, tmpDeco, tmpResult ) : */blend( buttonImage, tmpDeco, tmpResult );
						if (bs > ButtonActive) blend( btnIcons[bt], tmpResult, tmpResult );
						ButtonPixmap_[style][ active ][ bt ][ bs ][small] = QPixmap( tmpResult );
					}
				}
				
				// pressed buttons // as pressed milk buttons differ
				buttonImage = uic_findImage( "button_milk_down");
				for (int bt = MinButton; bt < MenuButton; bt++){
					/*brushedMode ? blend( buttonImage, tmpDeco, tmpResult ) : */blend( buttonImage, tmpDeco, tmpResult );
					blend( btnIcons[bt], tmpResult, tmpResult );
					ButtonPixmap_[style][ active ][ bt ][ Pressed ][small] = QPixmap( tmpResult );
				}
				
				// HelpButton
				buttonImage = uic_findImage( "icon_help");
				for (int bs = ButtonInactive; bs < ButtonStateAmount; bs++){
					/*brushedMode ? blend( buttonImage, tmpDeco, tmpResult ) : */blend( buttonImage, tmpDeco, tmpResult );
					ButtonPixmap_[style][ active ][ HelpButton ][ bs ][small] = QPixmap( tmpResult );
				}

				tmpDeco = *DecoImage( &tbar, 0, ( tbar.height() - BUTTONSIZE ) / 2, MENUBUTTONWIDTH, BUTTONSIZE );
				
				// MenuButton
				buttonImage = uic_findImage( "button_milk_menu");
				for (int bs = ButtonInactive; bs < Pressed; bs++){
					/*brushedMode ? blend( buttonImage, tmpDeco, tmpResult ) : */blend( buttonImage, tmpDeco, tmpResult );
					ButtonPixmap_[style][ active ][ MenuButton ][ bs ][small] = QPixmap( tmpResult );
				}

				// pressed MenuButton // as pressed milk buttons differ
				buttonImage = uic_findImage( "button_milk_menu_down");
				/*brushedMode ? blend( buttonImage, tmpDeco, tmpResult ) : */blend( buttonImage, tmpDeco, tmpResult );
				ButtonPixmap_[style][ active ][ MenuButton ][ Pressed ][small] = QPixmap( tmpResult );
				
				break;
				
			// Create Nostalgia Buttons =========================================================================
			
			case NostalgiaButton:
				createOS8Buttons(style);
				B_3 = (int)((BUTTONSIZE -1 )/ 3);
				B_6 = B_3*2;
				
                                tmpDeco = *DecoImage( &tbar, 0, ( tbar.height() - BUTTONSIZE ) / 2, MENUBUTTONWIDTH, BUTTONSIZE );
                                
				for (int bt = MinButton; bt < ButtonTypeAmount; bt++){
					for (int bs = ButtonInactive; bs < ButtonStateAmount; bs++){
						if (bs < Hovered)
							ButtonPixmap_[style][ active ][ bt ][ bs ][small] = QPixmap( nostalgia_[style][1][active] );
						else if (bs == Hovered)
							ButtonPixmap_[style][ active ][ bt ][ bs ][small] = QPixmap( nostalgia_[style][0][active] );
						else
							ButtonPixmap_[style][ active ][ bt ][ bs ][small] = QPixmap( nostalgia_[style][2][active] );
						if (bs > ButtonInactive){
							icyAPainter.begin(&ButtonPixmap_[style][ active ][ bt ][ bs ][small]);
							icyAPainter.setPen(Qt::black);
							if (bt == MinButton){
								icyAPainter.drawLine(2,B_3,BUTTONSIZE - 3,B_3);
								icyAPainter.drawLine(2,B_6,BUTTONSIZE - 3,B_6);
								icyAPainter.end();
								continue;
							}
							if (bt == MaxButton){
								icyAPainter.drawLine(2,B_6,B_6,B_6);
								icyAPainter.drawLine(B_6,2,B_6,B_6);
								icyAPainter.end();
								continue;
							}
 							if (bt == StickyButton){
								icyAPainter.fillRect(BUTTONSIZE/2-1,BUTTONSIZE/2-1,2,2,Qt::black);
								icyAPainter.end();
								continue;
							}
							if (bt == ShadeButton){
								icyAPainter.drawLine(2,B_3,BUTTONSIZE-3,B_3);
								icyAPainter.drawLine(2,B_3+3,BUTTONSIZE-3,B_3+3);
								icyAPainter.drawLine(2,B_3+6,BUTTONSIZE-3,B_3+6);
								icyAPainter.end();
								continue;
							}
							if (bt == HelpButton){
								QFont tmpFont = QFont(options() ->font( true, false ));
								tmpFont.setPixelSize(BUTTONSIZE - 4);
								icyAPainter.setFont(tmpFont);
								icyAPainter.drawText( 0, 0, BUTTONSIZE, BUTTONSIZE, Qt::AlignHCenter | Qt::AlignVCenter, "?" );
								icyAPainter.end();
								continue;
							}
							if (bt == MenuButton){
								icyAPainter.drawLine(2,B_3,BUTTONSIZE-3,B_3);
								icyAPainter.end();
								
							}
							icyAPainter.end();
						}
						if (bt == MenuButton){
							buttonImage = ButtonPixmap_[style][ active ][ bt ][ bs ][small].convertToImage();
							blend( buttonImage , tmpDeco, tmpDeco );
							ButtonPixmap_[style][ active ][ bt ][ bs ][small] = QPixmap(tmpDeco);
						}
					}
				}
				break;
				
			// Load Panther Buttons =========================================================================

			case Panther:
			default:
			
				tmpDeco = *DecoImage( &tbar, 0, ( tbar.height() - BUTTONSIZE ) / 2, BUTTONSIZE, BUTTONSIZE );
				
				// unpressed buttons
				buttonImage = uic_findImage( "button_panther");
				
				for (int bt = MinButton; bt < MenuButton; bt++){
					for (int bs = ButtonInactive; bs < ButtonStateAmount; bs++){
						tmpResult2 = *tintGlossy( buttonImage, bs == ButtonInactive ?
										     ButtonColor_[InactiveButton] :
										     bs == Pressed ?
										     ButtonColor_[bt].dark(120) :
										     ButtonColor_[bt] );
                                                if (bs > ButtonActive) blend( btnIcons[bt], tmpResult2, tmpResult2 );
                                                if (bt == MaxButton && bs > ButtonInactive) deMaximizer_.setPixmap(bs - 1, tmpResult2);
						blend( tmpResult2, tmpDeco, tmpResult );
						ButtonPixmap_[style][ active ][ bt ][ bs ][small] = QPixmap( tmpResult );
					}
				}
				
				
				// HelpButton
				buttonImage = uic_findImage( "icon_help");
				for (int bs = ButtonInactive; bs < ButtonStateAmount; bs++){
					tmpResult2 = *tintGlossy( buttonImage, bs == ButtonInactive ?
									ButtonColor_[InactiveButton] :
									bs == Hovered ?
									ButtonColor_[HelpButton].light(120) :
									bs == Pressed ?
									ButtonColor_[HelpButton].dark(120) :
									ButtonColor_[HelpButton] );
					/*brushedMode ? blend( tmpResult2, tmpDeco, tmpResult ) : */blend( tmpResult2, tmpDeco, tmpResult );
					ButtonPixmap_[style][ active ][ HelpButton ][ bs ][small] = QPixmap( tmpResult );
				}
				
				tmpDeco = *DecoImage( &tbar, 0, ( tbar.height() - BUTTONSIZE ) / 2, MENUBUTTONWIDTH, BUTTONSIZE );
				
				// MenuButton
				buttonImage = uic_findImage( "button_panther_menu");
				for (int bs = ButtonInactive; bs < ButtonStateAmount; bs++){
					tmpResult2 = *tintGlossy( buttonImage, bs < Hovered ?
									ButtonColor_[InactiveButton] :
									     bs == Pressed ?
									     ButtonColor_[MenuButton].dark(120) :
									ButtonColor_[MenuButton] );
					/*brushedMode ? blend( tmpResult2, tmpDeco, tmpResult ) : */blend( tmpResult2, tmpDeco, tmpResult );
					ButtonPixmap_[style][ active ][ MenuButton ][ bs ][small] = QPixmap( tmpResult );
				}
			}
		}
	}
        }
}

//////////////////////////////////////////////////////////////////////////////
// borderSizes()
// -------------
// Return list of valid border sizes

QValueList<BaghiraFactory::BorderSize> BaghiraFactory::borderSizes() const {
	return QValueList<BorderSize>() << BorderTiny << BorderNormal
	       << BorderLarge << BorderVeryLarge
	       << BorderHuge << BorderVeryHuge;
}

//////////////////////////////////////////////////////////////////////////////
// BaghiraButton Class                                                        //
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// BaghiraButton()
// --------------
// Constructor

BaghiraButton::BaghiraButton( BaghiraClient *parent, const char *name, const QString& tip, bool small, ButtonType type, int style )
: QButton( parent->widget(), name ), client_( parent ), type_( type ), deco_( 0 ), icon_(), small_( small ), lastmouse_( 0 ), mouseover_( false ), _blocked(false) {
	setBackgroundMode( NoBackground );
	setCursor( arrowCursor );
	QToolTip::add( this, tip );
	down_ = false;
	mouseover_ = false;
        mask = NULL;
        maskDown = NULL;

	setFixedSize( type == Baghira::MenuButton ? MENUBUTTONWIDTH : BUTTONSIZE, BUTTONSIZE );
	for ( int bs = Baghira::ButtonInactive; bs <= Baghira::Pressed; bs++ ) {
		pixmap[ Baghira::WindowInactive ][ ( Baghira::ButtonState ) bs ] =
		    &BaghiraFactory::ButtonPixmap( Baghira::WindowInactive, type,
		                                   ( Baghira::ButtonState ) bs, style, small );
		pixmap[ Baghira::WindowActive ][ ( Baghira::ButtonState ) bs ] =
		    &BaghiraFactory::ButtonPixmap( Baghira::WindowActive, type,
		                                   ( Baghira::ButtonState ) bs, style, small );
	}
}

BaghiraButton::~BaghiraButton()
{
   if ( deco_ )
      delete deco_;
}


//////////////////////////////////////////////////////////////////////////////
// sizeHint()
// ----------
// Return size hint

QSize BaghiraButton::sizeHint() const {
	return QSize( BUTTONSIZE, BUTTONSIZE );
}

//////////////////////////////////////////////////////////////////////////////
// enterEvent()
// ------------
// Mouse has entered the button

void BaghiraButton::enterEvent( QEvent *e ) {
#if 0
   if (type_ == CloseButton)
   {
      _blocked = true;
      QTimer::singleShot(250,this,SLOT(delayedEnterEvent()));
      QButton::enterEvent( e );
      return;
   }
#endif
	mouseover_ = true;
	repaint( false );
	QButton::enterEvent( e );
}

void BaghiraButton::delayedEnterEvent()
{
   if (hasMouse())
   {
      _blocked = false;
      mouseover_ = true;
      repaint( false );
   }
}

//////////////////////////////////////////////////////////////////////////////
// leaveEvent()
// ------------
// Mouse has left the button

void BaghiraButton::leaveEvent( QEvent *e )
{
   mouseover_ = false;
   if ( type_ == Baghira::MenuButton )
      down_ = false;
   repaint( false );
   QButton::leaveEvent( e );
}

//////////////////////////////////////////////////////////////////////////////
// mousePressEvent()
// -----------------
// Button has been pressed

void BaghiraButton::mousePressEvent( QMouseEvent* e )
{
   if (_blocked && !(e->state() & Qt::ControlButton))
      return;
   lastmouse_ = e->button();
   
   //update mask
   if (maskDown)
   {
      clearMask();
      setMask(*maskDown);
   }
   // translate and pass on mouse event
   int button = LeftButton;
   if ( ( type_ != MaxButton ) && ( e->button() != LeftButton ) )
   {
      button = NoButton; // middle & right buttons inappropriate
   }
   down_ = true;
   QMouseEvent me( e->type(), e->pos(), e->globalPos(), button, e->state() );
   repaint( false );
   QButton::mousePressEvent( &me );
}

// void BaghiraButton::setOn(bool on)
// {
//     QButton::setOn(on);
// }

//////////////////////////////////////////////////////////////////////////////
// mouseReleaseEvent()
// -----------------
// Button has been released

void BaghiraButton::mouseReleaseEvent( QMouseEvent* e ) {
	lastmouse_ = e->button();

	if (mask){
            clearMask();
            setMask(*mask);
        }    
        // translate and pass on mouse event
	int button = LeftButton;
	if ( ( type_ != MaxButton ) && ( e->button() != LeftButton ) ) {
		button = NoButton; // middle & right buttons inappropriate
	}
	down_ = false;
	QMouseEvent me( e->type(), e->pos(), e->globalPos(), button, e->state() );
	repaint( false );
	QButton::mouseReleaseEvent( &me );
}

inline ButtonState BaghiraButton::state() {
	if ( down_ || isDown() )
		return Pressed;
	else if ( mouseover_ )
		return Hovered;
	else if ( client_->isActive() )
		return ButtonActive;
	else
		return ButtonInactive;
}

//////////////////////////////////////////////////////////////////////////////
// drawButton()
// ------------
// Draw the button

void BaghiraButton::drawButton( QPainter *painter ) {
	if ( !BaghiraFactory::initialized() )
		return ;

	bool active = client_->isActive();
	
	if ( type_ == Baghira::StickyButton && client_->isOnAllDesktops() )
		painter->drawPixmap( 0, 0, BaghiraFactory::ButtonPixmap( ( Baghira::WindowState ) active, type_, state() == Pressed ? Pressed : Hovered, client_->currentStyle, client_->isTool() ));
	else
		painter->drawPixmap( 0, 0, BaghiraFactory::ButtonPixmap( ( Baghira::WindowState ) active, type_, state() , client_->currentStyle, client_->isTool() ));
   
   if (type_ == Baghira::AboveBelowButton)
   {
      if (client_->keepAbove())
      {
         painter->drawPixmap((width()-8)/2,height()/2-4,BaghiraFactory::arrowUp);
      }
      else if (client_->keepBelow())
      {
         painter->drawPixmap((width()-8)/2,height()/2,BaghiraFactory::arrowDown);
      }
   }
}

void BaghiraButton::setMasks(QBitmap *bitmask, QBitmap *bitmaskDown){
    clearMask();
    mask = bitmask;
    maskDown = bitmaskDown;
    if (mask) setMask(*mask);
}
//////////////////////////////////////////////////////////////////////////////
// BaghiraClient Class                                                        //
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// BaghiraClient()
// -------------
// Constructor

BaghiraClient::BaghiraClient( KDecorationBridge *b, KDecorationFactory *f )
		: KDecoration( b, f )
{
	;
}

BaghiraClient::~BaghiraClient() 
{
   if (noDeco_)
      return;
   BaghiraFactory::deMaximizer().setClient(NULL);
	for ( int n = 0; n < ButtonTypeAmount; n++ )
	{
      if ( button[ n ] )
         delete button[ n ];
	}
}

//////////////////////////////////////////////////////////////////////////////
// init()
// ------
// Real initialization

void BaghiraClient::init()
{
   if (BaghiraFactory::noModalDeco() && isModal() && !isMinimizable() && !isResizable())
   {
      noDeco_ = true;
      createMainWidget( WResizeNoErase | WRepaintNoErase );
      widget() ->setBackgroundMode( NoBackground );
      return;
   }
   noDeco_ = false;
#if KDE_IS_VERSION(3,3,91)
   //first try if there's an X setting
   unsigned char *data = 0;
   Atom actual;
   int format, result;
   unsigned long n, left;
   currentStyle = BaghiraFactory::defaultMode();
   result = XGetWindowProperty(qt_xdisplay(), windowId(), baghira_deco_design, 0L, 1L, False, XA_CARDINAL, &actual, &format, &n, &left, &data);
   if (result == Success && data != None) // found set design
   {
      memcpy (&currentStyle, data, sizeof (unsigned int));
      currentStyle = currentStyle>4?4:currentStyle;
   }
   else // try if there's a definition file
   {
      XClassHint classHint;
      FILE *file = NULL;
      if ( XGetClassHint( qt_xdisplay(), windowId(), &classHint ) )
      {
         QString tmpString;
         // overwrite file from bab starter? - don't worry, if there's been one and deleted by the style we've got the X setting (much better as it will take care of all subwindows)
         tmpString = QDir::homeDirPath() + "/.baghira/.bab/" + QString( classHint.res_class ).lower();
         if( (file = fopen(tmpString.latin1(), "r")) != NULL )
         {
            fscanf(file,"%d\n%d\n", &format, &currentStyle);
            fclose(file);
            remove(tmpString.latin1()); // this is only temporary
         }
         else
         {
            tmpString = QDir::homeDirPath() + "/.baghira/" + QString( classHint.res_class ).lower();
            if( (file = fopen(tmpString.latin1(), "r")) != NULL )
            {
               fscanf(file,"%d\n%d\n", &format, &currentStyle);
               fclose(file);
            }
         }
         XFree( classHint.res_name );
         XFree( classHint.res_class );
      }
      if (file == NULL)
      {
         QString tmpString = QDir::homeDirPath() + "/.baghira/.bab/.deco";
         if( (file = fopen(tmpString.latin1(), "r")) != NULL )
         {
            fscanf(file,"%d\n%d\n", &format, &currentStyle);
            fclose(file);
         }
      }
   }
   if (currentStyle > 4)
      currentStyle = BaghiraFactory::defaultMode();
#else
   QSettings config;
   config.beginGroup("/baghira/Style");
   if (config.readBoolEntry( "Design_Default", 1 ) == 2)
   {
      currentStyle = 2;
      config.endGroup();
   }
   else
   {
      config.endGroup();
      config.beginGroup("/baghira/Deco");
      int tmp = config.readNumEntry( "defaultMode", 1 );
      currentStyle = (tmp > -1 && tmp < 5) ? tmp : 1;
      config.endGroup();
   }
#endif
   createMainWidget( WResizeNoErase | WRepaintNoErase );
   widget() ->installEventFilter( this );
   widget() ->setBackgroundMode( NoBackground );

   if ( isTool() ) // tool windows have tiny titlebars
      titleheight_ = TITLESIZESMALL;
   else
      titleheight_ = TITLESIZE;

   // setup layout
   QVBoxLayout *mainlayout = new QVBoxLayout( widget() );
   QHBoxLayout *titlelayout = new QHBoxLayout();
   windowlayout = new QHBoxLayout();

   titlebar_ = new QSpacerItem( 1, titleheight_, QSizePolicy::Expanding, QSizePolicy::Fixed );
   mainlayout->setResizeMode( QLayout::FreeResize );
   mainlayout->addLayout( titlelayout );
   mainlayout->addLayout( windowlayout );

   windowlayout->addSpacing( BaghiraFactory::borderSize(currentStyle) );
   if ( isPreview() )
      windowlayout->addWidget( new QLabel( i18n( "<b><center>Baghira preview</center></b>" ), widget() ) );
   else
      windowlayout->addItem( new QSpacerItem( 0, 18 ) );
   
   windowlayout->addSpacing( BaghiraFactory::borderSize(currentStyle) );

   leftClose = false;
   rightClose = false;
   performEasyClose = false;
   
   // setup titlebar
   for ( int n = 0; n < ButtonTypeAmount; n++ )
      button[ n ] = 0;
   plusminus = 1;
   addButtons( titlelayout, options() ->titleButtonsLeft() );
   titlelayout->addItem( titlebar_ );
   plusminus = -1;
   isFaded = false;
   addButtons( titlelayout, options() ->titleButtonsRight() );
#if KDE_IS_VERSION(3,3,91)
   grip = 0;
#endif

   if (BaghiraFactory::effect(currentStyle, true) >= Baghira::Brushed || BaghiraFactory::effect(currentStyle, false) >= Baghira::Brushed)
      HandlePix = QPixmap(BaghiraFactory::pix(currentStyle, TitleBar, isActive(), false ));
   else if ( BaghiraFactory::bgStipple() )
   {
      HandlePix = QPixmap( 32, 32);
      QPainter painter;
      QColor c = widget()->colorGroup().background();
      HandlePix.fill(c);
      painter.begin( &HandlePix );
      painter.setPen( c.dark( 100 + (BaghiraFactory::contrast()/2) ) );
      
      for ( int i = 1; i < 32; i += 4 )
      {
         painter.drawLine( 0, i, 31, i );
         painter.drawLine( 0, i+2, 31, i+2 );
      }
      
      painter.setPen( c.dark( 100 + BaghiraFactory::contrast() ) );
      
      for ( int i = 2; i < 31; i += 4 )
         painter.drawLine( 0, i, 31, i );
      
      painter.end();
   }
}

QString BaghiraClient::caption() const
{
   if (!BaghiraFactory::delAppname())
      return KDecoration::caption();
   if (caption_.isNull())
   // generate first
   {
      const_cast<BaghiraClient*>( this )->caption_ = KDecoration::caption();
      int pos;
      if (caption_.startsWith( "tvtime" ))
      {
         pos = caption_.find(": ");
         if (pos > -1)
            const_cast<BaghiraClient*>( this )->caption_ = const_cast<BaghiraClient*>( this )->caption_.remove(0, pos+1);
      }
      else
      {
         pos = caption_.findRev(" - ");
         if (pos > -1)
            const_cast<BaghiraClient*>( this )->caption_ = caption_.left(pos);
      }
   }
   return caption_;
}

//////////////////////////////////////////////////////////////////////////////
// addButtons()
// ------------
// Add buttons to title layout

void BaghiraClient::addButtons( QBoxLayout *layout, const QString& s )
{
   if (noDeco_)
      return;
	//    const unsigned char *bitmap;
	QString tip;
        QBitmap *m = NULL;
        QBitmap *mn = NULL;
        QBitmap *mD = NULL;
        QBitmap *mnD = NULL;
        QBitmap *mH = NULL;
        if (BaghiraFactory::effect(currentStyle, true) == Baghira::Brushed){
            m = BaghiraFactory::buttonStyle(currentStyle) == Baghira::Panther ?
                                                &BaghiraFactory::pantherMask :
                BaghiraFactory::buttonStyle(currentStyle) == Baghira::Jaguar ?
                                                &BaghiraFactory::jaguarMask :
                BaghiraFactory::buttonStyle(currentStyle) == Baghira::Milk ?
                                                &BaghiraFactory::milkMask :
                                                NULL;
            mn = BaghiraFactory::buttonStyle(currentStyle) == Baghira::Panther ?
                                                &BaghiraFactory::pantherMenuMask :
                BaghiraFactory::buttonStyle(currentStyle) == Baghira::Jaguar ?
                                                &BaghiraFactory::jaguarMenuMask :
                BaghiraFactory::buttonStyle(currentStyle) == Baghira::Milk ?
                                                &BaghiraFactory::milkMenuMask :
                                                NULL;                
            mD = BaghiraFactory::buttonStyle(currentStyle) == Baghira::Panther ?
                                                NULL :
                BaghiraFactory::buttonStyle(currentStyle) == Baghira::Jaguar ?
                                                &BaghiraFactory::jaguarDownMask :
                BaghiraFactory::buttonStyle(currentStyle) == Baghira::Milk ?
                                                &BaghiraFactory::milkDownMask :
                                                NULL;
            mnD = BaghiraFactory::buttonStyle(currentStyle) == Baghira::Panther ?
                                                NULL :
                BaghiraFactory::buttonStyle(currentStyle) == Baghira::Jaguar ?
                                                &BaghiraFactory::jaguarMenuDownMask :
                BaghiraFactory::buttonStyle(currentStyle) == Baghira::Milk ?
                                                &BaghiraFactory::milkMenuDownMask :
                                                NULL;
            mH = &BaghiraFactory::helpMask;
        }
	if ( s.length() > 0 ) {
//                qWarning("%s",s.ascii());
                layout->addSpacing( MARGIN );
                excursion = excursion + plusminus * MARGIN;
		bool buttonAdded;
                for ( unsigned n = 0; n < s.length(); n++ ) {
                        buttonAdded = false;
			switch ( s[ n ].latin1() ) {
			case 'M':    // Menu button
				if ( !button[ MenuButton ] ) {
					button[ MenuButton ] = new BaghiraButton( this, "menu",
					                       i18n( "Menu" ), isTool(), MenuButton );
					connect( button[ MenuButton ], SIGNAL( pressed() ), this,
					         SLOT( menuButtonPressed() ) );
					layout->addWidget( button[ MenuButton ] );
                                        buttonAdded = true;
					excursion = excursion + plusminus * MENUBUTTONWIDTH;
                                        if (BaghiraFactory::effect(currentStyle, true) == Baghira::Brushed) button[ MenuButton ]->setMasks(mn, mnD);
				}
				break;

			case 'S':    // Sticky button
				if ( !button[ StickyButton ] ) {
					if ( isOnAllDesktops() ) {
						tip = i18n( "Un-Sticky" );
					} else {
						tip = i18n( "Sticky" );
					}
					button[ StickyButton ] = new BaghiraButton( this, "sticky",
					                         tip, isTool(), StickyButton );
					connect( button[ StickyButton ], SIGNAL( clicked() ), this,
					         SLOT( toggleOnAllDesktops() ) );
					layout->addWidget( button[ StickyButton ] );
                                        buttonAdded = true;
					excursion = excursion + plusminus * BUTTONSIZE;
                                        if (BaghiraFactory::effect(currentStyle, true) == Baghira::Brushed) button[ StickyButton ]->setMasks(m, mD);
				}
				break;

			case 'H':    // Help button
				if ( ( !button[ HelpButton ] ) && providesContextHelp() ) {
					button[ HelpButton ] = new BaghiraButton( this, "help",
					                       i18n( "Help" ), isTool(), HelpButton );
					connect( button[ HelpButton ], SIGNAL( clicked() ), this,
					         SLOT( showContextHelp() ) );
					layout->addWidget( button[ HelpButton ] );
                                        buttonAdded = true;
					excursion = excursion + plusminus * BUTTONSIZE;
                                        if (BaghiraFactory::effect(currentStyle, true) == Baghira::Brushed) button[ HelpButton ]->setMasks(mH, NULL);
				}
				break;

			case 'I':    // Minimize button
				if ( ( !button[ MinButton ] ) && isMinimizable() ) {
					button[ MinButton ] = new BaghiraButton( this, "iconify",
					                      i18n( "Minimize" ), isTool(), MinButton );
					connect( button[ MinButton ], SIGNAL( clicked() ), this,
					         SLOT( minimize() ) );
					layout->addWidget( button[ MinButton ] );
                                        buttonAdded = true;
					excursion = excursion + plusminus * BUTTONSIZE;
                                        if (BaghiraFactory::effect(currentStyle, true) == Baghira::Brushed) button[ MinButton ]->setMasks(m, mD);
				}
				break;

			case 'A':    // Maximize button
				if ( ( !button[ MaxButton ] ) && isMaximizable() ) {
					if ( maximizeMode() == MaximizeFull ) {
						tip = i18n( "Restore" );
					} else {
						tip = i18n( "Maximize" );
					}
					button[ MaxButton ] = new BaghiraButton( this, "maximize",
					                      tip, isTool(), MaxButton );
					connect( button[ MaxButton ], SIGNAL( clicked() ), this,
					         SLOT( maxButtonPressed() ) );
					layout->addWidget( button[ MaxButton ] );
                                        buttonAdded = true;
					excursion = excursion + plusminus * BUTTONSIZE;
                                        if (BaghiraFactory::effect(currentStyle, true) == Baghira::Brushed) button[ MaxButton ]->setMasks(m, mD);
				}
				break;

			case 'X':    // Close button
				if ( ( !button[ CloseButton ] ) && isCloseable() ) {
					button[ CloseButton ] = new BaghiraButton( this, "close",
					                        i18n( "Close" ), isTool(), CloseButton );
					connect( button[ CloseButton ], SIGNAL( clicked() ), this,
					         SLOT( closeWindow() ) );
					layout->addWidget( button[ CloseButton ] );
                                        buttonAdded = true;
					excursion = excursion + plusminus * BUTTONSIZE;
					if (plusminus == 1 && n == 0) // left Button
						leftClose = true;
					if (plusminus == -1 && n == s.length() - 1) // right Button
						rightClose = true;
                                        if (BaghiraFactory::effect(currentStyle, true) == Baghira::Brushed) button[ CloseButton ]->setMasks(m, mD);
				}
				break;
				
			case 'F': // Above button
			case 'B': // Below button
				if ((!button[AboveBelowButton]))
				{
               button[AboveBelowButton] = new BaghiraButton(this, "above", i18n("Keep Above/Below Others"), isTool(), AboveBelowButton);
   				button[AboveBelowButton]->setDown( keepAbove() || keepBelow() );
               connect(button[AboveBelowButton], SIGNAL(clicked()), this, SLOT(aboveBelowButtonPressed()));
               layout->addWidget(button[AboveBelowButton]);
               buttonAdded = true;
               excursion = excursion + plusminus * BUTTONSIZE;
               if (BaghiraFactory::effect(currentStyle, true) == Baghira::Brushed) button[ AboveBelowButton ]->setMasks(m, mD);
				}
				break;
			case 'L': // Shade button
				if ((!button[ShadeButton] && isShadeable())) {
				button[ShadeButton] = new BaghiraButton(this, "shade", isSetShade() ? tip = i18n("Unshade") : tip = i18n("Shade"), isTool(), ShadeButton);
                                button[ShadeButton]->setDown( isShade() );
				connect(button[ShadeButton], SIGNAL(clicked()), this, SLOT(shadeButtonPressed()));
				layout->addWidget(button[ShadeButton]);
                                buttonAdded = true;
				excursion = excursion + plusminus * BUTTONSIZE;
                                if (BaghiraFactory::effect(currentStyle, true) == Baghira::Brushed) button[ ShadeButton ]->setMasks(m, mD);
				}
				break;
// 			case 'R': // Shade button
// 				if ((!button[ShadeButton] && isShadeable())) {
// 				button[ShadeButton] = new BaghiraButton(this, "shade", isSetShade() ? tip = i18n("Unshade") : tip = i18n("Shade"), isTool(), ShadeButton);
//                                 button[ShadeButton]->setDown( isShade() );
// 				connect(button[ShadeButton], SIGNAL(clicked()), this, SLOT(shadeButtonPressed()));
// 				layout->addWidget(button[ShadeButton]);
//                                 buttonAdded = true;
// 				excursion = excursion + plusminus * BUTTONSIZE;
//                                 if (BaghiraFactory::effect(currentStyle, true) == Baghira::Brushed) button[ ShadeButton ]->setMasks(m, mD);
// 				}
// 				break;
		
			case '_':    // Spacer item
				layout->addSpacing( MARGIN );
				excursion = excursion + plusminus * MARGIN;
			}
                    if (BaghiraFactory::addAutoSpacing() && buttonAdded){
                        layout->addSpacing( MARGIN );
                        excursion = excursion + plusminus * MARGIN;
                    }
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
// eventFilter()
// -------------
// Event filter

bool BaghiraClient::eventFilter( QObject *obj, QEvent *e )
{
	if ( obj != widget() )
		return false;

	switch ( e->type() ) {
	case QEvent::MouseButtonDblClick: {
			mouseDoubleClickEvent( static_cast<QMouseEvent *>( e ) );
			return true;
		}
	case QEvent::MouseButtonPress: {
            if (titlebar_->geometry().contains( static_cast<QMouseEvent *>(e)->pos() ) && static_cast<QMouseEvent *>(e)->state() == Qt::ControlButton){
                currentStyle < 4 ? currentStyle++ : currentStyle = 0;
#if KDE_IS_VERSION(3,3,91)
                unsigned long wmDesign = currentStyle;
                XChangeProperty(qt_xdisplay(), windowId(), baghira_deco_design, XA_CARDINAL, 32, PropModeReplace, (unsigned char *) &wmDesign, 1L);
#endif
                widget()->repaint();
                QBitmap *m = NULL;
                QBitmap *mn = NULL;
                QBitmap *mD = NULL;
                QBitmap *mnD = NULL;
                QBitmap *mH = NULL;
                if (BaghiraFactory::effect(currentStyle, true) == Baghira::Brushed){
                    m = BaghiraFactory::buttonStyle(currentStyle) == Baghira::Panther ?
                        &BaghiraFactory::pantherMask :
                        BaghiraFactory::buttonStyle(currentStyle) == Baghira::Jaguar ?
                        &BaghiraFactory::jaguarMask :
                        BaghiraFactory::buttonStyle(currentStyle) == Baghira::Milk ?
                        &BaghiraFactory::milkMask :
                        NULL;
                    mn = BaghiraFactory::buttonStyle(currentStyle) == Baghira::Panther ?
                        &BaghiraFactory::pantherMenuMask :
                        BaghiraFactory::buttonStyle(currentStyle) == Baghira::Jaguar ?
                        &BaghiraFactory::jaguarMenuMask :
                        BaghiraFactory::buttonStyle(currentStyle) == Baghira::Milk ?
                        &BaghiraFactory::milkMenuMask :
                        NULL;                
                    mD = BaghiraFactory::buttonStyle(currentStyle) == Baghira::Panther ?
                        NULL :
                        BaghiraFactory::buttonStyle(currentStyle) == Baghira::Jaguar ?
                        &BaghiraFactory::jaguarDownMask :
                        BaghiraFactory::buttonStyle(currentStyle) == Baghira::Milk ?
                        &BaghiraFactory::milkDownMask :
                        NULL;
                    mnD = BaghiraFactory::buttonStyle(currentStyle) == Baghira::Panther ?
                        NULL :
                        BaghiraFactory::buttonStyle(currentStyle) == Baghira::Jaguar ?
                        &BaghiraFactory::jaguarMenuDownMask :
                        BaghiraFactory::buttonStyle(currentStyle) == Baghira::Milk ?
                        &BaghiraFactory::milkMenuDownMask :
                        NULL;
                    mH = &BaghiraFactory::helpMask;
                }
                for (int i = 0; i < ButtonTypeAmount; i++){
                    if (button[i]){
                        button[i]->setMasks(i < Baghira::MenuButton ? m :
                            i == Baghira::MenuButton ? mn :
                            mH, i < Baghira::MenuButton ? mD :
                            i == Baghira::MenuButton ? mnD :
                            NULL);
                        button[i]->repaint();
                    }
                }
#if KDE_IS_VERSION(3,3,91)
                        if (grip)
                           grip->updateLook(TRUE);
#endif
                           doShape();
                           return true;
                        }
			if (BaghiraFactory::allowEasyClosing()){
				if (maximizeMode() == MaximizeFull && isActive() && (leftClose && static_cast<QMouseEvent *>( e )->x() == 0 && static_cast<QMouseEvent *>( e )->y() == 0) || (rightClose && static_cast<QMouseEvent *>( e )->x() == ((QWidget *)obj)->width() -1 && static_cast<QMouseEvent *>( e )->y() == 0))
					performEasyClose = true;
				else processMousePressEvent( static_cast<QMouseEvent *>( e ) );
			}
			else processMousePressEvent( static_cast<QMouseEvent *>( e ) );
			return true;
		}
	case QEvent::MouseButtonRelease:{
			if (performEasyClose && maximizeMode() == MaximizeFull && (leftClose && static_cast<QMouseEvent *>( e )->x() == 0 && static_cast<QMouseEvent *>( e )->y() == 0) || (rightClose && static_cast<QMouseEvent *>( e )->x() == ((QWidget *)obj)->width() -1 && static_cast<QMouseEvent *>( e )->y() == 0)){
				closeWindow();
				performEasyClose = false;
				return true;
			}
			return FALSE;
		}
	case QEvent::Paint: {
			paintEvent( static_cast<QPaintEvent *>( e ) );
			return true;
		}
	case QEvent::Resize: {
			resizeEvent( static_cast<QResizeEvent *>( e ) );
			return true;
		}
	case QEvent::Show: {
#if KDE_IS_VERSION(3,3,91)
    	if (!grip && BaghiraFactory::resizeGrip() && isResizable()) grip = new ResizeHandle(this);
#endif
            showEvent( static_cast<QShowEvent *>( e ) );
                return true;
		}
	case QEvent::Hide: {
            emit hide();
        }
	case QEvent::Wheel: {
            wheelEvent(static_cast<QWheelEvent *>( e ));
            return true;
	}
	default: {
			return false;
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////
// mouseCoubleClickEvent()
// -----------------------
// Doubleclick on title

void BaghiraClient::mouseDoubleClickEvent( QMouseEvent *e )
{
	if ( titlebar_->geometry().contains( e->pos() ) )
		titlebarDblClickOperation();
}

void BaghiraClient::wheelEvent( QWheelEvent *e )
{
	if (titlebar_->geometry().contains( e->pos() ) && e->state() == Qt::ControlButton){
		// surf through the desktops :)
			int target, cD;
			if (e->delta() > 0){
				(cD = KWin::currentDesktop()) == KWin::numberOfDesktops() ? target = 1 : target = cD + 1;
			}
			else{
				(cD = KWin::currentDesktop()) == 1 ? target = KWin::numberOfDesktops()  : target = cD - 1;
			}
			setDesktop (target);
			KWin::setCurrentDesktop (target);
	}
}

//////////////////////////////////////////////////////////////////////////////
// paintEvent()
// ------------
// Repaint the window

void BaghiraClient::paintEvent( QPaintEvent* )
{
   if ( !BaghiraFactory::initialized() || noDeco_)
      return ;

   QPainter painter( widget() );
   QColorGroup group;
   
   QRect title( 0, 0, width(), titlebar_->geometry().height() );
   QRect left( 0, title.height(), BaghiraFactory::borderSize(currentStyle), height() -
               title.height() );
   QRect right( width() - BaghiraFactory::borderSize(currentStyle), title.height(),
                  BaghiraFactory::borderSize(currentStyle), height() - title.height() );
   QRect handle( 0, height() - BaghiraFactory::borderSize(currentStyle),
                  width(), BaghiraFactory::borderSize(currentStyle) );
   
   int x, y, x2, y2, h, w;
   
   title.rect( &x, &y, &w, &h );
	
   // draw title bar
   if (BaghiraFactory::effect(currentStyle, isActive()) == Baghira::Brushed)
   {
      if (w > 1024)
      {
         int w1 = (w - 1024)/2;
         int w2 = w - 1024 - w1;
         painter.drawTiledPixmap( 0,0, w1, h, BaghiraFactory::pix(currentStyle, TitleBar, isActive(), isTool() ),256-w1,0 );
         painter.drawPixmap( w1, 0/*, 1024, h*/, BaghiraFactory::pix(currentStyle, TitleCenter, isActive(), isTool() ) );
         painter.drawTiledPixmap( w1 + 1024, 0, w2, h, BaghiraFactory::pix(currentStyle, TitleBar, isActive(), isTool() ) );
      }
      else
         painter.drawTiledPixmap( 0, 0, w, h, BaghiraFactory::pix(currentStyle, TitleCenter, isActive(), isTool() ), (1024-w)/2, 0 );
   }
   else
      painter.drawTiledPixmap( title, BaghiraFactory::pix(currentStyle, TitleBar, isActive(), isTool() ) );

   x2 = title.right();
   y2 = title.bottom();
   int shapeW = w;
   
   const QColor *ttbar = &(BaghiraFactory::color(currentStyle, Baghira::ColorTitleBar, isActive()));

   int tmp3Dimpact_ = 100 + BaghiraFactory::_3DImpact(currentStyle);
     
   painter.setPen( ttbar->light(tmp3Dimpact_));
	painter.drawLine( x+1, y+1, x2-1, y+1 );

   if (!(BaghiraFactory::allowEasyClosing() && maximizeMode() == MaximizeFull))
   {
      // draw around the corners ;-)
      if (BaghiraFactory::shapeUL(currentStyle))
      {
         // Draw edge of top-left corner inside the area removed by the mask.
         painter.drawPoint( 3, 2 );
         painter.drawPoint( 4, 2 );
         painter.drawPoint( 2, 3 );
         painter.drawPoint( 2, 4 );
      }
      if (BaghiraFactory::shapeUR(currentStyle))
      {
         // Draw edge of top-right corner inside the area removed by the mask.
         painter.drawPoint( shapeW - 5, 2 );
         painter.drawPoint( shapeW - 4, 2 );
         painter.drawPoint( shapeW - 3, 3 );
         painter.drawPoint( shapeW - 3, 4 );
      }
   }
	
        
   painter.setPen( ttbar->dark(tmp3Dimpact_) );
   if (BaghiraFactory::drawComicFrame() && !(BaghiraFactory::allowEasyClosing() && maximizeMode() == MaximizeFull))
   {
      painter.drawLine(x,y,x, y2);
      painter.drawLine(x2,y,x2, y2);
   }
   painter.drawLine( x, y, x2, y );
	
   if (!(BaghiraFactory::allowEasyClosing() && maximizeMode() == MaximizeFull))
   {
      // draw around the corners ;-)
      if (BaghiraFactory::shapeUL(currentStyle))
      {
         // Draw edge of top-left corner inside the area removed by the mask.
         painter.drawPoint( 3, 1 );
         painter.drawPoint( 4, 1 );
         painter.drawPoint( 2, 2 );
         painter.drawPoint( 1, 3 );
         painter.drawPoint( 1, 4 );
      }
      if (BaghiraFactory::shapeUR(currentStyle))
      {
         // Draw edge of top-right corner inside the area removed by the mask.
         painter.drawPoint( shapeW - 5, 1 );
         painter.drawPoint( shapeW - 4, 1 );
         painter.drawPoint( shapeW - 3, 2 );
         painter.drawPoint( shapeW - 2, 3 );
         painter.drawPoint( shapeW - 2, 4 );
      }
   }
        
   const QColor *ttblend = &(BaghiraFactory::color(currentStyle, Baghira::ColorTitleBlend, isActive()));
   
   if (BaghiraFactory::LineImpact(currentStyle) > 0)
   {
      painter.setPen( ttblend->dark(100 + BaghiraFactory::LineImpact(currentStyle)) );
      painter.drawLine( x, y2, x2, y2 );
   }

   // draw title text
   titlebar_->geometry().rect( &x, &y, &w, &h );
   int iconLeft;
   int iconWidth;
   if (BaghiraFactory::showTitle())
   {
      painter.setFont( options() ->font( isActive(), isTool() ) );
      painter.setPen( options()->color( ColorFont, isActive() ) );
      BaghiraFactory::drawIcon(currentStyle) ? iconWidth = 16 : iconWidth = 0;
      if (BaghiraFactory::drawIcon(currentStyle) || BaghiraFactory::effect(currentStyle, isActive()) == Baghira::Nostalgia)
      {
         QRect textBound = painter.boundingRect ( x + 2 * MARGIN + iconWidth, y, w - MARGIN * 2 - iconWidth, h, BaghiraFactory::titleAlign() | AlignVCenter, caption() );
         iconLeft = textBound.x() - MARGIN - iconWidth;
         if ( iconLeft < x + MARGIN ) iconLeft = x + MARGIN;
         
         if (BaghiraFactory::drawIcon(currentStyle))
         {
            ICONX = iconLeft;
            ICONY = y + ( titleheight_ - 16 ) / 2;
            painter.drawPixmap( ICONX, ICONY, icon().pixmap( QIconSet::Small, QIconSet::Normal ) );
         }
         
         if (BaghiraFactory::effect(currentStyle, isActive()) == Baghira::Nostalgia)
         {
            painter.fillRect( iconLeft - 5, y + 1, textBound.right() - iconLeft + 10, h - 2, BaghiraFactory::color(currentStyle, Baghira::ColorTitleBar, isActive() ) );
            painter.fillRect( title.x(), y + 1, x - title.x()+5, h - 2, *ttbar );
            painter.fillRect( x+w-5, y + 1, title.right() - w + 5, h - 2, *ttbar );
         }
      }
      painter.drawText( x + 2 * MARGIN + iconWidth, y, w - MARGIN * 2 - iconWidth, h, BaghiraFactory::titleAlign() | AlignVCenter, caption() );
   }
   else if (BaghiraFactory::drawIcon(currentStyle))
   {
      iconLeft = (width()-16)/2;
      if ( iconLeft < x + MARGIN )
         iconLeft = x + MARGIN;
      ICONX = iconLeft;
      ICONY = y + ( titleheight_ - 16 ) / 2;
      painter.drawPixmap( ICONX, ICONY, icon().pixmap( QIconSet::Small, QIconSet::Normal ) );
   }

	// draw outer frame
	if ( !isShade() )
	{
		QColor background = widget() ->colorGroup().background();

                // left side
		left.rect( &x, &y, &w, &h );
                if (w > 0){
                if (BaghiraFactory::effect(currentStyle,isActive()) == Baghira::Brushed)
                {
                    width() > 1024 ?
                        painter.drawTiledPixmap( x, y, w, h, BaghiraFactory::pix(currentStyle, TitleBar, isActive(), isTool() ) ) :
                        painter.drawTiledPixmap( x, y, w, h, BaghiraFactory::pix(currentStyle, TitleCenter, isActive(), isTool() ), (1024-width())/2 );
                }
                else
                {
                   BaghiraFactory::bgStipple() ?
                   painter.drawTiledPixmap( x, y, w, h, HandlePix ) :
                   painter.fillRect( x, y, w, h, background );
                }
                }

		// right side
		right.rect( &x, &y, &w, &h );
                if (w > 0){
                if (BaghiraFactory::effect(currentStyle,isActive()) == Baghira::Brushed)
                {
                    width() > 1024 ?
                        painter.drawTiledPixmap( x, y, w, h, BaghiraFactory::pix(currentStyle, TitleBar, isActive(), isTool() ) ) :
                        painter.drawTiledPixmap( x, y, w, h, BaghiraFactory::pix(currentStyle, TitleCenter, isActive(), isTool() ), (1024-width())/2+width()-w );
                }
                else
                {
                    BaghiraFactory::bgStipple() ?
                        painter.drawTiledPixmap( x, y, w, h, HandlePix ) :
                        painter.fillRect( x, y, w, h, background );
                }
                }

	// draw handle =====================================================
	// bottom bar
		// handle middle
		handle.rect( &x, &y, &w, &h );
		if (BaghiraFactory::effect(currentStyle,isActive()) == Baghira::Brushed){
			if (w > 1024){
				int w1 = (w - 1024)/2;
				int w2 = w - 1024 - w1;
				painter.drawTiledPixmap( 0, y, w1, h, BaghiraFactory::pix(currentStyle, TitleBar, isActive(), isTool() ) );
				painter.drawPixmap( w1, y, BaghiraFactory::pix(currentStyle, TitleCenter, isActive(), isTool() ) );
				painter.drawTiledPixmap( w1 + 1024, y, w2, h, BaghiraFactory::pix(currentStyle, TitleBar, isActive(), isTool() ) );
			}
			else{
				painter.drawPixmap( x, y, BaghiraFactory::pix(currentStyle, TitleCenter, isActive(), isTool() ), (1024-w)/2, 0, w, h );
                        }
                        
         int b = handle.bottom() - 1;
//                        painter.setPen( ttbar->light(tmp3Dimpact_));
   		painter.setPen( ttbar->dark(tmp3Dimpact_*1.1) );
   		painter.drawLine( x, b, w-1, b );
   		if (maximizeMode() != MaximizeFull)
   		{
      		if (BaghiraFactory::shapeLL(currentStyle)){
               // Draw edge of bottom-left corner inside the area removed by the mask.
         		painter.drawPoint(3, b);
         		painter.drawPoint(4, b);
         		painter.drawPoint(2, b-1);
         		painter.drawPoint(1, b-2);
         		painter.drawPoint(1, b-3);
         		painter.drawPoint(0, b-4);
      		}
      		if (BaghiraFactory::shapeLR(currentStyle)){
               // Draw edge of bottom-right corner inside the area removed by the mask.
         		painter.drawPoint(shapeW - 5, b);
         		painter.drawPoint(shapeW - 4, b);
         		painter.drawPoint(shapeW - 3, b-1);
         		painter.drawPoint(shapeW - 2, b-2);
         		painter.drawPoint(shapeW - 2, b-3);
         		painter.drawPoint(shapeW - 1, b-4);
      		}
   		}
   		painter.setPen( ttbar->dark(tmp3Dimpact_*1.3) );
   		painter.drawLine( x+1, b+1, w-2, b+1 );
   		if (maximizeMode() != MaximizeFull)
   		{
            if (BaghiraFactory::shapeLL(currentStyle)){
                  // Draw edge of bottom-left corner inside the area removed by the mask.
                  painter.drawPoint(4, b);
                  painter.drawPoint(5, b);
                  painter.drawPoint(3, b-1);
                  painter.drawPoint(2, b-2);
//                   painter.drawPoint(2, b-3);
            }
            if (BaghiraFactory::shapeLR(currentStyle)){
                  // Draw edge of bottom-right corner inside the area removed by the mask.
                  painter.drawPoint(shapeW - 6, b);
                  painter.drawPoint(shapeW - 5, b);
                  painter.drawPoint(shapeW - 4, b-1);
                  painter.drawPoint(shapeW - 3, b-2);
//                   painter.drawPoint(shapeW - 3, b-3);
            }
   		}
    		        if (BaghiraFactory::drawComicFrame())
    		        {
                        painter.drawLine(x,title.y(),x, b);
                        painter.drawLine(handle.right(),title.y(),x2, b);
    		        }
		}
		else{
			BaghiraFactory::bgStipple() ? painter.drawTiledPixmap( x, y, w, h, HandlePix ) :
			painter.fillRect( x, y, w, h, background );
                        
                        x2 = handle.right();
                        y2 = handle.bottom();
                        if (BaghiraFactory::drawComicFrame() && maximizeMode() != MaximizeFull){
                            painter.setPen(ttblend->dark(tmp3Dimpact_));
                            painter.drawLine( handle.x()/*+7*/, y2, x2/*-7*/, y2 );
                            painter.drawLine(0,title.bottom(),0, y2);
                            painter.drawLine(x2,title.bottom(),x2, y2);
                    }
		}
                if (BaghiraFactory::drawComicFrame() && maximizeMode() != MaximizeFull){
                int b = handle.bottom() - 1;
                if (BaghiraFactory::shapeLL(currentStyle)){
                // Draw edge of bottom-left corner inside the area removed by the mask.
                    painter.drawPoint(3, b);
                    painter.drawPoint(4, b);
                    painter.drawPoint(2, b-1);
                    painter.drawPoint(1, b-2);
                    painter.drawPoint(1, b-3);
                }
                if (BaghiraFactory::shapeLR(currentStyle)){
                // Draw edge of bottom-right corner inside the area removed by the mask.
                    painter.drawPoint(shapeW - 5, b);
                    painter.drawPoint(shapeW - 4, b);
                    painter.drawPoint(shapeW - 3, b-1);
                    painter.drawPoint(shapeW - 2, b-2);
                    painter.drawPoint(shapeW - 2, b-3);
                }
                }
	}
}

//////////////////////////////////////////////////////////////////////////////
// resizeEvent()
// -------------
// Window is being resized

void BaghiraClient::resizeEvent( QResizeEvent* )
{
   if (noDeco_)
      return;
	/*if ( widget() ->isShown() ) {
		QRegion region = widget() ->rect();
		region = region.subtract( titlebar_->geometry() );
		widget() ->erase( region );
        }*/
//         widget()->repaint();
	doShape();
}

/////////////////////////////////////////////////////////////////////////////
// shape Corners
// =) ==================================================================0

void BaghiraClient::doShape()
{
// 	return;
	QRegion mask;

	if (!(BaghiraFactory::allowEasyClosing() && maximizeMode() == MaximizeFull)){
        int r = width();
        int b = height() - 1;
 	mask = QRegion( 0, 0, r, b+1 );
        
	if (BaghiraFactory::shapeUL(currentStyle)){
	// Remove top-left corner.
	mask -= QRegion( 0, 0, 5, 1 );
	mask -= QRegion( 0, 1, 3, 1 );
	mask -= QRegion( 0, 2, 2, 1 );
	mask -= QRegion( 0, 3, 1, 2 );
	}
	if (BaghiraFactory::shapeUR(currentStyle)){
	// Remove top-right corner.
	mask -= QRegion( r - 5, 0, 5, 1 );
	mask -= QRegion( r - 3, 1, 3, 1 );
	mask -= QRegion( r - 2, 2, 2, 1 );
	mask -= QRegion( r - 1, 3, 1, 2 );
	}

	if (BaghiraFactory::shapeLL(currentStyle)){
	// Remove bottom-left corner.
	mask -= QRegion(0, b-0, 5, 1);
	mask -= QRegion(0, b-1, 3, 1);
	mask -= QRegion(0, b-2, 2, 1);
	mask -= QRegion(0, b-4, 1, 2);
	}
	if (BaghiraFactory::shapeLR(currentStyle)){
	// Remove bottom-right corner.
	mask -= QRegion(r - 5, b-0, 5, 1);
	mask -= QRegion(r - 3, b-1, 3, 1);
	mask -= QRegion(r - 2, b-2, 2, 1);
	mask -= QRegion(r - 1, b-4, 1, 2);
	}
        }
        setMask( mask, 1 );
}


//////////////////////////////////////////////////////////////////////////////
// showEvent()
// -----------
// Window is being shown

void BaghiraClient::showEvent( QShowEvent* ) 
{
   if (noDeco_)
      return;
	doShape();
	widget() ->repaint();
}

//////////////////////////////////////////////////////////////////////////////
// activeChange()
// --------------
// window active state has changed

void BaghiraClient::activeChange()
{
   if (noDeco_)
      return;
   if (BaghiraFactory::fullSpec() && maximizeMode() == MaximizeFull)
   {
      if (isActive())
      {
            BaghiraFactory::deMaximizer().show();
            BaghiraFactory::deMaximizer().setClient(this);
      }
   }
   else
   {
      BaghiraFactory::deMaximizer().hide();
      for ( int n = 0; n < ButtonTypeAmount; n++ )
      {
               if ( button[ n ] )
               {
                        button[n]->setAutoMask( BaghiraFactory::effect(currentStyle, isActive()) == Baghira::Brushed);
                        button[ n ] ->reset();
               }
      }
      widget() ->repaint( false );
   }
}

//////////////////////////////////////////////////////////////////////////////
// captionChange()
// ---------------
// The title has changed

void BaghiraClient::captionChange()
{
   if (noDeco_)
      return;
   if (BaghiraFactory::showTitle())
   {
      if (BaghiraFactory::delAppname())
      {
         caption_ = KDecoration::caption();
         int pos;
         if (caption_.startsWith( "tvtime" ))
         {
         pos = caption_.find(": ");
            if (pos > -1)
               caption_ = caption_.remove(0, pos+1);
         }
         else
         {
            pos = caption_.findRev(" - ");
            if (pos > -1)
               caption_ = caption_.left(pos);
         }
      }
      widget() ->repaint( titlebar_->geometry(), false );
   }
}

//////////////////////////////////////////////////////////////////////////////
// desktopChange()
// ---------------
// Sticky state has changed

void BaghiraClient::desktopChange()
{
   if (noDeco_)
      return;
	bool d = isOnAllDesktops();
	if ( button[ StickyButton ] ) 
	{
		QToolTip::remove
			( button[ StickyButton ] );
		QToolTip::add
			( button[ StickyButton ], d ? i18n( "Un-Sticky" ) : i18n( "Sticky" ) );
	}
}

//////////////////////////////////////////////////////////////////////////////
// iconChange()
// ------------
// The title has changed

void BaghiraClient::iconChange()
{
   if (noDeco_)
      return;
	if (BaghiraFactory::drawIcon(currentStyle)) widget()->repaint(ICONX, ICONY, 16, 16, false);
}

//////////////////////////////////////////////////////////////////////////////
// maximizeChange()
// ----------------
// Maximized state has changed

void BaghiraClient::maximizeChange()
{
   if (noDeco_) //shouldn't happen - windo's not maximizable
      return;
	bool m = ( maximizeMode() == MaximizeFull );
        if (BaghiraFactory::fullSpec() && m){
            if (isActive()){
                BaghiraFactory::deMaximizer().show();
                BaghiraFactory::deMaximizer().setClient(this);
            }
        }
        else {
            BaghiraFactory::deMaximizer().setClient(NULL);
            BaghiraFactory::deMaximizer().hide();
        }
	if ( button[ MaxButton ] ) {
		QToolTip::remove
			( button[ MaxButton ] );
		QToolTip::add
			( button[ MaxButton ], m ? i18n( "Restore" ) : i18n( "Maximize" ) );
	}
}

//////////////////////////////////////////////////////////////////////////////
// shadeChange()
// -------------
// Called when window shading changes

void BaghiraClient::shadeChange() {
	;
}

//////////////////////////////////////////////////////////////////////////////
// borders()
// ----------
// Get the size of the borders

void BaghiraClient::borders( int &l, int &r, int &t, int &b ) const 
{
   if (noDeco_)
   {
      l = r = t = b = 0;
      return;
   }
   bool m = ( maximizeMode() == MaximizeFull);
   t = (BaghiraFactory::fullSpec() && m) ? 0 : titleheight_;
   if (m || maximizeMode() == MaximizeHorizontal)
      l = r = BaghiraFactory::maxResizable() ? 1 : 0;
   else
      l = r = BaghiraFactory::borderSize(currentStyle);
   if ( m || isShade() || (maximizeMode() == MaximizeVertical))
      b = BaghiraFactory::maxResizable() ? 1 : 0;
   else
      b = BaghiraFactory::borderSize(currentStyle);
}

//////////////////////////////////////////////////////////////////////////////
// resize()
// --------
// Called to resize the window

void BaghiraClient::resize( const QSize &size )
{
   if (noDeco_)
      return;
	widget() ->resize( size );
}

//////////////////////////////////////////////////////////////////////////////
// minimumSize()
// -------------
// Return the minimum allowable size for this decoration

QSize BaghiraClient::minimumSize() const
{
	return QSize( 100, 60 );
}

//////////////////////////////////////////////////////////////////////////////
// mousePosition()
// ---------------
// Return mouse position (for resizing)

KDecoration::Position BaghiraClient::mousePosition( const QPoint &point ) const 
{
   if (noDeco_)
      return PositionCenter;
   Position pos = PositionCenter;
   if ( point.y() < ( height() - BaghiraFactory::borderSize(currentStyle) ) ) { // inside client
		pos = KDecoration::mousePosition( point );
	} else {
		if ( point.x() >= ( width() - 16 ) )
			pos = PositionBottomRight;
		else if ( point.x() <= 16 )
			pos = PositionBottomLeft;
		else
			pos = PositionBottom;
	}
	return pos;
}


//////////////////////////////////////////////////////////////////////////////
// maxButtonPressed()
// -----------------
// Max button was pressed


void BaghiraClient::demaxButtonPressed() {
    maxButtonPressed();
}

void BaghiraClient::maxButtonPressed() {
	if ( !BaghiraFactory::fullSpec() || (maximizeMode() != MaximizeFull) && button[ MaxButton ]) {
		switch ( button[ MaxButton ] ->lastMousePress() ) {
		case MidButton:
			maximize( maximizeMode() ^ MaximizeVertical );
			break;
		case RightButton:
			maximize( maximizeMode() ^ MaximizeHorizontal );
			break;
		default:
			if ( maximizeMode() == MaximizeFull ) {
				maximize( MaximizeRestore );
			}
			else {
				maximize( MaximizeFull );
			}
		}
	}
        else{
            maximize( MaximizeRestore );
        }
}

//////////////////////////////////////////////////////////////////////////////
// menuButtonPressed()
// -------------------
// Menu button was pressed

void BaghiraClient::menuButtonPressed() {
	if ( button[ MenuButton ] ) {
		QPoint pt( button[ MenuButton ] ->rect().bottomLeft().x(),
		           button[ MenuButton ] ->rect().bottomLeft().y() + 4 );
		KDecorationFactory* f = factory();
		showWindowMenu( button[ MenuButton ] ->mapToGlobal( pt ) );
		if ( !f->exists( this ) )
			return ; // we have been destroyed
		button[ MenuButton ] ->setDown( false );
	}
}

//////////////////////////////////////////////////////////////////////////////
// new stuff

void BaghiraClient::aboveBelowButtonPressed()
{
   if (keepAbove())
   {
      button[ AboveBelowButton ] ->setDown(true);
      setKeepAbove(false );
      setKeepBelow(true );
      button[ AboveBelowButton ] ->repaint( false );
   }
   else if (keepBelow())
   {
      button[ AboveBelowButton ] ->setDown(false);
      setKeepBelow(false );
      setKeepAbove(false );
      button[ AboveBelowButton ] ->repaint( false );
   }
   else
   {
      button[ AboveBelowButton ] ->setDown(true);
      setKeepBelow(false );
      setKeepAbove(true );
      button[ AboveBelowButton ] ->repaint( false );
   }
}

void BaghiraClient::shadeButtonPressed(){
    button[ ShadeButton ]->setDown(!isShade());
    setShade( !isShade() );
}

//////////////////////////////////////////////////////////////////////////////
// isTool()
// --------
// Is this a tool window?

bool BaghiraClient::isTool() const {
	static const unsigned long winmask = NET::DesktopMask | NET::DialogMask |
	                                     NET::DockMask | NET::MenuMask |
	                                     NET::NormalMask | NET::OverrideMask |
	                                     NET::SplashMask | NET::ToolbarMask |
	                                     NET::TopMenuMask | NET::UtilityMask;

	NET::WindowType type = windowType( winmask );
	return ( type == NET::Menu || type == NET::Toolbar || type == NET::Utility );
}

#include "baghiraclient.moc"
