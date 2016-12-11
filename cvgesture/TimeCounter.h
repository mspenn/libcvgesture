#ifndef __TIME_COUNTER_H__
#define __TIME_COUNTER_H__

#include "Common.h"

namespace cvg
{
	class CVG_EXTERN TimeCounter
	{
	public:
		TimeCounter();

		//How many milliseconds have elapsed since getTimeElapsed was last called
		double GetTimeElapsed();

	private:
		double _lastTime;
	};
}

#endif	// __TIME_COUNTER_H__