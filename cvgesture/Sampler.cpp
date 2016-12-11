#include "Sampler.h"
#include "Util.h"
#include "Constant.h"

namespace cvg
{
	Sampler::Sampler():
		_sampleCount(SAMPLE_COUNT_DEFAULT),
		_sampleCellSize(SAMPLE_CELL_SIZE),
		_sampleTimes(0)
	{
	}

	Sampler::~Sampler()
	{

	}
	
	int Sampler::medoid(vector<int>& v) 
	{
		int median;
		size_t size = v.size();
		sort(v.begin(), v.end());
		if (size % 2) median = v[size / 2];
		else median = (v[size / 2 - 1] + v[size / 2]) / 2;
		return median;
	}

	void Sampler::genSampleRegion(Frame& frame)
	{
		float c[] = { 3.0f, 4.0f, 3.0f, 2.0f, 2.5f, 2.0f, 2.5f },
			r[] = { 6.0f, 2.0f, 1.5f, 2.0f, 2.5f, 1.5f, 1.8f };
		for (int i = 0; i < _sampleCount; ++i)
		{
			int oc = frame.width() / c[i], or = frame.height() / r[i];
			addSampleRegion(ROI(Point(oc, or), Point(oc + _sampleCellSize, or + _sampleCellSize)));
		}
	}

	void Sampler::addSampleRegion(ROI& roi)
	{
		_sampleRegions.push_back(roi);
	}

	void Sampler::doSample(Mat& m)
	{
		cv::cvtColor(m, _buffer, CV_BGR2HSV);
		Mat img = _buffer;
		for (size_t i = 0; i < _sampleRegions.size(); ++i) 
		{
			ColorSample3i colorSample;
			_sampleRegions[i].ref(_buffer);
			averageColor(_sampleRegions[i], colorSample);
			if (_sampleTimes == 0)
			{
				_colorSamples.push_back(colorSample);
			}
			else
			{
				_colorSamples[i] = ((_colorSamples[i] * _sampleTimes) + colorSample) / (_sampleTimes + 1);
			}
		}
		//cv::cvtColor(img, img, CV_HSV2BGR);
		_sampleTimes++;
	}

	void Sampler::averageColor(ROI& roi, ColorSample3i& _colorSample) {
		Mat& img = roi.data();

		_hueSamples.clear();
		_satSamples.clear();
		_valSamples.clear();

		for (int i = 2; i < img.rows - 2; i++) 
		{
			for (int j = 2; j < img.cols - 2; j++) 
			{
				_hueSamples.push_back(IMG_DATA(img, i, j, 0));
				_satSamples.push_back(IMG_DATA(img, i, j, 1));
				_valSamples.push_back(IMG_DATA(img, i, j, 2));
			}
		}

		_colorSample[HUE] = medoid(_hueSamples);
		_colorSample[SAT] = medoid(_satSamples);
		_colorSample[VAL] = medoid(_valSamples);
	}
}
