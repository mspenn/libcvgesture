#ifndef __DATA_TYPES_H__
#define __DATA_TYPES_H__

#ifndef __cplusplus_normal__
#include <functional>
#endif
#include <vector>
#include "Common.h"

namespace cvg
{
	enum CVG_EXTERN EventType
	{
		MOVE_LEFT = 0,
		MOVE_RIGHT,
		MOVE_UP,
		MOVE_DOWN,
		PALM_CLOSE,
		PALM_OPEN,
		HAND_IN,
		HAND_OUT,
		UNKWON
	};

	enum CVG_EXTERN HandType
	{
		PALM = 0,
		FIST
	};

	struct CVG_EXTERN Vec2i
	{
		int x, y;
		Vec2i(){}
		Vec2i(int x_, int y_) :x(x_), y(y_){}
	};

#ifndef NO_HAND_DATA
	struct CVG_EXTERN HandData
	{
		std::vector<Vec2i> _handTrace;
		int _fingerNum;
		HandType _handType;
		HandData() :_fingerNum(-1){}
	};
#else 
#define HandData
#define handData
#endif

#ifndef __cplusplus_normal__
	typedef std::function<void(HandData)> ActionHandler;
#else
	typedef void(*ActionHandler)(HandData);
#endif
}

#endif	// __DATA_TYPES_H__