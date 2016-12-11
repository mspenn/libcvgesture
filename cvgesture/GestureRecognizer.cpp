#include "GestureRecognizer.h"
#include "HandDetector.h"
#include "Constant.h"

#ifdef __cplusplus_normal__
#ifdef WIN32
#include <Windows.h>
#endif
#else
#include <thread>
#endif

using namespace std;

namespace cvg
{

#ifndef NO_HAND_DATA
	inline void makeHandData(HandData& handData, HandType handType, int fingerNum)
	{
		handData._fingerNum = fingerNum;
		handData._handType = handType;
	}
#endif


#ifdef __cplusplus_normal__

	struct GestureRecognitionData
	{
		GeometricRecognizer& _geometricRecognizer;
		Path2D _path;
		HandType _handType;
		int _fingerNum;
		GestureRecognitionData(GeometricRecognizer& gr, Path2D& p2d, HandType ht, int fn)
			:_geometricRecognizer(gr)
			, _path(p2d)
			, _handType(ht)
			, _fingerNum(fn)
		{}
	};

#ifdef WIN32
	DWORD WINAPI tGestureRecognition(LPVOID pM)
#else
#endif
	{
		GestureRecognitionData* grd = static_cast<GestureRecognitionData*>(pM);
		GeometricRecognizer& gr = grd->_geometricRecognizer;
		Path2D& p2d = grd->_path;
		HandType handType = grd->_handType;
		int fingerNum = grd->_fingerNum;

		RecognitionResult r = gr.recognize(p2d);
		//printf("Recognized Gesture: %s, Score: %f\n", r.name.c_str(), r.score);
#ifndef NO_HAND_DATA
		HandData handData;
		makeHandData(handData, handType, fingerNum);
		for (auto& p : p2d)
		{
			handData._handTrace.push_back(Vec2i(p.x, p.y));
		}
#endif
		EventType et = UNKWON;

		if (r.score > 0.75)
		{
			et = (r.name.compare(GESTURE_LEFT) == 0) ? MOVE_LEFT : MOVE_RIGHT;
		}

		auto& em = GestureRecognizer::EVENT_MAP;
		if (em.find(et) != em.end())
		{
			em[et](handData);
		}
		delete(grd);
		return 0;
	}

#ifdef WIN32
	DWORD WINAPI tInvokeGestureEvent(LPVOID pM)
#else
#endif
	{
		HandData handData;
		EventType* eventType = static_cast<EventType*>(pM);
		auto& em = GestureRecognizer::EVENT_MAP;
		if (em.find(*eventType) != em.end())
		{
			em[*eventType](handData);
		}
		delete eventType;
		return 0;
	}
#endif

	inline void dispatchEvent(EventType eventType)
	{
#ifdef __cplusplus_normal__
#ifdef WIN32
		EventType* p_et = new EventType(eventType);
		HANDLE handle = CreateThread(NULL, 0, tInvokeGestureEvent, (LPVOID)p_et, 0, NULL);
#endif
#else
		thread tGestureRecognition([=]()
		{
#ifndef NO_HAND_DATA
			HandData handData;
#endif
			auto& em = GestureRecognizer::EVENT_MAP;
			if (em.find(eventType) != em.end())
			{
#ifndef NO_HAND_DATA
				em[eventType](handData);
#else
				em[eventType]();
#endif
			}			
		});
		tGestureRecognition.detach();
#endif
	}

	map<EventType, ActionHandler> GestureRecognizer::EVENT_MAP;

	GestureRecognizer::GestureRecognizer(const Size& size) :
		_gestureState(GESTURE_NONE),
		_boundingBox(size),
		_frameStepsMin(FRAME_STEP_MIN)
	{
	}

	GestureRecognizer::~GestureRecognizer(){}

	void GestureRecognizer::step(GestureTranstionData& gtd)
	{
		switch (_gestureState)
		{
		case GESTURE_NONE:gesureFromNone(gtd); break;
		case GESTURE_PALM:gesureFromPalm(gtd); break;
		case GESTURE_FIST:gesureFromFist(gtd); break;
		}
	}
	
	bool GestureRecognizer::changeGestureStateTo(GestureState newState)
	{
		if (_gestureState == newState)
		{
			// Gesture State Not Changed
			return false;
		}
		_gestureState = newState;

#ifdef DEBUG_CHANGE_STATE
		if (newState == GESTURE_PALM)
			printf("Change Gesture State To PALM\n");
		else if (newState == GESTURE_FIST)
			printf("Change Gesture State To FIST\n");
		else if (newState == GESTURE_NONE)
			printf("Change Gesture State To NONE\n");
#endif
		return true;
	}
	
	void GestureRecognizer::gesureFromNone(GestureTranstionData& gtd)
	{
		// transition from none, if there is a hand in screen 
		if (gtd._hasHand)
		{
			HandDetector& hd = gtd._handDetector;
			int n = hd.determineFingerNumbers(_boundingBox.height / 2, _frameStepsMin);
			// if there are fingers
			if (n >= 0)
			{
				// if the number of fingers more than LIMIT?
				if (n > LIMIT)
				{
					changeGestureStateTo(GESTURE_PALM);
					dispatchEvent(HAND_IN);
				}
				else if (n <= LIMIT)
				{
					changeGestureStateTo(GESTURE_FIST);
					dispatchEvent(HAND_IN);
				}
			}
		}
	}

	void GestureRecognizer::handleTrace(vector<Point>& trace, HandType handType, int fingerNum)
	{

		if (trace.size() < 2)return;

#ifdef DEBUG_TRACE
		for (size_t i = 0; i < trace.size(); i++)
		{
			printf("<%d, %d> ", trace[i].x, trace[i].y);
		}
		printf("\n");
#endif
#ifdef __cplusplus_normal__
#ifdef WIN32
		GestureRecognitionData* pgrd =new GestureRecognitionData(_geometricRecognizer, p2d, handType, fingerNum);
		HANDLE handle = CreateThread(NULL, 0, tGestureRecognition, (LPVOID)pgrd, 0, NULL);
#endif
#else
		// start a thread to do event dispatcher
		// so that it may not affect the main UI thread
		thread tGestureRecognition([trace, handType, fingerNum]()
		{
#ifndef NO_HAND_DATA
			HandData handData;
			makeHandData(handData, handType, fingerNum);
			for (auto& p : trace)
			{
				handData._handTrace.push_back(Vec2i(p.x, p.y));
			}
#endif
			EventType et = UNKWON;

			// a direction vector 
			Point d = trace[1] - trace[0];
			
			// if the direction is right or right-up?
			if (0 <= d.x)
			{
				// if x>=y? less than 45 degrees?
				if (d.x >= abs(d.y))
					et = MOVE_RIGHT;
				else
					et = d.y >= 0 ? MOVE_UP : MOVE_DOWN;
				
			}
			else if (d.x <= 0)
			{
				// if x>=y? less than 45 degrees?
				if (abs(d.x) >= abs(d.y))
					et = MOVE_LEFT;
				else
					et = d.y >= 0 ? MOVE_UP : MOVE_DOWN;
				
			}
			
			// find event handler in EVENT_MAP, and handle it
			auto& em = GestureRecognizer::EVENT_MAP;
			if (em.find(et) != em.end())
			{
#ifndef NO_HAND_DATA
				em[et](handData);
#else
				em[et]();
#endif
			}
		});
		tGestureRecognition.detach();
#endif
		trace[0] = trace[1];
		trace.pop_back();
	}

	void GestureRecognizer::gesureFromPalm(GestureTranstionData& gtd)
	{
		HandDetector& hd = gtd._handDetector;
		GestureState nextState = GESTURE_PALM; 
		int n = -1;
		EventType et = UNKWON;
		if (gtd._hasHand)
		{
			n = hd.determineFingerNumbers(_boundingBox.height / 2, _frameStepsMin);
			if (n >= 0)
			{
				if (n > LIMIT)
				{
					// is moving palm now
					_handTrace.push_back(hd.handCentroid());
					handleTrace(_handTrace, HandType::PALM, n);
					return;
				}
				else if (n <= LIMIT)
				{
					nextState = GESTURE_FIST;
					et = PALM_CLOSE;
				}
			}
		}
		else
		{
			nextState = GESTURE_NONE;
			et = HAND_OUT;
		}

		if (changeGestureStateTo(nextState))
		{
			handleTrace(_handTrace, HandType::PALM, n);
			hd.clearFingerNumbersCache();
			dispatchEvent(et);
		}
	}

	void GestureRecognizer::gesureFromFist(GestureTranstionData& gtd)
	{
		HandDetector& hd = gtd._handDetector;
		GestureState nextState = GESTURE_FIST;
		int n = -1;
		EventType et = UNKWON;
		if (gtd._hasHand)
		{
			n = hd.determineFingerNumbers(_boundingBox.height / 2, _frameStepsMin);
			if (n >= 0)
			{
				if (n > LIMIT)
				{
					nextState = GESTURE_PALM;
					et = PALM_OPEN;
				}
				else if (n <= LIMIT)
				{
					// is moving fist now
					_handTrace.push_back(hd.handCentroid());
					handleTrace(_handTrace, HandType::FIST, n);
					return;
				}
			}
		}
		else
		{
			nextState = GESTURE_NONE;
			et = HAND_OUT;
		}

		if (changeGestureStateTo(nextState))
		{
			handleTrace(_handTrace, HandType::FIST, n);
			hd.clearFingerNumbersCache();
			dispatchEvent(et);
		}
	}
}