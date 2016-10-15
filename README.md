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
+ Let's start!
![screenshot-1](https://github.com/mspenn/libcvgesture/blob/master/screenshots/screenshot-1.png)
+ Detecting..
![screenshot-2](https://github.com/mspenn/libcvgesture/blob/master/screenshots/screenshot-2.png)
