#ifndef COLORPICKER_H
#define COLORPICKER_H

//#include <qvariant.h>
#include <qgroupbox.h>

class QSlider;
class QSpinBox;
class QColor;
//class QGroupBox;

class ColorPicker : public QGroupBox
{
    Q_OBJECT

public:
    ColorPicker( QWidget* parent = 0, const char* name = 0);
    ~ColorPicker();
    QColor Color();

public slots:
    void setColor(QColor color);
    void setRed(int red);
    void setGreen(int green);
    void setBlue(int blue);
    void reset();
    void init();
    
protected:
    QSlider *redSlider;
    QSlider *greenSlider;
    QSlider *blueSlider;
    QSpinBox *redValue;
    QSpinBox *greenValue;
    QSpinBox *blueValue;
    QColor color_;
//    QGroupBox *box;

signals:
    void colorChanged(QColor color);

};

#endif // COLORPICKER_H
