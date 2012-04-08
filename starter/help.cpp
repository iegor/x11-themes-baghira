#include <kdialog.h>
#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './help.ui'
**
** Created: Sun Apr 8 21:59:56 2012
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "help.h"

#include <qvariant.h>
#include <ktextedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "ktextedit.h"

/*
 *  Constructs a HelpDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
HelpDialog::HelpDialog( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "HelpDialog" );
    HelpDialogLayout = new QGridLayout( this, 1, 1, 11, 6, "HelpDialogLayout"); 

    kTextEdit2 = new KTextEdit( this, "kTextEdit2" );
    kTextEdit2->setFrameShape( KTextEdit::LineEditPanel );
    kTextEdit2->setFrameShadow( KTextEdit::Sunken );
    kTextEdit2->setMargin( 0 );
    kTextEdit2->setReadOnly( TRUE );
    kTextEdit2->setUndoRedoEnabled( FALSE );

    HelpDialogLayout->addMultiCellWidget( kTextEdit2, 0, 0, 0, 1 );

    buttonClose = new QPushButton( this, "buttonClose" );

    HelpDialogLayout->addWidget( buttonClose, 1, 1 );
    spacer4 = new QSpacerItem( 231, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    HelpDialogLayout->addItem( spacer4, 1, 0 );
    languageChange();
    resize( QSize(600, 480).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( buttonClose, SIGNAL( released() ), this, SLOT( close() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
HelpDialog::~HelpDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void HelpDialog::languageChange()
{
    setCaption( tr2i18n( "Help" ) );
    kTextEdit2->setText( tr2i18n( "<p align=\"center\"><font size=\"+3\"><b>Menu entry editor</b></font></p>\n"
"<p align=\"center\">----------------------------------------------------------------------------------</p>\n"
"<h3>Introduction</h3>\n"
"\n"
"By default, you will only see the basic settings for the new entry. Clicking \"Show More\" will give you access to some extended settings that can be interesting but mostly useless for you.<br><br>\n"
"<b>Bold options must be entered</b> for a usefull entry, i<i>talic ones are respected by the search feature</i>.\n"
"<br><br>\n"
"<h3>Basic settings</h3>\n"
"\n"
"<b>Name:</b><br>\n"
"This is the visible name of your new entry and can be any string, e.g. \"The Gimp\".<br>\n"
"(Necessary, searchkey)\n"
"<br><br>\n"
"<b>Is a:</b><br>\n"
"Describes the applications genre (generic name), e.g. \"Image manipulation\"<br>\n"
"(Optional, yet not searched - maybe later)\n"
"<br><br>\n"
"<b>Category:</b><br>\n"
"Choose an existing group or add a new one. The hierarchy is represented by seperating slashes (\"/\"), if you want to enter a slash, you must escape it (\"\\/\")<br>\n"
"(Necessary, searchkey)\n"
"<br><br>\n"
"<b>Command:</b><br>\n"
"The command to start the application, e.g. \"gimp-remote\". On *nix systems, is usually not necessary to pass the full path to the executably, but you can do so, if you want to start an executable that is shadowed by the executable in the path dir, e.g. \"/usr/local/gimp-1.3/gimp-remote\"<br>\n"
"(Necessary, not searched)\n"
"<br><br>\n"
"<b>Keywords:</b><br>\n"
"Comma separated list of keywords that refer to this application during search, e.g. \"image manipulation,pixel,photoshop\".<br>\n"
"Please note:<br>\n"
"1. search is <i>not</i> case sensitive<br>\n"
"2. search finds partial matches, so it's <i>not</i> necessary to add e.g. \"image,image manipulation\"<br>\n"
"3. different from the applications name, the keyword list will be translated (if) so if you think like \"'KImage' allready contains 'image', so i don't need it as keyword\" <b>you're wrong!</b><br>\n"
"4. Finding good keywords is not simple, but in general use striking ones! \"editor\" is not a very good keyword, as allmost everything is an editor. (Gimp is a pixel-editor, KHexedit is a hex-editor, KEdit is a text-editor, a config dialog is a config-editor, ...)<br>\n"
"(Optional, searchkey)\n"
"<br><br>\n"
"<b>Description:</b><br>\n"
"This is the longtext description of your application (not a helptext, though ;), e.g. \"A powerfull image manipulator with a UI similar to photoshop. Supports Layers, filters, scripting, blahblahblah...\"\n"
"You can use Qt richtext tags and there's no limit on the size, but keep it usefull ;) let's say something about 200 chars at max.<br>\n"
"(Optional, not searched)" ) );
    buttonClose->setText( tr2i18n( "Close" ) );
}

#include "help.moc"
