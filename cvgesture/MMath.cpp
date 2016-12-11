#include "MMath.h"

double LinearRegression(const vector<Point>& points, double& k, double& b)
{
	int i = 0;
	int j = 0;
	double A, B, C, D;
	double tmp = 0;
	int num = points.size();

	CvMat *mat1 = cvCreateMat(1, num, CV_64FC1);
	CvMat *mat2 = cvCreateMat(1, num, CV_64FC1);
	CvMat *mattmp = cvCreateMat(1, num, CV_64FC1);

	for (j = 0; j<mat1->cols; j++)
	{
		CV_MAT_ELEM(*mat1, double, 0, j) = points[j].x;
		CV_MAT_ELEM(*mat2, double, 0, j) = points[j].y;

	}

	cvMul(mat1, mat1, mattmp, 1);
	A = cvSum(mattmp).val[0];

	B = cvSum(mat1).val[0];

	cvMul(mat1, mat2, mattmp, 1);
	C = cvSum(mattmp).val[0];

	D = cvSum(mat2).val[0];

	tmp = A*mat1->cols - B*B;

	k = (C*mat1->cols - B*D) / tmp;
	b = (A*D - C*B) / tmp;

	cvReleaseMat(&mat1);
	cvReleaseMat(&mat2);

	return k;
}