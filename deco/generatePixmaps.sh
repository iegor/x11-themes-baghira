#!/bin/sh
top_srcdir="${1:-../..}"
imagebase="$top_srcdir/imagebase"
UIC=$(grep "UIC = " ../Makefile | cut -f3- -d" ")
echo -e "#ifndef DPIXMAPS_H\n#define DPIXMAPS_H\n" > pixmaps.h
$UIC -embed baghira \
$imagebase/brushed-gradient \
$imagebase/brushed-tile \
$imagebase/button_jaguar \
$imagebase/button_jaguar_down \
$imagebase/button_jaguar_menu \
$imagebase/button_jaguar_menu_down \
$imagebase/button_milk \
$imagebase/button_milk_down \
$imagebase/button_milk_menu \
$imagebase/button_milk_menu_down \
$imagebase/button_panther \
$imagebase/button_panther_menu \
$imagebase/deco_glossy \
$imagebase/icon_above \
$imagebase/icon_behind \
$imagebase/icon_close \
$imagebase/icon_help \
$imagebase/icon_maximize \
$imagebase/icon_minimize \
$imagebase/icon_shade \
$imagebase/icon_sticky >> pixmaps.h
echo -e "#endif //DPIXMAPS_H\n" >> pixmaps.h
