#ifndef AQUARIUSBUTTON_H
#define AQUARIUSBUTTON_H

//#include <qvariant.h>
#include <qwidget.h>
#include <qimage.h>

class QPixmap;
class QColor;

class AquariusButton : public QWidget
{
    Q_OBJECT

public:
    AquariusButton( QPixmap &pixmap, QWidget* parent = 0, const char* name = 0);
    ~AquariusButton();
    QColor Color();

public slots:
    void setColor(QColor c);
    
protected:
    QPixmap pixmap;
    QImage image;
    QColor color;
    void tint(QColor &c);
    void mousePressEvent( QMouseEvent *e );
    void paintEvent( QPaintEvent *e);


signals:
    void clicked();

};

#endif // AQUARIUSBUTTON_H 
