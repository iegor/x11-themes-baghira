#!/bin/sh
version=0.2
appname=Baghira
appversion=0.6d
kdialog --passivepopup $(pwd) 50&
if ! kdialog --title "$appname Installer $version" --yesno "<qt><p>This will install $appname $appversion on your system</p><p align="center"><b>Proceed?</b></p></qt>"
then
exit 0
fi
PREFIX=$(kde-config --prefix)
    if ! kdialog --title "$appname Installer $version" --yesno "<qt><p>Suggested destination is <b>$PREFIX</b></p><p>Shall $appname be installed to this location?</p></qt>"
then
PREFIX=$(kdialog --title "$appname Installer $version" --getexistingdirectory $PREFIX)
fi
if [ "$PREFIX" = "" ]
then
exit 0
fi
if kdialog --title "$appname Installer $version" --yesno "<qt>Compile with <b>Debug</b> support?</qt>"
then
DEBUG=""
else
DEBUG="--disable-debug"
fi
if kdialog --title "$appname Installer $version" --yesno "<qt><b>Optimize</b> build?<br>(Will need slightly more memory and time)</qt>"
then
FINAL="--enable-final"
else
FINAL=""
fi
DREF=$(kdialog --title "$appname Installer $version" --progressbar "<qt><b>Running configure</b><br></qt>" 4)
dcop $DREF setProgress 1
if [ $(./configure --prefix=$PREFIX $DEBUG $FINAL) != 0 ]
then
dcop $DREF close
kdialog --title "$appname Installer $version" --error "Configure Failed"
kdialog --title "$appname Installer $version" --textbox "config.log" 300 200
exit 1
fi
dcop $DREF setLabel "<qt><b>Making</b></qt>"
dcop $DREF setProgress 2
if ! make 2>&1 | echo > make.log
then
dcop $DREF close
kdialog --title "$appname Installer $version" --error "Make Failed"
kdialog --title "$appname Installer $version" --textbox "make.log" 300 200
exit 1
fi
dcop $DREF setLabel "<qt><b>Installing</b></qt>"
dcop $DREF setProgress 3
if ! kdesu make install 2>&1 | echo > install.log
then
dcop $DREF close
kdialog --title "$appname Installer $version" --error "Install Failed"
kdialog --title "$appname Installer $version" --textbox "install.log" 300 200
exit 1
fi
dcop $DREF close
kdialog --title "$appname Installer $version" --msgbox "Installation successfull"
exit 0