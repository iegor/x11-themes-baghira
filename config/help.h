/****************************************************************************
** Form interface generated from reading ui file './help.ui'
**
** Created: Sun Apr 8 21:58:24 2012
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef HELP_H
#define HELP_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QTabWidget;
class QWidget;
class KActiveLabel;
class QLabel;

class Help : public QDialog
{
    Q_OBJECT

public:
    Help( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~Help();

    QPushButton* closeButton;
    QTabWidget* tabWidget2;
    QWidget* tab;
    QPushButton* startBAB;
    KActiveLabel* kActiveLabel2;
    QWidget* tab_2;
    KActiveLabel* kActiveLabel4;
    QWidget* tab_3;
    KActiveLabel* kActiveLabel3;
    QWidget* tab_4;
    KActiveLabel* kActiveLabel1;
    QLabel* textLabel1;

protected:
    QGridLayout* HelpLayout;
    QHBoxLayout* layout2;
    QSpacerItem* spacer2_2;
    QGridLayout* tabLayout;
    QHBoxLayout* layout3;
    QSpacerItem* spacer3;
    QSpacerItem* spacer4;
    QGridLayout* tabLayout_2;
    QGridLayout* tabLayout_3;
    QGridLayout* tabLayout_4;

protected slots:
    virtual void languageChange();

};

#endif // HELP_H
