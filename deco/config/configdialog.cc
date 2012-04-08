#include <kdialog.h>
#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './configdialog.ui'
**
** Created: Sun Apr 8 21:58:38 2012
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "configdialog.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qframe.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qtabwidget.h>
#include <kcolorbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "kcolorbutton.h"

/*
 *  Constructs a ConfigDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
ConfigDialog::ConfigDialog( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "ConfigDialog" );
    ConfigDialogLayout = new QGridLayout( this, 1, 1, 11, 6, "ConfigDialogLayout"); 

    generalbox = new QGroupBox( this, "generalbox" );
    generalbox->setFrameShape( QGroupBox::GroupBoxPanel );
    generalbox->setFrameShadow( QGroupBox::Sunken );
    generalbox->setColumnLayout(0, Qt::Vertical );
    generalbox->layout()->setSpacing( 6 );
    generalbox->layout()->setMargin( 11 );
    generalboxLayout = new QGridLayout( generalbox->layout() );
    generalboxLayout->setAlignment( Qt::AlignTop );

    layout133 = new QHBoxLayout( 0, 0, 6, "layout133"); 

    textLabel1_3 = new QLabel( generalbox, "textLabel1_3" );
    layout133->addWidget( textLabel1_3 );
    spacer6 = new QSpacerItem( 53, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout133->addItem( spacer6 );

    minTH = new QSpinBox( generalbox, "minTH" );
    minTH->setMaxValue( 30 );
    minTH->setMinValue( 18 );
    layout133->addWidget( minTH );

    generalboxLayout->addLayout( layout133, 0, 0 );

    ResizeGrip = new QCheckBox( generalbox, "ResizeGrip" );

    generalboxLayout->addWidget( ResizeGrip, 1, 0 );

    addAutoSpacing = new QCheckBox( generalbox, "addAutoSpacing" );

    generalboxLayout->addWidget( addAutoSpacing, 4, 0 );

    fullSpec = new QCheckBox( generalbox, "fullSpec" );

    generalboxLayout->addWidget( fullSpec, 6, 0 );

    drawComicFrame = new QCheckBox( generalbox, "drawComicFrame" );

    generalboxLayout->addWidget( drawComicFrame, 2, 0 );

    noModalDeco = new QCheckBox( generalbox, "noModalDeco" );

    generalboxLayout->addWidget( noModalDeco, 7, 0 );

    maxResizable = new QCheckBox( generalbox, "maxResizable" );

    generalboxLayout->addWidget( maxResizable, 5, 0 );

    allowEasyClosing = new QCheckBox( generalbox, "allowEasyClosing" );

    generalboxLayout->addWidget( allowEasyClosing, 3, 0 );

    defaultMode = new QComboBox( FALSE, generalbox, "defaultMode" );

    generalboxLayout->addWidget( defaultMode, 11, 0 );

    textLabel1_7 = new QLabel( generalbox, "textLabel1_7" );

    generalboxLayout->addWidget( textLabel1_7, 10, 0 );

    line3 = new QFrame( generalbox, "line3" );
    line3->setFrameShape( QFrame::HLine );
    line3->setFrameShadow( QFrame::Sunken );
    line3->setFrameShape( QFrame::HLine );

    generalboxLayout->addWidget( line3, 9, 0 );

    delAppname = new QCheckBox( generalbox, "delAppname" );

    generalboxLayout->addWidget( delAppname, 8, 0 );

    ConfigDialogLayout->addMultiCellWidget( generalbox, 2, 2, 0, 1 );

    titlealign = new QButtonGroup( this, "titlealign" );
    titlealign->setColumnLayout(0, Qt::Vertical );
    titlealign->layout()->setSpacing( 6 );
    titlealign->layout()->setMargin( 11 );
    titlealignLayout = new QGridLayout( titlealign->layout() );
    titlealignLayout->setAlignment( Qt::AlignTop );

    AlignLeft = new QRadioButton( titlealign, "AlignLeft" );

    titlealignLayout->addWidget( AlignLeft, 0, 0 );
    spacer78 = new QSpacerItem( 30, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    titlealignLayout->addItem( spacer78, 0, 1 );

    AlignHCenter = new QRadioButton( titlealign, "AlignHCenter" );
    AlignHCenter->setChecked( TRUE );

    titlealignLayout->addWidget( AlignHCenter, 0, 2 );
    spacer77 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    titlealignLayout->addItem( spacer77, 0, 3 );

    AlignRight = new QRadioButton( titlealign, "AlignRight" );

    titlealignLayout->addWidget( AlignRight, 0, 4 );

    noTitle = new QRadioButton( titlealign, "noTitle" );

    titlealignLayout->addWidget( noTitle, 0, 6 );
    spacer23_2 = new QSpacerItem( 51, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    titlealignLayout->addItem( spacer23_2, 0, 5 );

    ConfigDialogLayout->addWidget( titlealign, 1, 0 );
    spacer79 = new QSpacerItem( 20, 59, QSizePolicy::Minimum, QSizePolicy::Expanding );
    ConfigDialogLayout->addItem( spacer79, 1, 1 );

    ButtonColorConfig = new QPushButton( this, "ButtonColorConfig" );

    ConfigDialogLayout->addWidget( ButtonColorConfig, 0, 0 );
    spacer33 = new QSpacerItem( 20, 25, QSizePolicy::Minimum, QSizePolicy::Expanding );
    ConfigDialogLayout->addItem( spacer33, 0, 1 );

    tab = new QTabWidget( this, "tab" );

    jaguarTab = new QWidget( tab, "jaguarTab" );
    jaguarTabLayout = new QGridLayout( jaguarTab, 1, 1, 11, 6, "jaguarTabLayout"); 

    layout7_3 = new QHBoxLayout( 0, 0, 6, "layout7_3"); 

    textLabel1_4_3 = new QLabel( jaguarTab, "textLabel1_4_3" );
    layout7_3->addWidget( textLabel1_4_3 );
    spacer4_3 = new QSpacerItem( 200, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout7_3->addItem( spacer4_3 );

    _3DImpact_J = new QSpinBox( jaguarTab, "_3DImpact_J" );
    _3DImpact_J->setMaxValue( 100 );
    _3DImpact_J->setValue( 20 );
    layout7_3->addWidget( _3DImpact_J );

    jaguarTabLayout->addMultiCellLayout( layout7_3, 2, 2, 0, 1 );

    layout10 = new QHBoxLayout( 0, 0, 6, "layout10"); 

    textLabel2 = new QLabel( jaguarTab, "textLabel2" );
    layout10->addWidget( textLabel2 );
    spacer5 = new QSpacerItem( 61, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout10->addItem( spacer5 );

    LineImpact_J = new QSpinBox( jaguarTab, "LineImpact_J" );
    LineImpact_J->setButtonSymbols( QSpinBox::UpDownArrows );
    LineImpact_J->setMaxValue( 100 );
    LineImpact_J->setValue( 40 );
    layout10->addWidget( LineImpact_J );

    jaguarTabLayout->addMultiCellLayout( layout10, 3, 3, 0, 1 );

    layout40 = new QHBoxLayout( 0, 0, 6, "layout40"); 

    layout35 = new QVBoxLayout( 0, 0, 6, "layout35"); 

    textLabel1_2_4_3 = new QLabel( jaguarTab, "textLabel1_2_4_3" );
    layout35->addWidget( textLabel1_2_4_3 );

    textLabel1_8_3 = new QLabel( jaguarTab, "textLabel1_8_3" );
    layout35->addWidget( textLabel1_8_3 );

    textLabel1_8_3_2 = new QLabel( jaguarTab, "textLabel1_8_3_2" );
    layout35->addWidget( textLabel1_8_3_2 );
    layout40->addLayout( layout35 );
    spacer18_2_3 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout40->addItem( spacer18_2_3 );

    layout39 = new QVBoxLayout( 0, 0, 6, "layout39"); 

    buttonStyle_J = new QComboBox( FALSE, jaguarTab, "buttonStyle_J" );
    layout39->addWidget( buttonStyle_J );

    titleeffect_J = new QComboBox( FALSE, jaguarTab, "titleeffect_J" );
    titleeffect_J->setMinimumSize( QSize( 0, 22 ) );
    layout39->addWidget( titleeffect_J );

    i_titleeffect_J = new QComboBox( FALSE, jaguarTab, "i_titleeffect_J" );
    i_titleeffect_J->setMinimumSize( QSize( 0, 22 ) );
    layout39->addWidget( i_titleeffect_J );
    layout40->addLayout( layout39 );

    layout38 = new QVBoxLayout( 0, 0, 6, "layout38"); 

    textLabel1 = new QLabel( jaguarTab, "textLabel1" );
    textLabel1->setAlignment( int( QLabel::AlignCenter ) );
    layout38->addWidget( textLabel1 );

    layout33 = new QHBoxLayout( 0, 0, 6, "layout33"); 

    activeColor1_J = new KColorButton( jaguarTab, "activeColor1_J" );
    layout33->addWidget( activeColor1_J );

    activeColor2_J = new KColorButton( jaguarTab, "activeColor2_J" );
    layout33->addWidget( activeColor2_J );
    layout38->addLayout( layout33 );

    layout34 = new QHBoxLayout( 0, 0, 6, "layout34"); 

    inactiveColor1_J = new KColorButton( jaguarTab, "inactiveColor1_J" );
    layout34->addWidget( inactiveColor1_J );

    inactiveColor2_J = new KColorButton( jaguarTab, "inactiveColor2_J" );
    layout34->addWidget( inactiveColor2_J );
    layout38->addLayout( layout34 );
    layout40->addLayout( layout38 );

    jaguarTabLayout->addMultiCellLayout( layout40, 1, 1, 0, 1 );

    groupBox2_4 = new QGroupBox( jaguarTab, "groupBox2_4" );
    groupBox2_4->setColumnLayout(0, Qt::Vertical );
    groupBox2_4->layout()->setSpacing( 6 );
    groupBox2_4->layout()->setMargin( 11 );
    groupBox2_4Layout = new QGridLayout( groupBox2_4->layout() );
    groupBox2_4Layout->setAlignment( Qt::AlignTop );

    layout45 = new QVBoxLayout( 0, 0, 6, "layout45"); 

    shapeUL_J = new QCheckBox( groupBox2_4, "shapeUL_J" );
    layout45->addWidget( shapeUL_J );
    spacer16 = new QSpacerItem( 20, 31, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout45->addItem( spacer16 );

    shapeLL_J = new QCheckBox( groupBox2_4, "shapeLL_J" );
    layout45->addWidget( shapeLL_J );

    groupBox2_4Layout->addMultiCellLayout( layout45, 0, 2, 0, 0 );

    layout44 = new QVBoxLayout( 0, 0, 6, "layout44"); 

    shapeUR_J = new QCheckBox( groupBox2_4, "shapeUR_J" );
    layout44->addWidget( shapeUR_J );
    spacer17 = new QSpacerItem( 20, 31, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout44->addItem( spacer17 );

    shapeLR_J = new QCheckBox( groupBox2_4, "shapeLR_J" );
    layout44->addWidget( shapeLR_J );

    groupBox2_4Layout->addMultiCellLayout( layout44, 0, 2, 5, 5 );
    spacer24_3 = new QSpacerItem( 60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox2_4Layout->addItem( spacer24_3, 1, 4 );

    borderSize_J = new QSpinBox( groupBox2_4, "borderSize_J" );
    borderSize_J->setMaxValue( 16 );

    groupBox2_4Layout->addWidget( borderSize_J, 1, 3 );

    textLabel1_6_3 = new QLabel( groupBox2_4, "textLabel1_6_3" );

    groupBox2_4Layout->addWidget( textLabel1_6_3, 1, 2 );
    spacer27_2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    groupBox2_4Layout->addItem( spacer27_2, 0, 2 );
    spacer27 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    groupBox2_4Layout->addItem( spacer27, 2, 2 );
    spacer15 = new QSpacerItem( 30, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox2_4Layout->addItem( spacer15, 1, 1 );

    jaguarTabLayout->addMultiCellWidget( groupBox2_4, 4, 4, 0, 1 );

    drawIcon_J = new QCheckBox( jaguarTab, "drawIcon_J" );

    jaguarTabLayout->addWidget( drawIcon_J, 0, 0 );
    tab->insertTab( jaguarTab, QString::fromLatin1("") );

    pantherTab = new QWidget( tab, "pantherTab" );
    pantherTabLayout = new QGridLayout( pantherTab, 1, 1, 11, 6, "pantherTabLayout"); 

    layout10_2 = new QHBoxLayout( 0, 0, 6, "layout10_2"); 

    textLabel2_2 = new QLabel( pantherTab, "textLabel2_2" );
    layout10_2->addWidget( textLabel2_2 );
    spacer5_2 = new QSpacerItem( 61, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout10_2->addItem( spacer5_2 );

    LineImpact_P = new QSpinBox( pantherTab, "LineImpact_P" );
    LineImpact_P->setButtonSymbols( QSpinBox::UpDownArrows );
    LineImpact_P->setMaxValue( 100 );
    LineImpact_P->setValue( 40 );
    layout10_2->addWidget( LineImpact_P );

    pantherTabLayout->addMultiCellLayout( layout10_2, 3, 3, 0, 1 );

    groupBox2_3 = new QGroupBox( pantherTab, "groupBox2_3" );
    groupBox2_3->setColumnLayout(0, Qt::Vertical );
    groupBox2_3->layout()->setSpacing( 6 );
    groupBox2_3->layout()->setMargin( 11 );
    groupBox2_3Layout = new QGridLayout( groupBox2_3->layout() );
    groupBox2_3Layout->setAlignment( Qt::AlignTop );

    layout51 = new QVBoxLayout( 0, 0, 6, "layout51"); 

    shapeUL_P = new QCheckBox( groupBox2_3, "shapeUL_P" );
    layout51->addWidget( shapeUL_P );
    spacer20 = new QSpacerItem( 20, 31, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout51->addItem( spacer20 );

    shapeLL_P = new QCheckBox( groupBox2_3, "shapeLL_P" );
    layout51->addWidget( shapeLL_P );

    groupBox2_3Layout->addMultiCellLayout( layout51, 0, 2, 0, 0 );

    layout52 = new QVBoxLayout( 0, 0, 6, "layout52"); 

    shapeUR_P = new QCheckBox( groupBox2_3, "shapeUR_P" );
    layout52->addWidget( shapeUR_P );
    spacer19 = new QSpacerItem( 20, 31, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout52->addItem( spacer19 );

    shapeLR_P = new QCheckBox( groupBox2_3, "shapeLR_P" );
    layout52->addWidget( shapeLR_P );

    groupBox2_3Layout->addMultiCellLayout( layout52, 0, 2, 5, 5 );
    spacer18 = new QSpacerItem( 50, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox2_3Layout->addItem( spacer18, 1, 1 );
    spacer24_2 = new QSpacerItem( 60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox2_3Layout->addItem( spacer24_2, 1, 4 );

    borderSize_P = new QSpinBox( groupBox2_3, "borderSize_P" );
    borderSize_P->setMaxValue( 16 );

    groupBox2_3Layout->addWidget( borderSize_P, 1, 3 );

    textLabel1_6_2 = new QLabel( groupBox2_3, "textLabel1_6_2" );

    groupBox2_3Layout->addWidget( textLabel1_6_2, 1, 2 );
    spacer28 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    groupBox2_3Layout->addItem( spacer28, 2, 2 );
    spacer28_2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    groupBox2_3Layout->addItem( spacer28_2, 0, 2 );

    pantherTabLayout->addMultiCellWidget( groupBox2_3, 4, 4, 0, 1 );

    layout7_2 = new QHBoxLayout( 0, 0, 6, "layout7_2"); 

    textLabel1_4_4 = new QLabel( pantherTab, "textLabel1_4_4" );
    layout7_2->addWidget( textLabel1_4_4 );
    spacer4_4 = new QSpacerItem( 200, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout7_2->addItem( spacer4_4 );

    _3DImpact_P = new QSpinBox( pantherTab, "_3DImpact_P" );
    _3DImpact_P->setMaxValue( 100 );
    _3DImpact_P->setValue( 20 );
    layout7_2->addWidget( _3DImpact_P );

    pantherTabLayout->addMultiCellLayout( layout7_2, 2, 2, 0, 1 );

    layout50 = new QHBoxLayout( 0, 0, 6, "layout50"); 

    layout31 = new QVBoxLayout( 0, 0, 6, "layout31"); 

    textLabel1_2_4 = new QLabel( pantherTab, "textLabel1_2_4" );
    layout31->addWidget( textLabel1_2_4 );

    textLabel1_8 = new QLabel( pantherTab, "textLabel1_8" );
    layout31->addWidget( textLabel1_8 );

    textLabel1_8_4 = new QLabel( pantherTab, "textLabel1_8_4" );
    layout31->addWidget( textLabel1_8_4 );
    layout50->addLayout( layout31 );
    spacer18_2 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout50->addItem( spacer18_2 );

    layout30 = new QVBoxLayout( 0, 0, 6, "layout30"); 

    buttonStyle_P = new QComboBox( FALSE, pantherTab, "buttonStyle_P" );
    layout30->addWidget( buttonStyle_P );

    titleeffect_P = new QComboBox( FALSE, pantherTab, "titleeffect_P" );
    titleeffect_P->setMinimumSize( QSize( 0, 22 ) );
    layout30->addWidget( titleeffect_P );

    i_titleeffect_P = new QComboBox( FALSE, pantherTab, "i_titleeffect_P" );
    i_titleeffect_P->setMinimumSize( QSize( 0, 22 ) );
    layout30->addWidget( i_titleeffect_P );
    layout50->addLayout( layout30 );

    layout49 = new QVBoxLayout( 0, 0, 6, "layout49"); 

    textLabel1_2 = new QLabel( pantherTab, "textLabel1_2" );
    textLabel1_2->setAlignment( int( QLabel::AlignCenter ) );
    layout49->addWidget( textLabel1_2 );

    layout47 = new QHBoxLayout( 0, 0, 6, "layout47"); 

    activeColor1_P = new KColorButton( pantherTab, "activeColor1_P" );
    layout47->addWidget( activeColor1_P );

    activeColor2_P = new KColorButton( pantherTab, "activeColor2_P" );
    layout47->addWidget( activeColor2_P );
    layout49->addLayout( layout47 );

    layout48 = new QHBoxLayout( 0, 0, 6, "layout48"); 

    inactiveColor1_P = new KColorButton( pantherTab, "inactiveColor1_P" );
    layout48->addWidget( inactiveColor1_P );

    inactiveColor2_P = new KColorButton( pantherTab, "inactiveColor2_P" );
    layout48->addWidget( inactiveColor2_P );
    layout49->addLayout( layout48 );
    layout50->addLayout( layout49 );

    pantherTabLayout->addMultiCellLayout( layout50, 1, 1, 0, 1 );

    drawIcon_P = new QCheckBox( pantherTab, "drawIcon_P" );

    pantherTabLayout->addWidget( drawIcon_P, 0, 0 );
    tab->insertTab( pantherTab, QString::fromLatin1("") );

    brushedTab = new QWidget( tab, "brushedTab" );
    brushedTabLayout = new QGridLayout( brushedTab, 1, 1, 11, 6, "brushedTabLayout"); 

    layout10_3 = new QHBoxLayout( 0, 0, 6, "layout10_3"); 

    textLabel2_3 = new QLabel( brushedTab, "textLabel2_3" );
    layout10_3->addWidget( textLabel2_3 );
    spacer5_3 = new QSpacerItem( 61, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout10_3->addItem( spacer5_3 );

    LineImpact_B = new QSpinBox( brushedTab, "LineImpact_B" );
    LineImpact_B->setButtonSymbols( QSpinBox::UpDownArrows );
    LineImpact_B->setMaxValue( 100 );
    LineImpact_B->setValue( 40 );
    layout10_3->addWidget( LineImpact_B );

    brushedTabLayout->addMultiCellLayout( layout10_3, 3, 3, 0, 1 );

    layout7 = new QHBoxLayout( 0, 0, 6, "layout7"); 

    textLabel1_4 = new QLabel( brushedTab, "textLabel1_4" );
    layout7->addWidget( textLabel1_4 );
    spacer4 = new QSpacerItem( 200, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout7->addItem( spacer4 );

    _3DImpact_B = new QSpinBox( brushedTab, "_3DImpact_B" );
    _3DImpact_B->setMaxValue( 100 );
    _3DImpact_B->setValue( 20 );
    layout7->addWidget( _3DImpact_B );

    brushedTabLayout->addMultiCellLayout( layout7, 2, 2, 0, 1 );

    layout57 = new QHBoxLayout( 0, 0, 6, "layout57"); 

    layout27 = new QVBoxLayout( 0, 0, 6, "layout27"); 

    textLabel1_2_4_2 = new QLabel( brushedTab, "textLabel1_2_4_2" );
    layout27->addWidget( textLabel1_2_4_2 );

    textLabel1_8_2 = new QLabel( brushedTab, "textLabel1_8_2" );
    layout27->addWidget( textLabel1_8_2 );

    textLabel1_8_2_2 = new QLabel( brushedTab, "textLabel1_8_2_2" );
    layout27->addWidget( textLabel1_8_2_2 );
    layout57->addLayout( layout27 );
    spacer18_2_2 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout57->addItem( spacer18_2_2 );

    layout26 = new QVBoxLayout( 0, 0, 6, "layout26"); 

    buttonStyle_B = new QComboBox( FALSE, brushedTab, "buttonStyle_B" );
    layout26->addWidget( buttonStyle_B );

    titleeffect_B = new QComboBox( FALSE, brushedTab, "titleeffect_B" );
    titleeffect_B->setMinimumSize( QSize( 0, 22 ) );
    layout26->addWidget( titleeffect_B );

    i_titleeffect_B = new QComboBox( FALSE, brushedTab, "i_titleeffect_B" );
    i_titleeffect_B->setMinimumSize( QSize( 0, 22 ) );
    layout26->addWidget( i_titleeffect_B );
    layout57->addLayout( layout26 );

    layout56 = new QVBoxLayout( 0, 0, 6, "layout56"); 

    textLabel1_5 = new QLabel( brushedTab, "textLabel1_5" );
    textLabel1_5->setAlignment( int( QLabel::AlignCenter ) );
    layout56->addWidget( textLabel1_5 );

    layout54 = new QHBoxLayout( 0, 0, 6, "layout54"); 

    activeColor1_B = new KColorButton( brushedTab, "activeColor1_B" );
    layout54->addWidget( activeColor1_B );

    activeColor2_B = new KColorButton( brushedTab, "activeColor2_B" );
    layout54->addWidget( activeColor2_B );
    layout56->addLayout( layout54 );

    layout55 = new QHBoxLayout( 0, 0, 6, "layout55"); 

    inactiveColor1_B = new KColorButton( brushedTab, "inactiveColor1_B" );
    layout55->addWidget( inactiveColor1_B );

    inactiveColor2_B = new KColorButton( brushedTab, "inactiveColor2_B" );
    layout55->addWidget( inactiveColor2_B );
    layout56->addLayout( layout55 );
    layout57->addLayout( layout56 );

    brushedTabLayout->addMultiCellLayout( layout57, 1, 1, 0, 1 );

    groupBox2_2 = new QGroupBox( brushedTab, "groupBox2_2" );
    groupBox2_2->setColumnLayout(0, Qt::Vertical );
    groupBox2_2->layout()->setSpacing( 6 );
    groupBox2_2->layout()->setMargin( 11 );
    groupBox2_2Layout = new QGridLayout( groupBox2_2->layout() );
    groupBox2_2Layout->setAlignment( Qt::AlignTop );

    layout58 = new QVBoxLayout( 0, 0, 6, "layout58"); 

    shapeUL_B = new QCheckBox( groupBox2_2, "shapeUL_B" );
    layout58->addWidget( shapeUL_B );
    spacer21 = new QSpacerItem( 20, 31, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout58->addItem( spacer21 );

    shapeLL_B = new QCheckBox( groupBox2_2, "shapeLL_B" );
    layout58->addWidget( shapeLL_B );

    groupBox2_2Layout->addMultiCellLayout( layout58, 0, 2, 0, 0 );

    layout59 = new QVBoxLayout( 0, 0, 6, "layout59"); 

    shapeUR_B = new QCheckBox( groupBox2_2, "shapeUR_B" );
    layout59->addWidget( shapeUR_B );
    spacer22 = new QSpacerItem( 20, 31, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout59->addItem( spacer22 );

    shapeLR_B = new QCheckBox( groupBox2_2, "shapeLR_B" );
    layout59->addWidget( shapeLR_B );

    groupBox2_2Layout->addMultiCellLayout( layout59, 0, 2, 5, 5 );
    spacer23 = new QSpacerItem( 60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox2_2Layout->addItem( spacer23, 1, 1 );

    textLabel1_6 = new QLabel( groupBox2_2, "textLabel1_6" );

    groupBox2_2Layout->addWidget( textLabel1_6, 1, 2 );

    borderSize_B = new QSpinBox( groupBox2_2, "borderSize_B" );
    borderSize_B->setMaxValue( 16 );
    borderSize_B->setValue( 6 );

    groupBox2_2Layout->addWidget( borderSize_B, 1, 3 );
    spacer24 = new QSpacerItem( 60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox2_2Layout->addItem( spacer24, 1, 4 );
    spacer29 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    groupBox2_2Layout->addItem( spacer29, 2, 2 );
    spacer29_2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    groupBox2_2Layout->addItem( spacer29_2, 0, 2 );

    brushedTabLayout->addMultiCellWidget( groupBox2_2, 4, 4, 0, 1 );

    drawIcon_B = new QCheckBox( brushedTab, "drawIcon_B" );

    brushedTabLayout->addWidget( drawIcon_B, 0, 0 );
    tab->insertTab( brushedTab, QString::fromLatin1("") );

    TabPage = new QWidget( tab, "TabPage" );
    TabPageLayout = new QGridLayout( TabPage, 1, 1, 11, 6, "TabPageLayout"); 

    layout7_4 = new QHBoxLayout( 0, 0, 6, "layout7_4"); 

    textLabel1_4_2 = new QLabel( TabPage, "textLabel1_4_2" );
    layout7_4->addWidget( textLabel1_4_2 );
    spacer4_2 = new QSpacerItem( 200, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout7_4->addItem( spacer4_2 );

    _3DImpact_T = new QSpinBox( TabPage, "_3DImpact_T" );
    _3DImpact_T->setMaxValue( 100 );
    _3DImpact_T->setValue( 20 );
    layout7_4->addWidget( _3DImpact_T );

    TabPageLayout->addLayout( layout7_4, 2, 0 );

    groupBox2_2_2 = new QGroupBox( TabPage, "groupBox2_2_2" );
    groupBox2_2_2->setColumnLayout(0, Qt::Vertical );
    groupBox2_2_2->layout()->setSpacing( 6 );
    groupBox2_2_2->layout()->setMargin( 11 );
    groupBox2_2_2Layout = new QGridLayout( groupBox2_2_2->layout() );
    groupBox2_2_2Layout->setAlignment( Qt::AlignTop );

    layout58_2 = new QVBoxLayout( 0, 0, 6, "layout58_2"); 

    shapeUL_T = new QCheckBox( groupBox2_2_2, "shapeUL_T" );
    layout58_2->addWidget( shapeUL_T );
    spacer21_2 = new QSpacerItem( 20, 31, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout58_2->addItem( spacer21_2 );

    shapeLL_T = new QCheckBox( groupBox2_2_2, "shapeLL_T" );
    layout58_2->addWidget( shapeLL_T );

    groupBox2_2_2Layout->addMultiCellLayout( layout58_2, 0, 2, 0, 0 );

    layout59_2 = new QVBoxLayout( 0, 0, 6, "layout59_2"); 

    shapeUR_T = new QCheckBox( groupBox2_2_2, "shapeUR_T" );
    layout59_2->addWidget( shapeUR_T );
    spacer22_2 = new QSpacerItem( 20, 31, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout59_2->addItem( spacer22_2 );

    shapeLR_T = new QCheckBox( groupBox2_2_2, "shapeLR_T" );
    layout59_2->addWidget( shapeLR_T );

    groupBox2_2_2Layout->addMultiCellLayout( layout59_2, 0, 2, 5, 5 );
    spacer23_3 = new QSpacerItem( 60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox2_2_2Layout->addItem( spacer23_3, 1, 1 );

    textLabel1_6_4 = new QLabel( groupBox2_2_2, "textLabel1_6_4" );

    groupBox2_2_2Layout->addWidget( textLabel1_6_4, 1, 2 );

    borderSize_T = new QSpinBox( groupBox2_2_2, "borderSize_T" );
    borderSize_T->setMaxValue( 16 );
    borderSize_T->setValue( 6 );

    groupBox2_2_2Layout->addWidget( borderSize_T, 1, 3 );
    spacer24_4 = new QSpacerItem( 60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox2_2_2Layout->addItem( spacer24_4, 1, 4 );
    spacer29_3 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    groupBox2_2_2Layout->addItem( spacer29_3, 2, 2 );
    spacer29_2_2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    groupBox2_2_2Layout->addItem( spacer29_2_2, 0, 2 );

    TabPageLayout->addWidget( groupBox2_2_2, 3, 0 );

    layout42 = new QHBoxLayout( 0, 0, 6, "layout42"); 

    layout27_2 = new QVBoxLayout( 0, 0, 6, "layout27_2"); 

    textLabel1_2_4_2_2 = new QLabel( TabPage, "textLabel1_2_4_2_2" );
    layout27_2->addWidget( textLabel1_2_4_2_2 );

    textLabel1_8_2_3 = new QLabel( TabPage, "textLabel1_8_2_3" );
    layout27_2->addWidget( textLabel1_8_2_3 );

    textLabel1_8_2_2_2 = new QLabel( TabPage, "textLabel1_8_2_2_2" );
    layout27_2->addWidget( textLabel1_8_2_2_2 );
    layout42->addLayout( layout27_2 );
    spacer18_2_2_2 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout42->addItem( spacer18_2_2_2 );

    layout82 = new QVBoxLayout( 0, 0, 6, "layout82"); 

    buttonStyle_T = new QComboBox( FALSE, TabPage, "buttonStyle_T" );
    layout82->addWidget( buttonStyle_T );

    textLabel1_9 = new QLabel( TabPage, "textLabel1_9" );
    layout82->addWidget( textLabel1_9 );

    textLabel1_9_2 = new QLabel( TabPage, "textLabel1_9_2" );
    layout82->addWidget( textLabel1_9_2 );
    layout42->addLayout( layout82 );

    layout41 = new QVBoxLayout( 0, 0, 6, "layout41"); 

    textLabel1_5_2 = new QLabel( TabPage, "textLabel1_5_2" );
    textLabel1_5_2->setAlignment( int( QLabel::AlignCenter ) );
    layout41->addWidget( textLabel1_5_2 );

    layout54_2 = new QHBoxLayout( 0, 0, 6, "layout54_2"); 

    activeColor1_T = new KColorButton( TabPage, "activeColor1_T" );
    layout54_2->addWidget( activeColor1_T );

    activeColor2_T = new KColorButton( TabPage, "activeColor2_T" );
    layout54_2->addWidget( activeColor2_T );
    layout41->addLayout( layout54_2 );

    textLabel1_10 = new QLabel( TabPage, "textLabel1_10" );
    textLabel1_10->setAlignment( int( QLabel::AlignCenter ) );
    layout41->addWidget( textLabel1_10 );
    layout42->addLayout( layout41 );

    TabPageLayout->addLayout( layout42, 1, 0 );

    drawIcon_T = new QCheckBox( TabPage, "drawIcon_T" );

    TabPageLayout->addWidget( drawIcon_T, 0, 0 );
    tab->insertTab( TabPage, QString::fromLatin1("") );

    TabPage_2 = new QWidget( tab, "TabPage_2" );
    TabPageLayout_2 = new QGridLayout( TabPage_2, 1, 1, 11, 6, "TabPageLayout_2"); 

    drawIcon_S = new QCheckBox( TabPage_2, "drawIcon_S" );

    TabPageLayout_2->addWidget( drawIcon_S, 0, 0 );

    layout7_5 = new QHBoxLayout( 0, 0, 6, "layout7_5"); 

    textLabel1_4_5 = new QLabel( TabPage_2, "textLabel1_4_5" );
    layout7_5->addWidget( textLabel1_4_5 );
    spacer4_5 = new QSpacerItem( 200, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout7_5->addItem( spacer4_5 );

    _3DImpact_S = new QSpinBox( TabPage_2, "_3DImpact_S" );
    _3DImpact_S->setMaxValue( 100 );
    _3DImpact_S->setValue( 20 );
    layout7_5->addWidget( _3DImpact_S );

    TabPageLayout_2->addLayout( layout7_5, 2, 0 );

    layout57_2 = new QHBoxLayout( 0, 0, 6, "layout57_2"); 

    layout27_3 = new QVBoxLayout( 0, 0, 6, "layout27_3"); 

    textLabel1_2_4_2_3 = new QLabel( TabPage_2, "textLabel1_2_4_2_3" );
    layout27_3->addWidget( textLabel1_2_4_2_3 );

    textLabel1_8_2_4 = new QLabel( TabPage_2, "textLabel1_8_2_4" );
    layout27_3->addWidget( textLabel1_8_2_4 );

    textLabel1_8_2_2_3 = new QLabel( TabPage_2, "textLabel1_8_2_2_3" );
    layout27_3->addWidget( textLabel1_8_2_2_3 );
    layout57_2->addLayout( layout27_3 );
    spacer18_2_2_3 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout57_2->addItem( spacer18_2_2_3 );

    layout26_2 = new QVBoxLayout( 0, 0, 6, "layout26_2"); 

    buttonStyle_S = new QComboBox( FALSE, TabPage_2, "buttonStyle_S" );
    layout26_2->addWidget( buttonStyle_S );

    titleeffect_S = new QComboBox( FALSE, TabPage_2, "titleeffect_S" );
    titleeffect_S->setMinimumSize( QSize( 0, 22 ) );
    layout26_2->addWidget( titleeffect_S );

    i_titleeffect_S = new QComboBox( FALSE, TabPage_2, "i_titleeffect_S" );
    i_titleeffect_S->setMinimumSize( QSize( 0, 22 ) );
    layout26_2->addWidget( i_titleeffect_S );
    layout57_2->addLayout( layout26_2 );

    layout56_2 = new QVBoxLayout( 0, 0, 6, "layout56_2"); 

    textLabel1_5_3 = new QLabel( TabPage_2, "textLabel1_5_3" );
    textLabel1_5_3->setAlignment( int( QLabel::AlignCenter ) );
    layout56_2->addWidget( textLabel1_5_3 );

    layout54_3 = new QHBoxLayout( 0, 0, 6, "layout54_3"); 

    activeColor1_S = new KColorButton( TabPage_2, "activeColor1_S" );
    layout54_3->addWidget( activeColor1_S );

    activeColor2_S = new KColorButton( TabPage_2, "activeColor2_S" );
    layout54_3->addWidget( activeColor2_S );
    layout56_2->addLayout( layout54_3 );

    layout55_2 = new QHBoxLayout( 0, 0, 6, "layout55_2"); 

    inactiveColor1_S = new KColorButton( TabPage_2, "inactiveColor1_S" );
    layout55_2->addWidget( inactiveColor1_S );

    inactiveColor2_S = new KColorButton( TabPage_2, "inactiveColor2_S" );
    layout55_2->addWidget( inactiveColor2_S );
    layout56_2->addLayout( layout55_2 );
    layout57_2->addLayout( layout56_2 );

    TabPageLayout_2->addLayout( layout57_2, 1, 0 );

    groupBox2_2_3 = new QGroupBox( TabPage_2, "groupBox2_2_3" );
    groupBox2_2_3->setColumnLayout(0, Qt::Vertical );
    groupBox2_2_3->layout()->setSpacing( 6 );
    groupBox2_2_3->layout()->setMargin( 11 );
    groupBox2_2_3Layout = new QGridLayout( groupBox2_2_3->layout() );
    groupBox2_2_3Layout->setAlignment( Qt::AlignTop );

    layout58_3 = new QVBoxLayout( 0, 0, 6, "layout58_3"); 

    shapeUL_S = new QCheckBox( groupBox2_2_3, "shapeUL_S" );
    layout58_3->addWidget( shapeUL_S );
    spacer21_3 = new QSpacerItem( 20, 31, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout58_3->addItem( spacer21_3 );

    shapeLL_S = new QCheckBox( groupBox2_2_3, "shapeLL_S" );
    layout58_3->addWidget( shapeLL_S );

    groupBox2_2_3Layout->addMultiCellLayout( layout58_3, 0, 2, 0, 0 );

    layout59_3 = new QVBoxLayout( 0, 0, 6, "layout59_3"); 

    shapeUR_S = new QCheckBox( groupBox2_2_3, "shapeUR_S" );
    layout59_3->addWidget( shapeUR_S );
    spacer22_3 = new QSpacerItem( 20, 31, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout59_3->addItem( spacer22_3 );

    shapeLR_S = new QCheckBox( groupBox2_2_3, "shapeLR_S" );
    layout59_3->addWidget( shapeLR_S );

    groupBox2_2_3Layout->addMultiCellLayout( layout59_3, 0, 2, 5, 5 );
    spacer23_4 = new QSpacerItem( 60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox2_2_3Layout->addItem( spacer23_4, 1, 1 );

    textLabel1_6_5 = new QLabel( groupBox2_2_3, "textLabel1_6_5" );

    groupBox2_2_3Layout->addWidget( textLabel1_6_5, 1, 2 );

    borderSize_S = new QSpinBox( groupBox2_2_3, "borderSize_S" );
    borderSize_S->setMaxValue( 16 );
    borderSize_S->setValue( 6 );

    groupBox2_2_3Layout->addWidget( borderSize_S, 1, 3 );
    spacer24_5 = new QSpacerItem( 60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox2_2_3Layout->addItem( spacer24_5, 1, 4 );
    spacer29_4 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    groupBox2_2_3Layout->addItem( spacer29_4, 2, 2 );
    spacer29_2_3 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    groupBox2_2_3Layout->addItem( spacer29_2_3, 0, 2 );

    TabPageLayout_2->addWidget( groupBox2_2_3, 4, 0 );

    layout10_3_2 = new QHBoxLayout( 0, 0, 6, "layout10_3_2"); 

    textLabel2_3_2 = new QLabel( TabPage_2, "textLabel2_3_2" );
    layout10_3_2->addWidget( textLabel2_3_2 );
    spacer5_3_2 = new QSpacerItem( 61, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout10_3_2->addItem( spacer5_3_2 );

    LineImpact_S = new QSpinBox( TabPage_2, "LineImpact_S" );
    LineImpact_S->setButtonSymbols( QSpinBox::UpDownArrows );
    LineImpact_S->setMaxValue( 100 );
    LineImpact_S->setValue( 40 );
    layout10_3_2->addWidget( LineImpact_S );

    TabPageLayout_2->addLayout( layout10_3_2, 3, 0 );
    tab->insertTab( TabPage_2, QString::fromLatin1("") );

    ConfigDialogLayout->addMultiCellWidget( tab, 0, 2, 2, 2 );
    languageChange();
    resize( QSize(778, 498).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
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
    setCaption( tr2i18n( "Configure Baghira Window Decoration" ) );
    generalbox->setTitle( tr2i18n( "Common Settings" ) );
    textLabel1_3->setText( tr2i18n( "Minimum Title Height" ) );
    QToolTip::add( minTH, tr2i18n( "The Titlebar height will follow your font setting.<br>However, you can set a minmum value to extend the Titlebar if you prefer small fonts.<br>18 is the minimum due to the Buttons :)" ) );
    ResizeGrip->setText( tr2i18n( "Show resize grip" ) );
    QToolTip::add( ResizeGrip, tr2i18n( "Starts in Brushed Metal mode instead of default, if the Style is set to Brushed Metal" ) );
    addAutoSpacing->setText( tr2i18n( "Add auto spacing" ) );
    fullSpec->setText( tr2i18n( "Fullscreen maximized" ) );
    QToolTip::add( fullSpec, tr2i18n( "Check this to have any window in fullscreen Mode - demaximize the window by clicking into the top right corner of your screen" ) );
    drawComicFrame->setText( tr2i18n( "Draw Comic Frame" ) );
    QToolTip::add( drawComicFrame, tr2i18n( "Draws a black frame around the window (not the Titlebar).<br>Anyway i suggest to get the kwin dropshadow patch." ) );
    noModalDeco->setText( tr2i18n( "Hide deco for fixed size modal windows" ) );
    maxResizable->setText( tr2i18n( "Keep maximized resizable" ) );
    allowEasyClosing->setText( tr2i18n( "Allow Easy Closing" ) );
    QToolTip::add( allowEasyClosing, tr2i18n( "Allows Closing by clicking into the top left or right corner <br> Close Button must be most left or right element (also no spacers) <br> Closes on Mouse Button release (so you have the chance to move the mouse away and keep the Window) <br> The Window must be active and maximized <br> Unfortunately you will not be able tho resize the Window from that corner" ) );
    defaultMode->clear();
    defaultMode->insertItem( tr2i18n( "Jaguar" ) );
    defaultMode->insertItem( tr2i18n( "Panther" ) );
    defaultMode->insertItem( tr2i18n( "Brushed Metal" ) );
    defaultMode->insertItem( tr2i18n( "Tiger" ) );
    defaultMode->insertItem( tr2i18n( "Milk" ) );
    textLabel1_7->setText( tr2i18n( "Default Mode is" ) );
    delAppname->setText( tr2i18n( "Try to remove application name\n"
"from multipart titles" ) );
    titlealign->setTitle( tr2i18n( "Title &Alignment" ) );
    QToolTip::add( titlealign, QString::null );
    QWhatsThis::add( titlealign, tr2i18n( "Use these buttons to set the alignment of the window title" ) );
    AlignLeft->setText( tr2i18n( "Left" ) );
    QToolTip::add( AlignLeft, QString::null );
    AlignHCenter->setText( tr2i18n( "Center" ) );
    QToolTip::add( AlignHCenter, QString::null );
    AlignRight->setText( tr2i18n( "Right" ) );
    QToolTip::add( AlignRight, QString::null );
    noTitle->setText( tr2i18n( "None" ) );
    ButtonColorConfig->setText( tr2i18n( "Configure Button Colors" ) );
    textLabel1_4_3->setText( tr2i18n( "3D Intensity" ) );
    textLabel2->setText( tr2i18n( "Titleshadow Intensity" ) );
    textLabel1_2_4_3->setText( tr2i18n( "ButtonStyle" ) );
    textLabel1_8_3->setText( tr2i18n( "Active Titlebar Effect" ) );
    textLabel1_8_3_2->setText( tr2i18n( "Inactive Titlebar Effect" ) );
    buttonStyle_J->clear();
    buttonStyle_J->insertItem( tr2i18n( "Panther" ) );
    buttonStyle_J->insertItem( tr2i18n( "Jaguar" ) );
    buttonStyle_J->insertItem( tr2i18n( "Milk" ) );
    buttonStyle_J->insertItem( tr2i18n( "Nostalgia" ) );
    titleeffect_J->clear();
    titleeffect_J->insertItem( tr2i18n( "Gradient" ) );
    titleeffect_J->insertItem( tr2i18n( "Stippled" ) );
    titleeffect_J->insertItem( tr2i18n( "Nostalgia" ) );
    titleeffect_J->insertItem( tr2i18n( "Glossy" ) );
    titleeffect_J->insertItem( tr2i18n( "Brushed" ) );
    titleeffect_J->insertItem( tr2i18n( "Scanlines" ) );
    QWhatsThis::add( titleeffect_J, tr2i18n( "Select an effect for the titlebar to change its appearance" ) );
    i_titleeffect_J->clear();
    i_titleeffect_J->insertItem( tr2i18n( "Gradient" ) );
    i_titleeffect_J->insertItem( tr2i18n( "Stippled" ) );
    i_titleeffect_J->insertItem( tr2i18n( "Nostalgia" ) );
    i_titleeffect_J->insertItem( tr2i18n( "Glossy" ) );
    i_titleeffect_J->insertItem( tr2i18n( "Brushed" ) );
    i_titleeffect_J->insertItem( tr2i18n( "Scanlines" ) );
    QWhatsThis::add( i_titleeffect_J, tr2i18n( "Select an effect for the titlebar to change its appearance" ) );
    textLabel1->setText( tr2i18n( "Colors" ) );
    activeColor1_J->setText( QString::null );
    activeColor2_J->setText( QString::null );
    inactiveColor1_J->setText( QString::null );
    inactiveColor2_J->setText( QString::null );
    groupBox2_4->setTitle( tr2i18n( "Round Corners" ) );
    shapeUL_J->setText( QString::null );
    shapeLL_J->setText( QString::null );
    shapeUR_J->setText( QString::null );
    shapeLR_J->setText( QString::null );
    textLabel1_6_3->setText( tr2i18n( "Border Size" ) );
    drawIcon_J->setText( tr2i18n( "Draw Icon" ) );
    tab->changeTab( jaguarTab, tr2i18n( "Jaguar" ) );
    textLabel2_2->setText( tr2i18n( "Titleshadow Intensity" ) );
    groupBox2_3->setTitle( tr2i18n( "Round Corners" ) );
    shapeUL_P->setText( QString::null );
    shapeLL_P->setText( QString::null );
    shapeUR_P->setText( QString::null );
    shapeLR_P->setText( QString::null );
    textLabel1_6_2->setText( tr2i18n( "Border Size" ) );
    textLabel1_4_4->setText( tr2i18n( "3D Intensity" ) );
    textLabel1_2_4->setText( tr2i18n( "ButtonStyle" ) );
    textLabel1_8->setText( tr2i18n( "Active Titlebar Effect" ) );
    textLabel1_8_4->setText( tr2i18n( "Inactive Titlebar Effect" ) );
    buttonStyle_P->clear();
    buttonStyle_P->insertItem( tr2i18n( "Panther" ) );
    buttonStyle_P->insertItem( tr2i18n( "Jaguar" ) );
    buttonStyle_P->insertItem( tr2i18n( "Milk" ) );
    buttonStyle_P->insertItem( tr2i18n( "Nostalgia" ) );
    titleeffect_P->clear();
    titleeffect_P->insertItem( tr2i18n( "Gradient" ) );
    titleeffect_P->insertItem( tr2i18n( "Stippled" ) );
    titleeffect_P->insertItem( tr2i18n( "Nostalgia" ) );
    titleeffect_P->insertItem( tr2i18n( "Glossy" ) );
    titleeffect_P->insertItem( tr2i18n( "Brushed" ) );
    titleeffect_P->insertItem( tr2i18n( "Scanlines" ) );
    QWhatsThis::add( titleeffect_P, tr2i18n( "Select an effect for the titlebar to change its appearance" ) );
    i_titleeffect_P->clear();
    i_titleeffect_P->insertItem( tr2i18n( "Gradient" ) );
    i_titleeffect_P->insertItem( tr2i18n( "Stippled" ) );
    i_titleeffect_P->insertItem( tr2i18n( "Nostalgia" ) );
    i_titleeffect_P->insertItem( tr2i18n( "Glossy" ) );
    i_titleeffect_P->insertItem( tr2i18n( "Brushed" ) );
    i_titleeffect_P->insertItem( tr2i18n( "Scanlines" ) );
    QWhatsThis::add( i_titleeffect_P, tr2i18n( "Select an effect for the titlebar to change its appearance" ) );
    textLabel1_2->setText( tr2i18n( "Colors" ) );
    activeColor1_P->setText( QString::null );
    activeColor2_P->setText( QString::null );
    inactiveColor1_P->setText( QString::null );
    inactiveColor2_P->setText( QString::null );
    drawIcon_P->setText( tr2i18n( "Draw Icon" ) );
    tab->changeTab( pantherTab, tr2i18n( "Panther" ) );
    textLabel2_3->setText( tr2i18n( "Titleshadow Intensity" ) );
    textLabel1_4->setText( tr2i18n( "3D Intensity" ) );
    textLabel1_2_4_2->setText( tr2i18n( "ButtonStyle" ) );
    textLabel1_8_2->setText( tr2i18n( "Active Titlebar Effect" ) );
    textLabel1_8_2_2->setText( tr2i18n( "Inactive Titlebar Effect" ) );
    buttonStyle_B->clear();
    buttonStyle_B->insertItem( tr2i18n( "Panther" ) );
    buttonStyle_B->insertItem( tr2i18n( "Jaguar" ) );
    buttonStyle_B->insertItem( tr2i18n( "Milk" ) );
    buttonStyle_B->insertItem( tr2i18n( "Nostalgia" ) );
    titleeffect_B->clear();
    titleeffect_B->insertItem( tr2i18n( "Gradient" ) );
    titleeffect_B->insertItem( tr2i18n( "Stippled" ) );
    titleeffect_B->insertItem( tr2i18n( "Nostalgia" ) );
    titleeffect_B->insertItem( tr2i18n( "Glossy" ) );
    titleeffect_B->insertItem( tr2i18n( "Brushed" ) );
    titleeffect_B->insertItem( tr2i18n( "Scanlines" ) );
    QWhatsThis::add( titleeffect_B, tr2i18n( "Select an effect for the titlebar to change its appearance" ) );
    i_titleeffect_B->clear();
    i_titleeffect_B->insertItem( tr2i18n( "Gradient" ) );
    i_titleeffect_B->insertItem( tr2i18n( "Stippled" ) );
    i_titleeffect_B->insertItem( tr2i18n( "Nostalgia" ) );
    i_titleeffect_B->insertItem( tr2i18n( "Glossy" ) );
    i_titleeffect_B->insertItem( tr2i18n( "Brushed" ) );
    i_titleeffect_B->insertItem( tr2i18n( "Scanlines" ) );
    QWhatsThis::add( i_titleeffect_B, tr2i18n( "Select an effect for the titlebar to change its appearance" ) );
    textLabel1_5->setText( tr2i18n( "Colors" ) );
    activeColor1_B->setText( QString::null );
    activeColor2_B->setText( QString::null );
    inactiveColor1_B->setText( QString::null );
    inactiveColor2_B->setText( QString::null );
    groupBox2_2->setTitle( tr2i18n( "Round Corners" ) );
    shapeUL_B->setText( QString::null );
    shapeLL_B->setText( QString::null );
    shapeUR_B->setText( QString::null );
    shapeLR_B->setText( QString::null );
    textLabel1_6->setText( tr2i18n( "Border Size" ) );
    drawIcon_B->setText( tr2i18n( "Draw Icon" ) );
    tab->changeTab( brushedTab, tr2i18n( "Brushed" ) );
    textLabel1_4_2->setText( tr2i18n( "3D Intensity" ) );
    groupBox2_2_2->setTitle( tr2i18n( "Round Corners" ) );
    shapeUL_T->setText( QString::null );
    shapeLL_T->setText( QString::null );
    shapeUR_T->setText( QString::null );
    shapeLR_T->setText( QString::null );
    textLabel1_6_4->setText( tr2i18n( "Border Size" ) );
    textLabel1_2_4_2_2->setText( tr2i18n( "ButtonStyle" ) );
    textLabel1_8_2_3->setText( tr2i18n( "Active Titlebar Effect" ) );
    textLabel1_8_2_2_2->setText( tr2i18n( "Inactive Titlebar Effect" ) );
    buttonStyle_T->clear();
    buttonStyle_T->insertItem( tr2i18n( "Panther" ) );
    buttonStyle_T->insertItem( tr2i18n( "Jaguar" ) );
    buttonStyle_T->insertItem( tr2i18n( "Milk" ) );
    buttonStyle_T->insertItem( tr2i18n( "Nostalgia" ) );
    textLabel1_9->setText( tr2i18n( "(Gradient)" ) );
    textLabel1_9_2->setText( tr2i18n( "(Scanlines)" ) );
    textLabel1_5_2->setText( tr2i18n( "Colors" ) );
    activeColor1_T->setText( QString::null );
    activeColor2_T->setText( QString::null );
    textLabel1_10->setText( tr2i18n( "(Background)" ) );
    drawIcon_T->setText( tr2i18n( "Draw Icon" ) );
    tab->changeTab( TabPage, tr2i18n( "Tiger" ) );
    drawIcon_S->setText( tr2i18n( "Draw Icon" ) );
    textLabel1_4_5->setText( tr2i18n( "3D Intensity" ) );
    textLabel1_2_4_2_3->setText( tr2i18n( "ButtonStyle" ) );
    textLabel1_8_2_4->setText( tr2i18n( "Active Titlebar Effect" ) );
    textLabel1_8_2_2_3->setText( tr2i18n( "Inactive Titlebar Effect" ) );
    buttonStyle_S->clear();
    buttonStyle_S->insertItem( tr2i18n( "Panther" ) );
    buttonStyle_S->insertItem( tr2i18n( "Jaguar" ) );
    buttonStyle_S->insertItem( tr2i18n( "Milk" ) );
    buttonStyle_S->insertItem( tr2i18n( "Nostalgia" ) );
    titleeffect_S->clear();
    titleeffect_S->insertItem( tr2i18n( "Gradient" ) );
    titleeffect_S->insertItem( tr2i18n( "Stippled" ) );
    titleeffect_S->insertItem( tr2i18n( "Nostalgia" ) );
    titleeffect_S->insertItem( tr2i18n( "Glossy" ) );
    titleeffect_S->insertItem( tr2i18n( "Brushed" ) );
    titleeffect_S->insertItem( tr2i18n( "Scanlines" ) );
    QWhatsThis::add( titleeffect_S, tr2i18n( "Select an effect for the titlebar to change its appearance" ) );
    i_titleeffect_S->clear();
    i_titleeffect_S->insertItem( tr2i18n( "Gradient" ) );
    i_titleeffect_S->insertItem( tr2i18n( "Stippled" ) );
    i_titleeffect_S->insertItem( tr2i18n( "Nostalgia" ) );
    i_titleeffect_S->insertItem( tr2i18n( "Glossy" ) );
    i_titleeffect_S->insertItem( tr2i18n( "Brushed" ) );
    i_titleeffect_S->insertItem( tr2i18n( "Scanlines" ) );
    QWhatsThis::add( i_titleeffect_S, tr2i18n( "Select an effect for the titlebar to change its appearance" ) );
    textLabel1_5_3->setText( tr2i18n( "Colors" ) );
    activeColor1_S->setText( QString::null );
    activeColor2_S->setText( QString::null );
    inactiveColor1_S->setText( QString::null );
    inactiveColor2_S->setText( QString::null );
    groupBox2_2_3->setTitle( tr2i18n( "Round Corners" ) );
    shapeUL_S->setText( QString::null );
    shapeLL_S->setText( QString::null );
    shapeUR_S->setText( QString::null );
    shapeLR_S->setText( QString::null );
    textLabel1_6_5->setText( tr2i18n( "Border Size" ) );
    textLabel2_3_2->setText( tr2i18n( "Titleshadow Intensity" ) );
    tab->changeTab( TabPage_2, tr2i18n( "Special" ) );
}

#include "configdialog.moc"
