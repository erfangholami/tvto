#pragma once
#pragma comment(lib , "winmm.lib")
#pragma comment(lib, "DBRx64.lib")
#include <Windows.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/types_c.h"
#include "opencv2/imgproc/imgproc_c.h"
#include "If_DBRP.h"
#include "opencv2\opencv.hpp"
#include <thread>
#include <iostream>

#include <opencv2\core\core.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2\nonfree\features2d.hpp>
#include <opencv2\objdetect\objdetect.hpp>
#include <opencv2\calib3d\calib3d.hpp>
#include "opencv2\opencv_modules.hpp"
using namespace cv;
using namespace std;


struct barcode_format
{
	const char *pszFormat;
	__int64 llFormat;
};
static struct barcode_format Barcode_Formats[] =
{
	{ "CODE_39", CODE_39 },
	{ "CODE_128", CODE_128 },
	{ "CODE_93", CODE_93 },
	{ "CODABAR", CODABAR },
	{ "ITF", ITF },
	{ "UPC_A", UPC_A },
	{ "UPC_E", UPC_E },
	{ "EAN_13", EAN_13 },
	{ "EAN_8", EAN_8 },
	{ "INDUSTRIAL_25",INDUSTRIAL_25 },
	{ "OneD", OneD },
	{ "QR_CODE", QR_CODE },
	{ "PDF417",PDF417 },
	{ "DATAMATRIX", DATAMATRIX }
};
class opencv
{
public:
	opencv();
	~opencv();
	void gwez();
	bool red();
	bool yellow();
	bool orange();
	bool green();
	bool blue();
	bool cyan();
	bool purple();
	bool lightGreen();
	bool brown();
	bool darkPurple();
	bool black();
	void playRed();
	void playYellow();
	void playOrange();
	void playGreen();
	void playBlue();
	void playCyan();
	void playLightGreen();
	void playBrown();
	void playPurple();
	void play(int color);
	Point2f detectColoring(Mat *);
	void check(int *i, int cam = 0);
	char* detectCode();
	const char *GetFormatStr(__int64);
	char* readCode(Mat  &image, CBarcodeReader  &reader);
	VideoCapture cap;
	char *pszTemp1;
	char *valu;
	int color;
	Mat frame, hsv, threshold_img;
	Mat low, high;
	bool off = true;
	bool end = false;
	bool gwezz;
	bool test;
	Point y;
	int maxArea = 0;
	int numCont = 0;

	int iLowHLow;
	int iHighHLow;
	int iLowHHigh;
	int iHighHHigh;

	int iLowS;
	int iHighS;

	int iLowV;
	int iHighV;
	Mat threshlod;
	struct barcode_format
	{
		const char *pszFormat;
		__int64 llFormat;
	};

};

