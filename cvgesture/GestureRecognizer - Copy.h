#ifndef __GESTURE_RECOGNIZER_H__
#define __GESTURE_RECOGNIZER_H__

#include <opencv2\opencv.hpp>
#include <vector>
#include <map>
#include "Common.h"
#include "DataTypes.h"
#include "gesture/GeometricRecognizer.h"

using namespace cv;
using namespace DollarRecognizer;

namespace cvg
{
	class  HandDetector;

	enum CVG_EXTERN GestureState
	{
		GESTURE_NONE = 0,
		GESTURE_PALM,
		GESTURE_FIST
	};

	struct CVG_EXTERN GestureTranstionData
	{
		bool _hasHand;
		HandDetector& _handDetector;
		GestureTranstionData(bool hasHand, HandDetector& handDector) :
			_hasHand(hasHand),
			_handDetector(handDector){}
	};

	class CVG_EXTERN GestureRecognizer
	{
	public:
		GestureRecognizer(const Size& size);

		~GestureRecognizer();

		void step(GestureTranstionData&);

		void setBoundingBox(Size box){ this->_boundingBox = box; }

	private:
		void handleTrace(vector<Point>& trace, HandType handType, int fingerNum);

		void gesureFromNone(GestureTranstionData&);

		void gesureFromPalm(GestureTranstionData&);

		void gesureFromFist(GestureTranstionData&);

		inline bool changeGestureStateTo(GestureState);

		GestureState _gestureState;

		Size _boundingBox;

		int _frameStepsMin;

		vector<Point> _handTrace;

		GeometricRecognizer _geometricRecognizer;

	public:
		static map<EventType, ActionHandler> EVENT_MAP;
	};

}

#endif