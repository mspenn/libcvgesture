#ifndef __UTIL_H__
#define __UTIL_H__

#include <opencv2\opencv.hpp>

namespace cvg
{
#define IMG_DATA(m, r, c, d) m.data[m.channels()*(m.cols*(r) + (c)) + (d)]

	/**
	* put text in mat: img, content, color, thickness
	*/
	void ShowText(cv::Mat&, std::string, cv::Point position, cv::Scalar, int thickness = 1);
	
	/**
	* print min max in mat
	*/
	void PrintMinMax(cv::Mat&);

	/**
	* calculate euclidean distance between 2 points
	*/
	float Distance(cv::Point&, cv::Point&);

	/**
	* read image from resource file
	* image to store
	* resource id
	*/
	cv::Mat& ReadFromResource(cv::Mat&, unsigned short);
}

#endif	// __UTIL_H__