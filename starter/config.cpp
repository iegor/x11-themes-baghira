#include <kdialog.h>
#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './config.ui'
**
** Created: Sun Apr 8 21:59:55 2012
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "config.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <kcombobox.h>
#include <qlabel.h>
#include <klineedit.h>
#include <kurlrequester.h>
#include <kicondialog.h>
#include <qcheckbox.h>
#include <ktextedit.h>
#include <qframe.h>
#include <kkeybutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "kcombobox.h"
#include "klineedit.h"
#include "kurlrequester.h"
#include "kpushbutton.h"
#include "kicondialog.h"
#include "ktextedit.h"
#include "kkeybutton.h"

/*
 *  Constructs a ConfigDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ConfigDialog::ConfigDialog( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "ConfigDialog" );
    setSizeGripEnabled( TRUE );
    ConfigDialogLayout = new QGridLayout( this, 1, 1, 11, 6, "ConfigDialogLayout"); 

    Layout1 = new QHBoxLayout( 0, 0, 6, "Layout1"); 

    buttonHelp = new QPushButton( this, "buttonHelp" );
    buttonHelp->setAutoDefault( TRUE );
    Layout1->addWidget( buttonHelp );
    Horizontal_Spacing2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( Horizontal_Spacing2 );

    buttonOk = new QPushButton( this, "buttonOk" );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    Layout1->addWidget( buttonOk );

    buttonCancel = new QPushButton( this, "buttonCancel" );
    buttonCancel->setAutoDefault( TRUE );
    Layout1->addWidget( buttonCancel );

    ConfigDialogLayout->addLayout( Layout1, 3, 0 );

    buttonDetail = new QPushButton( this, "buttonDetail" );
    buttonDetail->setToggleButton( TRUE );

    ConfigDialogLayout->addWidget( buttonDetail, 1, 0 );

    baseGroup = new QGroupBox( this, "baseGroup" );
    baseGroup->setFrameShape( QGroupBox::GroupBoxPanel );
    baseGroup->setColumnLayout(0, Qt::Vertical );
    baseGroup->layout()->setSpacing( 6 );
    baseGroup->layout()->setMargin( 11 );
    baseGroupLayout = new QGridLayout( baseGroup->layout() );
    baseGroupLayout->setAlignment( Qt::AlignTop );

    category = new KComboBox( FALSE, baseGroup, "category" );
    category->setEditable( TRUE );
    category->setInsertionPolicy( KComboBox::AfterCurrent );
    category->setAutoCompletion( TRUE );
    category->setDuplicatesEnabled( FALSE );
    category->setContextMenuEnabled( FALSE );

    baseGroupLayout->addMultiCellWidget( category, 2, 2, 1, 2 );

    textLabel2 = new QLabel( baseGroup, "textLabel2" );

    baseGroupLayout->addWidget( textLabel2, 1, 0 );

    appName = new KLineEdit( baseGroup, "appName" );

    baseGroupLayout->addWidget( appName, 0, 1 );

    command = new KURLRequester( baseGroup, "command" );
    command->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, command->sizePolicy().hasHeightForWidth() ) );

    baseGroupLayout->addMultiCellWidget( command, 3, 3, 1, 2 );

    genericName = new KLineEdit( baseGroup, "genericName" );

    baseGroupLayout->addWidget( genericName, 1, 1 );

    iconButton = new KIconButton( baseGroup, "iconButton" );
    iconButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, iconButton->sizePolicy().hasHeightForWidth() ) );
    iconButton->setMinimumSize( QSize( 60, 60 ) );
    iconButton->setMaximumSize( QSize( 60, 60 ) );
    iconButton->setFlat( FALSE );
    iconButton->setIconSize( 48 );

    baseGroupLayout->addMultiCellWidget( iconButton, 0, 1, 2, 2 );

    textLabel5 = new QLabel( baseGroup, "textLabel5" );

    baseGroupLayout->addWidget( textLabel5, 3, 0 );

    textLabel1_2 = new QLabel( baseGroup, "textLabel1_2" );

    baseGroupLayout->addWidget( textLabel1_2, 4, 0 );

    textLabel1 = new QLabel( baseGroup, "textLabel1" );

    baseGroupLayout->addWidget( textLabel1, 0, 0 );

    textLabel4 = new QLabel( baseGroup, "textLabel4" );

    baseGroupLayout->addWidget( textLabel4, 2, 0 );

    keywords = new KLineEdit( baseGroup, "keywords" );

    baseGroupLayout->addMultiCellWidget( keywords, 4, 4, 1, 2 );

    startupFeedback = new QCheckBox( baseGroup, "startupFeedback" );

    baseGroupLayout->addMultiCellWidget( startupFeedback, 5, 5, 0, 2 );

    showInSystray = new QCheckBox( baseGroup, "showInSystray" );

    baseGroupLayout->addMultiCellWidget( showInSystray, 6, 6, 0, 2 );

    description = new KTextEdit( baseGroup, "description" );
    description->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, 0, 0, description->sizePolicy().hasHeightForWidth() ) );
    description->setMinimumSize( QSize( 0, 0 ) );

    baseGroupLayout->addMultiCellWidget( description, 1, 4, 4, 4 );

    textLabel3 = new QLabel( baseGroup, "textLabel3" );

    baseGroupLayout->addWidget( textLabel3, 0, 4 );

    line4 = new QFrame( baseGroup, "line4" );
    line4->setFrameShape( QFrame::VLine );
    line4->setFrameShadow( QFrame::Sunken );
    line4->setFrameShape( QFrame::VLine );

    baseGroupLayout->addMultiCellWidget( line4, 0, 4, 3, 3 );

    ConfigDialogLayout->addWidget( baseGroup, 0, 0 );

    extendedGroup = new QGroupBox( this, "extendedGroup" );
    extendedGroup->setColumnLayout(0, Qt::Vertical );
    extendedGroup->layout()->setSpacing( 6 );
    extendedGroup->layout()->setMargin( 11 );
    extendedGroupLayout = new QGridLayout( extendedGroup->layout() );
    extendedGroupLayout->setAlignment( Qt::AlignTop );

    textLabel7 = new QLabel( extendedGroup, "textLabel7" );

    extendedGroupLayout->addMultiCellWidget( textLabel7, 0, 1, 0, 4 );

    line1 = new QFrame( extendedGroup, "line1" );
    line1->setFrameShape( QFrame::HLine );
    line1->setFrameShadow( QFrame::Sunken );
    line1->setFrameShape( QFrame::HLine );

    extendedGroupLayout->addMultiCellWidget( line1, 2, 2, 0, 5 );

    startInTerminal = new QCheckBox( extendedGroup, "startInTerminal" );

    extendedGroupLayout->addMultiCellWidget( startInTerminal, 3, 3, 0, 4 );

    textLabel9 = new QLabel( extendedGroup, "textLabel9" );
    textLabel9->setEnabled( FALSE );

    extendedGroupLayout->addMultiCellWidget( textLabel9, 4, 4, 0, 1 );

    terminalSettings = new KLineEdit( extendedGroup, "terminalSettings" );
    terminalSettings->setEnabled( FALSE );

    extendedGroupLayout->addMultiCellWidget( terminalSettings, 4, 4, 2, 5 );

    line2 = new QFrame( extendedGroup, "line2" );
    line2->setFrameShape( QFrame::HLine );
    line2->setFrameShadow( QFrame::Sunken );
    line2->setFrameShape( QFrame::HLine );

    extendedGroupLayout->addMultiCellWidget( line2, 5, 5, 0, 5 );

    startAsUser = new QCheckBox( extendedGroup, "startAsUser" );

    extendedGroupLayout->addMultiCellWidget( startAsUser, 6, 6, 0, 4 );

    textLabel6 = new QLabel( extendedGroup, "textLabel6" );
    textLabel6->setEnabled( FALSE );

    extendedGroupLayout->addMultiCellWidget( textLabel6, 7, 7, 0, 1 );

    username = new KLineEdit( extendedGroup, "username" );
    username->setEnabled( FALSE );

    extendedGroupLayout->addMultiCellWidget( username, 7, 7, 2, 5 );

    line3 = new QFrame( extendedGroup, "line3" );
    line3->setFrameShape( QFrame::HLine );
    line3->setFrameShadow( QFrame::Sunken );
    line3->setFrameShape( QFrame::HLine );

    extendedGroupLayout->addMultiCellWidget( line3, 8, 9, 0, 5 );

    textLabel8 = new QLabel( extendedGroup, "textLabel8" );

    extendedGroupLayout->addMultiCellWidget( textLabel8, 10, 11, 0, 0 );

    workingDir = new KURLRequester( extendedGroup, "workingDir" );

    extendedGroupLayout->addMultiCellWidget( workingDir, 1, 1, 4, 5 );
    spacer2 = new QSpacerItem( 250, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    extendedGroupLayout->addItem( spacer2, 11, 5 );

    shortcut = new KKeyButton( extendedGroup, "shortcut" );

    extendedGroupLayout->addMultiCellWidget( shortcut, 9, 11, 3, 4 );
    spacer3 = new QSpacerItem( 31, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    extendedGroupLayout->addItem( spacer3, 11, 1 );

    ConfigDialogLayout->addWidget( extendedGroup, 2, 0 );
    languageChange();
    resize( QSize(658, 629).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( startInTerminal, SIGNAL( toggled(bool) ), textLabel9, SLOT( setEnabled(bool) ) );
    connect( startInTerminal, SIGNAL( toggled(bool) ), terminalSettings, SLOT( setEnabled(bool) ) );
    connect( startAsUser, SIGNAL( toggled(bool) ), textLabel6, SLOT( setEnabled(bool) ) );
    connect( startAsUser, SIGNAL( toggled(bool) ), username, SLOT( setEnabled(bool) ) );
    connect( buttonDetail, SIGNAL( toggled(bool) ), extendedGroup, SLOT( setShown(bool) ) );
    connect( buttonDetail, SIGNAL( released() ), this, SLOT( adjustSize() ) );

    // tab order
    setTabOrder( appName, genericName );
    setTabOrder( genericName, category );
    setTabOrder( category, command );
    setTabOrder( command, keywords );
    setTabOrder( keywords, startupFeedback );
    setTabOrder( startupFeedback, showInSystray );
    setTabOrder( showInSystray, iconButton );
    setTabOrder( iconButton, description );
    setTabOrder( description, buttonDetail );
    setTabOrder( buttonDetail, workingDir );
    setTabOrder( workingDir, startInTerminal );
    setTabOrder( startInTerminal, terminalSettings );
    setTabOrder( terminalSettings, startAsUser );
    setTabOrder( startAsUser, username );
    setTabOrder( username, shortcut );
    setTabOrder( shortcut, buttonHelp );
    setTabOrder( buttonHelp, buttonOk );
    setTabOrder( buttonOk, buttonCancel );
}

/*
 *  Destroys the object and frees any allocated resources
 */
ConfigDialog::~ConfigDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ConfigDialog::languageChange()
{
    setCaption( tr2i18n( "New Entry" ) );
    buttonHelp->setText( tr2i18n( "&Help" ) );
    buttonHelp->setAccel( QKeySequence( tr2i18n( "F1" ) ) );
    buttonOk->setText( tr2i18n( "&OK" ) );
    buttonOk->setAccel( QKeySequence( QString::null ) );
    buttonCancel->setText( tr2i18n( "&Cancel" ) );
    buttonCancel->setAccel( QKeySequence( QString::null ) );
    buttonDetail->setText( tr2i18n( "Extended Mode" ) );
    baseGroup->setTitle( tr2i18n( "Basic Settings" ) );
    textLabel2->setText( tr2i18n( "Is a" ) );
    iconButton->setText( QString::null );
    textLabel5->setText( tr2i18n( "<b>Command</b>" ) );
    textLabel1_2->setText( tr2i18n( "<i>Keywords</i>" ) );
    textLabel1->setText( tr2i18n( "<i><b>Name</b></i>" ) );
    textLabel4->setText( tr2i18n( "<i><b>Category</b></i>" ) );
    startupFeedback->setText( tr2i18n( "Activate startup feedback" ) );
    showInSystray->setText( tr2i18n( "Show in system tray" ) );
    textLabel3->setText( tr2i18n( "Description" ) );
    extendedGroup->setTitle( tr2i18n( "Extended Settings" ) );
    textLabel7->setText( tr2i18n( "Working directory" ) );
    startInTerminal->setText( tr2i18n( "Start in terminal" ) );
    textLabel9->setText( tr2i18n( "Terminal settings" ) );
    startAsUser->setText( tr2i18n( "Start as different user" ) );
    textLabel6->setText( tr2i18n( "Username" ) );
    textLabel8->setText( tr2i18n( "Shortcut" ) );
    shortcut->setText( tr2i18n( "None" ) );
}

#include "config.moc"
