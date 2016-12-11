#ifndef __SKIN_COLOR_MODEL__
#define __SKIN_COLOR_MODEL__

#include <opencv2\opencv.hpp>
#include "ColorConstraint.h"
#include "Sampler.h"

using namespace cv;

namespace cvg
{
	class CVG_EXTERN SkinColorModel
	{
	public:
		SkinColorModel();
		Mat& extract(Mat& img, Sampler& sampler);

	private:
		HSVColorConstraint _hsvColorConstraint;

		Scalar _lowerBoundary;
		Scalar _upperBoundary;

		//Threshold image (black and white)
		Mat _threshold;
		Mat _thresholdTemp;
	};
}

#endif	// __SKIN_COLOR_MODEL__