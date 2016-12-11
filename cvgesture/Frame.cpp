#include "Frame.h"

namespace cvg
{
	Frame::Frame()
	{
	}

	
	Frame::Frame(VideoCapture& cap)
	{
		initProperties();
	}

	Frame::Frame(const string& videoPath)
	{
		_cap.open(videoPath);
		initProperties();

	}

	Frame::Frame(int camera)
	{
		_cap.open(camera);
		initProperties();
	}

	Frame::~Frame()
	{
	}

	void Frame::initProperties()
	{
		// capture properties
		_width = _cap.get(CV_CAP_PROP_FRAME_WIDTH);
		_height = _cap.get(CV_CAP_PROP_FRAME_HEIGHT);

		// background substractor settings
		_bgsm.set("nmixtures", 3);
		_bgsm.set("detectShadows", false);
	}

	Mat& Frame::readRaw()
	{
		_cap.read(_mat);
		if (_mat.empty())return _mat;

		flip(_mat, _mat, 1);
		return _mat;
	}

	Mat& Frame::read()
	{
		_cap.read(_mat);
		if (_mat.empty())return _mat;

		flip(_mat, _mat, 1);
		pyrDown(_mat, _matCopy);
		fore();
		blur(_matCopy, _matCopy, Size(3, 3));
		return _mat;
	}

	Mat& Frame::back()
	{
		_bgsm.getBackgroundImage(_back);
		return _back;
	}

	Mat& Frame::fore()
	{
		_bgsm.operator()(_matCopy, _fore);
		return _fore;
	}

	Mat& Frame::maskFore(Mat& mask)
	{
		bitwise_and(mask, _fore, mask);
		return mask;
	}
}
