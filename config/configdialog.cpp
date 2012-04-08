#include <kdialog.h>
#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './configdialog.ui'
**
** Created: Sun Apr 8 21:58:28 2012
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "configdialog.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qtabwidget.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qslider.h>
#include <qcombobox.h>
#include <kcolorbutton.h>
#include <qframe.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "kcolorbutton.h"

/*
 *  Constructs a Config as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
Config::Config( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "Config" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, sizePolicy().hasHeightForWidth() ) );
    ConfigLayout = new QGridLayout( this, 1, 1, 11, 6, "ConfigLayout"); 

    layout55 = new QHBoxLayout( 0, 0, 6, "layout55"); 

    buttonHelp = new QPushButton( this, "buttonHelp" );
    buttonHelp->setAutoDefault( TRUE );
    layout55->addWidget( buttonHelp );

    buttonAbout = new QPushButton( this, "buttonAbout" );
    layout55->addWidget( buttonAbout );
    spacer37_2 = new QSpacerItem( 210, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout55->addItem( spacer37_2 );

    statusWarning = new QLabel( this, "statusWarning" );
    QFont statusWarning_font(  statusWarning->font() );
    statusWarning_font.setBold( TRUE );
    statusWarning->setFont( statusWarning_font ); 
    layout55->addWidget( statusWarning );
    spacer35_2 = new QSpacerItem( 160, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout55->addItem( spacer35_2 );

    buttonLoad = new QPushButton( this, "buttonLoad" );
    layout55->addWidget( buttonLoad );

    buttonSaveAs = new QPushButton( this, "buttonSaveAs" );
    layout55->addWidget( buttonSaveAs );

    ConfigLayout->addLayout( layout55, 1, 0 );

    tabWidget = new QTabWidget( this, "tabWidget" );
    tabWidget->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, tabWidget->sizePolicy().hasHeightForWidth() ) );

    Widget8 = new QWidget( tabWidget, "Widget8" );
    Widget8Layout = new QGridLayout( Widget8, 1, 1, 11, 6, "Widget8Layout"); 
    spacer35_5 = new QSpacerItem( 20, 38, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Widget8Layout->addItem( spacer35_5, 4, 2 );

    groupBox13 = new QGroupBox( Widget8, "groupBox13" );
    groupBox13->setColumnLayout(0, Qt::Vertical );
    groupBox13->layout()->setSpacing( 6 );
    groupBox13->layout()->setMargin( 11 );
    groupBox13Layout = new QGridLayout( groupBox13->layout() );
    groupBox13Layout->setAlignment( Qt::AlignTop );

    layout22 = new QVBoxLayout( 0, 0, 6, "layout22"); 

    shadowGroups = new QCheckBox( groupBox13, "shadowGroups" );
    layout22->addWidget( shadowGroups );

    layout7 = new QVBoxLayout( 0, 0, 6, "layout7"); 

    textLabel5 = new QLabel( groupBox13, "textLabel5" );
    layout7->addWidget( textLabel5 );

    shadowIntensity = new QSlider( groupBox13, "shadowIntensity" );
    shadowIntensity->setMaxValue( 10 );
    shadowIntensity->setLineStep( 1 );
    shadowIntensity->setPageStep( 1 );
    shadowIntensity->setOrientation( QSlider::Horizontal );
    shadowIntensity->setTickmarks( QSlider::Below );
    shadowIntensity->setTickInterval( 10 );
    layout7->addWidget( shadowIntensity );

    layout6 = new QHBoxLayout( 0, 0, 6, "layout6"); 

    textLabel6 = new QLabel( groupBox13, "textLabel6" );
    layout6->addWidget( textLabel6 );
    spacer3 = new QSpacerItem( 81, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout6->addItem( spacer3 );

    textLabel7 = new QLabel( groupBox13, "textLabel7" );
    layout6->addWidget( textLabel7 );
    layout7->addLayout( layout6 );
    layout22->addLayout( layout7 );

    groupBox13Layout->addLayout( layout22, 0, 0 );

    Widget8Layout->addMultiCellWidget( groupBox13, 1, 1, 0, 2 );

    groupBox12 = new QGroupBox( Widget8, "groupBox12" );
    groupBox12->setColumnLayout(0, Qt::Vertical );
    groupBox12->layout()->setSpacing( 6 );
    groupBox12->layout()->setMargin( 11 );
    groupBox12Layout = new QGridLayout( groupBox12->layout() );
    groupBox12Layout->setAlignment( Qt::AlignTop );

    textLabel12 = new QLabel( groupBox12, "textLabel12" );

    groupBox12Layout->addWidget( textLabel12, 0, 0 );

    colorMode = new QComboBox( FALSE, groupBox12, "colorMode" );

    groupBox12Layout->addWidget( colorMode, 1, 0 );

    layout123 = new QHBoxLayout( 0, 0, 6, "layout123"); 

    inactiveColorType = new QComboBox( FALSE, groupBox12, "inactiveColorType" );
    layout123->addWidget( inactiveColorType );

    customInactiveColor = new KColorButton( groupBox12, "customInactiveColor" );
    layout123->addWidget( customInactiveColor );

    groupBox12Layout->addLayout( layout123, 4, 0 );

    textLabel13 = new QLabel( groupBox12, "textLabel13" );

    groupBox12Layout->addWidget( textLabel13, 3, 0 );

    line14_2 = new QFrame( groupBox12, "line14_2" );
    line14_2->setFrameShape( QFrame::HLine );
    line14_2->setFrameShadow( QFrame::Sunken );
    line14_2->setFrameShape( QFrame::HLine );

    groupBox12Layout->addWidget( line14_2, 2, 0 );

    Widget8Layout->addWidget( groupBox12, 0, 3 );

    groupBox11 = new QGroupBox( Widget8, "groupBox11" );
    groupBox11->setColumnLayout(0, Qt::Vertical );
    groupBox11->layout()->setSpacing( 6 );
    groupBox11->layout()->setMargin( 11 );
    groupBox11Layout = new QGridLayout( groupBox11->layout() );
    groupBox11Layout->setAlignment( Qt::AlignTop );

    stippleBackground = new QCheckBox( groupBox11, "stippleBackground" );

    groupBox11Layout->addWidget( stippleBackground, 0, 0 );

    layout4 = new QVBoxLayout( 0, 0, 6, "layout4"); 

    textLabel2 = new QLabel( groupBox11, "textLabel2" );
    layout4->addWidget( textLabel2 );

    stippleContrast = new QSlider( groupBox11, "stippleContrast" );
    stippleContrast->setMaxValue( 10 );
    stippleContrast->setLineStep( 1 );
    stippleContrast->setPageStep( 1 );
    stippleContrast->setTracking( TRUE );
    stippleContrast->setOrientation( QSlider::Horizontal );
    stippleContrast->setTickmarks( QSlider::NoMarks );
    stippleContrast->setTickInterval( 10 );
    layout4->addWidget( stippleContrast );

    layout3 = new QHBoxLayout( 0, 0, 6, "layout3"); 

    textLabel3 = new QLabel( groupBox11, "textLabel3" );
    layout3->addWidget( textLabel3 );
    spacer2 = new QSpacerItem( 221, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout3->addItem( spacer2 );

    textLabel4 = new QLabel( groupBox11, "textLabel4" );
    layout3->addWidget( textLabel4 );
    layout4->addLayout( layout3 );

    groupBox11Layout->addLayout( layout4, 2, 0 );

    line13 = new QFrame( groupBox11, "line13" );
    line13->setFrameShape( QFrame::HLine );
    line13->setFrameShadow( QFrame::Sunken );
    line13->setFrameShape( QFrame::HLine );

    groupBox11Layout->addWidget( line13, 1, 0 );

    Widget8Layout->addWidget( groupBox11, 1, 3 );

    groupButtonPreview = new QGroupBox( Widget8, "groupButtonPreview" );
    groupButtonPreview->setColumnLayout(0, Qt::Vertical );
    groupButtonPreview->layout()->setSpacing( 6 );
    groupButtonPreview->layout()->setMargin( 11 );
    groupButtonPreviewLayout = new QGridLayout( groupButtonPreview->layout() );
    groupButtonPreviewLayout->setAlignment( Qt::AlignTop );

    layout59 = new QHBoxLayout( 0, 0, 6, "layout59"); 

    layout58 = new QVBoxLayout( 0, 0, 6, "layout58"); 
    spacer38 = new QSpacerItem( 20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout58->addItem( spacer38 );

    pixmapButtonPreview = new QLabel( groupButtonPreview, "pixmapButtonPreview" );
    pixmapButtonPreview->setScaledContents( TRUE );
    layout58->addWidget( pixmapButtonPreview );
    spacer39 = new QSpacerItem( 20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout58->addItem( spacer39 );
    layout59->addLayout( layout58 );

    layout55_2 = new QVBoxLayout( 0, 0, 6, "layout55_2"); 

    textLabel1_5 = new QLabel( groupButtonPreview, "textLabel1_5" );
    layout55_2->addWidget( textLabel1_5 );

    textLabel2_5 = new QLabel( groupButtonPreview, "textLabel2_5" );
    layout55_2->addWidget( textLabel2_5 );

    textLabel3_4 = new QLabel( groupButtonPreview, "textLabel3_4" );
    layout55_2->addWidget( textLabel3_4 );
    layout59->addLayout( layout55_2 );

    layout53 = new QVBoxLayout( 0, 0, 6, "layout53"); 

    sliderButtonRed = new QSlider( groupButtonPreview, "sliderButtonRed" );
    sliderButtonRed->setMaxValue( 255 );
    sliderButtonRed->setOrientation( QSlider::Horizontal );
    layout53->addWidget( sliderButtonRed );

    sliderButtonGreen = new QSlider( groupButtonPreview, "sliderButtonGreen" );
    sliderButtonGreen->setMaxValue( 255 );
    sliderButtonGreen->setOrientation( QSlider::Horizontal );
    layout53->addWidget( sliderButtonGreen );

    sliderButtonBlue = new QSlider( groupButtonPreview, "sliderButtonBlue" );
    sliderButtonBlue->setMaxValue( 255 );
    sliderButtonBlue->setOrientation( QSlider::Horizontal );
    layout53->addWidget( sliderButtonBlue );
    layout59->addLayout( layout53 );

    layout56 = new QVBoxLayout( 0, 0, 6, "layout56"); 

    valueButtonRed = new QLabel( groupButtonPreview, "valueButtonRed" );
    layout56->addWidget( valueButtonRed );

    valueButtonGreen = new QLabel( groupButtonPreview, "valueButtonGreen" );
    layout56->addWidget( valueButtonGreen );

    valueButtonBlue = new QLabel( groupButtonPreview, "valueButtonBlue" );
    layout56->addWidget( valueButtonBlue );
    layout59->addLayout( layout56 );

    groupButtonPreviewLayout->addLayout( layout59, 0, 0 );

    Widget8Layout->addMultiCellWidget( groupButtonPreview, 3, 3, 0, 3 );

    textLabel1_10 = new QLabel( Widget8, "textLabel1_10" );

    Widget8Layout->addWidget( textLabel1_10, 2, 0 );

    colorPresets = new QComboBox( FALSE, Widget8, "colorPresets" );

    Widget8Layout->addWidget( colorPresets, 2, 1 );
    spacer36_2 = new QSpacerItem( 207, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Widget8Layout->addItem( spacer36_2, 2, 2 );
    spacer37_4 = new QSpacerItem( 316, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Widget8Layout->addItem( spacer37_4, 2, 3 );

    animateButtons = new QCheckBox( Widget8, "animateButtons" );

    Widget8Layout->addMultiCellWidget( animateButtons, 4, 4, 0, 1 );

    groupBox10 = new QGroupBox( Widget8, "groupBox10" );
    groupBox10->setColumnLayout(0, Qt::Vertical );
    groupBox10->layout()->setSpacing( 6 );
    groupBox10->layout()->setMargin( 11 );
    groupBox10Layout = new QGridLayout( groupBox10->layout() );
    groupBox10Layout->setAlignment( Qt::AlignTop );

    line2 = new QFrame( groupBox10, "line2" );
    line2->setFrameShape( QFrame::VLine );
    line2->setFrameShadow( QFrame::Sunken );
    line2->setFrameShape( QFrame::VLine );

    groupBox10Layout->addMultiCellWidget( line2, 0, 3, 1, 1 );

    layout48 = new QVBoxLayout( 0, 0, 6, "layout48"); 

    textLabel1 = new QLabel( groupBox10, "textLabel1" );
    layout48->addWidget( textLabel1 );

    defaultStyle = new QComboBox( FALSE, groupBox10, "defaultStyle" );
    layout48->addWidget( defaultStyle );

    line14 = new QFrame( groupBox10, "line14" );
    line14->setFrameShape( QFrame::HLine );
    line14->setFrameShadow( QFrame::Sunken );
    line14->setFrameShape( QFrame::HLine );
    layout48->addWidget( line14 );
    spacer36_3 = new QSpacerItem( 20, 49, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout48->addItem( spacer36_3 );

    groupBox10Layout->addMultiCellLayout( layout48, 0, 3, 0, 0 );

    textLabel1_9 = new QLabel( groupBox10, "textLabel1_9" );

    groupBox10Layout->addWidget( textLabel1_9, 0, 2 );

    textLabel2_8 = new QLabel( groupBox10, "textLabel2_8" );

    groupBox10Layout->addWidget( textLabel2_8, 2, 2 );

    buttonStyle = new QComboBox( FALSE, groupBox10, "buttonStyle" );

    groupBox10Layout->addWidget( buttonStyle, 1, 2 );

    toolbuttonStyle = new QComboBox( FALSE, groupBox10, "toolbuttonStyle" );

    groupBox10Layout->addWidget( toolbuttonStyle, 3, 2 );

    Widget8Layout->addMultiCellWidget( groupBox10, 0, 0, 0, 2 );
    tabWidget->insertTab( Widget8, QString::fromLatin1("") );

    Widget9 = new QWidget( tabWidget, "Widget9" );
    Widget9Layout = new QGridLayout( Widget9, 1, 1, 11, 6, "Widget9Layout"); 

    groupBox9 = new QGroupBox( Widget9, "groupBox9" );
    groupBox9->setColumnLayout(0, Qt::Vertical );
    groupBox9->layout()->setSpacing( 6 );
    groupBox9->layout()->setMargin( 11 );
    groupBox9Layout = new QGridLayout( groupBox9->layout() );
    groupBox9Layout->setAlignment( Qt::AlignTop );

    brushedTabs = new QComboBox( FALSE, groupBox9, "brushedTabs" );

    groupBox9Layout->addWidget( brushedTabs, 5, 0 );

    textLabel3_7 = new QLabel( groupBox9, "textLabel3_7" );

    groupBox9Layout->addWidget( textLabel3_7, 4, 0 );

    aquaTabs = new QComboBox( FALSE, groupBox9, "aquaTabs" );

    groupBox9Layout->addWidget( aquaTabs, 3, 0 );

    textLabel2_7 = new QLabel( groupBox9, "textLabel2_7" );

    groupBox9Layout->addWidget( textLabel2_7, 2, 0 );

    line4 = new QFrame( groupBox9, "line4" );
    line4->setFrameShape( QFrame::HLine );
    line4->setFrameShadow( QFrame::Sunken );
    line4->setFrameShape( QFrame::HLine );

    groupBox9Layout->addWidget( line4, 1, 0 );

    centerTabs = new QCheckBox( groupBox9, "centerTabs" );

    groupBox9Layout->addWidget( centerTabs, 0, 0 );

    Widget9Layout->addWidget( groupBox9, 2, 1 );

    groupBox2 = new QGroupBox( Widget9, "groupBox2" );
    groupBox2->setColumnLayout(0, Qt::Vertical );
    groupBox2->layout()->setSpacing( 6 );
    groupBox2->layout()->setMargin( 11 );
    groupBox2Layout = new QGridLayout( groupBox2->layout() );
    groupBox2Layout->setAlignment( Qt::AlignTop );

    animateSlider = new QCheckBox( groupBox2, "animateSlider" );

    groupBox2Layout->addWidget( animateSlider, 0, 0 );

    squeezeSlider = new QCheckBox( groupBox2, "squeezeSlider" );

    groupBox2Layout->addWidget( squeezeSlider, 2, 0 );

    shadowSlider = new QCheckBox( groupBox2, "shadowSlider" );

    groupBox2Layout->addWidget( shadowSlider, 1, 0 );

    Widget9Layout->addMultiCellWidget( groupBox2, 0, 1, 1, 1 );
    spacer31_2 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Widget9Layout->addItem( spacer31_2, 2, 2 );

    groupBox1 = new QGroupBox( Widget9, "groupBox1" );
    groupBox1->setColumnLayout(0, Qt::Vertical );
    groupBox1->layout()->setSpacing( 6 );
    groupBox1->layout()->setMargin( 11 );
    groupBox1Layout = new QGridLayout( groupBox1->layout() );
    groupBox1Layout->setAlignment( Qt::AlignTop );

    layout68 = new QVBoxLayout( 0, 0, 6, "layout68"); 

    layout67 = new QHBoxLayout( 0, 0, 6, "layout67"); 

    drawTreeLines = new QCheckBox( groupBox1, "drawTreeLines" );
    drawTreeLines->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)4, (QSizePolicy::SizeType)0, 0, 0, drawTreeLines->sizePolicy().hasHeightForWidth() ) );
    layout67->addWidget( drawTreeLines );
    spacer60 = new QSpacerItem( 31, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout67->addItem( spacer60 );
    layout68->addLayout( layout67 );

    layout66 = new QHBoxLayout( 0, 0, 6, "layout66"); 

    textLabel8 = new QLabel( groupBox1, "textLabel8" );
    textLabel8->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)4, (QSizePolicy::SizeType)5, 0, 0, textLabel8->sizePolicy().hasHeightForWidth() ) );
    layout66->addWidget( textLabel8 );
    spacer61 = new QSpacerItem( 31, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout66->addItem( spacer61 );
    layout68->addLayout( layout66 );

    layout65 = new QHBoxLayout( 0, 0, 6, "layout65"); 

    useCustomExpanderColor = new QCheckBox( groupBox1, "useCustomExpanderColor" );
    layout65->addWidget( useCustomExpanderColor );
    spacer59 = new QSpacerItem( 31, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout65->addItem( spacer59 );
    layout68->addLayout( layout65 );

    groupBox1Layout->addLayout( layout68, 2, 0 );

    layout71 = new QVBoxLayout( 0, 0, 6, "layout71"); 

    layout70 = new QHBoxLayout( 0, 0, 6, "layout70"); 

    treeLineMode = new QComboBox( FALSE, groupBox1, "treeLineMode" );
    layout70->addWidget( treeLineMode );

    treeLineColor = new KColorButton( groupBox1, "treeLineColor" );
    treeLineColor->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)4, (QSizePolicy::SizeType)0, 0, 0, treeLineColor->sizePolicy().hasHeightForWidth() ) );
    layout70->addWidget( treeLineColor );
    layout71->addLayout( layout70 );

    expanderMode = new QComboBox( FALSE, groupBox1, "expanderMode" );
    expanderMode->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, expanderMode->sizePolicy().hasHeightForWidth() ) );
    layout71->addWidget( expanderMode );

    layout69 = new QHBoxLayout( 0, 0, 6, "layout69"); 
    spacer15 = new QSpacerItem( 150, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout69->addItem( spacer15 );

    customExpanderColor = new KColorButton( groupBox1, "customExpanderColor" );
    layout69->addWidget( customExpanderColor );
    layout71->addLayout( layout69 );

    groupBox1Layout->addLayout( layout71, 2, 1 );

    line5 = new QFrame( groupBox1, "line5" );
    line5->setFrameShape( QFrame::HLine );
    line5->setFrameShadow( QFrame::Sunken );
    line5->setFrameShape( QFrame::HLine );

    groupBox1Layout->addMultiCellWidget( line5, 1, 1, 0, 1 );

    useRectLVH = new QCheckBox( groupBox1, "useRectLVH" );

    groupBox1Layout->addMultiCellWidget( useRectLVH, 0, 0, 0, 1 );

    bevelHighlights = new QCheckBox( groupBox1, "bevelHighlights" );

    groupBox1Layout->addMultiCellWidget( bevelHighlights, 3, 3, 0, 1 );

    Widget9Layout->addWidget( groupBox1, 0, 0 );

    groupBox4 = new QGroupBox( Widget9, "groupBox4" );
    groupBox4->setColumnLayout(0, Qt::Vertical );
    groupBox4->layout()->setSpacing( 6 );
    groupBox4->layout()->setMargin( 11 );
    groupBox4Layout = new QGridLayout( groupBox4->layout() );
    groupBox4Layout->setAlignment( Qt::AlignTop );

    roundTasks = new QCheckBox( groupBox4, "roundTasks" );

    groupBox4Layout->addWidget( roundTasks, 3, 0 );

    line6 = new QFrame( groupBox4, "line6" );
    line6->setFrameShape( QFrame::HLine );
    line6->setFrameShadow( QFrame::Sunken );
    line6->setFrameShape( QFrame::HLine );

    groupBox4Layout->addWidget( line6, 4, 0 );

    replaceMenubar = new QCheckBox( groupBox4, "replaceMenubar" );

    groupBox4Layout->addWidget( replaceMenubar, 5, 0 );

    layout49 = new QHBoxLayout( 0, 0, 6, "layout49"); 

    textLabel1_11 = new QLabel( groupBox4, "textLabel1_11" );
    layout49->addWidget( textLabel1_11 );

    menuMaxWidth = new QSpinBox( groupBox4, "menuMaxWidth" );
    menuMaxWidth->setMaxValue( 10000 );
    layout49->addWidget( menuMaxWidth );

    startKRuler = new QPushButton( groupBox4, "startKRuler" );
    layout49->addWidget( startKRuler );
    spacer37_5 = new QSpacerItem( 80, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout49->addItem( spacer37_5 );

    groupBox4Layout->addLayout( layout49, 6, 0 );

    removeKickerBevel = new QCheckBox( groupBox4, "removeKickerBevel" );

    groupBox4Layout->addWidget( removeKickerBevel, 2, 0 );

    Widget9Layout->addMultiCellWidget( groupBox4, 1, 2, 0, 0 );

    layout53_2 = new QGridLayout( 0, 1, 1, 0, 6, "layout53_2"); 

    groupBox3 = new QGroupBox( Widget9, "groupBox3" );
    groupBox3->setColumnLayout(0, Qt::Vertical );
    groupBox3->layout()->setSpacing( 6 );
    groupBox3->layout()->setMargin( 11 );
    groupBox3Layout = new QGridLayout( groupBox3->layout() );
    groupBox3Layout->setAlignment( Qt::AlignTop );

    layout39 = new QVBoxLayout( 0, 0, 6, "layout39"); 

    textLabel11 = new QLabel( groupBox3, "textLabel11" );
    layout39->addWidget( textLabel11 );

    progressType = new QComboBox( FALSE, groupBox3, "progressType" );
    layout39->addWidget( progressType );

    groupBox3Layout->addLayout( layout39, 0, 0 );

    layout52 = new QHBoxLayout( 0, 0, 6, "layout52"); 

    showProgressValue = new QCheckBox( groupBox3, "showProgressValue" );
    layout52->addWidget( showProgressValue );
    spacer39_2 = new QSpacerItem( 120, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout52->addItem( spacer39_2 );

    groupBox3Layout->addLayout( layout52, 1, 0 );

    layout53_2->addWidget( groupBox3, 0, 0 );

    groupBox5 = new QGroupBox( Widget9, "groupBox5" );
    groupBox5->setColumnLayout(0, Qt::Vertical );
    groupBox5->layout()->setSpacing( 6 );
    groupBox5->layout()->setMargin( 11 );
    groupBox5Layout = new QGridLayout( groupBox5->layout() );
    groupBox5Layout->setAlignment( Qt::AlignTop );

    unhoveredToolButtons = new QComboBox( FALSE, groupBox5, "unhoveredToolButtons" );

    groupBox5Layout->addWidget( unhoveredToolButtons, 2, 0 );

    textLabel1_8 = new QLabel( groupBox5, "textLabel1_8" );

    groupBox5Layout->addWidget( textLabel1_8, 1, 0 );

    layout50 = new QHBoxLayout( 0, 0, 6, "layout50"); 

    textLabel1_12 = new QLabel( groupBox5, "textLabel1_12" );
    layout50->addWidget( textLabel1_12 );

    toolbuttonHighColor = new KColorButton( groupBox5, "toolbuttonHighColor" );
    layout50->addWidget( toolbuttonHighColor );
    spacer38_2 = new QSpacerItem( 91, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout50->addItem( spacer38_2 );

    groupBox5Layout->addLayout( layout50, 3, 0 );

    layout53_2->addWidget( groupBox5, 0, 1 );

    Widget9Layout->addMultiCellLayout( layout53_2, 3, 3, 0, 1 );
    tabWidget->insertTab( Widget9, QString::fromLatin1("") );

    tab = new QWidget( tabWidget, "tab" );
    tabLayout = new QGridLayout( tab, 1, 1, 11, 6, "tabLayout"); 
    spacer36 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    tabLayout->addItem( spacer36, 0, 0 );
    spacer37_3 = new QSpacerItem( 20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding );
    tabLayout->addItem( spacer37_3, 0, 0 );

    transGroup = new QGroupBox( tab, "transGroup" );
    transGroup->setEnabled( TRUE );
    transGroup->setColumnLayout(0, Qt::Vertical );
    transGroup->layout()->setSpacing( 6 );
    transGroup->layout()->setMargin( 11 );
    transGroupLayout = new QGridLayout( transGroup->layout() );
    transGroupLayout->setAlignment( Qt::AlignTop );

    layout64 = new QHBoxLayout( 0, 0, 6, "layout64"); 

    textLabel1_4 = new QLabel( transGroup, "textLabel1_4" );
    textLabel1_4->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)5, 0, 0, textLabel1_4->sizePolicy().hasHeightForWidth() ) );
    layout64->addWidget( textLabel1_4 );

    menuBackground = new QComboBox( FALSE, transGroup, "menuBackground" );
    menuBackground->setEnabled( TRUE );
    menuBackground->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, menuBackground->sizePolicy().hasHeightForWidth() ) );
    layout64->addWidget( menuBackground );

    transGroupLayout->addMultiCellLayout( layout64, 0, 0, 0, 1 );

    layout55_3 = new QHBoxLayout( 0, 0, 6, "layout55_3"); 

    drawMenuStripe = new QCheckBox( transGroup, "drawMenuStripe" );
    layout55_3->addWidget( drawMenuStripe );

    menuStripeColor = new KColorButton( transGroup, "menuStripeColor" );
    layout55_3->addWidget( menuStripeColor );
    spacer27_2 = new QSpacerItem( 481, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout55_3->addItem( spacer27_2 );

    transGroupLayout->addMultiCellLayout( layout55_3, 10, 10, 0, 1 );

    line10 = new QFrame( transGroup, "line10" );
    line10->setFrameShape( QFrame::HLine );
    line10->setFrameShadow( QFrame::Sunken );
    line10->setFrameShape( QFrame::HLine );

    transGroupLayout->addMultiCellWidget( line10, 1, 1, 0, 1 );

    line7 = new QFrame( transGroup, "line7" );
    line7->setFrameShape( QFrame::HLine );
    line7->setFrameShadow( QFrame::Sunken );
    line7->setFrameShape( QFrame::HLine );

    transGroupLayout->addMultiCellWidget( line7, 8, 8, 0, 1 );

    layout61 = new QVBoxLayout( 0, 0, 6, "layout61"); 

    textLabel2_2 = new QLabel( transGroup, "textLabel2_2" );
    layout61->addWidget( textLabel2_2 );

    menuOpacity = new QSlider( transGroup, "menuOpacity" );
    menuOpacity->setMaxValue( 100 );
    menuOpacity->setLineStep( 1 );
    menuOpacity->setPageStep( 1 );
    menuOpacity->setOrientation( QSlider::Horizontal );
    menuOpacity->setTickmarks( QSlider::NoMarks );
    menuOpacity->setTickInterval( 10 );
    layout61->addWidget( menuOpacity );

    layout28 = new QHBoxLayout( 0, 0, 6, "layout28"); 

    textLabel3_2 = new QLabel( transGroup, "textLabel3_2" );
    layout28->addWidget( textLabel3_2 );
    spacer17 = new QSpacerItem( 81, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout28->addItem( spacer17 );

    textLabel4_2 = new QLabel( transGroup, "textLabel4_2" );
    layout28->addWidget( textLabel4_2 );
    layout61->addLayout( layout28 );

    transGroupLayout->addMultiCellLayout( layout61, 7, 7, 0, 1 );

    line8 = new QFrame( transGroup, "line8" );
    line8->setFrameShape( QFrame::HLine );
    line8->setFrameShadow( QFrame::Sunken );
    line8->setFrameShape( QFrame::HLine );

    transGroupLayout->addMultiCellWidget( line8, 6, 6, 0, 1 );

    layout54 = new QHBoxLayout( 0, 0, 6, "layout54"); 

    textLabel2_4 = new QLabel( transGroup, "textLabel2_4" );
    layout54->addWidget( textLabel2_4 );

    menuTextColorHigh = new KColorButton( transGroup, "menuTextColorHigh" );
    layout54->addWidget( menuTextColorHigh );
    spacer29_2 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout54->addItem( spacer29_2 );

    textLabel3_6 = new QLabel( transGroup, "textLabel3_6" );
    layout54->addWidget( textLabel3_6 );

    menuColorHigh = new KColorButton( transGroup, "menuColorHigh" );
    layout54->addWidget( menuColorHigh );

    transGroupLayout->addMultiCellLayout( layout54, 5, 5, 0, 1 );

    textLabel1_7 = new QLabel( transGroup, "textLabel1_7" );

    transGroupLayout->addWidget( textLabel1_7, 4, 0 );

    line9 = new QFrame( transGroup, "line9" );
    line9->setFrameShape( QFrame::HLine );
    line9->setFrameShadow( QFrame::Sunken );
    line9->setFrameShape( QFrame::HLine );

    transGroupLayout->addWidget( line9, 4, 1 );

    layout62 = new QHBoxLayout( 0, 0, 6, "layout62"); 

    textLabel1_2 = new QLabel( transGroup, "textLabel1_2" );
    layout62->addWidget( textLabel1_2 );

    menuTextColor = new KColorButton( transGroup, "menuTextColor" );
    menuTextColor->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)4, (QSizePolicy::SizeType)0, 0, 0, menuTextColor->sizePolicy().hasHeightForWidth() ) );
    layout62->addWidget( menuTextColor );
    spacer37 = new QSpacerItem( 280, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout62->addItem( spacer37 );

    textLabel2_3 = new QLabel( transGroup, "textLabel2_3" );
    layout62->addWidget( textLabel2_3 );

    menuColor = new KColorButton( transGroup, "menuColor" );
    layout62->addWidget( menuColor );

    textLabel2_3_2 = new QLabel( transGroup, "textLabel2_3_2" );
    layout62->addWidget( textLabel2_3_2 );

    menuColor2 = new KColorButton( transGroup, "menuColor2" );
    layout62->addWidget( menuColor2 );

    transGroupLayout->addMultiCellLayout( layout62, 3, 3, 0, 1 );

    layout46 = new QHBoxLayout( 0, 0, 6, "layout46"); 

    textLabel5_2 = new QLabel( transGroup, "textLabel5_2" );
    textLabel5_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)4, (QSizePolicy::SizeType)5, 0, 0, textLabel5_2->sizePolicy().hasHeightForWidth() ) );
    layout46->addWidget( textLabel5_2 );

    menuColorMode = new QComboBox( FALSE, transGroup, "menuColorMode" );
    menuColorMode->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, menuColorMode->sizePolicy().hasHeightForWidth() ) );
    layout46->addWidget( menuColorMode );

    transGroupLayout->addMultiCellLayout( layout46, 2, 2, 0, 1 );

    glossyMenus = new QCheckBox( transGroup, "glossyMenus" );

    transGroupLayout->addWidget( glossyMenus, 11, 0 );

    shadowMenuText = new QCheckBox( transGroup, "shadowMenuText" );

    transGroupLayout->addMultiCellWidget( shadowMenuText, 9, 9, 0, 1 );

    tabLayout->addWidget( transGroup, 1, 0 );
    spacer34_2 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    tabLayout->addItem( spacer34_2, 3, 0 );

    groupBox14 = new QGroupBox( tab, "groupBox14" );
    groupBox14->setColumnLayout(0, Qt::Vertical );
    groupBox14->layout()->setSpacing( 6 );
    groupBox14->layout()->setMargin( 11 );
    groupBox14Layout = new QGridLayout( groupBox14->layout() );
    groupBox14Layout->setAlignment( Qt::AlignTop );

    linkIconSize = new QComboBox( FALSE, groupBox14, "linkIconSize" );

    groupBox14Layout->addWidget( linkIconSize, 0, 4 );

    textLabel1_13 = new QLabel( groupBox14, "textLabel1_13" );

    groupBox14Layout->addWidget( textLabel1_13, 0, 0 );

    textLabel2_9 = new QLabel( groupBox14, "textLabel2_9" );

    groupBox14Layout->addWidget( textLabel2_9, 0, 3 );

    hwIconSize = new QComboBox( FALSE, groupBox14, "hwIconSize" );

    groupBox14Layout->addWidget( hwIconSize, 0, 1 );
    spacer39_3 = new QSpacerItem( 51, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox14Layout->addItem( spacer39_3, 0, 2 );

    tabLayout->addWidget( groupBox14, 2, 0 );
    tabWidget->insertTab( tab, QString::fromLatin1("") );

    tab_2 = new QWidget( tabWidget, "tab_2" );
    tabLayout_2 = new QGridLayout( tab_2, 1, 1, 11, 6, "tabLayout_2"); 

    useCustomColors = new QCheckBox( tab_2, "useCustomColors" );

    tabLayout_2->addWidget( useCustomColors, 0, 0 );

    colorFrame = new QFrame( tab_2, "colorFrame" );
    colorFrame->setFrameShape( QFrame::StyledPanel );
    colorFrame->setFrameShadow( QFrame::Raised );
    colorFrameLayout = new QGridLayout( colorFrame, 1, 1, 11, 6, "colorFrameLayout"); 

    layout62_2 = new QVBoxLayout( 0, 0, 6, "layout62_2"); 

    layout58_2 = new QHBoxLayout( 0, 0, 6, "layout58_2"); 

    textLabel4_3 = new QLabel( colorFrame, "textLabel4_3" );
    layout58_2->addWidget( textLabel4_3 );
    spacer27 = new QSpacerItem( 61, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout58_2->addItem( spacer27 );

    radioOffColor = new KColorButton( colorFrame, "radioOffColor" );
    layout58_2->addWidget( radioOffColor );
    layout62_2->addLayout( layout58_2 );

    layout60 = new QHBoxLayout( 0, 0, 6, "layout60"); 

    textLabel5_3 = new QLabel( colorFrame, "textLabel5_3" );
    layout60->addWidget( textLabel5_3 );
    spacer31 = new QSpacerItem( 71, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout60->addItem( spacer31 );

    checkOffColor = new KColorButton( colorFrame, "checkOffColor" );
    layout60->addWidget( checkOffColor );
    layout62_2->addLayout( layout60 );

    layout62_3 = new QHBoxLayout( 0, 0, 6, "layout62_3"); 

    textLabel7_3 = new QLabel( colorFrame, "textLabel7_3" );
    layout62_3->addWidget( textLabel7_3 );
    spacer30 = new QSpacerItem( 121, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout62_3->addItem( spacer30 );

    sliderColor = new KColorButton( colorFrame, "sliderColor" );
    layout62_3->addWidget( sliderColor );
    layout62_2->addLayout( layout62_3 );

    layout64_2 = new QHBoxLayout( 0, 0, 6, "layout64_2"); 

    textLabel9_2 = new QLabel( colorFrame, "textLabel9_2" );
    layout64_2->addWidget( textLabel9_2 );
    spacer33 = new QSpacerItem( 111, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout64_2->addItem( spacer33 );

    pressedSliderColor = new KColorButton( colorFrame, "pressedSliderColor" );
    layout64_2->addWidget( pressedSliderColor );
    layout62_2->addLayout( layout64_2 );

    layout65_2 = new QHBoxLayout( 0, 0, 6, "layout65_2"); 

    textLabel11_3 = new QLabel( colorFrame, "textLabel11_3" );
    layout65_2->addWidget( textLabel11_3 );
    spacer34 = new QSpacerItem( 71, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout65_2->addItem( spacer34 );

    inactiveTabColor = new KColorButton( colorFrame, "inactiveTabColor" );
    layout65_2->addWidget( inactiveTabColor );
    layout62_2->addLayout( layout65_2 );

    colorFrameLayout->addLayout( layout62_2, 0, 0 );

    layout61_2 = new QVBoxLayout( 0, 0, 6, "layout61_2"); 

    layout59_2 = new QHBoxLayout( 0, 0, 6, "layout59_2"); 

    textLabel3_3 = new QLabel( colorFrame, "textLabel3_3" );
    layout59_2->addWidget( textLabel3_3 );
    spacer28 = new QSpacerItem( 31, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout59_2->addItem( spacer28 );

    radioOnColor = new KColorButton( colorFrame, "radioOnColor" );
    layout59_2->addWidget( radioOnColor );
    layout61_2->addLayout( layout59_2 );

    layout61_3 = new QHBoxLayout( 0, 0, 6, "layout61_3"); 

    textLabel6_3 = new QLabel( colorFrame, "textLabel6_3" );
    layout61_3->addWidget( textLabel6_3 );
    spacer29 = new QSpacerItem( 61, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout61_3->addItem( spacer29 );

    checkOnColor = new KColorButton( colorFrame, "checkOnColor" );
    layout61_3->addWidget( checkOnColor );
    layout61_2->addLayout( layout61_3 );

    layout63 = new QHBoxLayout( 0, 0, 6, "layout63"); 

    textLabel8_3 = new QLabel( colorFrame, "textLabel8_3" );
    layout63->addWidget( textLabel8_3 );
    spacer32 = new QSpacerItem( 121, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout63->addItem( spacer32 );

    hoverSliderColorColor = new KColorButton( colorFrame, "hoverSliderColorColor" );
    layout63->addWidget( hoverSliderColorColor );
    layout61_2->addLayout( layout63 );

    layout52_2 = new QHBoxLayout( 0, 0, 6, "layout52_2"); 

    textLabel1_3 = new QLabel( colorFrame, "textLabel1_3" );
    layout52_2->addWidget( textLabel1_3 );
    spacer35_3 = new QSpacerItem( 231, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout52_2->addItem( spacer35_3 );

    sliderGrooveColor = new KColorButton( colorFrame, "sliderGrooveColor" );
    layout52_2->addWidget( sliderGrooveColor );
    layout61_2->addLayout( layout52_2 );

    layout66_2 = new QHBoxLayout( 0, 0, 6, "layout66_2"); 

    textLabel10 = new QLabel( colorFrame, "textLabel10" );
    layout66_2->addWidget( textLabel10 );
    spacer35 = new QSpacerItem( 71, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout66_2->addItem( spacer35 );

    activeTabColor = new KColorButton( colorFrame, "activeTabColor" );
    layout66_2->addWidget( activeTabColor );
    layout61_2->addLayout( layout66_2 );

    colorFrameLayout->addLayout( layout61_2, 0, 2 );

    line11 = new QFrame( colorFrame, "line11" );
    line11->setFrameShape( QFrame::VLine );
    line11->setFrameShadow( QFrame::Sunken );
    line11->setFrameShape( QFrame::VLine );

    colorFrameLayout->addWidget( line11, 0, 1 );
    spacer32_2 = new QSpacerItem( 20, 51, QSizePolicy::Minimum, QSizePolicy::Expanding );
    colorFrameLayout->addItem( spacer32_2, 4, 2 );

    brushBox = new QGroupBox( colorFrame, "brushBox" );
    brushBox->setColumnLayout(0, Qt::Vertical );
    brushBox->layout()->setSpacing( 6 );
    brushBox->layout()->setMargin( 11 );
    brushBoxLayout = new QGridLayout( brushBox->layout() );
    brushBoxLayout->setAlignment( Qt::AlignTop );

    layout62_4 = new QVBoxLayout( 0, 0, 6, "layout62_4"); 

    layout59_3 = new QHBoxLayout( 0, 0, 6, "layout59_3"); 

    textLabel1_6 = new QLabel( brushBox, "textLabel1_6" );
    layout59_3->addWidget( textLabel1_6 );

    sliderBrushRed = new QSlider( brushBox, "sliderBrushRed" );
    sliderBrushRed->setMaxValue( 255 );
    sliderBrushRed->setOrientation( QSlider::Horizontal );
    layout59_3->addWidget( sliderBrushRed );
    layout62_4->addLayout( layout59_3 );

    layout60_2 = new QHBoxLayout( 0, 0, 6, "layout60_2"); 

    textLabel2_6 = new QLabel( brushBox, "textLabel2_6" );
    layout60_2->addWidget( textLabel2_6 );

    sliderBrushGreen = new QSlider( brushBox, "sliderBrushGreen" );
    sliderBrushGreen->setMaxValue( 255 );
    sliderBrushGreen->setOrientation( QSlider::Horizontal );
    layout60_2->addWidget( sliderBrushGreen );
    layout62_4->addLayout( layout60_2 );

    layout61_4 = new QHBoxLayout( 0, 0, 6, "layout61_4"); 

    textLabel3_5 = new QLabel( brushBox, "textLabel3_5" );
    layout61_4->addWidget( textLabel3_5 );

    sliderBrushBlue = new QSlider( brushBox, "sliderBrushBlue" );
    sliderBrushBlue->setMaxValue( 255 );
    sliderBrushBlue->setOrientation( QSlider::Horizontal );
    layout61_4->addWidget( sliderBrushBlue );
    layout62_4->addLayout( layout61_4 );

    brushBoxLayout->addLayout( layout62_4, 0, 1 );

    pixmapBrushPreview = new QLabel( brushBox, "pixmapBrushPreview" );
    pixmapBrushPreview->setScaledContents( TRUE );

    brushBoxLayout->addWidget( pixmapBrushPreview, 0, 0 );

    colorFrameLayout->addMultiCellWidget( brushBox, 3, 3, 0, 2 );

    tintBrush = new QCheckBox( colorFrame, "tintBrush" );

    colorFrameLayout->addMultiCellWidget( tintBrush, 2, 2, 0, 2 );

    line12 = new QFrame( colorFrame, "line12" );
    line12->setFrameShape( QFrame::HLine );
    line12->setFrameShadow( QFrame::Sunken );
    line12->setFrameShape( QFrame::HLine );

    colorFrameLayout->addMultiCellWidget( line12, 1, 1, 0, 2 );

    tabLayout_2->addWidget( colorFrame, 1, 0 );
    tabWidget->insertTab( tab_2, QString::fromLatin1("") );

    ConfigLayout->addWidget( tabWidget, 0, 0 );
    languageChange();
    resize( QSize(643, 646).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
Config::~Config()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void Config::languageChange()
{
    setCaption( tr2i18n( "Configure Your Cat" ) );
    buttonHelp->setText( tr2i18n( "&Help" ) );
    buttonHelp->setAccel( QKeySequence( tr2i18n( "F1" ) ) );
    buttonAbout->setText( tr2i18n( "About" ) );
    statusWarning->setText( QString::null );
    buttonLoad->setText( tr2i18n( "Load..." ) );
    buttonSaveAs->setText( tr2i18n( "Save As..." ) );
    groupBox13->setTitle( tr2i18n( "Groupboxes" ) );
    shadowGroups->setText( tr2i18n( "Sink Groupboxes" ) );
    textLabel5->setText( tr2i18n( "Depth" ) );
    textLabel6->setText( tr2i18n( "Flat" ) );
    textLabel7->setText( tr2i18n( "Deep" ) );
    groupBox12->setTitle( tr2i18n( "Coloring" ) );
    textLabel12->setText( tr2i18n( "Color function" ) );
    colorMode->clear();
    colorMode->insertItem( tr2i18n( "Aquarius" ) );
    colorMode->insertItem( tr2i18n( "Liquid (depreached)" ) );
    QToolTip::add( colorMode, tr2i18n( "\"Aquarius\" is very Aqua like<br>\"Liquid\" bases upon mosfet's original Code" ) );
    inactiveColorType->clear();
    inactiveColorType->insertItem( tr2i18n( "Fixed" ) );
    inactiveColorType->insertItem( tr2i18n( "Background" ) );
    inactiveColorType->insertItem( tr2i18n( "Active Button" ) );
    inactiveColorType->insertItem( tr2i18n( "Custom" ) );
    QToolTip::add( inactiveColorType, tr2i18n( "\"Background\" is ok, as long as you don't use a very dark color scheme. In this case use \"Fixed\" or \"Custom\" to get more control over the appereance" ) );
    customInactiveColor->setText( QString::null );
    textLabel13->setText( tr2i18n( "Unhovered / Inactive Widget Color" ) );
    groupBox11->setTitle( tr2i18n( "Scanlines" ) );
    stippleBackground->setText( tr2i18n( "Use Scanlines in Aqua Design" ) );
    textLabel2->setText( tr2i18n( "Scanline contrast" ) );
    textLabel3->setText( tr2i18n( "Low" ) );
    textLabel4->setText( tr2i18n( "High" ) );
    groupButtonPreview->setTitle( tr2i18n( "Button Color" ) );
    QToolTip::add( groupButtonPreview, tr2i18n( "<b>This now will affect your button color, but <i>not</i> touch the color settings</b>" ) );
    textLabel1_5->setText( tr2i18n( "R" ) );
    textLabel2_5->setText( tr2i18n( "G" ) );
    textLabel3_4->setText( tr2i18n( "B" ) );
    valueButtonRed->setText( tr2i18n( "0" ) );
    valueButtonGreen->setText( tr2i18n( "0" ) );
    valueButtonBlue->setText( tr2i18n( "0" ) );
    textLabel1_10->setText( tr2i18n( "Color presets" ) );
    animateButtons->setText( tr2i18n( "Animate Buttons" ) );
    groupBox10->setTitle( tr2i18n( "Generation" ) );
    textLabel1->setText( tr2i18n( "Default Design is" ) );
    defaultStyle->clear();
    defaultStyle->insertItem( tr2i18n( "Jaguar" ) );
    defaultStyle->insertItem( tr2i18n( "Panther" ) );
    defaultStyle->insertItem( tr2i18n( "iTunes" ) );
    defaultStyle->insertItem( tr2i18n( "Tiger" ) );
    defaultStyle->insertItem( tr2i18n( "Milk" ) );
    QToolTip::add( defaultStyle, QString::null );
    textLabel1_9->setText( tr2i18n( "Buttons look like" ) );
    textLabel2_8->setText( tr2i18n( "Toolbuttons look like" ) );
    buttonStyle->clear();
    buttonStyle->insertItem( tr2i18n( "Jaguar" ) );
    buttonStyle->insertItem( tr2i18n( "Panther" ) );
    buttonStyle->insertItem( tr2i18n( "iTunes" ) );
    buttonStyle->insertItem( tr2i18n( "Tiger" ) );
    buttonStyle->insertItem( tr2i18n( "Milk" ) );
    toolbuttonStyle->clear();
    toolbuttonStyle->insertItem( tr2i18n( "Jaguar" ) );
    toolbuttonStyle->insertItem( tr2i18n( "Panther" ) );
    toolbuttonStyle->insertItem( tr2i18n( "iTunes" ) );
    toolbuttonStyle->insertItem( tr2i18n( "Tiger" ) );
    toolbuttonStyle->insertItem( tr2i18n( "Milk" ) );
    tabWidget->changeTab( Widget8, tr2i18n( "General Design" ) );
    groupBox9->setTitle( tr2i18n( "Tabs" ) );
    QToolTip::add( groupBox9, tr2i18n( "<qt>Panther introduced so called choosers<br>Qt has only Tabs, but you can influence their appereance here a bit</qt>" ) );
    brushedTabs->clear();
    brushedTabs->insertItem( tr2i18n( "Tabs" ) );
    brushedTabs->insertItem( tr2i18n( "Clever" ) );
    brushedTabs->insertItem( tr2i18n( "Choosers" ) );
    textLabel3_7->setText( tr2i18n( "Brushed Tabs are" ) );
    aquaTabs->clear();
    aquaTabs->insertItem( tr2i18n( "Tabs" ) );
    aquaTabs->insertItem( tr2i18n( "Clever" ) );
    aquaTabs->insertItem( tr2i18n( "Choosers" ) );
    textLabel2_7->setText( tr2i18n( "Aqua tabs are" ) );
    centerTabs->setText( tr2i18n( "Center Align Tabs" ) );
    groupBox2->setTitle( tr2i18n( "Scrollbars" ) );
    animateSlider->setText( tr2i18n( "Animate Hovered Slider" ) );
    squeezeSlider->setText( tr2i18n( "Squeeze pressed Slider" ) );
    shadowSlider->setText( tr2i18n( "Shadow Slider" ) );
    groupBox1->setTitle( tr2i18n( "ListViews" ) );
    drawTreeLines->setText( tr2i18n( "Draw Tree Lines" ) );
    textLabel8->setText( tr2i18n( "Expanders look like" ) );
    useCustomExpanderColor->setText( tr2i18n( "Use Custom Expander Color" ) );
    treeLineMode->clear();
    treeLineMode->insertItem( tr2i18n( "Lines" ) );
    treeLineMode->insertItem( tr2i18n( "Dots" ) );
    treeLineColor->setText( QString::null );
    expanderMode->clear();
    expanderMode->insertItem( tr2i18n( "Apple" ) );
    expanderMode->insertItem( tr2i18n( "M$" ) );
    QToolTip::add( expanderMode, tr2i18n( "Expanders are the little icons you can click to open a new Layer in the tree" ) );
    customExpanderColor->setText( QString::null );
    useRectLVH->setText( tr2i18n( "Use flatter Listview Header" ) );
    QToolTip::add( useRectLVH, tr2i18n( "Makes Listview Headers look like the rectangular Buttons" ) );
    bevelHighlights->setText( tr2i18n( "Use Bevel as Highlight Color" ) );
    groupBox4->setTitle( tr2i18n( "Kicker" ) );
    roundTasks->setText( tr2i18n( "Use Round Taskbuttons" ) );
    replaceMenubar->setText( tr2i18n( "Replace menubar extension (display app name)" ) );
    textLabel1_11->setText( tr2i18n( "Maximum width" ) );
    menuMaxWidth->setSuffix( tr2i18n( "px" ) );
    startKRuler->setText( tr2i18n( "Start KRuler" ) );
    removeKickerBevel->setText( tr2i18n( "Remove Bevel" ) );
    groupBox3->setTitle( tr2i18n( "Progressbars" ) );
    textLabel11->setText( tr2i18n( "Style" ) );
    progressType->clear();
    progressType->insertItem( tr2i18n( "Apple" ) );
    progressType->insertItem( tr2i18n( "Liquid" ) );
    progressType->insertItem( tr2i18n( "Baghira" ) );
    showProgressValue->setText( tr2i18n( "Show Value" ) );
    groupBox5->setTitle( tr2i18n( "Toolbars" ) );
    unhoveredToolButtons->clear();
    unhoveredToolButtons->insertItem( tr2i18n( "Aqua" ) );
    unhoveredToolButtons->insertItem( tr2i18n( "Brushed Metal" ) );
    unhoveredToolButtons->insertItem( tr2i18n( "Both" ) );
    unhoveredToolButtons->insertItem( tr2i18n( "None" ) );
    textLabel1_8->setText( tr2i18n( "Show unhovered buttons for" ) );
    textLabel1_12->setText( tr2i18n( "Highlight Color" ) );
    toolbuttonHighColor->setText( QString::null );
    tabWidget->changeTab( Widget9, tr2i18n( "Special Widgets" ) );
    transGroup->setTitle( tr2i18n( "Look" ) );
    textLabel1_4->setText( tr2i18n( "Background" ) );
    menuBackground->clear();
    menuBackground->insertItem( tr2i18n( "Default" ) );
    menuBackground->insertItem( tr2i18n( "Plain" ) );
    menuBackground->insertItem( tr2i18n( "Stipples" ) );
    menuBackground->insertItem( tr2i18n( "Gradient" ) );
    QToolTip::add( menuBackground, tr2i18n( "Solid is fastest, Gradient is slowest - choose by your personal preferences and the speed of your machine" ) );
    drawMenuStripe->setText( tr2i18n( "Draw menu stripe" ) );
    menuStripeColor->setText( QString::null );
    textLabel2_2->setText( tr2i18n( "Opacity" ) );
    QToolTip::add( menuOpacity, QString::null );
    textLabel3_2->setText( tr2i18n( "Translucent" ) );
    textLabel4_2->setText( tr2i18n( "Opaque" ) );
    textLabel2_4->setText( tr2i18n( "Text" ) );
    menuTextColorHigh->setText( QString::null );
    textLabel3_6->setText( tr2i18n( "Background" ) );
    menuColorHigh->setText( QString::null );
    textLabel1_7->setText( tr2i18n( "Highlighted" ) );
    textLabel1_2->setText( tr2i18n( "Text" ) );
    menuTextColor->setText( QString::null );
    textLabel2_3->setText( tr2i18n( "Background" ) );
    menuColor->setText( QString::null );
    textLabel2_3_2->setText( tr2i18n( "Background 2" ) );
    menuColor2->setText( QString::null );
    textLabel5_2->setText( tr2i18n( "Color" ) );
    menuColorMode->clear();
    menuColorMode->insertItem( tr2i18n( "Background" ) );
    menuColorMode->insertItem( tr2i18n( "Button" ) );
    menuColorMode->insertItem( tr2i18n( "Custom" ) );
    QToolTip::add( menuColorMode, tr2i18n( "Qt usually uses the button color for the menus... well, Apple does not." ) );
    glossyMenus->setText( tr2i18n( "Use glossy highlights" ) );
    shadowMenuText->setText( tr2i18n( "Shadow itemtext" ) );
    groupBox14->setTitle( tr2i18n( "Sidebar" ) );
    linkIconSize->clear();
    linkIconSize->insertItem( tr2i18n( "16x16" ) );
    linkIconSize->insertItem( tr2i18n( "22x22" ) );
    linkIconSize->insertItem( tr2i18n( "32x32" ) );
    linkIconSize->insertItem( tr2i18n( "48x48" ) );
    linkIconSize->insertItem( tr2i18n( "64x64" ) );
    linkIconSize->insertItem( tr2i18n( "128x128" ) );
    textLabel1_13->setText( tr2i18n( "Hardware Iconsize" ) );
    textLabel2_9->setText( tr2i18n( "Link Iconsize" ) );
    hwIconSize->clear();
    hwIconSize->insertItem( tr2i18n( "16x16" ) );
    hwIconSize->insertItem( tr2i18n( "22x22" ) );
    hwIconSize->insertItem( tr2i18n( "32x32" ) );
    hwIconSize->insertItem( tr2i18n( "48x48" ) );
    hwIconSize->insertItem( tr2i18n( "64x64" ) );
    hwIconSize->insertItem( tr2i18n( "128x128" ) );
    tabWidget->changeTab( tab, tr2i18n( "Menus" ) );
    useCustomColors->setText( tr2i18n( "Use Custom Widget Colors" ) );
    QToolTip::add( useCustomColors, tr2i18n( "These Widgets default to the Button Color, however you can customize them here" ) );
    textLabel4_3->setText( tr2i18n( "Radiobutton (\"Off\")" ) );
    radioOffColor->setText( QString::null );
    textLabel5_3->setText( tr2i18n( "Unchecked Box" ) );
    checkOffColor->setText( QString::null );
    textLabel7_3->setText( tr2i18n( "Scrollbar Slider" ) );
    sliderColor->setText( QString::null );
    textLabel9_2->setText( tr2i18n( "Pressed Slider" ) );
    pressedSliderColor->setText( QString::null );
    textLabel11_3->setText( tr2i18n( "Inactive Tab" ) );
    inactiveTabColor->setText( QString::null );
    textLabel3_3->setText( tr2i18n( "Radiobutton (\"On\")" ) );
    radioOnColor->setText( QString::null );
    textLabel6_3->setText( tr2i18n( "Checked Box" ) );
    checkOnColor->setText( QString::null );
    textLabel8_3->setText( tr2i18n( "Hovered Slider" ) );
    hoverSliderColorColor->setText( QString::null );
    textLabel1_3->setText( tr2i18n( "Slider Groove" ) );
    sliderGrooveColor->setText( QString::null );
    textLabel10->setText( tr2i18n( "Active Tab" ) );
    activeTabColor->setText( QString::null );
    brushBox->setTitle( tr2i18n( "Tint the Brush" ) );
    textLabel1_6->setText( tr2i18n( "R" ) );
    textLabel2_6->setText( tr2i18n( "G" ) );
    textLabel3_5->setText( tr2i18n( "B" ) );
    tintBrush->setText( tr2i18n( "Tint Brushed Metal" ) );
    tabWidget->changeTab( tab_2, tr2i18n( "Custom Colors" ) );
}

#include "configdialog.moc"
