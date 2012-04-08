#include "baghira.h"
#include <qapplication.h>
#include <qbrush.h>
#include <qcolor.h>
#include <qevent.h> 
#include <qimage.h>
#include <qobject.h>
#include <qpainter.h>
#include <qpalette.h>
#include <qpopupmenu.h>
#include <qrect.h>
#include <qsettings.h>
#include <qsize.h>
#include <qstring.h>
// #include <kconfig.h>
#include <kpixmapeffect.h>
#include <stdio.h>
#include <cmath>

#define _SCALE_    65536
#define _NORMALIZE_ >>16

#define CLAMP(x,l,u) x < l ? l :\
x > u ? u :\
x

OptionHandler::OptionHandler( QObject *parent ) : QObject( parent ) {
	scrollerActive = false;
	groupShadowButton = false;
	reloadSettings();
	}

	
const QColor& OptionHandler::textColor() {
	return ( useCustomMenuColor ? fgColor : qApp->palette().active().text() );
	}

const QColor& OptionHandler::bgColor() {
   return qApp->palette().active().background();
	}

const QColor& OptionHandler::buttonColor() {
	LiquidStyle * style = ( LiquidStyle* ) parent();
	/*if (style->isKicker) return style->origPanelBrush.color();
        else */if (style->isOOO) return CustomButtonColor();
        else return qApp->palette().active().button();
	}

void OptionHandler::reloadSettings() {
	// gather options...
        QSettings config;
        config.beginGroup("/baghira/Style");
	//we need that first to make sure we use some proper settings ;)
	customButtonColor = QColor(config.readNumEntry("Design_ButtonColor",(int)qApp->palette().active().button().rgb()));
        // design handling:
        // first try file:
        QString tmpString;
        FILE *file = NULL;
        wmDesign = 5;
         for (int i = 0; i < 8; i++)
            custCols[i] = -1;
        if (qstrcmp( qApp->argv() [ 0 ], "ksplash" ) == 0)
        {
            style_ = Panther;
            _toolbuttonStyle = Panther;
            _buttonStyle = Panther;
            tabStyle_ = Clever;
            bgStipple = false;
            inactiveButtonColor = Background;
        }
        else
        {
        // first try for a tmp file from bab starter
         int tmpFile = 0;
         tmpString = QDir::homeDirPath() + "/.baghira/.bab/" + qApp->argv() [ 0 ];
         file = fopen(tmpString.latin1(), "r");
         if( file == NULL )
         {
            tmpFile = 1;
            tmpString = QDir::homeDirPath() + "/.baghira/" + qApp->argv() [ 0 ];
            file = fopen(tmpString.latin1(), "r");
            if( file == NULL )
            {
               tmpFile = 2;
               tmpString = QDir::homeDirPath() + "/.baghira/.bab/.style";
               file = fopen(tmpString.latin1(), "r");
            }
         }
         if (file != NULL)
         {
            style_ = _buttonStyle = _toolbuttonStyle = Panther; int i3 = inactiveButtonColor = -1; tabStyle_ = (tabStyle)-1;
            fscanf(file,"%u\n%u\n%u\n%u\n%u\n%u\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",&style_, &wmDesign, &_buttonStyle, &_toolbuttonStyle, &tabStyle_, &i3, &custCols[0], &custCols[1], &custCols[2], &custCols[3], &custCols[4], &custCols[5], &custCols[6], &custCols[7], &inactiveButtonColor);
            fclose(file);
            if (_toolbuttonStyle < 0 || _toolbuttonStyle >= NUMSTYLES)
               _toolbuttonStyle = (Style)config.readNumEntry( "Special_ToolbuttonStyle", style_);
            if (_buttonStyle < 0 || _buttonStyle >= NUMSTYLES)
               _buttonStyle = (Style)config.readNumEntry( "Special_ButtonStyle", style_);
            if (tabStyle_ < 0 || tabStyle_ > 2)
               tabStyle_ = (tabStyle) config.readNumEntry( (style_ == Brushed)?"Design_TabStyleBrushed":"Design_TabStyleAqua", Clever);
            if (inactiveButtonColor < 0 || inactiveButtonColor > 3)
               inactiveButtonColor = config.readNumEntry( "Design_InactiveButtonStyle", Background);
            if (style_ == Brushed)
               bgStipple = TRUE;
            else if (i3 == 0)
               bgStipple = FALSE;
            else
               bgStipple = config.readBoolEntry( "Design_StippleBackground", true );
            	// inactive Button
            if (tmpFile == 0)
               remove(tmpString.latin1()); // remove TEMPORARY file
            else if (tmpFile == 2)
            {
               tmpString = QDir::homeDirPath() + "/.baghira/.bab/.deco";
               if ((file = fopen(tmpString.latin1(), "r")) != NULL)
               {
                  fscanf(file,"%u\n%u\n",&i3, &wmDesign);
                  fclose(file);
               }
            }
         }
         else
         {
            style_ = (Style)config.readNumEntry( "Design_Default", Panther );
            _toolbuttonStyle = (Style)config.readNumEntry( "Special_ToolbuttonStyle", style_);
            _buttonStyle = (Style)config.readNumEntry( "Special_ButtonStyle", style_);
            tabStyle_ = (tabStyle) config.readNumEntry( (style_ == Brushed)?"Design_TabStyleBrushed":"Design_TabStyleAqua", Clever);
            bgStipple = (style_ == Brushed) ? true : config.readBoolEntry( "Design_StippleBackground", true );
            inactiveButtonColor = config.readNumEntry( "Design_InactiveButtonStyle", Background);
         }
        }
        contrast = 0;
        if (wmDesign > 4)
           wmDesign = style_;
        if (style_ == Jaguar)
            contrast = 4;
        else if (style_ == Brushed)
            {
            tintBrush = config.readBoolEntry( "Colors_TintBrushedMetal", false );
            if (tintBrush)
                brushTint.setRgb( config.readNumEntry( "Colors_BrushTint", ( int ) bgColor().rgb()));
            }
	// menu stuff
   glossyMenus_ = config.readBoolEntry( "Menu_Glossy", true );
        menuBackground = config.readNumEntry( "Menu_Background", Standard);
	menuOpacity = config.readNumEntry( "Menu_Opacity", 70);
        int menuColorStyle = config.readNumEntry( "Menu_ColorStyle", 0);
        menuColorButton = (menuColorStyle == 1);
        useCustomMenuColor = (menuColorStyle == 2);
	shadowText = config.readBoolEntry( "Menu_ShadowText", false);
	if (useCustomMenuColor){
		color = QColor( config.readNumEntry( "Menu_Color1", 0 ) );
                color2 = QColor( config.readNumEntry( "Menu_Color2", 0 ) );
                colorHigh = QColor( config.readNumEntry( "Menu_ColorHighlight", 0 ) );
		fgColor = QColor( config.readNumEntry( "Menu_TextColor", 0 ) );
                fgColorHigh = QColor( config.readNumEntry( "Menu_TextColorHighlight", 0 ) );
	}
        else if (menuColorButton){
            color = customButtonColor;
            color2 = customButtonColor.dark(130);
        }
        else {
            color = qApp->palette().active().background();
            color2 = qApp->palette().active().background().dark(130);
        }
        drawMenuStripe_ = config.readBoolEntry("Menu_DrawMenuStripe", false);
        if (drawMenuStripe_)
            menuStripeColor_ = QColor(config.readNumEntry("Menu_StripeColor"),(int)Qt::white.rgb());
	// color stuff
	// widgets
	customWidgetColor  =  config.readBoolEntry( "Colors_UseCustomColors", false);
	if ( customWidgetColor ) {
		customColors[ CustomRadioOn ].setRgb( config.readNumEntry( "Colors_RadioOn", ( int ) buttonColor().rgb() ) );
		customColors[ CustomRadioOff ].setRgb( config.readNumEntry( "Colors_RadioOff", ( int ) bgColor().rgb() ) );
		customColors[ CustomCBOn ].setRgb( config.readNumEntry( "Colors_CheckOn", ( int ) buttonColor().rgb() ) );
		customColors[ CustomCBOff ].setRgb( config.readNumEntry( "Colors_CheckOff", ( int ) bgColor().rgb() ) );
		customColors[ CustomTabOn ].setRgb( config.readNumEntry( "Colors_TabActive", ( int ) buttonColor().rgb() ) );
		customColors[ CustomTabOff ].setRgb( config.readNumEntry( "Colors_TabInactive", ( int ) bgColor().rgb() ) );
		customColors[ CustomSBSlider ].setRgb( config.readNumEntry( "Colors_Slider", ( int ) bgColor().rgb() ) );
		customColors[ CustomSBSliderHover ].setRgb( config.readNumEntry( "Colors_SliderHovered", ( int ) buttonColor().rgb() ) );
		customColors[ CustomSBSliderPressed ].setRgb( config.readNumEntry( "Colors_SliderPressed", ( int ) buttonColor().dark(110).rgb() ) );
		customColors[ CustomSBGroove ].setRgb( config.readNumEntry( "Colors_SliderGroove", ( int ) bgColor().rgb() ) );
	}
   if (inactiveButtonColor == Custom)
      customInactiveButtonColor = QColor( config.readNumEntry( "Design_InactiveButtonColor", (int) bgColor().rgb()));
	contrast += config.readNumEntry( "Design_StippleContrast", 3);
        bevelHighlights_ = config.readBoolEntry( "Design_BevelAsHighlight", true);
	//shadows
groupboxshadow = (style_ == Brushed) ? false : config.readBoolEntry( "Design_ShadowGroupBoxes", true );
	shadowDarkness = config.readNumEntry( "Design_GroupBoxeShadowDarkness", 6);
	//ListViews
	expanderStyle = config.readNumEntry( "Special_ExpanderStyle", Apple);
	useCustomExpanderColor = config.readBoolEntry( "Special_CustomExpanderColor", false);
	if (useCustomExpanderColor)
		expanderColor = QColor( config.readNumEntry( "Special_ExpanderColor", (int) qApp->palette().active().text().rgb()));
	drawDotlines = config.readBoolEntry( "Special_DrawTreeLines", true);
	if (drawDotlines){
		dotlineStyle = config.readNumEntry( "Special_TreelineStyle", Line);
		dotlineColor = QColor( config.readNumEntry( "Special_TreelineColor", (int) qApp->palette().active().mid().rgb()));
	}
	//slider
	squeezesbslider = config.readBoolEntry( "Special_SqueezeSlider", false );
	shadowsbslider = config.readBoolEntry( "Special_ShadowSlider", false );
	animateSlider = config.readBoolEntry( "Special_AnimateSlider", true );
	// toolbar
        int utb = config.readNumEntry( "Special_UnhoveredToolButtons", 2 );
   tbFrame = (utb == 2) || ((style_ == Brushed) && (utb == 1)) || ((style_ != Brushed) && (utb == 0));

	// tweaks
	centerTabs = config.readBoolEntry( "Design_CenterTabs", true);
	smoothListViewHeaders = (style_ == Milk) ? true : !config.readBoolEntry( "Special_UseFlatLVH", false);
	smootherLVH = config.readBoolEntry( "Special_RoundTaskbuttons", false);
	icyButtons = config.readNumEntry( "Design_ButtonStyle", 0) == 0;
	progressBar = config.readNumEntry( "Special_ProgressStyle", baghira);
	removeKickerBevel = config.readBoolEntry( "Special_RemoveKickerBevel", false);
         animateButton = config.readBoolEntry( "Design_AnimateButtons", true);
      animateProgress = config.readBoolEntry( "Design_AnimateProgress", true);
	drawProgressLabel = config.readBoolEntry( "Special_ShowProgressValue", false);
   config.endGroup();
   
   config.beginGroup("/baghira/Deco");
   titleButtonColor_[0] = QColor((unsigned int)config.readNumEntry( "CloseButtonColor", QColor(200,85,70).rgb()));
   titleButtonColor_[1] = QColor((unsigned int)config.readNumEntry( "MinButtonColor", QColor(230,155,40).rgb()));
   titleButtonColor_[2] = QColor((unsigned int)config.readNumEntry( "MaxButtonColor", QColor(121,180,54).rgb()));
   if (style_ == Jaguar)
   {
      titleColor_[0] = QColor((unsigned int)config.readNumEntry( "inactiveColor1_1", QColor(204,214,230).rgb()));
      titleColor_[1] = QColor((unsigned int)config.readNumEntry( "inactiveColor2_1", QColor(194,196,211).rgb()));
   }
   else if (style_ != Brushed)
   {
      titleColor_[0] = QColor((unsigned int)config.readNumEntry( "activeColor1_2", QColor(238,238,238).rgb()));
      titleColor_[1] = QColor((unsigned int)config.readNumEntry( "activeColor2_2", QColor(205,202,205).rgb()));
   }
   if (style_ == Tiger)
   {
      int r,g,b;
      aDecoColor1_ = QColor((unsigned int)config.readNumEntry( "activeColor2_4", (unsigned int) QColor(205,202,205).rgb()));
      aDecoColor2_ = QColor((unsigned int)config.readNumEntry( "activeColor1_4", (unsigned int) QColor(238,238,238).rgb()));
      r = (int)CLAMP(aDecoColor1_.red() * pow((double)aDecoColor1_.red() / (double)aDecoColor2_.red(),2.0),0,255);
      g = (int)CLAMP(aDecoColor1_.green() * pow((double)aDecoColor1_.green() / (double)aDecoColor2_.green(),2.0),0,255);
      b = (int)CLAMP(aDecoColor1_.blue() * pow((double)aDecoColor1_.blue() / (double)aDecoColor2_.blue(),2.0),0,255);
      aDecoColor2_.setRgb(r,g,b);
   }
   else
   {
      aDecoColor1_ = Qt::black;
      aDecoColor2_ = Qt::black;
   }
   config.endGroup();
	// Option gathered
	}
