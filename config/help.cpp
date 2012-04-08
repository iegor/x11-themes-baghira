#include <kdialog.h>
#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './help.ui'
**
** Created: Sun Apr 8 21:58:30 2012
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "help.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <kactivelabel.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "kactivelabel.h"

/*
 *  Constructs a Help as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
Help::Help( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "Help" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)5, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setMaximumSize( QSize( 32767, 32767 ) );
    setSizeGripEnabled( FALSE );
    HelpLayout = new QGridLayout( this, 1, 1, 11, 6, "HelpLayout"); 

    layout2 = new QHBoxLayout( 0, 0, 6, "layout2"); 
    spacer2_2 = new QSpacerItem( 60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout2->addItem( spacer2_2 );

    closeButton = new QPushButton( this, "closeButton" );
    layout2->addWidget( closeButton );

    HelpLayout->addLayout( layout2, 1, 0 );

    tabWidget2 = new QTabWidget( this, "tabWidget2" );

    tab = new QWidget( tabWidget2, "tab" );
    tabLayout = new QGridLayout( tab, 1, 1, 11, 6, "tabLayout"); 

    layout3 = new QHBoxLayout( 0, 0, 6, "layout3"); 
    spacer3 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout3->addItem( spacer3 );

    startBAB = new QPushButton( tab, "startBAB" );
    layout3->addWidget( startBAB );
    spacer4 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout3->addItem( spacer4 );

    tabLayout->addLayout( layout3, 1, 0 );

    kActiveLabel2 = new KActiveLabel( tab, "kActiveLabel2" );
    kActiveLabel2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, kActiveLabel2->sizePolicy().hasHeightForWidth() ) );
    kActiveLabel2->setResizePolicy( KActiveLabel::Manual );
    kActiveLabel2->setVScrollBarMode( KActiveLabel::Auto );
    kActiveLabel2->setHScrollBarMode( KActiveLabel::Auto );

    tabLayout->addWidget( kActiveLabel2, 0, 0 );
    tabWidget2->insertTab( tab, QString::fromLatin1("") );

    tab_2 = new QWidget( tabWidget2, "tab_2" );
    tabLayout_2 = new QGridLayout( tab_2, 1, 1, 11, 6, "tabLayout_2"); 

    kActiveLabel4 = new KActiveLabel( tab_2, "kActiveLabel4" );

    tabLayout_2->addWidget( kActiveLabel4, 0, 0 );
    tabWidget2->insertTab( tab_2, QString::fromLatin1("") );

    tab_3 = new QWidget( tabWidget2, "tab_3" );
    tabLayout_3 = new QGridLayout( tab_3, 1, 1, 11, 6, "tabLayout_3"); 

    kActiveLabel3 = new KActiveLabel( tab_3, "kActiveLabel3" );

    tabLayout_3->addWidget( kActiveLabel3, 0, 0 );
    tabWidget2->insertTab( tab_3, QString::fromLatin1("") );

    tab_4 = new QWidget( tabWidget2, "tab_4" );
    tabLayout_4 = new QGridLayout( tab_4, 1, 1, 11, 6, "tabLayout_4"); 

    kActiveLabel1 = new KActiveLabel( tab_4, "kActiveLabel1" );
    kActiveLabel1->setVScrollBarMode( KActiveLabel::Auto );
    kActiveLabel1->setHScrollBarMode( KActiveLabel::Auto );

    tabLayout_4->addWidget( kActiveLabel1, 1, 0 );

    textLabel1 = new QLabel( tab_4, "textLabel1" );
    QFont textLabel1_font(  textLabel1->font() );
    textLabel1_font.setBold( TRUE );
    textLabel1->setFont( textLabel1_font ); 

    tabLayout_4->addWidget( textLabel1, 0, 0 );
    tabWidget2->insertTab( tab_4, QString::fromLatin1("") );

    HelpLayout->addWidget( tabWidget2, 0, 0 );
    languageChange();
    resize( QSize(457, 720).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( closeButton, SIGNAL( clicked() ), this, SLOT( close() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
Help::~Help()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void Help::languageChange()
{
    setCaption( tr2i18n( "Baghira Help" ) );
    closeButton->setText( tr2i18n( "Close" ) );
    startBAB->setText( tr2i18n( "Start BAB" ) );
    kActiveLabel2->setText( tr2i18n( "<b>\"BAB\"</b> is a little Systray Application to give you more control about the look of your Baghiranced Desktop.<br>\n"
"You can start if from a shell or the KDE executor or by just simply clicking the button below.<br>\n"
"<br>\n"
"<b>How does it work?</b><br>\n"
"a) Simply leftclick the icon to switch between the different generations ( Blue:Jaguar, Black:Panther, Grey:Brushed Metal, Yellow:Tiger, White:Milk)<br><br>\n"
"b) Righclick to get a config menu, providing direct acces to Baghiras config dialog<br><br>\n"
"c) BAB is also a DCOP interface to the Baghira Style and Windeco" ) );
    tabWidget2->changeTab( tab, tr2i18n( "BAB" ) );
    kActiveLabel4->setText( tr2i18n( "<br><b>How to acces bab via DCOP</br>\n"
"<br>Well, if you're a coder, i assume you know hot to access the dcop server from within your code\n"
"<br>If you're a user, the main way to acces dcop is the <b>dcop command line tool</b><br><br>\n"
"To see, which functions are provided by bab (the list will probably grow), open a shell and enter<br><br>\n"
"<i><b>dcop bab default</b></i><br>\n"
"<br>\n"
"The most interesting function is<br><br>\n"
"<i><b>start(QString name,QString settings)</b></i><br>\n"
"<br>\n"
"use it like:<br><br>\n"
"<i><b>start &lt;appName&gt; &lt;design&gt;</b></i><br><br>\n"
"where <i><b>&lt;appName&gt;</b></i> is the (KDE) application command (try \"konqueror\") you want to start and <i><b>&lt;design&gt;</b></i> is a predefined Design you set in the rules section (Baghiracomes with \"Jaguar\", \"Panther\", \"Brushed\", \"Tiger\" and \"Milk\")" ) );
    tabWidget2->changeTab( tab_2, tr2i18n( "DCOP" ) );
    kActiveLabel3->setText( tr2i18n( "The new coloring Function <i>\"Aquarius\"</i> replaces the former \"<i>icy</i>\" style.\n"
"<br>Though you can use any color, i suggest to avoid very high or very low saturated colors.\n"
"<br>- High saturation will lead to a flat look\n"
"<br>- Low saturation will look uncolored\n"
"<br>\n"
"<br>I suggest to use a button color like rgb = 84,140,242 as a starting Point and modify the color to yoour wishes\n"
"<br><b>Notice</b> that the coloring function is probably not fixed yet - especially not in the high saturated area\n"
"<br>So please do not complain if you worked hard to optimize the color and get a different result in one of the next versions.\n"
"<br>\n"
"<br>Additionally, there are two color schemes by Christian Lehmann included." ) );
    tabWidget2->changeTab( tab_3, tr2i18n( "Coloring" ) );
    kActiveLabel1->setText( tr2i18n( "<b>Apple Computer, Inc.</b><br>For being such great Designers =)<p>\n"
"<b>Christian Lehmann</b><br>For the SuSE binaries and Supporting my Ego<p>\n"
"<b>Elias Probst</b><br>For the Gentoo ebuild<p>\n"
"<b>Mosfet</b> (Of Course)<br>For the initial Liquid Code<p>\n"
"<b>Narcis Tesa</b><br>For the Mandrake binaries and being the worst nitpicker on this Planet<p><b>Timur</b><br>For the cute icons<p>\n"
"Everyone else who supported me with suggestions, bug reports and Help on the fores.<p>\n"
"<a href=\"http://www.kde-look.org/\">KDE-Look</a><br>\n"
"and<br>\n"
"<a href=\"http://www.sf.net/\">Sourceforge</a><br>\n"
"for hosting the Project" ) );
    textLabel1->setText( tr2i18n( "Thanks To (alphabetical):" ) );
    tabWidget2->changeTab( tab_4, tr2i18n( "Credits" ) );
}

#include "help.moc"
