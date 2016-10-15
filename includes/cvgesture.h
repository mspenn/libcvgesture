#ifndef __CV_GESTURE_H__
#define __CV_GESTURE_H__

#include "Common.h"
#include "DataTypes.h"

void CVG_EXTERN startup_with_camera(int camera, bool showMask = false);
void CVG_EXTERN startup_with_file(const char* path, bool showMask = false);
void CVG_EXTERN record_video(const char* path);
void CVG_EXTERN bind_handler(cvg::EventType, cvg::ActionHandler);
void CVG_EXTERN bind_handler_default();
#endif