#include <kdialog.h>
#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './starterconfig.ui'
**
** Created: Sun Apr 8 21:59:53 2012
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "starterconfig.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <kurlrequester.h>
#include <qlabel.h>
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <qradiobutton.h>
#include <qframe.h>
#include <qcombobox.h>
#include <qlistbox.h>
#include <kkeybutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "kurlrequester.h"
#include "klineedit.h"
#include "kpushbutton.h"
#include "kkeybutton.h"

/*
 *  Constructs a StarterConfig as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
StarterConfig::StarterConfig( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "StarterConfig" );
    setSizeGripEnabled( TRUE );
    StarterConfigLayout = new QGridLayout( this, 1, 1, 11, 6, "StarterConfigLayout"); 

    groupBox5 = new QGroupBox( this, "groupBox5" );
    groupBox5->setColumnLayout(0, Qt::Vertical );
    groupBox5->layout()->setSpacing( 6 );
    groupBox5->layout()->setMargin( 11 );
    groupBox5Layout = new QGridLayout( groupBox5->layout() );
    groupBox5Layout->setAlignment( Qt::AlignTop );

    BaseURL = new KURLRequester( groupBox5, "BaseURL" );

    groupBox5Layout->addWidget( BaseURL, 0, 1 );

    textLabel2_3 = new QLabel( groupBox5, "textLabel2_3" );

    groupBox5Layout->addWidget( textLabel2_3, 1, 0 );

    HoverURL = new KURLRequester( groupBox5, "HoverURL" );

    groupBox5Layout->addWidget( HoverURL, 1, 1 );

    textLabel1_2 = new QLabel( groupBox5, "textLabel1_2" );

    groupBox5Layout->addWidget( textLabel1_2, 0, 0 );

    textLabel3 = new QLabel( groupBox5, "textLabel3" );

    groupBox5Layout->addWidget( textLabel3, 2, 0 );

    DownURL = new KURLRequester( groupBox5, "DownURL" );

    groupBox5Layout->addWidget( DownURL, 2, 1 );

    StarterConfigLayout->addWidget( groupBox5, 0, 0 );

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

    StarterConfigLayout->addMultiCellLayout( Layout1, 4, 4, 0, 1 );

    buttonGroup1 = new QButtonGroup( this, "buttonGroup1" );
    buttonGroup1->setColumnLayout(0, Qt::Vertical );
    buttonGroup1->layout()->setSpacing( 6 );
    buttonGroup1->layout()->setMargin( 11 );
    buttonGroup1Layout = new QGridLayout( buttonGroup1->layout() );
    buttonGroup1Layout->setAlignment( Qt::AlignTop );

    layout10 = new QHBoxLayout( 0, 0, 6, "layout10"); 

    customDialogSize = new QCheckBox( buttonGroup1, "customDialogSize" );
    customDialogSize->setEnabled( TRUE );
    layout10->addWidget( customDialogSize );

    dialogW = new QSpinBox( buttonGroup1, "dialogW" );
    dialogW->setEnabled( FALSE );
    dialogW->setMaxValue( 3000 );
    layout10->addWidget( dialogW );

    textLabel2 = new QLabel( buttonGroup1, "textLabel2" );
    layout10->addWidget( textLabel2 );

    dialogH = new QSpinBox( buttonGroup1, "dialogH" );
    dialogH->setEnabled( FALSE );
    dialogH->setMaxValue( 2000 );
    layout10->addWidget( dialogH );
    spacer4 = new QSpacerItem( 111, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout10->addItem( spacer4 );

    buttonGroup1Layout->addLayout( layout10, 0, 0 );

    dialogFollowMouse = new QRadioButton( buttonGroup1, "dialogFollowMouse" );
    dialogFollowMouse->setEnabled( FALSE );

    buttonGroup1Layout->addWidget( dialogFollowMouse, 4, 0 );

    layout12 = new QHBoxLayout( 0, 0, 6, "layout12"); 

    fixedDialogPos = new QRadioButton( buttonGroup1, "fixedDialogPos" );
    fixedDialogPos->setEnabled( FALSE );
    layout12->addWidget( fixedDialogPos );

    dialogX = new QSpinBox( buttonGroup1, "dialogX" );
    dialogX->setEnabled( FALSE );
    layout12->addWidget( dialogX );

    textLabel2_2 = new QLabel( buttonGroup1, "textLabel2_2" );
    layout12->addWidget( textLabel2_2 );

    dialogY = new QSpinBox( buttonGroup1, "dialogY" );
    dialogY->setEnabled( FALSE );
    layout12->addWidget( dialogY );
    spacer5 = new QSpacerItem( 80, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout12->addItem( spacer5 );

    buttonGroup1Layout->addLayout( layout12, 3, 0 );

    customDialogPos = new QCheckBox( buttonGroup1, "customDialogPos" );

    buttonGroup1Layout->addWidget( customDialogPos, 2, 0 );

    line1 = new QFrame( buttonGroup1, "line1" );
    line1->setFrameShape( QFrame::HLine );
    line1->setFrameShadow( QFrame::Sunken );
    line1->setFrameShape( QFrame::HLine );

    buttonGroup1Layout->addWidget( line1, 1, 0 );

    showDialogTitlebar = new QCheckBox( buttonGroup1, "showDialogTitlebar" );

    buttonGroup1Layout->addWidget( showDialogTitlebar, 8, 0 );

    line2 = new QFrame( buttonGroup1, "line2" );
    line2->setFrameShape( QFrame::HLine );
    line2->setFrameShadow( QFrame::Sunken );
    line2->setFrameShape( QFrame::HLine );

    buttonGroup1Layout->addWidget( line2, 7, 0 );

    line2_3 = new QFrame( buttonGroup1, "line2_3" );
    line2_3->setFrameShape( QFrame::HLine );
    line2_3->setFrameShadow( QFrame::Sunken );
    line2_3->setFrameShape( QFrame::HLine );

    buttonGroup1Layout->addWidget( line2_3, 5, 0 );

    layout6 = new QHBoxLayout( 0, 0, 6, "layout6"); 

    textLabel1_3 = new QLabel( buttonGroup1, "textLabel1_3" );
    textLabel1_3->setEnabled( TRUE );
    layout6->addWidget( textLabel1_3 );

    dialogPanelPos = new QComboBox( FALSE, buttonGroup1, "dialogPanelPos" );
    dialogPanelPos->setEnabled( TRUE );
    layout6->addWidget( dialogPanelPos );

    buttonGroup1Layout->addLayout( layout6, 6, 0 );

    StarterConfigLayout->addWidget( buttonGroup1, 3, 0 );

    groupBox6 = new QGroupBox( this, "groupBox6" );
    groupBox6->setColumnLayout(0, Qt::Vertical );
    groupBox6->layout()->setSpacing( 6 );
    groupBox6->layout()->setMargin( 11 );
    groupBox6Layout = new QGridLayout( groupBox6->layout() );
    groupBox6Layout->setAlignment( Qt::AlignTop );

    layout11 = new QHBoxLayout( 0, 0, 6, "layout11"); 

    customPopupSize = new QCheckBox( groupBox6, "customPopupSize" );
    customPopupSize->setEnabled( TRUE );
    layout11->addWidget( customPopupSize );

    popupW = new QSpinBox( groupBox6, "popupW" );
    popupW->setEnabled( FALSE );
    popupW->setMaxValue( 3000 );
    layout11->addWidget( popupW );

    textLabel1 = new QLabel( groupBox6, "textLabel1" );
    layout11->addWidget( textLabel1 );

    popupH = new QSpinBox( groupBox6, "popupH" );
    popupH->setEnabled( FALSE );
    popupH->setMaxValue( 2000 );
    layout11->addWidget( popupH );
    spacer3 = new QSpacerItem( 141, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout11->addItem( spacer3 );

    groupBox6Layout->addLayout( layout11, 0, 0 );

    line2_2 = new QFrame( groupBox6, "line2_2" );
    line2_2->setFrameShape( QFrame::HLine );
    line2_2->setFrameShadow( QFrame::Sunken );
    line2_2->setFrameShape( QFrame::HLine );

    groupBox6Layout->addWidget( line2_2, 1, 0 );

    layout6_2 = new QHBoxLayout( 0, 0, 6, "layout6_2"); 

    textLabel1_3_2 = new QLabel( groupBox6, "textLabel1_3_2" );
    textLabel1_3_2->setEnabled( TRUE );
    layout6_2->addWidget( textLabel1_3_2 );

    popupPanelPos = new QComboBox( FALSE, groupBox6, "popupPanelPos" );
    popupPanelPos->setEnabled( TRUE );
    layout6_2->addWidget( popupPanelPos );

    groupBox6Layout->addLayout( layout6_2, 2, 0 );

    StarterConfigLayout->addWidget( groupBox6, 2, 0 );

    groupBox4 = new QGroupBox( this, "groupBox4" );
    groupBox4->setColumnLayout(0, Qt::Vertical );
    groupBox4->layout()->setSpacing( 6 );
    groupBox4->layout()->setMargin( 11 );
    groupBox4Layout = new QGridLayout( groupBox4->layout() );
    groupBox4Layout->setAlignment( Qt::AlignTop );

    layout9 = new QHBoxLayout( 0, 0, 6, "layout9"); 

    textLabel2_4 = new QLabel( groupBox4, "textLabel2_4" );
    layout9->addWidget( textLabel2_4 );

    favItemAmount = new QSpinBox( groupBox4, "favItemAmount" );
    favItemAmount->setValue( 10 );
    layout9->addWidget( favItemAmount );
    spacer7 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout9->addItem( spacer7 );

    groupBox4Layout->addLayout( layout9, 2, 0 );

    line1_2 = new QFrame( groupBox4, "line1_2" );
    line1_2->setFrameShape( QFrame::HLine );
    line1_2->setFrameShadow( QFrame::Sunken );
    line1_2->setFrameShape( QFrame::HLine );

    groupBox4Layout->addWidget( line1_2, 1, 0 );

    useKTTS = new QCheckBox( groupBox4, "useKTTS" );

    groupBox4Layout->addWidget( useKTTS, 0, 0 );

    layout9_2 = new QHBoxLayout( 0, 0, 6, "layout9_2"); 

    textLabel1_4 = new QLabel( groupBox4, "textLabel1_4" );
    layout9_2->addWidget( textLabel1_4 );

    iconSize = new QComboBox( FALSE, groupBox4, "iconSize" );
    layout9_2->addWidget( iconSize );

    groupBox4Layout->addLayout( layout9_2, 3, 0 );

    StarterConfigLayout->addWidget( groupBox4, 1, 0 );

    groupBox7 = new QGroupBox( this, "groupBox7" );
    groupBox7->setColumnLayout(0, Qt::Vertical );
    groupBox7->layout()->setSpacing( 6 );
    groupBox7->layout()->setMargin( 11 );
    groupBox7Layout = new QGridLayout( groupBox7->layout() );
    groupBox7Layout->setAlignment( Qt::AlignTop );

    categoryList = new QListBox( groupBox7, "categoryList" );
    categoryList->setHScrollBarMode( QListBox::AlwaysOff );

    groupBox7Layout->addWidget( categoryList, 0, 0 );

    layout18 = new QHBoxLayout( 0, 0, 6, "layout18"); 
    spacer14 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout18->addItem( spacer14 );

    buttonShortcut = new KKeyButton( groupBox7, "buttonShortcut" );
    layout18->addWidget( buttonShortcut );
    spacer15 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout18->addItem( spacer15 );

    groupBox7Layout->addLayout( layout18, 1, 0 );

    StarterConfigLayout->addMultiCellWidget( groupBox7, 0, 3, 1, 1 );
    languageChange();
    resize( QSize(492, 674).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( customPopupSize, SIGNAL( toggled(bool) ), popupW, SLOT( setEnabled(bool) ) );
    connect( customPopupSize, SIGNAL( toggled(bool) ), popupH, SLOT( setEnabled(bool) ) );
    connect( buttonCancel, SIGNAL( toggled(bool) ), dialogX, SLOT( setEnabled(bool) ) );
    connect( customDialogSize, SIGNAL( toggled(bool) ), dialogW, SLOT( setEnabled(bool) ) );
    connect( customDialogSize, SIGNAL( toggled(bool) ), dialogH, SLOT( setEnabled(bool) ) );
    connect( customDialogPos, SIGNAL( toggled(bool) ), fixedDialogPos, SLOT( setEnabled(bool) ) );
    connect( customDialogPos, SIGNAL( toggled(bool) ), dialogFollowMouse, SLOT( setEnabled(bool) ) );
    connect( fixedDialogPos, SIGNAL( toggled(bool) ), dialogX, SLOT( setEnabled(bool) ) );
    connect( fixedDialogPos, SIGNAL( toggled(bool) ), dialogY, SLOT( setEnabled(bool) ) );
    connect( customDialogPos, SIGNAL( toggled(bool) ), fixedDialogPos, SLOT( setChecked(bool) ) );
    connect( customDialogPos, SIGNAL( toggled(bool) ), dialogFollowMouse, SLOT( setChecked(bool) ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
StarterConfig::~StarterConfig()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void StarterConfig::languageChange()
{
    setCaption( tr2i18n( "Baghira Starter Config" ) );
    groupBox5->setTitle( tr2i18n( "Startbutton images" ) );
    textLabel2_3->setText( tr2i18n( "<b>Hover</b>" ) );
    textLabel1_2->setText( tr2i18n( "<b>Base</b>" ) );
    textLabel3->setText( tr2i18n( "<b>Down</b>" ) );
    buttonHelp->setText( tr2i18n( "&Help" ) );
    buttonHelp->setAccel( QKeySequence( tr2i18n( "F1" ) ) );
    buttonOk->setText( tr2i18n( "&OK" ) );
    buttonOk->setAccel( QKeySequence( QString::null ) );
    buttonCancel->setText( tr2i18n( "&Cancel" ) );
    buttonCancel->setAccel( QKeySequence( QString::null ) );
    buttonGroup1->setTitle( tr2i18n( "Dialog options" ) );
    customDialogSize->setText( tr2i18n( "Custom dialog size" ) );
    textLabel2->setText( tr2i18n( "x" ) );
    dialogFollowMouse->setText( tr2i18n( "Show dialog under mouse" ) );
    fixedDialogPos->setText( tr2i18n( "Fixed" ) );
    textLabel2_2->setText( tr2i18n( ":" ) );
    customDialogPos->setText( tr2i18n( "Custom dialog position" ) );
    showDialogTitlebar->setText( tr2i18n( "Show titlebar for dialog" ) );
    textLabel1_3->setText( tr2i18n( "Panel Position" ) );
    dialogPanelPos->clear();
    dialogPanelPos->insertItem( tr2i18n( "North" ) );
    dialogPanelPos->insertItem( tr2i18n( "South" ) );
    dialogPanelPos->insertItem( tr2i18n( "West" ) );
    dialogPanelPos->insertItem( tr2i18n( "East" ) );
    dialogPanelPos->insertItem( tr2i18n( "Nowhere" ) );
    dialogPanelPos->setCurrentItem( 1 );
    groupBox6->setTitle( tr2i18n( "Popup options" ) );
    customPopupSize->setText( tr2i18n( "Custom popup size" ) );
    textLabel1->setText( tr2i18n( "x" ) );
    textLabel1_3_2->setText( tr2i18n( "Panel Position" ) );
    popupPanelPos->clear();
    popupPanelPos->insertItem( tr2i18n( "North" ) );
    popupPanelPos->insertItem( tr2i18n( "South" ) );
    popupPanelPos->insertItem( tr2i18n( "West" ) );
    popupPanelPos->insertItem( tr2i18n( "East" ) );
    popupPanelPos->insertItem( tr2i18n( "Nowhere" ) );
    popupPanelPos->setCurrentItem( 2 );
    groupBox4->setTitle( QString::null );
    textLabel2_4->setText( tr2i18n( "Items in favorite list" ) );
    useKTTS->setText( tr2i18n( "Talk to me (via KTTS)" ) );
    textLabel1_4->setText( tr2i18n( "Icon SIze" ) );
    iconSize->clear();
    iconSize->insertItem( tr2i18n( "16/22" ) );
    iconSize->insertItem( tr2i18n( "22/32" ) );
    iconSize->insertItem( tr2i18n( "32/48" ) );
    iconSize->insertItem( tr2i18n( "48/64" ) );
    iconSize->insertItem( tr2i18n( "64/128" ) );
    groupBox7->setTitle( tr2i18n( "Shortcuts" ) );
    buttonShortcut->setText( tr2i18n( "None" ) );
}

#include "starterconfig.moc"
