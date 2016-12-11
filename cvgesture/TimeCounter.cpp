#include <Windows.h>
#include "TimeCounter.h"

namespace cvg
{
	TimeCounter::TimeCounter()
	{
		_lastTime = GetTickCount();
	}

	double TimeCounter::GetTimeElapsed()
	{
		double currentTime = GetTickCount();
		double timeElapsed = currentTime - _lastTime;
		_lastTime = currentTime;
		return timeElapsed;
	}
}