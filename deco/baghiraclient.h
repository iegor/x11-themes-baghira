//////////////////////////////////////////////////////////////////////////////
// baghiraclient.h
// -------------------
// Baghira window decoration for KDE
// -------------------
// Copyright (c) 2004, 2005 Thomas Lübking
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//////////////////////////////////////////////////////////////////////////////

#ifndef BAGHIRACLIENT_H
#define BAGHIRACLIENT_H

#include <qcolor.h>
#include <qbutton.h>
#include <kdecoration.h>
#include <kdecorationfactory.h>
#include <kpixmap.h>
#include <kdeversion.h>
/*
#define BUTTONSIZE 15;
#define MENUBUTTONWIDTH 20;
#define DECOSIZE 8;
#define TITLESIZESMALL 18;
#define MARGIN 4;*/

class QSpacerItem;
class QPoint;
class QBitmap;
class KWinModule;

namespace Baghira {


enum PixmapType {
    TitleBar = 0,
    TitleCenter,
    PixmapTypeCount
};

enum ColorType {
    ColorTitleBar=0,
    ColorTitleBlend,
    ColorTypeCount
};

enum ButtonType{
	MinButton = 0,
	MaxButton,
	CloseButton,
	StickyButton,
	ShadeButton,
   AboveBelowButton,
	MenuButton,
	HelpButton,
	ButtonTypeAmount,
	InactiveButton = ButtonTypeAmount
};

enum ButtonStyle{
	Panther = 0,
	Jaguar,
	Milk,
	NostalgiaButton
};

enum ButtonState{
	ButtonInactive,
	ButtonActive,
	Hovered,
	Pressed,
	ButtonStateAmount
};

enum WindowState{
	WindowInactive,
	WindowActive,
	WindowStateAmount
};

enum TitlebarType{
	Gradient,
	Stipples,
	Nostalgia,
	Glossy,
	Brushed,
	FlatStipples
};

class BaghiraClient;

// #if KDE_IS_VERSION(3,3,91)
class ResizeHandle : public QWidget
{
//#if KDE_IS_VERSION(3,3,91)
friend class BaghiraClient;
    Q_OBJECT
#if KDE_IS_VERSION(3,3,91)
public:
    ResizeHandle(BaghiraClient *parent);
protected:
    virtual void mousePressEvent ( QMouseEvent * );
    virtual void mouseReleaseEvent ( QMouseEvent * );
    virtual void paintEvent ( QPaintEvent * );
    void updateLook(bool rp = FALSE);
private:
    bool eventFilter(QObject *obj, QEvent *e);
    void shape();
    BaghiraClient *client;
    QPixmap pix;
#endif
};
// #endif

class DeMaximizer : public QWidget
{
friend class BaghiraClient;
friend class BaghiraFactory;
    Q_OBJECT
public:
    DeMaximizer();
    void setClient(BaghiraClient *);
protected:
    void mouseReleaseEvent( QMouseEvent * e );
    void mousePressEvent ( QMouseEvent * e );
    void enterEvent ( QEvent * e );
    void leaveEvent ( QEvent * e );
    void setPixmap(int i, QImage &image);
private:
    BaghiraClient *client_;
    QPixmap pixmap[5];
    void setState(int i);
};

// BaghiraFactory //////////////////////////////////////////////////////////////

class BaghiraFactory: public KDecorationFactory
{
public:
    BaghiraFactory();
    virtual ~BaghiraFactory();
    virtual KDecoration *createDecoration(KDecorationBridge *b);
    virtual bool reset(unsigned long changed);
    virtual QValueList<BorderSize> borderSizes() const;
    static QPixmap &ButtonPixmap(Baghira::WindowState wState, Baghira::ButtonType type, Baghira::ButtonState state, int current, bool small);
    QImage* DecoImage(QPixmap *pix, int x, int y, int width, int height);

    static QBitmap helpMask, jaguarDownMask, jaguarMenuDownMask,
            jaguarMenuMask, jaguarMask, milkMenuDownMask,
            milkMenuMask, milkMask, pantherMenuMask, pantherMask, milkDownMask, arrowUp, arrowDown;

    static bool initialized();
    static bool noModalDeco();
    static bool delAppname();
    static QColor &color(int current, Baghira::ColorType c, bool active=true);
    static KPixmap &pix(int current, PixmapType p, bool active=true, bool small=false);
    static TitlebarType &effect(int current, bool active);
    static ButtonStyle buttonStyle(int current);
    static Qt::AlignmentFlags titleAlign();
    static int borderSize(int current);
    static int contrast();
    static int _3DImpact(int current);
    static int LineImpact(int current);
    static bool bgStipple();
    static bool resizeGrip();
    static uint defaultMode();
    static bool drawComicFrame();
    static bool shapeUL(int current);
    static bool shapeUR(int current);
    static bool shapeLL(int current);
    static bool shapeLR(int current);
    static bool drawIcon(int current);
    static bool showTitle();
    static bool allowEasyClosing();
    static bool addAutoSpacing();
    static bool maxResizable();
    TitlebarType titleEffect();
    static DeMaximizer &deMaximizer();
    static bool fullSpec();
    static QColor &BrushedMetalColor();

private:
//    bool blend( const QImage & upper, const QImage & lower, QImage & output);
    unsigned long readConfig();
    void createPixmaps();
   void createGradient(KPixmap &pix, const QColor &light, const QColor &dark, Baghira::TitlebarType effect);
    void createOS8Buttons(int style);
    QString indexedString(QString s, int index);
    QImage* tintBrush( const QImage &img, const QColor &c ) const;
    QImage* tintGlossy(const QImage &src, const QColor &c ) const;

private:
    
    static bool initialized_;
    static bool noModalDeco_;
   static bool delAppname_;
    static bool bgStipple_;
    static bool comicFrame_;
    static bool resizeGrip_;
    static bool shape_ul[5], shape_ur[5], shape_ll[5], shape_lr[5];
    static bool allowEasyClosing_;
    static bool addAutoSpacing_;
    static bool maxResizable_;
    static bool drawIcon_[5];
    static ButtonStyle buttonStyle_[5];
    static TitlebarType effect_[5][2];
    static Qt::AlignmentFlags titlealign_;
    static QColor colors_[5][ColorTypeCount][2];
    static KPixmap pix_[5][PixmapTypeCount][2][2];
    static QPixmap ButtonPixmap_[5][WindowStateAmount][ButtonTypeAmount][ButtonStateAmount][2];
    static QColor ButtonColor_[ButtonTypeAmount+1];
    static QColor brushedMetalColor;
    static QPixmap nostalgia_[5][3][2];
    
    static int contrast_;
   static int bordersize_[5];
    static uint defaultMode_;
    static int minimumTitleHeight_;
    static int _3DImpact_[5];
    static int LineImpact_[5];
    
    static bool tintBrush_;
    static bool showTitle_;
    static QColor brushTint;
    static DeMaximizer deMaximizer_;
    static bool fullSpec_;
    
};

inline bool BaghiraFactory::fullSpec()
    { return fullSpec_; }
inline bool BaghiraFactory::noModalDeco()
{ return noModalDeco_; }
inline bool BaghiraFactory::delAppname()
{ return delAppname_; }

inline DeMaximizer &BaghiraFactory::deMaximizer()
    { return deMaximizer_; }

inline bool BaghiraFactory::shapeUL(int current)
    { return shape_ul[current]; }
inline bool BaghiraFactory::shapeUR(int current)
    { return shape_ur[current]; }
inline bool BaghiraFactory::shapeLL(int current)
    { return shape_ll[current]; }
inline bool BaghiraFactory::shapeLR(int current)
    { return shape_lr[current]; }

inline bool BaghiraFactory::allowEasyClosing()
    { return allowEasyClosing_; }

inline bool BaghiraFactory::addAutoSpacing()
    { return addAutoSpacing_; }
    
inline bool BaghiraFactory::maxResizable()
    { return maxResizable_; }
    
inline bool BaghiraFactory::resizeGrip()
    { return resizeGrip_; }
    
inline uint BaghiraFactory::defaultMode()
    { return defaultMode_; }
    
inline bool BaghiraFactory::drawIcon(int current)
    { return drawIcon_[current]; }

inline bool BaghiraFactory::showTitle()
{ return showTitle_; }

inline int BaghiraFactory::contrast()
    { return contrast_; }
    
inline int BaghiraFactory::_3DImpact(int current)
    { return _3DImpact_[current]; }
    
inline int BaghiraFactory::LineImpact(int current)
    { return LineImpact_[current]; }

inline bool BaghiraFactory::drawComicFrame()
    { return comicFrame_; }
    
inline bool BaghiraFactory::bgStipple()
    { return bgStipple_; }

inline ButtonStyle BaghiraFactory::buttonStyle(int current)
    { return BaghiraFactory::buttonStyle_[current]; } 
    
inline bool BaghiraFactory::initialized()
    { return initialized_; }

inline QColor &BaghiraFactory::BrushedMetalColor()
    { return brushedMetalColor; }

inline QColor &BaghiraFactory::color(int current, Baghira::ColorType c, bool active)
    { return colors_[current][c][active]; }

inline KPixmap &BaghiraFactory::pix(int current, PixmapType p, bool active, bool small)
    { return pix_[current][p][active][small]; }

inline Baghira::TitlebarType &BaghiraFactory::effect(int current, bool active)
    { return effect_[current][active]; }

inline Qt::AlignmentFlags BaghiraFactory::titleAlign()
    { return titlealign_; }

inline int BaghiraFactory::borderSize(int current)
{ return bordersize_[current]; }

// BaghiraButton ///////////////////////////////////////////////////////////////

class BaghiraButton : public QButton
{
   Q_OBJECT
public:
    BaghiraButton(BaghiraClient *parent=0, const char *name=0, const QString &tip=NULL, bool small=false, ButtonType type=MinButton, int style=0);
    ~BaghiraButton();

    QSize sizeHint() const;
    int lastMousePress() const;
    void reset();
    inline void setMasks(QBitmap *bitmask, QBitmap *bitmaskDown);
    Baghira::ButtonState state();
    inline void setType(Baghira::ButtonType type)
	{type_ = type; }
    QBitmap *mask;
    QBitmap *maskDown;

private:
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void drawButton(QPainter *painter);
   
private slots:
   void delayedEnterEvent();

private:
   bool _blocked;
   BaghiraClient *client_;
   ButtonType type_;
   QBitmap *deco_;
   QPixmap *pixmap[2][5];
   KPixmap icon_;
   bool small_;
   int lastmouse_;
   bool mouseover_;
   bool down_;
};

inline int BaghiraButton::lastMousePress() const
    { return lastmouse_; }

inline void BaghiraButton::reset()
    { repaint(false); }
    
// BaghiraClient ///////////////////////////////////////////////////////////////

class BaghiraClient : public KDecoration
{
friend class BaghiraButton;
friend class ResizeHandle;
    Q_OBJECT
public:
   BaghiraClient(KDecorationBridge *b, KDecorationFactory *f);
   virtual ~BaghiraClient();

   virtual void init();
   QString caption() const;

   virtual void activeChange();
   virtual void desktopChange();
   virtual void captionChange();
   virtual void iconChange();
   virtual void maximizeChange();
   virtual void shadeChange();
   void demaxButtonPressed();

   virtual void borders(int &l, int &r, int &t, int &b) const;
   virtual void resize(const QSize &size);
   virtual QSize minimumSize() const;
   virtual Position mousePosition(const QPoint &point) const;
   uint currentStyle;

signals:
    void hide();

protected:
    bool isTool() const;
    int  titleheight_;

private:
    void addButtons(QBoxLayout* layout, const QString& buttons);

    bool eventFilter(QObject *obj, QEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void showEvent(QShowEvent *);

private slots:
    void doShape();
    void maxButtonPressed();
    void menuButtonPressed();
    void aboveBelowButtonPressed();
    void shadeButtonPressed();

private:
    BaghiraButton *button[ButtonTypeAmount];
    
    QSpacerItem *titlebar_;
    QPixmap HandlePix;
    int excursion; // skewness because of unequality of left/right buttons - bad for center aligned titles...
    int plusminus;
    bool isFaded;
    bool leftClose;
    bool rightClose;
    bool performEasyClose;
   bool noDeco_;
    QHBoxLayout *windowlayout;
    int ICONX, ICONY;
    ResizeHandle *grip;
   QString caption_;
};

} // namespace Baghira

#endif // BAGHIRACLIENT_H
