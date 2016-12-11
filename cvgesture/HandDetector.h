#ifndef __HAND_DETECTOR_H__
#define __HAND_DETECTOR_H__

#include <opencv2\opencv.hpp>
#include "Common.h"

using namespace cv;

namespace cvg
{

	class CVG_EXTERN HandDetector
	{
	public:
		HandDetector(); 

		bool determineContours(Mat&);

		void applyMorphs(Mat&);

		void drawFingerTips(Mat&);
		
		void displayContours(Mat&);
		
		void showHandGestInfo(Mat&);

		Point handCentroid()const { return _boundingCenter; }

		int determineFingerNumbers(int boundPositionYLimit, int frameSteps);

		void clearFingerNumbersCache();

	private:
		int findLargestContour(vector<vector<Point>>& contours);

		void eliminateConvexDefects();

		void removeEndPoints(vector<Vec4i>& new_defects);

		void removeCloseTips(int epsilon);

		void findFingerTips(int);

		void oneFingerCheck(int);

		int calcFingerNumber();

		vector<vector<Point>> _contours;
		vector<vector<int>> _hullIndices;	
		vector<vector<Point>> _hullPoints;	
		vector<vector<Vec4i>> _hullDefects;	
		vector<Point> _fingerTips;			
		
		vector<int> _fingerNumbers;

		int nFrames;

		int _largestContourIndex;
		int n_defects;

		Mat _threshold;

		Rect _boundingBox;
		Point _boundingCenter;

		Mat _erodeElement;
		Mat _dilateElement;

		int _handRegionAreaMin;
		int _fingerDistanceMin;
	};
}
#endif	// __HAND_DETECTOR_H__