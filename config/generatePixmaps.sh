#!/bin/sh
top_srcdir="${1:-../..}"
imagebase="$top_srcdir/imagebase"
UIC=$(grep "UIC = " ../Makefile | cut -f3- -d" ")
echo -e "#ifndef SCPIXMAPS_H\n#define SCPIXMAPS_H\n" > pixmaps.h
$UIC -embed baghira \
$imagebase/button-base \
$imagebase/button-shadow \
$imagebase/brush-me | \
sed -e "s/void qInitImages_baghira()$/static void qInitImages_baghira()/" -e "s/void qCleanupImages_baghira()$/static void qCleanupImages_baghira()/ ">> pixmaps.h
echo -e "#endif //SCPIXMAPS_H\n" >> pixmaps.h

#$imagebase/AboutBaghira \
