#ifndef __M_MATH_H__
#define __M_MATH_H__

#include <opencv2\opencv.hpp>

using namespace cv;

double LinearRegression(const vector<Point>& points, double& k, double& b);

#endif	// __M_MATH_H__