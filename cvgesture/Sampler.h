#ifndef __SAMPLER_H__
#define __SAMPLER_H__

#include <vector>
#include <opencv2\opencv.hpp>
#include "ROI.h"
#include "Frame.h"
#include "ColorConstraint.h"

namespace cvg
{
	/**
	* Color sampler
	*/
	class CVG_EXTERN Sampler
	{
	public:
		Sampler();
		~Sampler();
		
		int medoid(std::vector<int>&);
		void doSample(Mat& img);
		void averageColor(ROI& roi, ColorSample3i& colorSample);
		void genSampleRegion(Frame& frame);
		inline void addSampleRegion(ROI& roi);

		inline vector<ROI>& sampleRegions(){ return _sampleRegions; }
		inline vector<HSVColorSample>& colorSamples(){ return _colorSamples; }

	private:
		vector<ROI> _sampleRegions;

		int _sampleCellSize;
		int _sampleCount;
		int _sampleTimes;

		// hsv color samples
		vector<int> _hueSamples;
		vector<int> _satSamples;
		vector<int> _valSamples;
		
		vector<HSVColorSample> _colorSamples;

		Mat _buffer;
	};

}

#endif	// __SAMPLER_H__