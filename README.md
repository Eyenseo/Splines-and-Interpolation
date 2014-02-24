Splines and Interpolation
=========================

This is a project from my third semester at the [Hochschule Bremen](http://www.hs-bremen.de/).<br \>
The Project started at the 07.01.2014 and ended at the 21.01.2014

The makefile was only tested under Ubuntu 13.10 x64.<br \>
The [glfw 2.7](http://www.glfw.org/download.html) library is needed. 

###Program functions:
```
In this program you are able to play with points!
  Use your mouse like this:
    Select   - Press the left mouse button anywhere on a polygon or 'curve'
    Move     - Select a point and drag it around by pressing the left mouse button
    Deselect - Press the right mouse button anywhere
               (don't try this with one point - you will most probably lose it)
  
    Add a Point   - Press the left mouse button anywhere with a selection
    Add a Polygon - Press the left mouse button anywhere without a selection
  
  You might want to use your keyboard like this:
    Pressing  1  will draw the selected polygon as polygon
    Pressing  2  will draw the selected polygon as cubic interpolation curve
    Pressing  3  will draw the selected polygon as quadratic spline curve
    Pressing  4  will draw the selected polygon as cubic spline curve
    Pressing  5  will draw the selected polygon as Bezier curve
    Pressing  8  will delete the selected point
    Pressing  9  will delete the selected polygon
    Pressing  0  will delete all polygons / points
    Pressing  C  will change the color of the selected polygon
    Pressing  S  will smooth the selected polygon
    Pressing ESC will end the program
```
