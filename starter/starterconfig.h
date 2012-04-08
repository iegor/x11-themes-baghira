/****************************************************************************
** Form interface generated from reading ui file './starterconfig.ui'
**
** Created: Sun Apr 8 21:59:47 2012
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef STARTERCONFIG_H
#define STARTERCONFIG_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QGroupBox;
class KURLRequester;
class QLabel;
class QPushButton;
class QButtonGroup;
class QCheckBox;
class QSpinBox;
class QRadioButton;
class QFrame;
class QComboBox;
class QListBox;
class QListBoxItem;
class KKeyButton;

class StarterConfig : public QDialog
{
    Q_OBJECT

public:
    StarterConfig( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~StarterConfig();

    QGroupBox* groupBox5;
    KURLRequester* BaseURL;
    QLabel* textLabel2_3;
    KURLRequester* HoverURL;
    QLabel* textLabel1_2;
    QLabel* textLabel3;
    KURLRequester* DownURL;
    QPushButton* buttonHelp;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;
    QButtonGroup* buttonGroup1;
    QCheckBox* customDialogSize;
    QSpinBox* dialogW;
    QLabel* textLabel2;
    QSpinBox* dialogH;
    QRadioButton* dialogFollowMouse;
    QRadioButton* fixedDialogPos;
    QSpinBox* dialogX;
    QLabel* textLabel2_2;
    QSpinBox* dialogY;
    QCheckBox* customDialogPos;
    QFrame* line1;
    QCheckBox* showDialogTitlebar;
    QFrame* line2;
    QFrame* line2_3;
    QLabel* textLabel1_3;
    QComboBox* dialogPanelPos;
    QGroupBox* groupBox6;
    QCheckBox* customPopupSize;
    QSpinBox* popupW;
    QLabel* textLabel1;
    QSpinBox* popupH;
    QFrame* line2_2;
    QLabel* textLabel1_3_2;
    QComboBox* popupPanelPos;
    QGroupBox* groupBox4;
    QLabel* textLabel2_4;
    QSpinBox* favItemAmount;
    QFrame* line1_2;
    QCheckBox* useKTTS;
    QLabel* textLabel1_4;
    QComboBox* iconSize;
    QGroupBox* groupBox7;
    QListBox* categoryList;
    KKeyButton* buttonShortcut;

protected:
    QGridLayout* StarterConfigLayout;
    QGridLayout* groupBox5Layout;
    QHBoxLayout* Layout1;
    QSpacerItem* Horizontal_Spacing2;
    QGridLayout* buttonGroup1Layout;
    QHBoxLayout* layout10;
    QSpacerItem* spacer4;
    QHBoxLayout* layout12;
    QSpacerItem* spacer5;
    QHBoxLayout* layout6;
    QGridLayout* groupBox6Layout;
    QHBoxLayout* layout11;
    QSpacerItem* spacer3;
    QHBoxLayout* layout6_2;
    QGridLayout* groupBox4Layout;
    QHBoxLayout* layout9;
    QSpacerItem* spacer7;
    QHBoxLayout* layout9_2;
    QGridLayout* groupBox7Layout;
    QHBoxLayout* layout18;
    QSpacerItem* spacer14;
    QSpacerItem* spacer15;

protected slots:
    virtual void languageChange();

};

#endif // STARTERCONFIG_H
