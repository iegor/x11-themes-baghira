/****************************************************************************
** Form interface generated from reading ui file './linkconfig.ui'
**
** Created: Sun Apr 8 21:59:47 2012
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef LINKCONFIG_H
#define LINKCONFIG_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class KIconButton;
class QLabel;
class KLineEdit;
class QLineEdit;

class LinkConfig : public QDialog
{
    Q_OBJECT

public:
    LinkConfig( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~LinkConfig();

    QPushButton* buttonOk;
    QPushButton* buttonCancel;
    KIconButton* icon;
    QLabel* textLabel1;
    KLineEdit* title;
    QLabel* textLabel2;
    QLineEdit* command;

protected:
    QGridLayout* LinkConfigLayout;
    QHBoxLayout* layout10;
    QSpacerItem* Horizontal_Spacing2;
    QVBoxLayout* layout3;

protected slots:
    virtual void languageChange();

};

#endif // LINKCONFIG_H
