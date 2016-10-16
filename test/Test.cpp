#include "cvgesture.h"

using namespace cvg;

//#define BIND_DEFAULT

int main()
{
#ifndef BIND_DEFAULT
	// init bind handlers first
	bind_handler(EventType::UNKWON, [](HandData handData){
		printf("Gesture Not Recognized.\n");
	});
	bind_handler(EventType::HAND_IN, [](HandData handData){
		printf("Hand In.\n");
	});
	bind_handler(EventType::HAND_OUT, [](HandData handData){
		printf("Hand Out.\n");
	});
	bind_handler(EventType::MOVE_LEFT, [](HandData handData){
		printf("Move Left.\n");
	});
	bind_handler(EventType::MOVE_RIGHT, [](HandData handData){
		printf("Move Right.\n");
	});
	bind_handler(EventType::PALM_OPEN, [](HandData handData){
		printf("Palm Open.\n");
	});	
	bind_handler(EventType::PALM_CLOSE, [](HandData handData){
		printf("Palm Closed.\n");
	});
#else
	bind_handler_default();
#endif
	// start up from a video file, press 'q' to quit
	//startup_with_file("test.avi", true);

	// start up from web camera, 0 by default, press 'q' to quit
	startup_with_camera(0, true);

	// record video if you want, press 'q' to quit
	//record_video("cvg-001.avi");
	return 0;
}