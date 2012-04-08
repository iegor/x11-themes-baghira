#include <kdialog.h>
#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './linkconfig.ui'
**
** Created: Sun Apr 8 22:00:18 2012
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "linkconfig.h"

#include <qvariant.h>
#include <qlabel.h>
#include <klineedit.h>
#include <kurlrequester.h>
#include <qpushbutton.h>
#include <kicondialog.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "klineedit.h"
#include "kurlrequester.h"
#include "kpushbutton.h"
#include "kicondialog.h"

/*
 *  Constructs a LinkConfig as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
LinkConfig::LinkConfig( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "LinkConfig" );
    setSizeGripEnabled( TRUE );
    LinkConfigLayout = new QGridLayout( this, 1, 1, 11, 6, "LinkConfigLayout"); 

    layout11 = new QVBoxLayout( 0, 0, 6, "layout11"); 

    textLabel1 = new QLabel( this, "textLabel1" );
    layout11->addWidget( textLabel1 );

    title = new KLineEdit( this, "title" );
    layout11->addWidget( title );

    textLabel2 = new QLabel( this, "textLabel2" );
    textLabel2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, textLabel2->sizePolicy().hasHeightForWidth() ) );
    layout11->addWidget( textLabel2 );

    url = new KURLRequester( this, "url" );
    layout11->addWidget( url );

    LinkConfigLayout->addLayout( layout11, 0, 0 );

    layout10 = new QHBoxLayout( 0, 0, 6, "layout10"); 
    Horizontal_Spacing2 = new QSpacerItem( 206, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout10->addItem( Horizontal_Spacing2 );

    buttonOk = new QPushButton( this, "buttonOk" );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    layout10->addWidget( buttonOk );

    buttonCancel = new QPushButton( this, "buttonCancel" );
    buttonCancel->setAutoDefault( TRUE );
    layout10->addWidget( buttonCancel );

    LinkConfigLayout->addMultiCellLayout( layout10, 1, 1, 0, 1 );

    icon = new KIconButton( this, "icon" );
    icon->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, icon->sizePolicy().hasHeightForWidth() ) );
    icon->setMinimumSize( QSize( 64, 64 ) );
    icon->setMaximumSize( QSize( 64, 64 ) );
    icon->setIconSize( 48 );
    icon->setStrictIconSize( FALSE );

    LinkConfigLayout->addWidget( icon, 0, 1 );
    languageChange();
    resize( QSize(403, 164).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
LinkConfig::~LinkConfig()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void LinkConfig::languageChange()
{
    setCaption( tr2i18n( "Configure Link" ) );
    textLabel1->setText( tr2i18n( "<b>Title</b>" ) );
    textLabel2->setText( tr2i18n( "<b>URL</b>" ) );
    buttonOk->setText( tr2i18n( "&OK" ) );
    buttonOk->setAccel( QKeySequence( QString::null ) );
    buttonCancel->setText( tr2i18n( "&Cancel" ) );
    buttonCancel->setAccel( QKeySequence( QString::null ) );
    icon->setText( QString::null );
}

#include "linkconfig.moc"
