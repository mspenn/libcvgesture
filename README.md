# libcvgesture
camera gesture recognition based on opencv.
download [libcvgesture](https://github.com/mspenn/libcvgesture/blob/master/libcvgesture.zip "Download Link").

## General Idea

### Using sking color model
+ Convert RGB color to HSV
+ Sample color from HSV color space
+ Extract color using HSV color space

### Using simple motion detection
+ Split foreground and background
+ Mask image extract skin color with foreground threshold

## Screenshots
+ Let's start! Ooops, it looks like the Frostbite logo, aha!!
![screenshot-1](https://github.com/mspenn/libcvgesture/blob/master/screenshots/screenshot-1.png)

+ Moving my hand and detecting, and it output the gesture info in the console window.
![screenshot-2](https://github.com/mspenn/libcvgesture/blob/master/screenshots/screenshot-2.png)
