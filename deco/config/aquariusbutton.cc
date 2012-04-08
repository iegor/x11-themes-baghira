#include "aquariusbutton.h"
#include <qcolor.h>
#include <qpixmap.h>
#include <qpainter.h>
#include <kimageeffect.h>
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

#define SATURATION_COLOR2(S,R,G,B) \
   int max = 255+0.65*(100-S); \
   destR = CLAMP((srcR + R - 128), 0, max); \
   destG = CLAMP((srcG + G - 128), 0, max); \
   destB = CLAMP((srcB + B - 128), 0, max); \
   destR = (S*destR + (100-S)*R)/100; \
   destG = (S*destG + (100-S)*G)/100; \
   destB = (S*destB + (100-S)*B)/100;
   
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

AquariusButton::AquariusButton( QPixmap &pixmap, QWidget* parent, const char* name) : QWidget( parent, name){
    pixmap = pixmap;
    image = pixmap.convertToImage();
    setFixedSize( pixmap.size() );
}

AquariusButton::~AquariusButton(){
}

QColor AquariusButton::Color(){
    return color;
}

void AquariusButton::setColor(QColor c){
    color = c;
    tint(color);
    repaint(false);
}

void AquariusButton::tint(QColor &c){
    QImage dest( image.width(), image.height(), 32, 0 );
    dest.setAlphaBuffer( true );
    unsigned int *data = ( unsigned int * ) image.bits();
    unsigned int *destData = ( unsigned int* ) dest.bits();
    int total = image.width() * image.height();
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
	SATURATION_COLOR2(sq, red, green, blue);
	// force back to valid colorspace !
	COLOR_SPACE(destR, destG, destB);
	destData[ current ] = qRgba( destR, destG, destB, alpha );
    }
    QPixmap backPix = QPixmap(dest.size());
    QPainter tmpPainter(&backPix);
    tmpPainter.fillRect(0, 0, dest.width(),dest.height(), backgroundBrush());
    tmpPainter.end();
    QImage back = backPix.convertToImage();
    blend(dest,back,back);
    pixmap = QPixmap(back);
}

void AquariusButton::mousePressEvent( QMouseEvent *e ){
    emit clicked();
}

void AquariusButton::paintEvent( QPaintEvent *e){
    QPainter tmpPainter(this);
    tmpPainter.drawPixmap(0,0, pixmap);
}

// void AquariusButton::clicked(){
// }
