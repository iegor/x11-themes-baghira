#include "colordialog.h"
#include <qlabel.h>
#include <qdir.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qpushbutton.h>
#include <qstyle.h>
#include <kglobal.h>
#include <kglobalsettings.h>
#include <kcolordialog.h>
#include <kiconloader.h>
#include "colorpicker.h"
#include "pixmaps.h"

#include "config.h"

#ifndef CLAMP
#define CLAMP(x,l,u) x < l ? l :\
x > u ? u :\
x
#endif

#define SATURATION_COLOR2(S,R,G,B) \
int max = (int)(255+0.65*(100-S)); \
destR = CLAMP((srcR + R - 128), 0, max); \
destG = CLAMP((srcG + G - 128), 0, max); \
destB = CLAMP((srcB + B - 128), 0, max); \
destR = (S*destR + (100-S)*R)/100; \
destG = (S*destG + (100-S)*G)/100; \
destB = (S*destB + (100-S)*B)/100;

#define COLOR_SPACE(R,G,B) \
if ( R < 0 ) R = 0; else if ( R > 255 ) R = 255; \
if ( G < 0 ) G = 0; else if ( G > 255 ) G = 255; \
if ( B < 0 ) B = 0; else if ( B > 255 ) B = 255;

ColorDialog::ColorDialog(QWidget* parent, const char* name) : QDialog( parent, name)
{
   QVBoxLayout *vertical = new QVBoxLayout(this);
   QHBoxLayout *top = new QHBoxLayout(vertical);
   demo = new DemoWindow(this);
   top->addWidget(demo);
   QVBoxLayout *topRight = new QVBoxLayout(top);
   QLabel *info = new QLabel("<qt>Select custom colors or grab directly from screen.<br>Click image left to switch between items.</qt>",this);
   topRight->addWidget(info);
   QIconSet icon = KGlobal::iconLoader()->loadIconSet("colorpicker", KIcon::Small);
   buttonCP = new QPushButton(icon, QString::null, this);
   topRight->addWidget(buttonCP);
   QLabel *lb = new QLabel("From other app:",this);
   topRight->addWidget(lb);
   other = new QComboBox(this);
   topRight->addWidget(other);
   topRight->addStretch();
   buttonOk = new QPushButton("&Ok", this);
   topRight->addWidget(buttonOk);
   buttonCancel = new QPushButton("&Cancel", this);
   topRight->addWidget(buttonCancel);
   const char *title[NUMCOLORS] = {"Background", "Button", "Base", "Text", "Highlight", "Highlighted Text", "Button Text", "Alternate Background"};
   for (int i = 0; i < NUMCOLORS; i++)
   {
      picker[i] = new ColorPicker(this, title[i]);
      vertical->addWidget(picker[i]);
      connect (picker[i], SIGNAL(colorChanged(QColor)), demo, SLOT(smartRepaint()));
      picker[i]->hide();
   }
   for (int i = 0; i < NUMCOLORS; i++)
      picker[i]->blockSignals(true);
   picker[Back]->setColor(colorGroup().background());
   picker[Button]->setColor(colorGroup().button());
   picker[Base]->setColor(colorGroup().base());
   picker[Text]->setColor(colorGroup().text());
   picker[High]->setColor(colorGroup().highlight());
   picker[HighText]->setColor(colorGroup().highlightedText());
   picker[ButText]->setColor(colorGroup().buttonText ());
   picker[Alternate]->setColor(KGlobalSettings::alternateBackgroundColor());
   for (int i = 0; i < NUMCOLORS; i++)
      picker[i]->blockSignals(false);
//    demo->repaint(false);

   picker[Button]->show();
   connect (buttonOk, SIGNAL(clicked()), this, SLOT(close()));
   connect (buttonCP, SIGNAL(clicked()), this, SLOT(grabColor()));
   connect (buttonCancel, SIGNAL(clicked()), this, SLOT(close()));
   connect (other, SIGNAL(activated (const QString &)), this, SLOT(getFromOther(const QString &)));
}

ColorDialog::~ColorDialog()
{
}

void ColorDialog::show()
{
   other->clear();
   QDir d( QDir::homeDirPath() + "/.baghira", 0L, QDir::Name | QDir::IgnoreCase, QDir::Files | QDir::Readable | QDir::Writable );
   if (d.exists())
   {
      for ( uint i = 0; i < d.count(); i++ )
         other->insertItem(d[i]);
   }
   QDialog::show();
   demo->show();
//    demo->repaint(false);
}

void ColorDialog::getFromOther( const QString & string )
{
   FILE *file = NULL;
   QString tmpString = QDir::homeDirPath() + "/.baghira/" + string;
   if( (file = fopen(tmpString.latin1(), "r")) != NULL )
   {
      uint k;
      int colors[NUMCOLORS];
      for (int i = 0; i < NUMCOLORS; i++)
         colors[i] = -1;
      fscanf(file,"%u\n%u\n%u\n%u\n%u\n%u\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",&k,&k,&k,&k,&k,&k,&colors[0],&colors[1],&colors[2],&colors[3],&colors[4],&colors[5],&colors[6],&colors[7]);
      fclose(file);
      for (int i = 0; i < NUMCOLORS; i++)
      {
         picker[i]->blockSignals(true);
         if (colors[i] != -1) picker[i]->setColor(QColor(colors[i]));
         picker[i]->blockSignals(false);
      }
      demo->repaint(false);
   }
}

void ColorDialog::grabColor()
{
   gettingColorFromScreen = true;
   grabMouse(crossCursor);
   grabKeyboard();
}

void ColorDialog::mouseReleaseEvent(QMouseEvent* me)
{
   if (gettingColorFromScreen)
   {
      gettingColorFromScreen = false;
      releaseMouse();
      releaseKeyboard();
      for (int i = 0; i < NUMCOLORS; i++)
      {
         if (picker[i]->isShown())
         {
            picker[i]->setColor(KColorDialog::grabColor(me->globalPos()));
            break;
         }
      }
   }
   else
      QDialog::mouseReleaseEvent(me);
}

//extern QImage uic_findiImage(const QString& name);
//extern void qInitImages_baghira();
//extern void qCleanupImages_baghira();


DemoWindow::DemoWindow( ColorDialog* parent, const char* name) : QWidget(parent, name)
{
   setBackgroundMode ( Qt::NoBackground );
   colorDialog_ = parent;
   setFixedSize(320,120);
   pm = new QPixmap(320,120);
   pp = new QPainter();
   p = new QPainter();
   (const_cast<QFont*>(&p->font()))->setPixelSize(16);
   baseRect = QRect(20, 10, 200, 100);
   buttonRect = QRect(230, 90, 37, 21);
   buttonTextRect = QRect(236, 91, 20, 18);
   highlightRect = QRect(21, 42, 198, 22);
   textRect = QRect(25, 21, 100, 18);
   highTextRect = QRect(25, 42, 120, 22);
   alternateRect = QRect(21, 86, 198, 22);
   baseImage = uic_findImage("button-base");
   dest = QImage( baseImage.width(), baseImage.height(), 32 );
   dest.setAlphaBuffer( true );
}

DemoWindow::~DemoWindow(){}

void DemoWindow::mousePressEvent ( QMouseEvent * me )
{
   for (int i = 0; i < NUMCOLORS; i++)
      colorDialog_->picker[i]->hide();
   
   if (buttonTextRect.contains(me->pos()))
      colorDialog_->picker[ButText]->show();
   else if (buttonRect.contains(me->pos()))
      colorDialog_->picker[Button]->show();
   else if (textRect.contains(me->pos()))
      colorDialog_->picker[Text]->show();
   else if (highTextRect.contains(me->pos()))
      colorDialog_->picker[HighText]->show();
   else if (highlightRect.contains(me->pos()))
      colorDialog_->picker[High]->show();
   else if (alternateRect.contains(me->pos()))
      colorDialog_->picker[Alternate]->show();
   else if (baseRect.contains(me->pos()))
      colorDialog_->picker[Base]->show();
   else
      colorDialog_->picker[Back]->show();
}

void DemoWindow::paintEvent ( QPaintEvent * )
{
   pp->begin(pm);
   pp->fillRect(0,0,pm->width(),pm->height(), colorDialog_->picker[Back]->color());
   style().drawPrimitive( QStyle::PE_PanelLineEdit, pp, rect(), colorGroup() );
   pp->fillRect(baseRect, colorDialog_->picker[Base]->color());
   pp->fillRect(highlightRect, colorDialog_->picker[High]->color());
   pp->fillRect(alternateRect, colorDialog_->picker[Alternate]->color());
   pp->setPen ( colorDialog_->picker[Text]->color() );
   pp->drawText ( textRect, Qt::AlignAuto | Qt::AlignVCenter, "Common Text");
   pp->drawText ( alternateRect, Qt::AlignAuto | Qt::AlignVCenter, "Alt. Background");
   pp->setPen ( colorDialog_->picker[HighText]->color() );
   pp->drawText ( highTextRect, Qt::AlignAuto | Qt::AlignVCenter, "Highlighted Text");
   pp->drawPixmap(buttonRect, tintButton(baseImage, colorDialog_->picker[Button]->color()));
   pp->setPen ( colorDialog_->picker[ButText]->color() );
   pp->drawText ( buttonTextRect, Qt::AlignCenter, "B");
   style().drawPrimitive( QStyle::PE_PanelLineEdit, pp, baseRect, colorGroup());
   pp->end();
   p->begin(this);
   p->drawPixmap(0,0, *pm);
   p->end();
}

void DemoWindow::smartRepaint()
{
   if (colorDialog_->picker[Back]->isShown())
   {
      QWidget::repaint(false);
      return;
   }
   if (colorDialog_->picker[Button]->isShown())
   {
      QWidget::repaint(buttonRect, false);
      return;
   }
   if (colorDialog_->picker[Base]->isShown())
   {
      QWidget::repaint(false);
      return;
   }
   if (colorDialog_->picker[Text]->isShown())
   {
      QWidget::repaint(textRect, false);
      return;
   }
   if (colorDialog_->picker[High]->isShown())
   {
      QWidget::repaint(highlightRect, false);
      return;
   }
   if (colorDialog_->picker[HighText]->isShown())
   {
      QWidget::repaint(highTextRect, false);
      return;
   }
   if (colorDialog_->picker[Alternate]->isShown())
   {
      QWidget::repaint(alternateRect, false);
      return;
   }
   if (colorDialog_->picker[ButText]->isShown())
   {
      QWidget::repaint(buttonTextRect, false);
      return;
   }
}

QImage & DemoWindow::tintButton(QImage &src, QColor & c)
{
//    dest = QImage( src.width(), src.height(), 32, 0, _ENDIAN_ );
   unsigned int *data = ( unsigned int * ) src.bits();
   unsigned int *destData = ( unsigned int* ) dest.bits();
   int total = src.width() * src.height();
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
      blue = qBlue( data[ current ] );
      red = qRed( data[ current ] );
      green = qGreen( data[ current ] );
      SATURATION_COLOR2(sq, red, green, blue);
	// force back to valid colorspace !
      COLOR_SPACE(destR, destG, destB);
      destData[ current ] = qRgba( destR, destG, destB, alpha );
   }
   return ( dest );
}
