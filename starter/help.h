/****************************************************************************
** Form interface generated from reading ui file './help.ui'
**
** Created: Sun Apr 8 21:59:47 2012
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class KTextEdit;
class QPushButton;

class HelpDialog : public QDialog
{
    Q_OBJECT

public:
    HelpDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~HelpDialog();

    KTextEdit* kTextEdit2;
    QPushButton* buttonClose;

protected:
    QGridLayout* HelpDialogLayout;
    QSpacerItem* spacer4;

protected slots:
    virtual void languageChange();

};

#endif // HELPDIALOG_H
