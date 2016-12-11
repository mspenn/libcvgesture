#ifndef __CV_GESTURE_H__
#define __CV_GESTURE_H__

#include "Common.h"
#include "DataTypes.h"

/**
* startup program with camera
* camera         - camera id
* showMask=true  - show threshold mask
* showMask=false - hide threshold mask
*/
void CVG_EXTERN startup_with_camera(int camera, bool showMask = false);

/**
* startup program with file
* showMask=true  - show threshold mask
* showMask=false - hide threshold mask
*/
void CVG_EXTERN startup_with_file(const char* path, bool showMask = false);

/**
* record video, and save video to path
*/
void CVG_EXTERN record_video(const char* path);

/** 
* bind custom handler
*/
void CVG_EXTERN bind_handler(cvg::EventType, cvg::ActionHandler);

/** 
* bind default handler
*/
void CVG_EXTERN bind_handler_default();

#endif	// __CV_GESTURE_H__