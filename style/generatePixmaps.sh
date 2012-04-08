#!/bin/sh
top_srcdir="${1:-../..}"
imagebase="$top_srcdir/imagebase"
UIC=$(grep "UIC = " ../Makefile | cut -f3- -d" ")
echo -e "#ifndef SPIXMAPS_H\n#define SPIXMAPS_H\n" > pixmaps.h
$UIC -embed baghira \
$imagebase/brushed-gradient \
$imagebase/brushed-tile \
$imagebase/button-base \
$imagebase/button-milk \
$imagebase/button-glow \
$imagebase/button-jaguar \
$imagebase/button-shadow \
$imagebase/checkbox \
$imagebase/checkboxdown \
$imagebase/checkbox-milk \
$imagebase/checkboxdown-milk \
$imagebase/combo \
$imagebase/combo-milk \
$imagebase/combo-jaguar \
$imagebase/combo-shadow \
$imagebase/htmlradio \
$imagebase/htmlradio_down \
$imagebase/progress \
$imagebase/progress2 \
$imagebase/radio \
$imagebase/radio_down \
$imagebase/radio-milk \
$imagebase/radio_down-milk \
$imagebase/rectbutton \
$imagebase/roundFrame \
$imagebase/sbgroove_btm \
$imagebase/sbgroove_mid \
$imagebase/sbgroove_top \
$imagebase/sbIslider_mid \
$imagebase/sbslider_btm \
$imagebase/sbslider_btm_shd \
$imagebase/sbslider_mid \
$imagebase/sbslider_top \
$imagebase/sbslider_top_shd \
$imagebase/sb_subadd \
$imagebase/sliderarrow \
$imagebase/sbgroove_btm-milk \
$imagebase/sbgroove_mid-milk \
$imagebase/sbgroove_top-milk \
$imagebase/sbslider_btm-milk \
$imagebase/sbslider_mid-milk \
$imagebase/sbslider_top-milk \
$imagebase/sb_subadd-milk \
$imagebase/sliderarrow-milk \
$imagebase/tab \
$imagebase/tab-milk \
$imagebase/tab-jaguar >> pixmaps.h
echo -e "#endif //SPIXMAPS_H\n" >> pixmaps.h
