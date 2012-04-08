#include "colorpicker.h"
#include <qcolor.h>
#include <qlayout.h>
#include <knuminput.h>

ColorPicker::ColorPicker(QWidget* parent, const char* name) : QGroupBox( parent, name)
{
   setTitle(name);
   setColumnLayout(0, Qt::Vertical );
   layout()->setSpacing( 6 );
   layout()->setMargin( 11 );
    
   QVBoxLayout *vLayout = new QVBoxLayout(layout());
    
   red = new KIntNumInput(this, "red");
   red->setRange(0, 255, 1, true);
   red->setLabel("R");
   vLayout->addWidget(red);
   green = new KIntNumInput(this, "green");
   green->setRange(0, 255, 1, true);
   green->setLabel("G");
   vLayout->addWidget(green);
   blue = new KIntNumInput(this, "blue");
   blue->setRange(0, 255, 1, true);
   blue->setLabel("B");
   vLayout->addWidget(blue);
   connect (red, SIGNAL(valueChanged( int )), this, SLOT(emitChange()));
   connect (green, SIGNAL(valueChanged( int )), this, SLOT(emitChange()));
   connect (blue, SIGNAL(valueChanged( int )), this, SLOT(emitChange()));
}

ColorPicker::~ColorPicker()
{
}

void ColorPicker::emitChange()
{
   emit colorChanged(QColor(red->value(), green->value(), blue->value()));
}

void ColorPicker::setColor(const QColor & color)
{
    red->setValue( color.red() );
    green->setValue( color.green() );
    blue->setValue( color.blue() );
    emit colorChanged(color);
}

void ColorPicker::setRed(int r)
{
    red->setValue( r );
    emit colorChanged(QColor(r, green->value(), blue->value()));
}

void ColorPicker::setGreen(int g)
{
    green->setValue( g );
    emit colorChanged(QColor(red->value(), g, blue->value()));
}

void ColorPicker::setBlue(int b)
{
    blue->setValue( b );
    emit colorChanged(QColor(red->value(), green->value(), b));
}

QColor & ColorPicker::color()
{
   color__ = QColor(red->value(), green->value(), blue->value());
   return color__;
}

void ColorPicker::reset()
{
    setColor(color_);
}

void ColorPicker::init()
{
    color_ = color();
}
