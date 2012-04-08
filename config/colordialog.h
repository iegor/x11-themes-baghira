#ifndef COLORDIALOG_H
#define COLORDIALOG_H

#include <qdialog.h>
#include <qrect.h>
#include <qimage.h>

class QPushButton;
class QComboBox;
class QGroupBox;
class QPainter;
class ColorDialog;
class QImage;
class QPixmap;

enum Colors { Back = 0, Button, Base, Text, High, HighText, ButText, Alternate, NUMCOLORS };

class DemoWindow : public QWidget
{
   Q_OBJECT
      
public:
   DemoWindow( ColorDialog* parent = 0, const char* name = 0);
   ~DemoWindow();

private slots:
   void smartRepaint();
   
protected:
   void mousePressEvent ( QMouseEvent * e );
   void paintEvent ( QPaintEvent * );
   
private:
   QPainter *p;
   QPainter *pp;
   QPixmap *pm;
   ColorDialog* colorDialog_;
   QRect baseRect;
   QRect buttonRect;
   QRect buttonTextRect;
   QRect highlightRect;
   QRect textRect;
   QRect highTextRect;
   QRect alternateRect;
   QImage baseImage;
   QImage dest;
   
private:
   QImage & tintButton(QImage &src, QColor & c);
   
signals:
   void selected(Colors c);
   
};

class ColorPicker;

class ColorDialog : public QDialog
{
   friend class DemoWindow;
   friend class AppSetter;
    Q_OBJECT

public:
   ColorDialog( QWidget* parent = 0, const char* name = 0);
   ~ColorDialog();
   QPushButton *buttonOk;
   QPushButton *buttonCancel;
   void show();

protected:
   ColorPicker *picker[NUMCOLORS];
   void mouseReleaseEvent(QMouseEvent* me);
   DemoWindow *demo;
   
private:
   QPushButton *buttonCP;
   bool gettingColorFromScreen;
   QComboBox *other;
   
private slots:
   void grabColor();
   void getFromOther( const QString & string );

signals:
    void colorChanged(QColor color);

};

#endif // COLORPICKER_H
