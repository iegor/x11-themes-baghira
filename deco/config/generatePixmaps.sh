#!/bin/sh
top_srcdir="${1:-../..}"
imagebase="$top_srcdir/imagebase"
UIC=$(grep "UIC = " ../../Makefile | cut -f3- -d" ")
echo -e "#ifndef DCPIXMAPS_H\n#define DCPIXMAPS_H\n" > pixmaps.h
$UIC -embed baghira \
$imagebase/icon_help \
$imagebase/preview \
$imagebase/preview-menu >> pixmaps.h
echo -e "#endif //DCPIXMAPS_H\n" >> pixmaps.h
