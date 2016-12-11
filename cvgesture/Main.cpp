#include "Sampler.h"
#include "Frame.h"
#include "SkinColorModel.h"
#include "Util.h"
#include "TimeCounter.h"
#include "HandDetector.h"
#include "GestureRecognizer.h"
#include "resource.h"
#include "Constant.h"
#include "cvgesture.h"

using namespace cv;
using namespace cvg;

#define WIN_MAIN		"Main"
#define WIN_THRESHOLD	"Threshold"
#define WIN_FOREGROUND	"foreground"
#define WIN_BACKGROUND	"background"

#define HINT_TEXT_COLOR Scalar(0, 0, 255)
#define WAIT_TIME 30

Mat MAT_LOGO_24U, MAT_LOGO_8U, MAT_HAND;

inline void initGlobal(Frame& frame)
{
	vector<Mat> channels;
	Mat mat_logo;
	ReadFromResource(mat_logo, IDB_LOGO);
	split(mat_logo, channels);
	MAT_LOGO_8U = channels[0];
	channels.pop_back();
	channels[1].setTo(0);
	channels[2].setTo(0);
	merge(channels, MAT_LOGO_24U);

	channels.clear();
	ReadFromResource(MAT_HAND, IDB_HAND);
	split(MAT_HAND, channels);
	channels.pop_back();
	channels[0].setTo(0);
	channels[2].setTo(0);
	merge(channels, MAT_HAND);
	float scale = (float)(frame.width()) / 300;
	Size dsize = Size(MAT_HAND.cols*scale, MAT_HAND.rows*scale);
	Mat image2 = Mat(dsize, CV_32S);
	resize(MAT_HAND, MAT_HAND, dsize);
}

inline void showHandSampleRegion(Frame& frame, Mat& img)
{
	int x = MIN(img.rows / 5.2, frame.width() - MAT_HAND.rows);
	int y = MIN(img.cols / 20, frame.height() - MAT_HAND.rows);

	Mat imageROI = img(Rect(x, y, MAT_HAND.cols, MAT_HAND.rows));
	addWeighted(imageROI, 1.0, MAT_HAND, 0.6, 0, imageROI);
}

inline void showAboutInfo(Mat& img)
{
	Scalar text_color(255, 0, 0);
	int x = img.cols / 8;
	int y = img.rows / 1.1; 

	Mat imageROI = img(Rect(x - 50, y - 20, MAT_LOGO_8U.cols, MAT_LOGO_8U.rows));
	if (img.channels() == 1)addWeighted(imageROI, 1.0, MAT_LOGO_8U, 0.8, 0, imageROI);
	else if (img.channels() == 3)addWeighted(imageROI, 1.0, MAT_LOGO_24U, 0.8, 0, imageROI);
	
#ifdef SHOW_ABOUT_DETAIL
	int next_line = 20;
	ShowText(img, "NJU SE, MF1632006, Zak Penn", Point(x, y), text_color);
	y += next_line;
	ShowText(img, "Copyleft (c) 2016 MicroSmadio Workstation.", Point(x, y), text_color);
#else
	ShowText(img, "NJU SE, MF1632006, Zak Penn, Copyleft (c) 2016.", Point(x, y), text_color);
#endif
}

inline void imgShow(const string& winname, Mat& mat)
{
	showAboutInfo(mat);
	imshow(winname, mat);
}

#define IM_SHOW(NAME, MAT)do{\
	showAboutInfo(MAT); \
	imshow(NAME, MAT); \
}while(0)

void doSample(Frame& frame, Sampler& sampler)
{


	namedWindow(WIN_MAIN, WINDOW_AUTOSIZE);

	TimeCounter tc;

	// read one frame and gen sample regions
	sampler.genSampleRegion(frame);
	vector<ROI> rois = sampler.sampleRegions();
#define HINT_TEXT_POS(mat) Point(mat.cols / 6, mat.rows / 10)
	
	// ready for sample
	for (int i = 0; i < SAMPLE_FRAMES; ++i) 
	{
		Mat& mat = frame.readRaw();

		// draw sample regions
		for (auto& pRoi = rois.begin(); pRoi != rois.end(); pRoi++)
		{
			pRoi->displayRegion(mat, SAMPLE_REGION_COLOR, SAMPLE_REGION_THICKNESS);
		}

		// show help text
		string text = "Please cover rectangles with your palm";
		ShowText(mat, text, HINT_TEXT_POS(mat), HINT_TEXT_COLOR);
		showHandSampleRegion(frame, mat);
		// draw frame
		IM_SHOW(WIN_MAIN, mat);

		// wait
		if (waitKey(WAIT_TIME) >= 0) break;
	}
	// record sample values
	for (int i = 0; i < SAMPLE_FRAMES; ++i) 
	{
		Mat& mat = frame.readRaw();
		sampler.doSample(mat);

		for (auto& pRoi = rois.begin(); pRoi != rois.end(); pRoi++)
		{
			pRoi->displayRegion(mat, SAMPLE_REGION_COLOR, SAMPLE_REGION_THICKNESS);
		}

		// show help text
		string text = "Sampling color of hand...";
		ShowText(mat, text, HINT_TEXT_POS(mat), HINT_TEXT_COLOR);
		showHandSampleRegion(frame, mat);
		// draw frame
		IM_SHOW(WIN_MAIN, mat);

		// wait
		if (waitKey(WAIT_TIME) >= 0) break;
	}

	double timeElapsed = tc.GetTimeElapsed();
	printf("SAMPLE TIME COST: %f\n", timeElapsed);
}

//#define RECORD_VIDEO 
#define READ_FROM_VIDEO

void startup(Frame& frame, bool showMask = false)
{
	initGlobal(frame);
	Sampler sampler;
	doSample(frame, sampler);
	SkinColorModel skinColor;
	HandDetector handDetector;
	GestureRecognizer gestureRecognizer(frame.size());
#ifdef RECORD_VIDEO 
	VideoWriter writer("1.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30.0, Size(frame.width(), frame.height()));
#endif
	bool stop = false;
	while (!stop)
	{
		frame.read();
		if (frame.isEnd())
			break;

		Mat& mask = skinColor.extract(frame.matSmall(), sampler);
		frame.maskFore(mask);

		bool hasHand = handDetector.determineContours(mask);
		if (hasHand)
		{
			handDetector.drawFingerTips(frame.matData());
			handDetector.displayContours(frame.matData());
		}

		gestureRecognizer.step(GestureTranstionData(hasHand, handDetector));

		//imshow(WIN_FOREGROUND, frame.fore());
		//imshow(WIN_BACKGROUND, frame.back());
		if (showMask)
		{
			IM_SHOW(WIN_THRESHOLD, mask);
		}
		IM_SHOW(WIN_MAIN, frame.matData());
#ifdef RECORD_VIDEO 
		Mat mat;
		flip(frame.matData(), mat, 1);
		writer.write(mat);
#endif
		if (cvWaitKey(WAIT_TIME) == 'q')
			break;
	}
#ifdef RECORD_VIDEO 
	writer.release();
#endif
}

void startup_with_camera(int camera, bool showMask)
{
	Frame frame(camera);
	startup(frame, showMask);
}

void startup_with_file(const char* path, bool showMask)
{
	const string sPath = string(path);
	Frame frame(sPath);
	startup(frame, showMask);
}

void record_video(const char* path)
{
	Frame frame(0);
	printf("W/H: %d/%d\n", frame.width(), frame.height());
	// record video via VideoWriter, 25.0 fps
	VideoWriter writer(string(path), CV_FOURCC('M', 'J', 'P', 'G'), 25.0, Size(frame.width(), frame.height()));
	bool stop = false;
	while (!stop)
	{
		frame.readRaw();
		imshow(WIN_MAIN, frame.matData());
		Mat mat;
		flip(frame.matData(), mat, 1);
		writer.write(mat);
		if (cvWaitKey(30) == 'q')
			break;
	}
	writer.release();
}