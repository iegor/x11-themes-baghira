/****************************************************************************
** Form interface generated from reading ui file './about.ui'
**
** Created: Sun Apr 8 21:58:24 2012
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef ABOUT_H
#define ABOUT_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QPushButton;

class About : public QDialog
{
    Q_OBJECT

public:
    About( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~About();

    QLabel* crLabel;
    QPushButton* buttonInfo;
    QLabel* textLabel2_2;
    QLabel* cpuLabel;
    QLabel* kdeLabel;
    QLabel* textLabel5;
    QLabel* systemLabel;
    QLabel* textLabel1_2;
    QLabel* textLabel3;
    QPushButton* buttonDownload;
    QLabel* textLabel2;
    QLabel* versionLabel;
    QLabel* textLabel1;

protected:
    QGridLayout* AboutLayout;
    QSpacerItem* spacer19;
    QSpacerItem* spacer24;
    QSpacerItem* spacer25;
    QSpacerItem* spacer19_2;
    QSpacerItem* spacer19_4;
    QSpacerItem* spacer19_5;
    QSpacerItem* spacer26;
    QHBoxLayout* layout4;
    QSpacerItem* spacer3_2;
    QHBoxLayout* layout11;
    QHBoxLayout* layout6;
    QSpacerItem* spacer7;
    QSpacerItem* spacer8;
    QGridLayout* layout11_2;
    QSpacerItem* spacer20;
    QSpacerItem* spacer2;
    QSpacerItem* spacer18;
    QSpacerItem* spacer19_6;
    QSpacerItem* spacer1;
    QSpacerItem* spacer9_4;
    QSpacerItem* spacer17;
    QSpacerItem* spacer19_3;
    QSpacerItem* spacer9_3;
    QSpacerItem* spacer9_2;
    QSpacerItem* spacer9;
    QHBoxLayout* layout5;
    QSpacerItem* spacer5_2;
    QSpacerItem* spacer5;
    QHBoxLayout* layout8;
    QHBoxLayout* layout10;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;

};

#endif // ABOUT_H
