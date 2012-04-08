#include "baghira.h"

#include <qbitmap.h>
#include <qimage.h>
#include <qcolor.h>
#include <qpalette.h>
#include <qrect.h>
#include <qpoint.h>
#include <qapplication.h>
#include <qpainter.h>
#include <qpen.h>
//#include <kwordwrap.h>


#include "config.h"

#ifndef CLAMP
#define CLAMP(x,l,u) x < l ? l :\
   x > u ? u :\
   x
#endif

#define ALPHA_COLOR(A,R,G,B) \
   destR = ((A * destR) + (255 - A) * R) / 255; \
   destG = ((A * destG) + (255 - A) * G) / 255; \
   destB = ((A * destB) + (255 - A) * B) / 255; \
   A = 255;

#define ALPHA_COLOR2(A1,A2,R,G,B) \
   destR = ((A1 * destR) + (255 - A1) * R) / 255; \
   destG = ((A1 * destG) + (255 - A1) * G) / 255; \
   destB = ((A1 * destB) + (255 - A1) * B) / 255; \
   A1 = CLAMP(A1 + A2,0,255);
    
#define DELTA_COLOR(D,R,G,B) \
   delta = (255 - qRed(D))/2; \
   destR = R - delta; \
   destG = G - delta; \
   destB = B - delta;
    
#define SATURATION_COLOR(R,G,B) \
   grey = (299 * R + 587 * G + 114 * B) / 1000; \
   delta = 255 - grey; \
   grey = (grey *(10 - 5)) / 10; \
   iGrey = 255 - grey;\
   destR = (iGrey * (srcR - delta) + grey * R) / 255; \
   destG = (iGrey * (srcG - delta) + grey * G) / 255; \
   destB = (iGrey * (srcB - delta) + grey * B) / 255;
    
#define SATURATION_COLOR2(S,R,G,B) \
   int max = 255+(int)(0.65*(100-S)); \
   destR = CLAMP((srcR + R - 128), 0, max); \
   destG = CLAMP((srcG + G - 128), 0, max); \
   destB = CLAMP((srcB + B - 128), 0, max); \
   destR = (S*destR + (100-S)*R)/100; \
   destG = (S*destG + (100-S)*G)/100; \
   destB = (S*destB + (100-S)*B)/100;
   
#define TINT_WHITE(S,R,G,B) \
   int max = 255+(int)(0.65*(100-S)); \
   destR = CLAMP((128 + R), 0, max); \
   destG = CLAMP((128 + G), 0, max); \
   destB = CLAMP((128 + B), 0, max); \
   destR = (S*destR + (100-S)*R)/100; \
   destG = (S*destG + (100-S)*G)/100; \
   destB = (S*destB + (100-S)*B)/100;

   
#define SATURATION_COLOR3(S,R,G,B) \
   destR = (S*(srcR + R - 128)/100 + R)/2; \
   destG = (S*(srcG + G - 128)/100 + G)/2; \
   destB = (S*(srcB + B - 128)/100 + B)/2; 
    
#define GM_COLOR(R,G,B)\
    destR = CLAMP(destR + R - 128, 0, 255);\
    destG = CLAMP(destG + G - 128, 0, 255);\
    destB = CLAMP(destB + B - 128, 0, 255);

    
#define COLOR_SPACE(R,G,B) \
    if ( R < 0 ) R = 0; else if ( R > 255 ) R = 255; \
    if ( G < 0 ) G = 0; else if ( G > 255 ) G = 255; \
    if ( B < 0 ) B = 0; else if ( B > 255 ) B = 255;
    
#ifndef PRINTDEVICE
#define PRINTDEVICE(_p_) qWarning("device is %s", (_p_->device()->devType() == QInternal::Widget) ?\
"Widget": (_p_->device()->devType() == QInternal::Pixmap) ?\
"Pixmap": (_p_->device()->devType() == QInternal::Printer) ?\
"Printer": (_p_->device()->devType() == QInternal::Picture) ?\
"Picture": (_p_->device()->devType() == QInternal::UndefinedDevice) ?\
"UndefinedDevice": "fuckdevice!" );
#endif

int LiquidStyle::getBrightness(unsigned int rgb) const {
   int red = qRed( rgb );
   int green = qGreen( rgb );
   int blue = qBlue( rgb );
	int V = red;
	if (green > V)  V = green;
	if (blue > V)  V = blue;
	return V;
}

QPixmap* LiquidStyle::adjustHSV( QImage &img, const QColor &c, const QColor *bg ) const 
{
    QImage * tmp = adjustHSVImage( img, c, bg );

    QPixmap *pix = new QPixmap;
    pix->convertFromImage( *tmp );
    delete tmp;
    return ( pix );
}


ButtonTile* LiquidStyle::createRoundFrameTile(QImage &img, const QColor &color, const QColor *bg, const QColor &baseColor ) const
{
   QColor bgColor( bg ? *bg : qApp->palette().active().background() );

   if ( img.depth() != 32 )
      img = img.convertDepth( 32 );
   QImage *dest = new QImage( img.width(), img.height(), 32, 0 );
   dest->setAlphaBuffer( true );
   unsigned int *data = ( unsigned int * ) img.bits();
   unsigned int *destData = ( unsigned int* ) dest->bits();
   int total = img.width() * img.height();
   int current;
   int delta;
   int red, green, blue, grey, iGrey;
   int destR, destG, destB, alpha;
   int x,y;

   int srcR = color.red();
   int srcG = color.green();
   int srcB = color.blue();

   if (!optionHandler->IcyButtons())
   {
      srcR += 20;
      srcG += 20;
      srcB += 20;
      COLOR_SPACE(srcR, srcG, srcB);
   }

   for ( current = 0 ; current < total ; ++current )
   {
      alpha = qAlpha( data[ current ] );
      if (alpha)
      {
         if (optionHandler->IcyButtons())
         {
            red = qRed( data[ current ] );
            green = qGreen( data[ current ] );
            blue = qBlue( data[ current ] );
            SATURATION_COLOR(red, green, blue);
         }
         else
         {
            // the coloring itself
            DELTA_COLOR(data[ current ], srcR, srcG, srcB);
         }
      }
#if 1
      if ( alpha && alpha != 255 )
      {
      // only on translucent pixels - alphablending
         y = (int)(current/img.width());
         x = current - y*img.width();
         if (red < 180 || x < 2 || y < 2 || y > img.height() - 2 || x > img.width() - 3)
         {
            ALPHA_COLOR(alpha, bgColor.red(), bgColor.green(), bgColor.blue());
         }
         else
         {
            ALPHA_COLOR(alpha, baseColor.red(), baseColor.green(), baseColor.blue());
         }
      }
#endif
      // force back to valid colorspace !
      COLOR_SPACE(destR, destG, destB);
      destData[ current ] = qRgba( destR, destG, destB, alpha );
   }
   QPixmap destPix;
   destPix = *dest;
   ButtonTile *ret = separateTiles( &destPix, 11, 10, 6, 1, 0, false);
   delete dest;
   return ret;
}

QImage* LiquidStyle::adjustHSVImage( QImage &img, const QColor &c, const QColor *bg ) const
{
   QColor bgColor( bg ? *bg : qApp->palette().active().background() );

   if ( img.depth() != 32 )
   img = img.convertDepth( 32 );
   QImage *dest = new QImage( img.width(), img.height(), 32, 0 );
   dest->setAlphaBuffer( true );
   unsigned int *data = ( unsigned int * ) img.bits();
   unsigned int *destData = ( unsigned int* ) dest->bits();
   int total = img.width() * img.height();
   int current;
   int delta;
   int red, green, blue;
   int destR, destG, destB, alpha;

   int srcR = c.red();
   int srcG = c.green();
   int srcB = c.blue();
   
   int hue, s, v;
   c.getHsv( &hue, &s, &v );
   int sq = CLAMP((int)((45.0/128.0)*s+55),0,100);


   if (!optionHandler->IcyButtons())
   {
      srcR += 20;
      srcG += 20;
      srcB += 20;
      COLOR_SPACE(srcR, srcG, srcB);
   }

   // float srcPercent, destPercent;
   for ( current = 0 ; current < total ; ++current )
   {
      alpha = qAlpha( data[ current ] );
      if (optionHandler->IcyButtons())
      {
         red = qRed( data[ current ] );
         green = qGreen( data[ current ] );
         blue = qBlue( data[ current ] );
         SATURATION_COLOR2(sq, red, green, blue);
      }
      else
      {
         // the coloring itself
         DELTA_COLOR(data[ current ], srcR, srcG, srcB);
      }
      // force back to valid colorspace !
      COLOR_SPACE(destR, destG, destB);
      destData[ current ] = qRgba( destR, destG, destB, alpha );
   }
   return ( dest );
}

QImage* LiquidStyle::tintBrush( const QImage &img, const QColor &c ) const
{
//     if ( img.depth() != 32 )
//     	img = img.convertDepth( 32 );
   QImage *dest = new QImage( img.width(), img.height(), 32, 0 );
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
   
   // float srcPercent, destPercent;
   for ( current = 0 ; current < total ; ++current )
   {
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

QColor LiquidStyle::brushedMetalColor(QColor &c) const
{
   if (optionHandler->TintBrush())
   {
      int srcR = c.red();
      int srcG = c.green();
      int srcB = c.blue();
      int destR, destG, destB, grey, iGrey, delta;
      SATURATION_COLOR(180, 180, 180);
      COLOR_SPACE(destR, destG, destB);
      return QColor(destR, destG, destB);
   }
   else
      return QColor (180,180,180);
}




void LiquidStyle::clearImage( QImage &img ) const
{
    int x, y;
    int w = img.width();
    int h = img.height();
    unsigned int pixel = qRgba( 0, 0, 0, 0 );
    unsigned int *data;

    for ( y = 0 ; y < h ; ++y ) 
    	for ( data = (unsigned int *) img.scanLine( y ), x = 0 ; x < w ; data[ x++ ] = pixel ) ;
    // img.fill( pixel );
}

void LiquidStyle::adjustHSV( QPixmap &pix, const QColor &c ) const
{
    QImage img = pix.convertToImage();
    QPixmap *result = adjustHSV( img, c);
    pix = *result;
    delete result;
}


/* buttons are fractioned to 9 parts.
w, h are width and height of the whole button.
xOff  (yOff) is the distance of the central fraction from left (upper) button side.
centerW (centerH) is the width (height) of the central button fraction.
shadowH is the height of the buttons shadow*/
ButtonTile* LiquidStyle::createButtonTile( const QColor &c, 
                                          const QColor &bgColor, 
                                          QImage *buttonImage, 
                                          QImage *shadowImage, 
                                          QImage *glowImage, 
                                          QIntDict<ButtonTile>*buttonDict, 
                                          QIntDict<ButtonTile>*shadowDict, 
                                          QIntDict<ButtonTile>*glowDict, 
                                          int w, int h, int xOff, int yOff, 
                                          int centerW, int centerH, 
                                          int shadowH, int glowWH, 
                                          bool sunken) const
{
   int x, y, delta;
   int red, green, blue;
   int destR, destG, destB, alpha;
   bool rect = (buttonDict == &rectBtnDict);
   
   int hue, s, v;
   c.getHsv( &hue, &s, &v );
   int sq = CLAMP((int)((45.0/128.0)*s+55),0,100);
   
   int srcR = c.red();
   int srcG = c.green();
   int srcB = c.blue();

   if (!optionHandler->IcyButtons())
   {
      srcR += 20;
      srcG += 20;
      srcB += 20;
   
      COLOR_SPACE(srcR, srcG, srcB);
   }
   
   unsigned int *data, *destData;
   ButtonTile *tile;

   int shadowOverhead;
   shadowH > glowWH ? shadowOverhead = shadowH - glowWH : shadowOverhead = 0;
   QImage img( w + 2*glowWH, h + 2*glowWH + shadowOverhead, 32, 0 );
   img.setAlphaBuffer( !isPlain() || optionHandler->drawGroupBoxShadow() ? true : false);
   clearImage( img );

   // For unpressed buttons things are a little more complex. First
   // we color adjust the shadow with the background color then we
   // adjust the button to the button color. Finally we overlay the
   // button on the shadow, (in the same loop as color adjusting).
   // Technically we probably could of gotten away
   // with just adjusting both the shadow and the button to the button
   // color since the shadow is quite dark, but this is more correct and
   // would allow configurable shadow brightness. If the shadow was
   // just the button color, if someone configured it to be a rather
   // light brightness, and they had a dark background color it wouldn't
   // look right if based off the button color.
   //
   // Luckily this is only done once when the button is created for any
   // given color ;-)

   // first do the shadow
   if ((!sunken || optionHandler->buttonStyle() == Milk) && shadowImage)
   for ( y = 0 ; y < h + shadowH ; ++y )
   {
      data = ( unsigned int * ) shadowImage->scanLine( y );
      destData = ( unsigned int * ) img.scanLine( y + glowWH ); // glowWH is the yOffset -> distance of visable button to top of complete button
      if (optionHandler->buttonStyle() == Jaguar) // has brighter shadows...
      {
         for ( x = 0 ; x < w ; ++x )
            if ((alpha = qAlpha( data[ x ] )))
            {
               destData[ x + glowWH ] = qRgba( qRed(data[ x ]), qGreen(data[ x ]), qBlue(data[ x ]), alpha >> 1 );
            }
      }
      else
      {
         for ( x = 0 ; x < w ; ++x )
            if ((alpha = qAlpha( data[ x ] )))
            {
               destData[ x + glowWH ] = data[x];
            }
      }
   }
    // ... or the glow
   else if (glowImage)
      for ( y = 0 ; y < h + 2*glowWH ; ++y )
      {
         data = ( unsigned int * ) glowImage->scanLine( y );
         destData = ( unsigned int * ) img.scanLine( y );
         for ( x = 0 ; x < w + 2*glowWH ; ++x )
            if ((alpha = qAlpha( data[ x ] )))
            {
               DELTA_COLOR(data[ x ], srcR, srcG, srcB);
               COLOR_SPACE(destR, destG, destB);
               destData[ x ] = qRgba( destR, destG, destB, alpha );
            }
      }

    // then the button and overlay
   for ( y = 0 ; y < h ; ++y )
   {
      data = ( unsigned int * ) buttonImage->scanLine( y );
      destData = ( unsigned int * ) img.scanLine( y + glowWH );
      for ( x = 0 ; x < w ; ++x )
         if ((alpha = qAlpha( data[ x ] )))
         {
            if (!rect || (y > 1 && y < h-2 && x > 0 && x < w-1) )
            {
               if (optionHandler->IcyButtons())
               {
                  red = qRed( data[ x ] );
                  green = qGreen( data[ x ] );
                  blue = qBlue( data[ x ] );
                  SATURATION_COLOR2(sq, red, green, blue);
               }
               else
               {
                  DELTA_COLOR(data[ x ], srcR, srcG, srcB);
               }
            }
            else
            {
               red = qRed( data[ x ] );
               green = qGreen( data[ x ] );
               blue = qBlue( data[ x ] );
               TINT_WHITE(55, red, green, blue);
            }
            if (alpha != 255)
            {
               if ( qAlpha( destData[ x + glowWH ] ) )
               {
                  ALPHA_COLOR2(alpha, qAlpha( destData[ x + glowWH ] ), qRed( destData[ x + glowWH ]), qGreen( destData[ x + glowWH ]), qBlue( destData[ x + glowWH ]));
               }
            }
            COLOR_SPACE(destR, destG, destB);
            destData[ x + glowWH ] = qRgba( destR, destG, destB, alpha );
         }
   }

   QPixmap *pix = new QPixmap;
   pix->convertFromImage( img );
   tile = separateTiles( pix, xOff+glowWH, yOff+glowWH, centerW, centerH, shadowH, sunken);
   
   if ( sunken && glowDict)
      glowDict->insert(c.rgb(),tile);
   else if (!sunken && shadowDict)
      shadowDict->insert( c.rgb(), tile );
   else
      buttonDict->insert( c.rgb(), tile );
   
   delete pix;
   return ( tile );
}

QPixmap* LiquidStyle::createSliderEnd( const QColor &c, const QColor &bgColor, bool top) const
{

    int x, y, delta;
    int red, green, blue;
    int destR, destG, destB, alpha;
    
    int h, s, v;
    c.getHsv( &h, &s, &v );
    int sq = CLAMP((int)((45.0/128.0)*s+55),0,100);
    
    int srcR = bgColor.red();
    int srcG = bgColor.green();
    int srcB = bgColor.blue();

    unsigned int *data, *destData;

    QImage img( 13, 9, 32, 0  );
    img.setAlphaBuffer( true ); // we need this always, as the slider groove is nevernever plain!
    clearImage( img );

    // just stole that from above...
    // first do the shadow
    for ( y = 0 ; y < 9 ; ++y ) {
	top ? data = ( unsigned int * ) slider_top_shd->scanLine( y ) : data = ( unsigned int * ) slider_btm_shd->scanLine( y );
	destData = ( unsigned int * ) img.scanLine( y );
	for ( x = 0; x < 13; ++x )
            if ((alpha = qAlpha( data[ x ] ))) {
      DELTA_COLOR(data[ x ], bgColor.red(), bgColor.green(), bgColor.blue());
		COLOR_SPACE(destR, destG, destB);
		destData[ x ] = qRgba( destR, destG, destB, alpha );
	    }
    }
    // then the button and overlay
    
    srcR = c.red();
    srcG = c.green();
    srcB = c.blue();

    if (!optionHandler->IcyButtons()){
    srcR +=20;
    srcG += 20;
    srcB += 20;

    COLOR_SPACE(srcR, srcG, srcB);
    }
    
    for ( y = 0 ; y < 7 ; ++y ) {
	top ? data = ( unsigned int * ) slider_top->scanLine( y ) : data = ( unsigned int * ) slider_btm->scanLine( y );
	destData = ( unsigned int * ) img.scanLine( top ? y + 2 : y );
	for ( x = 0 ; x < 13 ; ++x )
            if ((alpha = qAlpha( data[ x ] ))) {
		if (optionHandler->IcyButtons()) {
	    	    red = qRed( data[ x ] );
	    	    green = qGreen( data[ x] );
                    blue = qBlue( data[ x] );
		    SATURATION_COLOR2(sq, red, green, blue);
		}
		else {
	    	    DELTA_COLOR(data[ x ], srcR, srcG, srcB);
		}
		if (alpha != 255) {
         if ( qAlpha( destData[ x ] ) ) {
            ALPHA_COLOR2(alpha, qAlpha( destData[ x ]), qRed( destData[ x ]), qGreen( destData[ x ]), qBlue( destData[ x ]));
      }
		}
		COLOR_SPACE(destR, destG, destB);
		destData[ x ] = qRgba( destR, destG, destB, alpha );
	    }
    }
    QPixmap *pix = new QPixmap;
    pix->convertFromImage( img );
    return ( pix );
}



//   0     /  xO  /  xO + w
// -------------------------------------
// | xO   | w   | width()-xO-w | : yO 			/   0
// -------------------------------------
// | xO   | w   | width()-xO-w | : h 				/   yO
// -------------------------------------
// | xO   | w   | width()-xO-w | : height()-yO-h  	/  yO+h
// -------------------------------------
// Overload of mosfet's original function, takes 6 more parameters to use the funktion on variable bitmaps (mosfet's function does only work for very special bitmaps)
// by giving position and dimension of the central tile + the shadow height (panther doesn't seem to have a right shadow-offset)
ButtonTile* LiquidStyle::separateTiles( QPixmap *pix, 
					int xO, int yO, int w, int h, 
					int, bool sunken) const
{
    ButtonTile * tile = new ButtonTile();
    QPixmap *tmp;

    int w1 = xO;
    int w2 = w;
    int w3 = pix->width() - xO - w;
    int h1 = yO;
    int h2 = h;
    int h3 = pix->height() - yO - h;

    int xO1 = 0;
    int xO2 = xO;
    int xO3 = xO + w;
    int yO1 = 0;
    int yO2 = yO;
    int yO3 = yO + h;

    if ( !sunken ) {
	// Top tiles
	tmp = new QPixmap( w1, h1 );
	// bitBlt(...,x,y,width,height)
	copyBlt( tmp, 0, 0, pix, xO1, yO1, w1, h1 );
	tile->setPixmap( TileTopLeft, tmp );
	tmp = new QPixmap( w2, h1 );
   copyBlt( tmp, 0, 0, pix, xO2, yO1, w2, h1 );
	tile->setPixmap( TileTop, tmp );
	tmp = new QPixmap( w3, h1 );
   copyBlt( tmp, 0, 0, pix, xO3, yO1, w3, h1 );
	tile->setPixmap( TileTopRight, tmp );

	// Middle tiles
	tmp = new QPixmap( w1, h2 );
   copyBlt( tmp, 0, 0, pix, xO1, yO2, w1, h2 );
	tile->setPixmap( TileLeft, tmp );
	tmp = new QPixmap( w2, h2 );
   copyBlt( tmp, 0, 0, pix, xO2, yO2, w2, h2 );
	tile->setPixmap( TileMiddle, tmp );
	tmp = new QPixmap( w3, h2 );
   copyBlt( tmp, 0, 0, pix, xO3, yO2, w3, h2 );
	tile->setPixmap( TileRight, tmp );

	// Bottom tiles
	tmp = new QPixmap( w1, h3 );
   copyBlt( tmp, 0, 0, pix, xO1, yO3, w1, h3 );
	tile->setPixmap( TileBtmLeft, tmp );
	tmp = new QPixmap( w2, h3 );
   copyBlt( tmp, 0, 0, pix, xO2, yO3, w2, h3 );
	tile->setPixmap( TileBtm, tmp );
	tmp = new QPixmap( w3, h3 );
   copyBlt( tmp, 0, 0, pix, xO3, yO3, w3, h3 );
	tile->setPixmap( TileBtmRight, tmp );

    } 
    else {
	// Top tiles
	tmp = new QPixmap( w1, h3 );
	// bitBlt(...,x,y,width,height)
   copyBlt( tmp, 0, 0, pix, xO1, yO1, w1, h3 );
	tile->setPixmap( TileTopLeft, tmp );
	tmp = new QPixmap( w2, h3 );
   copyBlt( tmp, 0, 0, pix, xO2, yO1, w2, h3 );
	tile->setPixmap( TileTop, tmp );
	tmp = new QPixmap( w3, h3 );
   copyBlt( tmp, 0, 0, pix, xO3, yO1, w3, h3 );
	tile->setPixmap( TileTopRight, tmp );

	// Middle tiles
	tmp = new QPixmap( w1, h2 );
   copyBlt( tmp, 0, 0, pix, xO1, yO2 + h3 - h1, w1, h2 );
	tile->setPixmap( TileLeft, tmp );
	tmp = new QPixmap( w2, h2 );
   copyBlt( tmp, 0, 0, pix, xO2, yO2 + h3 - h1, w2, h2 );
	tile->setPixmap( TileMiddle, tmp );
	tmp = new QPixmap( w3, h2 );
   copyBlt( tmp, 0, 0, pix, xO3, yO2 + h3 - h1, w3, h2 );
	tile->setPixmap( TileRight, tmp );

	// Bottom tiles
	tmp = new QPixmap( w1, h1 );
   copyBlt( tmp, 0, 0, pix, xO1, yO3 + h3 - h1, w1, h1 );
	tile->setPixmap( TileBtmLeft, tmp );
	tmp = new QPixmap( w2, h1 );
   copyBlt( tmp, 0, 0, pix, xO2, yO3 + h3 - h1, w2, h1 );
	tile->setPixmap( TileBtm, tmp );
	tmp = new QPixmap( w3, h1 );
   copyBlt( tmp, 0, 0, pix, xO3, yO3 + h3 - h1, w3, h1 );
	tile->setPixmap( TileBtmRight, tmp );
    }
    return ( tile );
}

#define _LOFF_ tile->pixmap( TileLeft )->width()
#define _ROFF_ tile->pixmap( TileRight )->width()
#define _TOFF_ tile->pixmap( TileTop )->height()
#define _BOFF_ tile->pixmap( TileBtm )->height()
#define _MWIDTH_ tile->pixmap( TileMiddle )->width()
#define _MHEIGHT_ tile->pixmap( TileMiddle )->height()
#define _LRWIDTH_ (_LOFF_ + _ROFF_)
#define _TBHEIGHT_ (_TOFF_ + _BOFF_)

void LiquidStyle::drawRoundButton( QPainter *painter, const QColorGroup &cg, const QColor &c, int x, int y, int w, int h, bool pushedDown, bool hover, int bgX, int bgY ) const
{

   QColor myC =(isOOO && (hover || pushedDown))?optionHandler->CustomButtonColor():c;
   LiquidStyle *ptr = const_cast<LiquidStyle*>( this );
   ButtonTile *tile = pushedDown ? btnDict.find( myC.rgb() ) : btnShadowedDict.find( myC.rgb() );

   if ( !tile )
   {
         tile = createButtonTile( myC, qApp->palette().color(QPalette::Active, QColorGroup::Background), ptr->btnBorderImg, ptr->btnShadowImg, ptr->buttonGlow, &(ptr->btnDict), &(ptr->btnShadowedDict), &(ptr->buttonGlowDict), 37, 21, 10, 11, 17, 1, 2, 2, pushedDown);
   }

   if ( !tile )
   {
      qWarning( "Button tile is NULL!" );
      return ;
   }

   QPainter *p;
   QPen oldPen = painter->pen();
   if (!isHTMLButton)
   {
      if ( !tmpBtnPix )
         ptr->tmpBtnPix = new QPixmap( w, h );
      else if ( w > tmpBtnPix->width() || h > tmpBtnPix->height() )
      {
         // make temp pixmap size == largest button
         ptr->tmpBtnPix->resize(w,h);
      }
      p = new QPainter(tmpBtnPix);
      
      if (!isPlain())
      {
         QPixmap *stipple;
         if (paintWidget && paintWidget->parentWidget())
         {
            stipple = const_cast<QPixmap*>(paintWidget->parentWidget()->palette().brush(QPalette::Active, QColorGroup::Background).pixmap());
            if (!stipple || stipple->isNull())
               p->fillRect(0,0,w,h, paintWidget->parentWidget()->palette().brush(QPalette::Active, QColorGroup::Background) );
            else
               p->drawTiledPixmap( 0, 0, w, h, *stipple, bgX, bgY );
         }
         else
         {
            stipple = cg.brush( QColorGroup::Background ).pixmap();
            stipple ? p->drawTiledPixmap( 0, 0, w, h, *stipple, bgX, bgY ) :
            p->fillRect(0,0,w,h, cg.brush( QColorGroup::Background ));
         }
      }
      else
         p->fillRect(0,0,w,h, cg.brush( QColorGroup::Background ));
   }
   else
   {
      p = painter;
   }
   
//    QPainter p(tmpBtnPix);

   // tiled fills
   if ( w > _LRWIDTH_ )
   {
      p->drawTiledPixmap( _LOFF_, 0, w - _LRWIDTH_, _TOFF_, *tile->pixmap( TileTop ) );
      p->drawTiledPixmap( _LOFF_, h - _BOFF_, w - _LRWIDTH_, _BOFF_, *tile->pixmap( TileBtm ) );
   }
   if ( h > _TBHEIGHT_ )
   {
      p->drawTiledPixmap( 0, _TOFF_, _LOFF_, h - _TBHEIGHT_, *tile->pixmap( TileLeft ) );
      p->drawTiledPixmap( w - _ROFF_, _TOFF_, _ROFF_, h - _TBHEIGHT_, *tile->pixmap( TileRight ) );
   }
   if ( w > _LRWIDTH_ && h > _TBHEIGHT_ )
      p->drawTiledPixmap( _LOFF_, _TOFF_, w - _LRWIDTH_, h - _TBHEIGHT_, *tile->pixmap( TileMiddle ) );
   // corners
   p->drawPixmap( 0, 0, *tile->pixmap( TileTopLeft ) );
   p->drawPixmap( w - _ROFF_, 0, *tile->pixmap( TileTopRight ) );
   p->drawPixmap( 0, h - _BOFF_, *tile->pixmap( TileBtmLeft ) );
   p->drawPixmap( w - _ROFF_, h - _BOFF_, *tile->pixmap( TileBtmRight ) );

   if (!isHTMLButton)
   {
      p->end();
      delete p;
      painter->drawPixmap( x, y, *tmpBtnPix, 0, 0, w, h );
   }
   else
      p->setPen(oldPen);
}


void LiquidStyle::drawRectangularButton( QPainter *painter, 
					 const QColorGroup &cg, const QColor &c,
					 int x, int y, int w, int h, 
               bool sunken, bool hover, bool isCombo, int position,  bool isHTML) const
{
#define _HEIGHT_ ptr->rectbutton->height()
#define _WIDTH_ ptr->rectbutton->width()
   LiquidStyle *ptr = const_cast<LiquidStyle*>( this );

   QColor tmpColor;
   tmpColor = isCombo && !(hover || sunken) ? optionHandler->InactiveButtonColor() : c;

   int xOffset, width;
   int cw = 0;
   
   ButtonTile *tile = rectBtnDict.find( tmpColor.rgb() );
   if ( !tile )
      tile = createButtonTile( tmpColor, cg.background(), ptr->rectbutton, ((QImage*)0L), ((QImage*)0L), &(ptr->rectBtnDict), (QIntDict<ButtonTile>*)0L, (QIntDict<ButtonTile>*)0L, 33, 25, 10, 5, 13, 15, 0, 0, sunken);

   if ( !tile )
   {
      qWarning( "Button tile is NULL!" );
      return ;
   }
   
   switch (position)
   {
   case left:
      xOffset = _LOFF_;
      width = w - _LOFF_;
      cw = 24;
      break;
   case center:
      xOffset = 0;
      width = w;
      cw = 24;
      break;
   case right:
      xOffset = 0;
      width = w - _ROFF_;
      cw = 24-_ROFF_;
      break;
   case full:
      xOffset = _LOFF_;
      width = w - _LRWIDTH_;
      cw = 24-_ROFF_;
      break;
   }
   
   QPainter *p;
   QPen oldPen = painter->pen();
   if (!isHTML)
   {
      if ( !tmpBtnPix )
         ptr->tmpBtnPix = new QPixmap( w, h );
      else if ( w > tmpBtnPix->width() || h > tmpBtnPix->height() )
      {
         // make temp pixmap size == largest button
         ptr->tmpBtnPix->resize(w,h);
      }
      p = new QPainter(tmpBtnPix);
      
      if (position != center && paintWidget && paintWidget->parentWidget())
      {
         if (this->paintWidget->parentWidget()->paletteBackgroundPixmap ())
            p->drawTiledPixmap(QRect(0, 0, w, h), *this->paintWidget->parentWidget()->paletteBackgroundPixmap(), ((QWidget *)this->paintWidget)->backgroundOffset());
         else
            p->fillRect( 0, 0, w, h, this->paintWidget->parentWidget()->paletteBackgroundColor());
      }
      else
         p->fillRect( 0, 0, w, h, cg.background() );
   }
   else
   {
      p = painter;
   }

   int tileH = 0;
   QPixmap tmpPix(_LOFF_, 1);
   
   if (position == left || position == full)
   {
      p->drawPixmap( 0, 0, *tile->pixmap( TileTopLeft ) );
      p->drawPixmap( 0, h - _BOFF_, *tile->pixmap( TileBtmLeft ) );
   }

   if (!isCombo || hover || sunken)
   {
      if ( w > _LRWIDTH_ )
      {
         p->drawTiledPixmap( xOffset, 0, width, _TOFF_, *tile->pixmap( TileTop ) );
         p->drawTiledPixmap( xOffset, h - _BOFF_, width, _BOFF_, *tile->pixmap( TileBtm ) );
      }

      if ( h > _TBHEIGHT_ )
      {
         tileH = (int)((h-_HEIGHT_)/2);
         if (position == left || position == full)
         {
            if (h > _HEIGHT_)
            {
               p->drawPixmap(0, _TOFF_+tileH, *tile->pixmap( TileLeft ));
               copyBlt( &tmpPix, 0, 0, tile->pixmap(TileLeft), 0, 0, _LOFF_, 1 );
               p->drawTiledPixmap(0, _TOFF_, _LOFF_, tileH, tmpPix);
               copyBlt( &tmpPix, 0, 0, tile->pixmap(TileLeft), 0, _MHEIGHT_-1, _LOFF_, 1 );
               p->drawTiledPixmap(0, _TOFF_+tileH+_MHEIGHT_, _LOFF_, h-_HEIGHT_-tileH, tmpPix);
            }
            else //tileH is negative in this case
               p->drawPixmap( 0, _TOFF_, *tile->pixmap( TileLeft ), 0, -tileH, _LOFF_, _MHEIGHT_+2*tileH+1);
         }
         if (position == right || position == full)
         {
            if (h > _HEIGHT_)
            {
               tmpPix.resize(_ROFF_, 1);
               p->drawPixmap(w-_ROFF_, _TOFF_+tileH, *tile->pixmap( TileRight ));
               copyBlt( &tmpPix, 0, 0, tile->pixmap(TileRight), 0, 0, _ROFF_, 1 );
               p->drawTiledPixmap(w-_ROFF_, _TOFF_, _ROFF_, tileH, tmpPix);
               copyBlt( &tmpPix, 0, 0, tile->pixmap(TileRight), 0, _MHEIGHT_-1, _ROFF_, 1 );
               p->drawTiledPixmap(w-_ROFF_, _TOFF_+tileH+_MHEIGHT_, _ROFF_, h-_HEIGHT_-tileH, tmpPix);
            }
            else
               p->drawPixmap( w-_ROFF_, _TOFF_, *tile->pixmap(TileRight), 0, -tileH, _ROFF_, _MHEIGHT_+2*tileH+1 /*tileH is negative in this case*/ );
         }
      }


      if ( w > _LRWIDTH_ && h > _TBHEIGHT_ )
      {
         if (h > _HEIGHT_)
         {
            tmpPix.resize(_MWIDTH_, 1);
            p->drawTiledPixmap(xOffset, _TOFF_+tileH, width, _MHEIGHT_, *tile->pixmap(TileMiddle));
            copyBlt( &tmpPix, 0, 0, tile->pixmap(TileMiddle), 0, 0, _MWIDTH_, 1 );
            p->drawTiledPixmap(xOffset, _TOFF_, width, tileH, tmpPix);
            copyBlt( &tmpPix, 0, 0, tile->pixmap(TileMiddle), 0, _MHEIGHT_-1, _MWIDTH_, 1 );
            p->drawTiledPixmap(xOffset, _TOFF_+tileH+_MHEIGHT_, width, h-_HEIGHT_-tileH, tmpPix);
         }
         else  //tileH is negative in this case
            p->drawTiledPixmap( xOffset, _TOFF_, width, h-_TBHEIGHT_, *tile->pixmap(TileMiddle), 0, -tileH);
      }


      if (position == right || position == full)
      {
         p->drawPixmap( w - _ROFF_, 0, *tile->pixmap( TileTopRight ) );
         p->drawPixmap( w - _ROFF_, h - _BOFF_, *tile->pixmap( TileBtmRight ) );
      }

   }
   else // split combo...
   {
      if ( w > _LRWIDTH_ )
      {
         p->drawTiledPixmap( xOffset, 0, width-(24-_ROFF_), _TOFF_, *tile->pixmap( TileTop ) );
         p->drawTiledPixmap( xOffset, h - _BOFF_, width-(24-_ROFF_), _BOFF_, *tile->pixmap( TileBtm ) );
      }
      if ( h > _TBHEIGHT_)
      {
         tileH = (int)((h-_HEIGHT_)/2);
         if (position == left || position == full)
         {
            if (h > _HEIGHT_)
            {
               tmpPix.resize(_LOFF_, 1);
               p->drawPixmap(0, _TOFF_+tileH, *tile->pixmap( TileLeft ));
               copyBlt( &tmpPix, 0, 0, tile->pixmap(TileLeft), 0, 0, _LOFF_, 1 );
               p->drawTiledPixmap(0, _TOFF_, _LOFF_, tileH, tmpPix);
               copyBlt( &tmpPix, 0, 0, tile->pixmap(TileLeft), 0, _MHEIGHT_-1, _LOFF_, 1 );
               p->drawTiledPixmap(0, _TOFF_+tileH+_MHEIGHT_, _LOFF_, h-_HEIGHT_-tileH, tmpPix);
            }
            else //tileH is negative in this case
               p->drawPixmap( 0, _TOFF_, *tile->pixmap( TileLeft ), 0, -tileH, _LOFF_, _MHEIGHT_+2*tileH+1);
         }
      }
      if ( w > _LRWIDTH_ && h > _TBHEIGHT_ )
      {
         if (h > _HEIGHT_)
         {
            tmpPix.resize(_MWIDTH_, 1);
            p->drawTiledPixmap(xOffset, _TOFF_+tileH, width-(24-_ROFF_), _MHEIGHT_, *tile->pixmap(TileMiddle));
            copyBlt( &tmpPix, 0, 0, tile->pixmap(TileMiddle), 0, 0, _MWIDTH_, 1 );
            p->drawTiledPixmap(xOffset, _TOFF_, width-(24-_ROFF_), tileH, tmpPix);
            copyBlt( &tmpPix, 0, 0, tile->pixmap(TileMiddle), 0, _MHEIGHT_-1, _MWIDTH_, 1 );
            p->drawTiledPixmap(xOffset, _TOFF_+tileH+_MHEIGHT_, width-(24-_ROFF_), h-_HEIGHT_-tileH, tmpPix);
         }
         else  //tileH is negative in this case
            p->drawTiledPixmap( xOffset, _TOFF_, width-(24-_ROFF_), h-_TBHEIGHT_, *tile->pixmap(TileMiddle), 0, -tileH);
      }

      // second part of combos if needed
      tile = rectBtnDict.find( c.rgb() );
      if ( !tile )
         tile = createButtonTile( c, cg.background(), ptr->rectbutton, ((QImage*)0L), ((QImage*)0L), &(ptr->rectBtnDict), (QIntDict<ButtonTile>*)0L, (QIntDict<ButtonTile>*)0L, 33, 25, 10, 5, 13, 15, 0, 0, sunken);
      if ( !tile )
      {
         qWarning( "Button tile is NULL!" );
         return ;
      }

      if ( w > _LRWIDTH_ )
      {
         p->drawTiledPixmap( w-24, 0, cw, _TOFF_, *tile->pixmap( TileTop ) );
         p->drawTiledPixmap( w-24, h - _BOFF_, cw, _BOFF_, *tile->pixmap( TileBtm ) );
      }
      
      tileH = (int)((h-_HEIGHT_)/2);
      if (position == right || position == full)
      {
         if (h > _HEIGHT_)
         {
            tileH = (int)((h-_HEIGHT_)/2);
            tmpPix.resize(_ROFF_, 1);
            p->drawPixmap(w-_ROFF_, _TOFF_+tileH, *tile->pixmap( TileRight ));
            copyBlt( &tmpPix, 0, 0, tile->pixmap(TileRight), 0, 0, _ROFF_, 1 );
            p->drawTiledPixmap(w-_ROFF_, _TOFF_, _ROFF_, tileH, tmpPix);
            copyBlt( &tmpPix, 0, 0, tile->pixmap(TileRight), 0, _MHEIGHT_-1, _ROFF_, 1 );
            p->drawTiledPixmap(w-_ROFF_, _TOFF_+tileH+_MHEIGHT_, _ROFF_, h-_HEIGHT_-tileH, tmpPix);
         }
         else if ( h > _TBHEIGHT_)//tileH is negative in this case
         {
            tileH = (int)((h-_HEIGHT_)/2);
            p->drawPixmap( w-_ROFF_, _TOFF_, *tile->pixmap( TileRight ), 0, -tileH, _ROFF_, _MHEIGHT_+2*tileH+1);
         }
         p->drawPixmap( w - _ROFF_, 0, *tile->pixmap( TileTopRight ) );
         p->drawPixmap( w - _ROFF_, h - _BOFF_, *tile->pixmap( TileBtmRight ) );
      }

      if ( w > _LRWIDTH_ && h > _TBHEIGHT_ )
      {
         if (h > _HEIGHT_)
         {
            tmpPix.resize(_MWIDTH_, 1);
            p->drawTiledPixmap(w-24, _TOFF_+tileH, cw, _MHEIGHT_, *tile->pixmap(TileMiddle));
            copyBlt( &tmpPix, 0, 0, tile->pixmap(TileMiddle), 0, 0, _MWIDTH_, 1 );
            p->drawTiledPixmap(w-24, _TOFF_, cw, tileH, tmpPix);
            copyBlt( &tmpPix, 0, 0, tile->pixmap(TileMiddle), 0, _MHEIGHT_-1, _MWIDTH_, 1 );
            p->drawTiledPixmap(w-24, _TOFF_+tileH+_MHEIGHT_, cw, h-_HEIGHT_-tileH, tmpPix);
         }
         else  //tileH is negative in this case
            p->drawTiledPixmap( w-24, _TOFF_, cw, h-_TBHEIGHT_, *tile->pixmap(TileMiddle), 0, -tileH);
      }
   }
   if (position == center)
   {
      p->setPen(c.dark(130));
      if (sunken || hover) p->drawLine(0,1,0,h-2);
      p->drawLine(w-1,2,w-1,h-2);
   }
   else if (position == left)
   {
      p->setPen(c.dark(130));
      p->drawLine(w-1,2,w-1,h-2);
   }
   else if (position == right && (sunken || hover))
   {
      p->setPen(c.dark(130));
      p->drawLine(0,2,0,h-2);
   }
   if (isCombo)
   {
      p->setPen(c);
      p->drawLine(w-24,2,w-24,h-2);
      
      int ax = w - 15;
      int ay = h/3 - 1;
      
      p->setPen(cg.text());
      p->setBrush( cg.text() );
      p->drawPixmap(ax, ay-1, *sbUp, 1, 0, 5, 4);
      p->drawPixmap(ax, ay+6, *sbDown, 1, 2, 5, 4);
   }
   if (!isHTML)
   {
      p->end();
      delete p;
      painter->drawPixmap( x, y, *tmpBtnPix, 0, 0, w, h );
   }
   else
      p->setPen(oldPen);
#undef _HEIGHT_
#undef _WIDTH_
}


void LiquidStyle::drawCombo( QPainter *painter, 
			    const QColorGroup &cg, 
			    const QColor &c, 
			    int x, int y, int w, int h, 
			    bool sunken, bool hover, bool isCombo, 
			    int position, bool isHTML ) const
{

   LiquidStyle *ptr = const_cast<LiquidStyle*>( this );

   QColor myC =(isOOO && isCombo)?optionHandler->CustomButtonColor():c;
   QColor tmpColor = isCombo && !(hover || sunken) ? optionHandler->InactiveButtonColor() : myC;

   ButtonTile *tile = sunken ? comboDict.find( tmpColor.rgb() ) : comboShadowedDict.find( tmpColor.rgb() );
   if ( !tile )
   {
      if (optionHandler->buttonStyle() == Milk && sunken)
         tile = createButtonTile( tmpColor, cg.background(), ptr->btnBorderImg, ptr->comboShadow, ((QImage*)0L), &(ptr->comboDict), &(ptr->comboShadowedDict), (QIntDict<ButtonTile>*)0L, 44, 21, 7, 9, 30, 1, 2, 0, true);
      else
         tile = createButtonTile( tmpColor, cg.background(), ptr->combo, ptr->comboShadow, ((QImage*)0L), &(ptr->comboDict), &(ptr->comboShadowedDict), (QIntDict<ButtonTile>*)0L, 44, 21, 7, 9, 30, 1, 2, 0, sunken);
   }

   if ( !tile )
   {
      qWarning( "Button tile is NULL!" );
      return ;
   }
   
   int xOffset, width, cw;
   
   switch (position)
   {
   case full:
      xOffset = _LOFF_;
      width = w - _LRWIDTH_;
      cw = 24 - _ROFF_;
      break;
   case left:
      xOffset = _LOFF_;
      width = w - _LOFF_;
      cw = 24;
      break;
   case center:
      xOffset = 0;
      width = w;
      cw = 24;
      break;
   case right:
      xOffset = 0;
      width = w - _ROFF_;
      cw = 24 - _ROFF_;
      break;
   }
   
   QPainter *p;
   QPen oldPen = painter->pen();
   if (!isHTML)
   {
      if ( !tmpBtnPix )
         ptr->tmpBtnPix = new QPixmap( w, h );
      else if ( w > tmpBtnPix->width() || h > tmpBtnPix->height() )
      {
         // make temp pixmap size == largest button
         ptr->tmpBtnPix->resize(w,h);
      }
      p = new QPainter(tmpBtnPix);
      
#if 0
         QPixmap *stipple = cg.brush( QColorGroup::Background ).pixmap();
         if ( !stipple )  // button may have custom colorgroup
            stipple = qApp->palette().active().brush( QColorGroup::Background ).pixmap();
         if ( stipple )
            p->drawTiledPixmap( 0, 0, w, h, *stipple);
         else
            p->fillRect( 0, 0, w, h, cg.background() );
#endif
         if (position != center && paintWidget && paintWidget->parentWidget())
         {
            if (this->paintWidget->parentWidget()->paletteBackgroundPixmap ())
               p->drawTiledPixmap(QRect(0, 0, w, h), *this->paintWidget->parentWidget()->paletteBackgroundPixmap(), ((QWidget *)this->paintWidget)->backgroundOffset());
            else
               p->fillRect( 0, 0, w, h, this->paintWidget->parentWidget()->paletteBackgroundColor());
         }
         else
            p->fillRect( 0, 0, w, h, cg.background() );
   }
   else
   {
      p = painter;
   }
   
   if (position == left || position == full)
   {
      p->drawPixmap( 0, 0, *tile->pixmap( TileTopLeft ) );
      p->drawPixmap( 0, h - _BOFF_, *tile->pixmap( TileBtmLeft ) );
   }
   
   if (!isCombo || hover || sunken)
   {
      if ( w > _LRWIDTH_ )
      {
         p->drawTiledPixmap( xOffset, 0, width, _TOFF_, *tile->pixmap( TileTop ) );
         p->drawTiledPixmap( xOffset, h - _BOFF_, width, _BOFF_, *tile->pixmap( TileBtm ) );
      }
      if ( h > _TBHEIGHT_ )
      {
         if (position == left || position == full)
            p->drawTiledPixmap( 0, _TOFF_, _LOFF_, h - _TBHEIGHT_, *tile->pixmap( TileLeft ) );
         if (position == right || position == full)
            p->drawTiledPixmap( w - _ROFF_, _TOFF_, _ROFF_, h - _TBHEIGHT_, *tile->pixmap( TileRight ) );
      }
      if ( w > _LRWIDTH_ && h > _TBHEIGHT_ )
         p->drawTiledPixmap( xOffset, _TOFF_, width, h - _TBHEIGHT_, *tile->pixmap( TileMiddle ) );
      if (position == right || position == full)
      {
         p->drawPixmap( w - _ROFF_, 0, *tile->pixmap( TileTopRight ) );
         p->drawPixmap( w - _ROFF_, h - _BOFF_, *tile->pixmap( TileBtmRight ) );
      }
   }
   else // split combo...
   {
      if ( w > _LRWIDTH_ )
      {
         p->drawTiledPixmap( xOffset, 0, width-(24 - _ROFF_), _TOFF_, *tile->pixmap( TileTop ) );
         p->drawTiledPixmap( xOffset, h - _BOFF_, width-(24 - _ROFF_), _BOFF_, *tile->pixmap( TileBtm ) );
      }
      if ( h > _TBHEIGHT_ && (position == left || position == full))
         p->drawTiledPixmap( 0, _TOFF_, _LOFF_, h - _TBHEIGHT_, *tile->pixmap( TileLeft ) );
      if ( w > _LRWIDTH_ && h > _TBHEIGHT_ )
         p->drawTiledPixmap( xOffset, _TOFF_, width-(24 - _ROFF_), h - _TBHEIGHT_, *tile->pixmap( TileMiddle ) );
      
      // second part of combos if needed
      tile = comboShadowedDict.find( myC.rgb() );
      if ( !tile )
      {
         tile = createButtonTile( myC, cg.background(), ptr->combo, ptr->comboShadow, ((QImage*)0L), &(ptr->comboDict), &(ptr->comboShadowedDict), (QIntDict<ButtonTile>*)0L, 44, 21, 7, 9, 30, 1, 2, 0, sunken);
      }
      if ( !tile )
      {
         qWarning( "Button tile is NULL!" );
         return ;
      }
      if ( w > _LRWIDTH_ )
      {
      p->drawTiledPixmap( w-24, 0, cw, _TOFF_, *tile->pixmap( TileTop ) );
      p->drawTiledPixmap( w-24, h - _BOFF_, cw, _BOFF_, *tile->pixmap( TileBtm ) );
      }
      if ( h > _TBHEIGHT_ )
         p->drawTiledPixmap( w - _ROFF_, _TOFF_, _ROFF_, h - _TBHEIGHT_, *tile->pixmap( TileRight ) );
      if ( w > _LRWIDTH_ && h > _TBHEIGHT_ )
         p->drawTiledPixmap( w-24, _TOFF_, cw, h - _TBHEIGHT_, *tile->pixmap( TileMiddle ) );
      if (position == right || position == full)
      {
         p->drawPixmap( w - _ROFF_, 0, *tile->pixmap( TileTopRight ) );
         p->drawPixmap( w - _ROFF_, h - _BOFF_, *tile->pixmap( TileBtmRight ) );
      }
   }
   
   switch (position)
   {
   case left:
      p->setPen(optionHandler->InactiveButtonColor().dark(130));
      p->drawLine(w-1,3,w-1,h-6);
      break;
   case center:
      p->setPen(optionHandler->InactiveButtonColor().dark(130));
      if (sunken || hover) p->drawLine(0,3,0,h-6);
      p->drawLine(w-1,3,w-1,h-6);
      break;
   case right:
      if (sunken || hover)
      {
         p->setPen(optionHandler->InactiveButtonColor().dark(130));
         p->drawLine(0,3,0,h-6);
      }
      break;
   default:
      break;
   }

   if (isCombo)
   {
      p->setPen(c);
      p->drawLine(w-24,1,w-24,h-4);
      
      int ax = w - 15;
      int ay = h/3 - 1;
      
      p->setPen(cg.text());
      p->setBrush( cg.text() );
      p->drawPixmap(ax, ay-1, *sbUp, 1, 0, 5, 4);
      p->drawPixmap(ax, ay+6, *sbDown, 1, 2, 5, 4);
   }
   
   if (!isHTML)
   {
      p->end();
      delete p;
      painter->drawPixmap( x, y, *tmpBtnPix, 0, 0, w, h );
   }
   else
      p->setPen(oldPen);
}

void LiquidStyle::drawRoundFrame( QPainter *p, const QRect &r,
                                 const QColorGroup &cg, bool focused, QPoint offset ) const
{
   ButtonTile *tile = focused ? activeRoundFrame : inactiveRoundFrame;
   if (!tile) tile = createRoundFrameTile(*roundFrame, focused ? cg.button() : optionHandler->InactiveButtonColor(), &cg.background(), cg.base() );
   
   // first: fill background (tiled)
   // outer
   QPixmap *stipple = cg.brush( QColorGroup::Background ).pixmap();
   if (stipple)
   {
      p->drawTiledPixmap( r.x(),r.y(), 7, 8, *stipple, offset.x(), offset.y());
      p->drawTiledPixmap( r.right() - 7, r.y(), 8, 8, *stipple, offset.x()+r.width()-6, offset.y());
      p->drawTiledPixmap( r.x(), r.bottom()-7, 8, 8, *stipple, offset.x(), offset.y() + r.height()-6);
      p->drawTiledPixmap( r.right() - 7, r.bottom()-7, 8, 8, *stipple, offset.x() + r.width() - 6, offset.y() + r.height()-6);
   }
   else
   {
      p->fillRect(r.x(),r.y(), 7, 8, cg.background());
      p->fillRect(r.right() - 7, r.y(), 8, 8, cg.background());
      p->fillRect(r.x(), r.bottom()-7, 8, 8, cg.background());
      p->fillRect(r.right() - 7, r.bottom()-7, 8, 8, cg.background());
   }
   // inner
   p->fillRect(r.x()+3,r.y()+4,r.width()-6,r.height()-7,cg.base());
    
   // now draw frame
   p->drawPixmap(0,0, *tile->pixmap( TileTopLeft ));
   p->drawPixmap(r.right()-10,0, *tile->pixmap( TileTopRight ));
   p->drawPixmap(0,r.bottom()-10, *tile->pixmap( TileBtmLeft ));
   p->drawPixmap(r.right()-10,r.bottom()-10, *tile->pixmap( TileBtmRight ));
   if (r.width() > 22)
   {
      p->drawTiledPixmap(11,0,r.width()-22,10, *tile->pixmap( TileTop ));
      p->drawTiledPixmap(11,r.bottom()-10,r.width()-22,11, *tile->pixmap( TileBtm ));
   }
   if (r.height() > 21)
   {
      p->drawTiledPixmap(0,10,11,r.height()-21, *tile->pixmap( TileLeft ));
      p->drawTiledPixmap(r.right()-10,10,11,r.height()-21, *tile->pixmap( TileRight ));
   }
   if (focused)
   {
      p->setPen(cg.button());
      p->drawLine(r.x()+8,r.bottom()-1,r.right()-8,r.bottom()-1);
   }
}

void LiquidStyle::drawEditFrame( QPainter *p, const QRect &r,
                                 const QColorGroup &cg, bool isHTML, bool focused, bool inverse ) const
{
   QColor fill( cg.background().dark( 105 ) );
   QColor tmp(0,0,0);
   QColor light1( 0, 0, 0 );
   QColor light2( 0, 0, 0 );

   QColor dark1( 0, 0, 0 );
   QColor dark2( 0, 0, 0 );
   QColor dark3( 0, 0, 0 );

   if ( !focused )
   {
      if (optionHandler->style() == Brushed)
      {
         light1 = cg.background().light( 140 );
         light2 = cg.background().light( 120 );
      }
      else
      {
         light1 = cg.background().dark( 115 );
         light2 = cg.background().dark( 120 );
      }
      
      dark1 = cg.background().dark( 140 );
      dark2 = cg.background().dark( 160 );
      dark3 = cg.background().dark( 200 );
   }
   else
   {
      int h,s,v;
      cg.button().getHsv( &h, &s, &v );
      tmp.setHsv(h, (int)(s/1.5) ,v);
      light1 = tmp;
      light2 = tmp.light( 120 );
      
      dark1 = tmp.dark( 110 );
      dark2 = tmp.dark( 120 );
      dark3 = tmp.dark( 140 );
   }
    
   int x = r.x();
   int y = inverse ? r.bottom() - 1: r.y();
   int right = r.right();
   int bottom = inverse ? r.y() + 1 : r.bottom();

   if ( !isHTML )
   {
      p->setPen( fill );
      p->drawPoint( x, y );
      p->drawPoint( x, bottom );
      p->drawPoint( right, y );
      p->drawPoint( right, bottom );
   
      // outer rect
      // top
      p->setPen( light1 );
      p->drawPoint( x + 1, y );
      p->drawPoint( right - 1, y );
      p->drawPoint( x, y + 1 );
      p->drawPoint( right, y + 1 );
      // bottom
      p->drawPoint( x, bottom - 1 );
      p->drawPoint( right, bottom - 1 );
      p->drawPoint( x + 1, bottom );
      p->drawPoint( right - 1, bottom );
      // top
      focused ? p->setPen( light2 ) : p->setPen( dark1 );
      p->drawLine( x + 2, y, right - 2, y );
      // bottom, left and right
      if (!focused) p->setPen( light2 );
      p->drawLine( x, y + 2, x, bottom - 2 );
      p->drawLine( right, y + 2, right, bottom - 2 );
      if (!focused) p->setPen( light1 );
      p->drawLine( x + 2, bottom, right - 2, bottom );
   }
   else
   {
      p->setPen( light2 );
      p->drawLine( x, y, right, y );
      p->drawLine( x, y, x, bottom );
      p->setPen( dark1 );
      p->drawLine( x, bottom, right, bottom );
      p->drawLine( right, y, right, bottom );
   }
   // inner rect
   p->setPen( dark1 );
   p->drawPoint( x + 1, y + 1 );
   p->drawPoint( x + 1, bottom - 1 );
   p->drawPoint( right - 1, y + 1 );
   p->drawPoint( right - 1, bottom - 1 );
   p->drawLine( x + 2, bottom - 1, right - 2, bottom - 1 );
   p->setPen( dark3 );
   p->drawLine( x + 2, y + 1, right - 2, y + 1 );
   p->setPen( dark2 );
   p->drawLine( x + 1, y + 2, x + 1, bottom - 2 );
   p->drawLine( right - 1, y + 2, right - 1, bottom - 2 );
}

bool LiquidStyle::isHTMLWidget( const QWidget *widget ) const
{
   const QObject * w = widget->parent();
   if ( w )
   {
      if ( !w->inherits( "QClipperWidget" ) )
         return ( false );
      w = w->parent();
      if ( w )
      {
         w = w->parent();
         if ( w && w->inherits( "KHTMLView" ) )
         return ( true );
      }
   }
   return ( false );
}

void LiquidStyle::drawHTMLCBBorder( const QPixmap &pix, const QColor &c ) const
{
    QPainter p;
    p.begin( &pix );
    p.setPen( c.dark( 200 ) );
    p.drawRect( 0, 0, 16, 16 );
    p.end();
}
