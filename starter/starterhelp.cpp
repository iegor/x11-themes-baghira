#include <kdialog.h>
#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './starterhelp.ui'
**
** Created: Sun Apr 8 21:59:54 2012
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "starterhelp.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a StarterHelp as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
StarterHelp::StarterHelp( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "StarterHelp" );
    StarterHelpLayout = new QGridLayout( this, 1, 1, 11, 6, "StarterHelpLayout"); 

    tabWidget2 = new QTabWidget( this, "tabWidget2" );

    tab = new QWidget( tabWidget2, "tab" );
    tabLayout = new QGridLayout( tab, 1, 1, 11, 6, "tabLayout"); 

    textLabel5 = new QLabel( tab, "textLabel5" );

    tabLayout->addWidget( textLabel5, 0, 0 );
    tabWidget2->insertTab( tab, QString::fromLatin1("") );

    tab_2 = new QWidget( tabWidget2, "tab_2" );
    tabLayout_2 = new QGridLayout( tab_2, 1, 1, 11, 6, "tabLayout_2"); 

    textLabel6 = new QLabel( tab_2, "textLabel6" );

    tabLayout_2->addWidget( textLabel6, 0, 0 );
    tabWidget2->insertTab( tab_2, QString::fromLatin1("") );

    TabPage = new QWidget( tabWidget2, "TabPage" );
    TabPageLayout = new QGridLayout( TabPage, 1, 1, 11, 6, "TabPageLayout"); 

    textLabel1 = new QLabel( TabPage, "textLabel1" );

    TabPageLayout->addWidget( textLabel1, 0, 0 );
    tabWidget2->insertTab( TabPage, QString::fromLatin1("") );

    StarterHelpLayout->addWidget( tabWidget2, 0, 0 );
    languageChange();
    resize( QSize(510, 582).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
StarterHelp::~StarterHelp()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void StarterHelp::languageChange()
{
    setCaption( tr2i18n( "Baghira Starter Config Help" ) );
    textLabel5->setText( tr2i18n( "<qt>The filterline provides several functions\n"
"<ul>\n"
"<li>It filters the below entry list</li>\n"
"<li>It autocompletes to executable entries in $PATH</li>\n"
"<li>The applied listbox lets you select the available categories</li>\n"
"<li>You can navigate through the categories either by using the mousewheel or by holding <b>ctrl+up/down</b></li>\n"
"<li>The category is also selected when calling the applied shortcut</li>\n"
"<li>It supports all protocols you know from Konqueror, e.g. \"gg:Baghira\" will google for \"Baghira\" etc.</li>\n"
"</ul>\n"
"Pressing <b>Enter</b> will execute the (autocompleted) binary in $PATH or the entered kfm protocol call<br>\n"
"Pressing <b>down</b>  will move the keyboard focus to the below entry list\n"
"</qt>" ) );
    tabWidget2->changeTab( tab, tr2i18n( "Filterline" ) );
    textLabel6->setText( tr2i18n( "<qt>The entry field shows up all your menu entries (filtered)<br><br>\n"
"You can start an application by clicking the <b>left mouse button</b> above the item (highlighted text) or by pressing <b>Enter</b> if the entry has the keyboard focus (highlighted background).<br><br>\n"
"You may navigate by either scrolling the <b>mousewheel</b> or using the <b>up/down</b> keys (this will also shift the keyboard focus)<br><br>\n"
"Pressing the <b>left</b> key will put focus on the filterline and select the whole text (i.e. start typing will change a complete new filter action)<br><br>\n"
"Pressing the <b>up</b> key on the topmost item will act as above.</qt>" ) );
    tabWidget2->changeTab( tab_2, tr2i18n( "Entry Field" ) );
    textLabel1->setText( tr2i18n( "<qt>The panel allow you to store often used links, independent from the application links in the entry list<br><br>\n"
"To <b>add a link</b>, either:<br>\n"
"- rightclick the panel<br>\n"
"- drag and drop a link out of the entry list<br>\n"
"- drag and drop any url or text or command (uris and mails are handled, the rest is interpreted as simple command, any command that works on the filterline works here as well)<br>\n"
"<br>\n"
"To <b>remove a link</b>, simply drag it out and drop it outside. (The panel interacts with the linklist from the baghira sidebar, links won't be removed, but copied if dragged from one to the other)<br>\n"
"<br>\n"
"To <b>configure a link</b>, simply rightclick it<br><br>\n"
"To <b>move a link</b>, just drag it and move i around.<br><br>\n"
"\n"
"Te panel is <b>scrollable (mousewheel)</b> and you can configure <b>individual positions</b> for the popup and the dialog<br><br>\n"
"More poofs can be found e.g. here: <a href=\"http://www.resexcellence.com/user_poofs.shtml\">www.resexcellence.com/user_poofs.shtml</a>\n"
"</qt>" ) );
    tabWidget2->changeTab( TabPage, tr2i18n( "Panel" ) );
}

#include "starterhelp.moc"
