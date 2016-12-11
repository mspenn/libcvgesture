#include <opencv2\imgproc.hpp>
#include <opencv2\highgui.hpp>

#include "HandDetector.h"
#include "Constant.h"
#include "Util.h"

namespace cvg
{
	HandDetector::HandDetector()
	{
		nFrames = 0;

		_handRegionAreaMin = HAND_REGION_AREA_MIN;
		_erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
		_dilateElement = getStructuringElement(MORPH_RECT, Size(8, 8));
	}

	// Erode the image to remove the background noise
	// Dilate the threshold with larger structuring elements to ensure that the hand object is nicely visible
	void HandDetector::applyMorphs(Mat& threshold)
	{
		pyrUp(threshold, threshold);

		erode(threshold, threshold, _erodeElement);
		erode(threshold, threshold, _erodeElement);

		dilate(threshold, threshold, _dilateElement);
		dilate(threshold, threshold, _dilateElement);

		threshold.copyTo(_threshold);
	}

	// find the biggest contour out of all
	// Unless there is a lot of background noise or interference, 
	// the largest contour should be of the hand
	int HandDetector::findLargestContour(vector<vector<Point>>& contours)
	{
		int largestContorIdx = -1; //initialize index to -1, in case things go wrong and no biggest contour is found
		int largestContorSize = 0;

		for (int i = 0; i < contours.size(); i++) {
			if (contours[i].size() > largestContorSize) {
				largestContorSize = contours[i].size();
				largestContorIdx = i;
			}
		}
		return largestContorIdx;
	}

	void HandDetector::eliminateConvexDefects() {
		int minDist = _boundingBox.height / 5;

		//Acceptable maximum angle between two tip points
		//float maxAngle = 95;

		//Store all uneliminated defect points in this vector
		vector<Vec4i> new_defects;

		int start_index;
		int end_index;
		int far_index;

		//Iterator to go through the elements of the defects vector
		vector<Vec4i>::iterator it = _hullDefects[_largestContourIndex].begin();
		while (it != _hullDefects[_largestContourIndex].end()) {
			Vec4i& x = (*it);

			start_index = x[0];
			Point start_point(_contours[_largestContourIndex][start_index]);

			end_index = x[1];
			Point end_point(_contours[_largestContourIndex][end_index]);

			far_index = x[2];
			Point far_point(_contours[_largestContourIndex][far_index]);

			if (Distance(start_point, far_point) > minDist
				&& Distance(end_point, far_point) > minDist)
			{

				if ((end_point.y > (_boundingBox.y + _boundingBox.height - (_boundingBox.height / 4)))
					|| start_point.y > (_boundingBox.y + _boundingBox.height - (_boundingBox.height / 4))) {

				}
				else {
					new_defects.push_back(x);
				}
			}
			it++;
		}
		n_defects = new_defects.size();
		_hullDefects[_largestContourIndex].swap(new_defects);
		removeEndPoints(_hullDefects[_largestContourIndex]);
	}

	void HandDetector::removeEndPoints(vector<Vec4i>& new_defects) {
		float minWidth = this->_boundingBox.width / 6;

		int start_index, start_index2, end_index, end_index2;

		// if prev-end point is closed to next-start point
		// merge closed point
		for (int i = 0; i < new_defects.size(); ++i) {
			for (int j = i; j < new_defects.size(); ++j) {
				start_index = new_defects[i][0];
				Point start_point(_contours[_largestContourIndex][start_index]);

				end_index = new_defects[i][1];
				Point end_point(_contours[_largestContourIndex][end_index]);

				start_index2 = new_defects[j][0];
				Point start_point2(_contours[_largestContourIndex][start_index2]);

				end_index2 = new_defects[j][1];
				Point end_point2(_contours[_largestContourIndex][end_index2]);

				if (Distance(start_point, end_point2) < minWidth) {
					_contours[_largestContourIndex][start_index] = end_point2;
					break;
				}
				if (Distance(end_point, start_point2) < minWidth) {
					_contours[_largestContourIndex][start_index2] = end_point;
				}
			}
		}
	}

	// check if there is only one finger
	// if we have a highest point
	void HandDetector::oneFingerCheck(int highestBound) {
		int minHeight = _boundingBox.height / 6;

		Point highest_point;

		highest_point.y = highestBound;

		vector<Point>::iterator it = _contours[_largestContourIndex].begin();

		while (it != _contours[_largestContourIndex].end()) {
			Point x = (*it);

			if (x.y < highest_point.y) {
				highest_point = x;
			}
			it++;
		}
		int n = 0;

		it = _hullPoints[_largestContourIndex].begin();
		while (it != _hullPoints[_largestContourIndex].end()) {
			Point v = (*it);

			if (v.y < highest_point.y + minHeight
				&& v.y != highest_point.y
				&& v.x != highest_point.x) {
				n++;
			}
			it++;
		}
		if (n == 0) {
			_fingerTips.push_back(highest_point);
		}
	}

	// find finger tips
	void HandDetector::findFingerTips(int highestBound) {
		_fingerTips.clear();

		bool first = true;

		vector<Vec4i>::iterator it = _hullDefects[_largestContourIndex].begin();

		while (it != _hullDefects[_largestContourIndex].end()) {
			Vec4i& x = (*it);

			int start_index = x[0];
			Point start_point(_contours[_largestContourIndex][start_index]);

			int end_index = x[1];
			Point end_point(_contours[_largestContourIndex][end_index]);

			int far_index = x[2];
			Point far_point(_contours[_largestContourIndex][far_index]);

			if (first == true) {
				_fingerTips.push_back(start_point);
				first = false;
			}

			_fingerTips.push_back(end_point);
			it++;
			first = false;

			if (_fingerTips.size() == 0) {
				oneFingerCheck(highestBound);
			}
		}
	}

	bool HandDetector::determineContours(Mat& threshold) {
		//cvtColor(img->sourceCopy, img->sourceCopy, CV_HLS2BGR);
		//cvtColor(img->sourceCopy, img->sourceCopy, CV_HSV2BGR);

		//Apply structuring elements to make the thresholded image more precise
		applyMorphs(threshold);

		//Finds contour points from the threshold and saves them in the contours vector of points
		findContours(_threshold, _contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

		//Initalize all vetors
		_hullIndices = vector<vector<int>>(_contours.size());
		_hullPoints = vector<vector<Point>>(_contours.size());
		_hullDefects = vector<vector<Vec4i>>(_contours.size());

		//Find the index of the biggest contour from the vector of contours
		_largestContourIndex = findLargestContour(_contours);

		//If the index is found proceed
		if (_largestContourIndex != -1) {
			//Create a bounding box (rectangle) around the biggest contour
			_boundingBox = boundingRect(Mat(_contours[_largestContourIndex]));
			_boundingCenter = Point(_boundingBox.x + _boundingBox.width / 2, _boundingBox.y + _boundingBox.height / 2);

			//printf("AREA: %d\n", _boundingBox.area());
			if (_boundingBox.area() < _handRegionAreaMin)return false;

			//Find all convex points in the biggest contour set of points and save them to _hullPoints
			convexHull(Mat(_contours[_largestContourIndex]), _hullPoints[_largestContourIndex], false, true);

			//Find all indeces of the convex points within the contours vector
			convexHull(Mat(_contours[_largestContourIndex]), _hullIndices[_largestContourIndex], false, false);

			//Find the approxymate polygon cure around the biggest contour with a given precision of 15
			approxPolyDP(Mat(_hullPoints[_largestContourIndex]), _hullPoints[_largestContourIndex], 15, false);

			//then eliminate convex defect points
			if (_contours[_largestContourIndex].size() > 3) {
				convexityDefects(_contours[_largestContourIndex], _hullIndices[_largestContourIndex], _hullDefects[_largestContourIndex]);
				eliminateConvexDefects();
			}

			findFingerTips(threshold.rows);
			removeCloseTips(_fingerDistanceMin);
			return true;
		}
		return false;
	}

	// remove if two finger tips is closed
	void HandDetector::removeCloseTips(int epsilon) {
		vector<Point> new_tips;

		for (int i = 0; i < _fingerTips.size(); i++) {
			for (int j = 0; j < _fingerTips.size(); j++) {
				if (Distance(_fingerTips[i], _fingerTips[j]) < epsilon && i != j) {

				}
				else if (Distance(_fingerTips[i], _fingerTips[j]) > epsilon) {
					new_tips.push_back(_fingerTips[i]);
					break;
				}
			}
		}
		_fingerTips.swap(new_tips);
	}

	void HandDetector::clearFingerNumbersCache()
	{
		nFrames = 0;
		_fingerNumbers.clear();
	}

	int HandDetector::determineFingerNumbers(int handPostionYLimit, int frameSteps) 
	{
		int n = -1;
		if (_boundingBox.height > handPostionYLimit) 
		{
			_fingerNumbers.push_back(_fingerTips.size()); 
			if (nFrames >= frameSteps) {
				nFrames = 0;
				n = calcFingerNumber();
				_fingerNumbers.clear();
			}
			else nFrames++;
		}
		return n;
	}

	int HandDetector::calcFingerNumber()
	{
		// statistics frequent finger in frames
		// choose the most frequent finger number
		std::sort(_fingerNumbers.begin(), _fingerNumbers.end());

		int mostFreqNum;
		int thisNFrequency = 1;
		int highestFreq = 1;

		mostFreqNum = _fingerNumbers[0];

		for (int i = 1; i < _fingerNumbers.size(); ++i) {
			if (_fingerNumbers[i - 1] != _fingerNumbers[i]) {
				if (thisNFrequency > highestFreq) {
					mostFreqNum = _fingerNumbers[i - 1];
					highestFreq = thisNFrequency;
				}
				thisNFrequency = 0;
			}
			thisNFrequency++;
		}
		if (thisNFrequency > highestFreq) {
			mostFreqNum = _fingerNumbers[_fingerNumbers.size() - 1];
		}
		return mostFreqNum;
	}
	
	// draw finger tips in matrix (image)
	void HandDetector::drawFingerTips(Mat& img) {
		Point p;
		char info[100];
		for (int i = 0; i < _fingerTips.size(); ++i) {
			p = _fingerTips[i];
			sprintf(info, "%d", i);
			ShowText(img, string(info), p - Point(0, 30), Scalar(200, 200, 200));
			circle(img, p, 5, Scalar(0, 255, 0), 4);
		}
	}

	// draw contours in matrix (image)
	void HandDetector::displayContours(Mat& img)
	{
		drawContours(img, _hullPoints, _largestContourIndex, cv::Scalar(200, 0, 0), 2, 8, std::vector<Vec4i>(), 0, Point());

		rectangle(img, _boundingBox.tl(), _boundingBox.br(), cv::Scalar(0, 0, 200));

		vector<Vec4i>::iterator d = _hullDefects[_largestContourIndex].begin();
		int fontFace = FONT_HERSHEY_PLAIN;


		while (d != _hullDefects[_largestContourIndex].end()) {
			Vec4i& v = (*d);
			int startidx = v[0]; Point ptStart(_contours[_largestContourIndex][startidx]);
			int endidx = v[1]; Point ptEnd(_contours[_largestContourIndex][endidx]);
			int faridx = v[2]; Point ptFar(_contours[_largestContourIndex][faridx]);
			float depth = v[3] / 256;

			line(img, ptStart, ptFar, Scalar(0, 255, 0), 1);
			line(img, ptEnd, ptFar, Scalar(0, 255, 0), 1);
			circle(img, ptFar, 4, Scalar(0, 255, 0), 2);
			circle(img, ptEnd, 4, Scalar(0, 0, 255), 2);
			circle(img, ptStart, 4, Scalar(255, 0, 0), 2);
			d++;
		}
	}

	// draw hand gesture info in matrix (image)
	void HandDetector::showHandGestInfo(Mat& m) {
		Scalar text_color(255, 255, 255);

		int x = m.cols / 8;
		int y = m.rows / 1.1;

		float font_size = 0.6;

		int next_line = 20;

		char info[100];

		sprintf(info, "Hand Gesture Information:");
		ShowText(m, string(info), Point(x, y), text_color);
		y += next_line;

		sprintf(info, "Number of defects: %d", n_defects);
		ShowText(m, info, Point(x, y), text_color);
		y += next_line;

		sprintf(info, "Bounding Box: (W: %d, H:%d)", _boundingBox.width, _boundingBox.height);
		ShowText(m, info, Point(x, y), text_color);
	}

}