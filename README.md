# libcvgesture

camera gesture recognition based on opencv.
download [libcvgesture](https://github.com/mspenn/libcvgesture/raw/master/libcvgesture.zip "Download Link").

## Development Env
+ Windows 7
+ Visual Studio 2013
+ [OpenCV 2.4.13](http://downloads.sourceforge.net/project/opencvlibrary/opencv-win/2.4.13/opencv-2.4.13.exe?r=http%3A%2F%2Fopencv.org%2F&ts=1476585624&use_mirror=nchc "OpenCV2413")


## General Idea

### Sking Color Model
+ Convert RGB color to HSV
+ Sample color from HSV color space
+ Extract color using HSV color space

### Average Motion Detection
+ Split foreground and background
+ Mask image extracted by skin color with foreground threshold

### Simple Gesture Recognition
+ recognize gesture by [$1 algorithm](http://depts.washington.edu/aimgroup/proj/dollar/index.html "DollarOne Algorithm")

## Screenshots

+ Let's start! Ooops, it looks like the Frostbite logo, aha!!
![screenshot-1](https://github.com/mspenn/libcvgesture/blob/master/screenshots/screenshot-1.png)

+ Moving my hand and detecting, and it output the gesture info in the console window.
![screenshot-2](https://github.com/mspenn/libcvgesture/blob/master/screenshots/screenshot-2.png)

## How to use it?

### Setup OpenCV

+ Link modules  
  link opencv modules([view introductions](http://docs.opencv.org/2.4/modules/refman.html "OpenCV Modules")) you need.  
  minimal modules(\* stands for Debug/Release version):
<pre><code>
opencv_core2413*.lib
opencv_highgui2413*.lib
opencv_imgproc2413*.lib
opencv_gpu2413*.lib
opencv_video2413*.lib
</code></pre>

+ Copy shared library to exectution root.
<pre><code>
opencv_core2413*.dll
opencv_highgui2413*.dll
opencv_imgproc2413*.dll
opencv_gpu2413*.dll
opencv_video2413*.dll
<strong>opencv_ffmpeg2413.dll</strong>
</code></pre>

Just remember to copy _opencv_ffmpeg2413.dll_

### Use libcvgesture

+ Setup includes
`
cvgesture.h
`

> Configure in VS IDE:

`Project Properties > VC++ Directories > Include Directories` or `Project Properties > C/C++ > General > Additional Include Directories`

+ Setup libraies
`
cvgesture*.lib
`

> Configure in VS IDE:

`Project Properties > VC++ Directories > Library Directories`

  And copy _cvgesture\*.dll_ to execution root

### Demos
use C-Style functions, just like [Test.cpp](https://github.com/mspenn/libcvgesture/blob/master/test/Test.cpp)
