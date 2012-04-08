#include "colorpicker.h"
#include <qcolor.h>
#include <qlayout.h>
#include <qslider.h>
#include <qspinbox.h>

ColorPicker::ColorPicker(QWidget* parent, const char* name) : QGroupBox( parent, name){
//    box = new QGroupBox(parent);
//     setFrameShape(QFrame::GroupBoxPanel);
//     setFrameShadow(QFrame::Sunken);
    setColumnLayout(0, Qt::Vertical );
    layout()->setSpacing( 6 );
    layout()->setMargin( 11 );
    
    QVBoxLayout *vLayout = new QVBoxLayout(layout());
//     gridLayout->setAlignment( Qt::AlignTop );
    
    QHBoxLayout *redLayout = new QHBoxLayout();
    redSlider = new QSlider(0, 255, 1, 0, Qt::Horizontal, this, "redSlider");
    redLayout->addWidget(redSlider);
    
    redValue = new QSpinBox(0,255,1,this);
    redValue->setValue(0);
    redLayout->addWidget(redValue);
    
    vLayout->addLayout(redLayout);
    
    QHBoxLayout *greenLayout = new QHBoxLayout();
    greenSlider = new QSlider(0, 255, 1, 0, Qt::Horizontal, this, "greenSlider");
    greenLayout->addWidget(greenSlider);
    
    greenValue = new QSpinBox(0,255,1,this);
    greenValue->setValue(0);
    greenLayout->addWidget(greenValue);
    
    vLayout->addLayout(greenLayout);
    
    QHBoxLayout *blueLayout = new QHBoxLayout();
    blueSlider = new QSlider(0, 255, 1, 0, Qt::Horizontal, this, "blueSlider");
    blueLayout->addWidget(blueSlider);
    
    blueValue = new QSpinBox(0,255,1,this);
    blueValue->setValue(0);
    blueLayout->addWidget(blueValue);
    
    vLayout->addLayout(blueLayout);
    
//     resize( QSize(350, 100).expandedTo(minimumSizeHint()) );
    
    //connections
    connect(redSlider, SIGNAL(valueChanged (int)), this, SLOT(setRed(int)));
    connect(greenSlider, SIGNAL(valueChanged (int)), this, SLOT(setGreen(int)));
    connect(blueSlider, SIGNAL(valueChanged (int)), this, SLOT(setBlue(int)));
    connect(redValue, SIGNAL(valueChanged (int)), this, SLOT(setRed(int)));
    connect(greenValue, SIGNAL(valueChanged (int)), this, SLOT(setGreen(int)));
    connect(blueValue, SIGNAL(valueChanged (int)), this, SLOT(setBlue(int)));
}

ColorPicker::~ColorPicker(){
}

void ColorPicker::setColor(QColor color){
    redSlider->blockSignals(true);
    redSlider->setValue( color.red() );
    redSlider->blockSignals(false);
    redValue->blockSignals(true);
    redValue->setValue(redSlider->value());
    redValue->blockSignals(false);
    greenSlider->blockSignals(true);
    greenSlider->setValue( color.green() );
    greenSlider->blockSignals(false);
    greenValue->blockSignals(true);
    greenValue->setValue(greenSlider->value());
    greenValue->blockSignals(false);
    blueSlider->blockSignals(true);
    blueSlider->setValue( color.blue() );
    blueSlider->blockSignals(false);
    blueValue->blockSignals(true);
    blueValue->setValue(blueSlider->value());
    blueValue->blockSignals(false);
    emit colorChanged(color);
}

void ColorPicker::setRed(int red){
    redSlider->blockSignals(true);
    redSlider->setValue( red );
    redSlider->blockSignals(false);
    redValue->blockSignals(true);
    redValue->setValue(red);
    redValue->blockSignals(false);
    emit colorChanged(QColor(red, greenSlider->value(), blueSlider->value()));
}

void ColorPicker::setGreen(int green){
    greenSlider->blockSignals(true);
    greenSlider->setValue( green );
    greenSlider->blockSignals(false);
    greenValue->blockSignals(true);
    greenValue->setValue(green);
    greenValue->blockSignals(false);
    emit colorChanged(QColor(redSlider->value(), green, blueSlider->value()));
}

void ColorPicker::setBlue(int blue){
    blueSlider->blockSignals(true);
    blueSlider->setValue( blue );
    blueSlider->blockSignals(false);
    blueValue->blockSignals(true);
    blueValue->setValue(blue);
    blueValue->blockSignals(false);
    emit colorChanged(QColor(redSlider->value(), greenSlider->value(), blue));
}

QColor ColorPicker::Color(){
    return QColor(redSlider->value(), greenSlider->value(), blueSlider->value());
}

void ColorPicker::reset(){
    setColor(color_);
}

void ColorPicker::init(){
    color_ = Color();
}