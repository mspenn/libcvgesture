#include <Windows.h>
#include <opencv2/opencv.hpp>
#include "resource.h"
#include "Util.h"

using namespace cv;

//Define a global variable to save handle of dll
HMODULE g_hModule = NULL;

// Dll Entry
BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		g_hModule = hModule;
		break;
	case DLL_PROCESS_DETACH:
		g_hModule = NULL;
		break;
	}
	   return TRUE;
}

namespace cvg
{
	IplImage* hBitmap2Ipl(HBITMAP hBmp)
	{
		BITMAP bmp;

		::GetObject(hBmp, sizeof(BITMAP), &bmp);
		int size = bmp.bmWidth * bmp.bmWidthBytes;
		unsigned char * pBits = (unsigned char *)malloc(size);
		int nBytes = GetBitmapBits(hBmp, size, pBits);

		int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
		int depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;

		IplImage* img = cvCreateImageHeader(cvSize(bmp.bmWidth, bmp.bmHeight)
			, depth, nChannels);

		img->imageData =
			(char*)malloc(bmp.bmHeight*bmp.bmWidth*nChannels*sizeof(char));
		memcpy(img->imageData, (char*)(pBits), bmp.bmHeight*bmp.bmWidth*nChannels);

		free(pBits);
		return img;
	}

	cv::Mat& ReadFromResource(cv::Mat& mat, unsigned short idb)
	{
		HBITMAP hBitmap = (HBITMAP)::LoadImage(g_hModule, MAKEINTRESOURCE(idb), IMAGE_BITMAP, 0, 0, 0);
		IplImage* iplLogo = hBitmap2Ipl(hBitmap);
		mat = cv::Mat(iplLogo, 0);
		return mat;
	}
}

