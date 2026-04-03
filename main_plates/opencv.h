#pragma once
#pragma comment(lib , "winmm.lib")
#include <Windows.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/types_c.h"
#include "opencv2/imgproc/imgproc_c.h"
#include <thread>
#include <iostream>
using namespace cv;

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
	void check(int *i);

	VideoCapture cap;

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
};

