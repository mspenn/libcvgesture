#ifndef __ROI_H__
#define __ROI_H__

#include <opencv2\opencv.hpp>
#include "Common.h"

using namespace cv;

namespace cvg
{
	/**
	* Region of interest
	*/
	class CVG_EXTERN ROI
	{
	public:

		ROI(Point lower, Point upper);

		inline Rect region() const
		{
			return _region;
		}

		inline void displayRegion(Mat& img, Scalar color, int thickness)
		{
			rectangle(img, _bot_corner, _top_corner, color, thickness);
		}

		/**
		* get reference matrix
		*/
		inline Mat& ref()
		{
			return _ref;
		}
		
		/**
		* get roi data in reference matrix
		*/
		inline Mat& data()
		{
			return _data;
		}
		
		/**
		* set roi data in reference matrix
		*/
		inline void ref(Mat& img)
		{
			_ref = img(_region); 
			_ref.copyTo(_data);
		}

		inline int width() const
		{
			return _width;
		}

		inline int height() const
		{
			return _height;
		}

		inline Point bottonCorner() const
		{
			return _bot_corner;
		}

		inline Point topCorner() const
		{
			return _top_corner;
		}

	private:
		int _height;
		int _width;

		Point _bot_corner;
		Point _top_corner;

		Rect _region;	// region area

		Mat _ref;		// orignal matrix
		Mat _data;		// roi data in reference matrix
	};
}
#endif	// __ROI_H__