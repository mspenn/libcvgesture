#include "Util.h"

using namespace cv;
using namespace std;

#define FONT_FACE FONT_HERSHEY_COMPLEX

namespace cvg
{
	void ShowText(Mat& des, string text, Point position, Scalar color, int thickness)
	{
		putText(des, text, position, FONT_FACE, 0.6f, color, thickness);
	}

	void PrintMinMax(Mat& img)
	{
		double minv = 0.0, maxv = 0.0;
		double* minp = &minv;
		double* maxp = &maxv;
		minMaxIdx(img, minp, maxp);
		printf("MIN: %f, MAX: %f\n", minv, maxv);
	}

#define SQR(x) ((x)*(x))

	float Distance(Point& a, Point& b)
	{
		return sqrt(SQR(a.x - b.x) + SQR(a.y - b.y));
	}
}
