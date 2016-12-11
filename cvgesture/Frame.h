#ifndef __FRAME_H__
#define __FRAME_H__

#include <opencv2\opencv.hpp>
#include "Common.h"

using namespace cv;

namespace cvg
{
	class CVG_EXTERN Frame
	{
	public:
		Frame();
		Frame(VideoCapture& cap);
		Frame(const string& videoPath);
		Frame(int camera);

		~Frame();

		/**
		* read image to _mat, 
		* without any other process
		*/
		Mat& readRaw();
		
		/**
		* read image to _mat,
		* with a small copy to _matCopy
		*/
		Mat& read();

		Mat& matData(){ return _mat; }
		Mat& matSmall(){ return _matCopy; }

		int width()const { return _width; }
		int height()const { return _height; }

		/**
		* get background of current frame
		*/
		inline Mat& back();

		/**
		* get foreground of current frame
		*/
		inline Mat& fore();

		/**
		* do mask with foreground matrix
		*/
		inline Mat& maskFore(Mat& mask);

		bool isEnd()const { return _mat.empty(); }
		VideoCapture capture() const{ return _cap; }

		Size size()const { return Size(_width, _height); }
	private:
		void initProperties();

		Mat _mat;						// orignal image read from video capture
		Mat _matCopy;					// cache, a smaller image, to speed up process operations

		int _width;
		int _height;

		Mat _back;						// background mat
		Mat _fore;						// foreground mat
		BackgroundSubtractorMOG2 _bgsm;	// background subtractor, it can seperate foreground and background

		VideoCapture _cap;
	};
}

#endif	// __FRAME_H__