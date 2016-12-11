#include "SkinColorModel.h"
#include "Util.h"
#include "Constant.h"

namespace cvg
{
	SkinColorModel::SkinColorModel()
	{
		InitColorConstraintHSV(_hsvColorConstraint);
	}

	Mat& SkinColorModel::extract(Mat& img, Sampler& sampler)
	{
		cvtColor(img, img, CV_BGR2HSV);

		vector<HSVColorSample>& colorSamples = sampler.colorSamples();
		bool first = true;
		for (size_t i = 0; i < colorSamples.size(); ++i)
		{
			int minHue = MAX(colorSamples[i][HUE] - _hsvColorConstraint.min(HUE), COLOR_MIN);
			int minSat = MAX(colorSamples[i][SAT] - _hsvColorConstraint.min(SAT), COLOR_MIN);
			int minVal = MAX(colorSamples[i][VAL] - _hsvColorConstraint.min(VAL), COLOR_MIN);
			_lowerBoundary = Scalar(minHue, minSat, minVal);

			int maxHue = MIN(colorSamples[i][HUE] + _hsvColorConstraint.max(HUE), COLOR_MAX);
			int maxSat = MIN(colorSamples[i][SAT] + _hsvColorConstraint.max(SAT), COLOR_MAX);
			int maxVal = MIN(colorSamples[i][VAL] + _hsvColorConstraint.max(VAL), COLOR_MAX);
			_upperBoundary = Scalar(maxHue, maxSat, maxVal);

			if (first)
			{
				inRange(img, _lowerBoundary, _upperBoundary, _threshold);
				first = false;
			}
			else
			{
				// add as weight
				inRange(img, _lowerBoundary, _upperBoundary, _thresholdTemp);
				_threshold += _thresholdTemp;
			}
		}
		// do median blur to make a more clearly color bounder
		medianBlur(_threshold, _threshold, MEDIAN_BLUR_KSIZE);
		return _threshold;
	}
}
