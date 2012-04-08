#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <qgroupbox.h>

class KIntNumInput;
class QColor;

class ColorPicker : public QGroupBox
{
    Q_OBJECT

public:
    ColorPicker( QWidget* parent = 0, const char* name = 0);
    ~ColorPicker();
    QColor & color();

public slots:
    void setColor(const QColor & color);
    void setRed(int red);
   void emitChange();
    void setGreen(int green);
    void setBlue(int blue);
    void reset();
    void init();
    
protected:
   KIntNumInput *red;
   KIntNumInput *green;
   KIntNumInput *blue;
   QColor color_;
   QColor color__;

signals:
    void colorChanged(QColor color);

};

#endif // COLORPICKER_H
