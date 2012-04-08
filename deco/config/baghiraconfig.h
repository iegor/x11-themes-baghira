//////////////////////////////////////////////////////////////////////////////
// baghiraconfig.h
// -------------------
// Config module for Baghira window decoration
// -------------------
// Copyright (c) 2004 Thomas Lübking
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

#ifndef BAGHIRACONFIG_H
#define BAGHIRACONFIG_H

#include <qobject.h>
#include <qdialog.h>
#include "colorpicker.h"
#include "aquariusbutton.h"

class KConfig;
class QSettings;
class ConfigDialog;
class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QPushButton;
class QComboBox;

class ButtonColors : public QDialog
{
    Q_OBJECT
public:
    ButtonColors(QWidget* parent, const char * name = 0);
    ~ButtonColors();
    ColorPicker *inactiveColor;
    ColorPicker *closeColor;
    ColorPicker *minColor;
    ColorPicker *maxColor;
    ColorPicker *menuColor;
    ColorPicker *helpColor;
    ColorPicker *stickyColor;
    ColorPicker *aboveColor;
    ColorPicker *behindColor;
    ColorPicker *shadeColor;
    ColorPicker *currentPicker;
    
    AquariusButton *inactiveButton;
    AquariusButton *closeButton;
    AquariusButton *minButton;
    AquariusButton *maxButton;
    AquariusButton *menuButton;
    AquariusButton *helpButton;
    AquariusButton *stickyButton;
    AquariusButton *aboveButton;
    AquariusButton *behindButton;
    AquariusButton *shadeButton;
    
    QHBoxLayout *twoButts;
    QPushButton *cancel;
    QPushButton *ok;
    QComboBox *presets_;
    bool save;
    
public slots:
//     void defaults();
    void setColorGroup(int);
    void init();
    
    
private:

/*
    |------------|---|
    | Indicator  | B |
    |------------|---|
    |            | T |
    |   Picker   |---|
    |            | N |
    |------------|---|
*/

    QVBoxLayout *buttonLayout;
    QVBoxLayout *pickerLayout;
    QGridLayout *layout;
    QLabel *indicator;
    
    void setPicker(ColorPicker *picker);
    
private slots:
    void activateInactive();
    void activateClose();
    void activateMin();
    void activateMax();
    void activateMenu();
    void activateHelp();
    void activateSticky();
    void activateAbove();
    void activateBehind();
    void activateShade();
    void setSave(){
        save = true;
    }
    void reset();
};


class BaghiraConfig : public QObject
{
    Q_OBJECT
public:
    BaghiraConfig(KConfig* config, QWidget* parent);
    ~BaghiraConfig();
    
signals:
    void changed();

public slots:
    void load(KConfig* conf);
    void save(KConfig* conf);
    void defaults();

private:
//     QSettings *config_;
    ConfigDialog *dialog_;
    ButtonColors *buttonDialog_;
};


#endif // BAGHIRACONFIG_H
