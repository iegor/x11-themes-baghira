/****************************************************************************
** Form interface generated from reading ui file './config.ui'
**
** Created: Sun Apr 8 21:59:47 2012
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QGroupBox;
class KComboBox;
class QLabel;
class KLineEdit;
class KURLRequester;
class KIconButton;
class QCheckBox;
class KTextEdit;
class QFrame;
class KKeyButton;

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    ConfigDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ConfigDialog();

    QPushButton* buttonHelp;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;
    QPushButton* buttonDetail;
    QGroupBox* baseGroup;
    KComboBox* category;
    QLabel* textLabel2;
    KLineEdit* appName;
    KURLRequester* command;
    KLineEdit* genericName;
    KIconButton* iconButton;
    QLabel* textLabel5;
    QLabel* textLabel1_2;
    QLabel* textLabel1;
    QLabel* textLabel4;
    KLineEdit* keywords;
    QCheckBox* startupFeedback;
    QCheckBox* showInSystray;
    KTextEdit* description;
    QLabel* textLabel3;
    QFrame* line4;
    QGroupBox* extendedGroup;
    QLabel* textLabel7;
    QFrame* line1;
    QCheckBox* startInTerminal;
    QLabel* textLabel9;
    KLineEdit* terminalSettings;
    QFrame* line2;
    QCheckBox* startAsUser;
    QLabel* textLabel6;
    KLineEdit* username;
    QFrame* line3;
    QLabel* textLabel8;
    KURLRequester* workingDir;
    KKeyButton* shortcut;

protected:
    QGridLayout* ConfigDialogLayout;
    QHBoxLayout* Layout1;
    QSpacerItem* Horizontal_Spacing2;
    QGridLayout* baseGroupLayout;
    QGridLayout* extendedGroupLayout;
    QSpacerItem* spacer2;
    QSpacerItem* spacer3;

protected slots:
    virtual void languageChange();

};

#endif // CONFIGDIALOG_H
