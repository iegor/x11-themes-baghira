#ifndef FRACTS_H
#define FRACTS_H

/*
If you wish to replace the button or combo Images, you may end up with very querky results.
The reason is, that the style internally fracts those images into 9 tiles.

To get valid results, you need to adjust the values below, so they fit your images.
the format is:
w, h, xOff, yOff, centerW, centerH, shadowH, glowWH

- w, h are width and height of the whole button.
- xOff  (yOff) is the distance of the central fraction from left (upper) button side.
- centerW (centerH) is the width (height) of the central button fraction.
- shadowH is the height of the buttons shadow
- glowWH is the size of the frame the glow draws around the button

an image may be helpful (make sure your editor uses a monospace font)

[- - - - - - - - - - - - - - -   w   - - - - - - - - - - - - -]---
|----------------|--------------------------|-----------------| |
|                |                          |                 | 
|              yOff                         |                 | |
|                |                          |                 | 
|----- xOff----->o--------- centerW --------|-----------------| |
|                |                          |                 | 
|                |                       centerH              | h
|                |                          |                 | 
|----------------|--------------------------|-----------------| |
|                |                          |                 | 
|                |                          |                 | |
|                |                          |                 | 
|                |                          |                 | |
|----------------|--------------------------|-----------------| |
.                                                              ---

Also please notice, that the Button tiles are treated different from the rectangualr ones, namely, (S: static, T: tiled, TH: tiled horizontally, TV: tiled vertically)

Round Button, Combo:
|----------------|--------------------------|-----------------|
|                |                          |                 |
|       S        |            TH            |        S        |
|                |                          |                 |
|----------------|--------------------------|-----------------|
|                |                          |                 |
|       TV       |            T             |       TV        |
|                |                          |                 |
|----------------|--------------------------|-----------------|
|                |                          |                 |
|                |                          |                 |
|       S        |           TH             |        S        |
|                |                          |                 |
|----------------|--------------------------|-----------------|

Rectangular Button:
|----------------|--------------------------|-----------------|
|                |                          |                 |
|       TV       |            TH            |        TV       |
|                |                          |                 |
|----------------|--------------------------|-----------------|
|                |                          |                 |
|       S        |            TH            |        S        |
|                |                          |                 |
|----------------|--------------------------|-----------------|
|                |                          |                 |
|                |                          |                 |
|       TV       |           TH             |        TV       |
|                |                          |                 |
|----------------|--------------------------|-----------------|

*/


#endif
