/****************************************************************************
** Form interface generated from reading ui file './starterhelp.ui'
**
** Created: Sun Apr 8 21:59:47 2012
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef STARTERHELP_H
#define STARTERHELP_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QTabWidget;
class QWidget;
class QLabel;

class StarterHelp : public QDialog
{
    Q_OBJECT

public:
    StarterHelp( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~StarterHelp();

    QTabWidget* tabWidget2;
    QWidget* tab;
    QLabel* textLabel5;
    QWidget* tab_2;
    QLabel* textLabel6;
    QWidget* TabPage;
    QLabel* textLabel1;

protected:
    QGridLayout* StarterHelpLayout;
    QGridLayout* tabLayout;
    QGridLayout* tabLayout_2;
    QGridLayout* TabPageLayout;

protected slots:
    virtual void languageChange();

};

#endif // STARTERHELP_H
