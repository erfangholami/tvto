#include "opencv.h"
#include <Windows.h>							
opencv::opencv() {
	test = false;
}
void opencv::gwez() {
	test = true;
	bool manual;
manuall:
	std::cout << "manual ?  ";
	std::cin >> manual;
	while (true) {
		char start;
		std::cout << "what do u want?!..." << std::endl;
		if (manual)
			std::cout << "(H, S, V)..." << std::endl;
		else
			std::cout << "(r, g, b, o, y, c, l, w, p, d, B)..." << std::endl;
		std::cin >> start;
		if (start == 'e')
			return;
		if (!manual)
			if (start == 'r')
				gwezz = red();
			else if (start == 'y')
				gwezz = yellow();
			else if (start == 'g')
				gwezz = green();
			else if (start == 'b')
				gwezz = blue();
			else if (start == 'o')
				gwezz = orange();
			else if (start == 'c')
				gwezz = cyan();
			else if (start == 'l')
				gwezz = lightGreen();
			else if (start == 'w')
				gwezz = brown();
			else if (start == 'p')
				gwezz = purple();
			else if (start == 'd')
				gwezz = darkPurple();
			else if (start == 'B')
				gwezz = black();
			else
				goto manuall;

		else
			if (start == 'h')
				std::cin >> iLowHLow >> iHighHLow;
			else if (start == 's')
				std::cin >> iLowS >> iHighS;
			else if (start == 'v')
				std::cin >> iLowV >> iHighV;
			else
				goto manuall;

		int j = -1;
		check(&j);
	}
}
bool opencv::red() {
	color = 0;
	iLowHHigh = 0;
	iHighHHigh = 2;
	iLowHLow = 150;
	iHighHLow = 180;
	iLowS = 130;
	iHighS = 255;
	iLowV = 70;
	iHighV = 160;
	return true;

}
bool opencv::yellow() {
	color = 1;
	iLowHLow = 15;
	iHighHLow = 40;
	iLowS = 100;
	iHighS = 255;
	iLowV = 25;
	iHighV = 255;
	return false;
}
bool opencv::orange() {
	color = 4;
	iLowHLow = 5;
	iHighHLow = 15;
	iLowS = 100;
	iHighS = 255;
	iLowV = 100;
	iHighV = 255;
	return false;
}
bool opencv::green() {
	color = 2;
	iLowHLow = 70;
	iHighHLow = 100;
	iLowS = 50;
	iHighS = 255;
	iLowV = 25;
	iHighV = 100;
	return false;
}
bool opencv::lightGreen() {
	color = 7;
	iLowHLow = 35;
	iHighHLow = 75;
	iLowS = 50;
	iHighS = 255;
	iLowV = 40;
	iHighV = 255;
	return false;
}
bool opencv::blue() {
	color = 3;
	iLowHLow = 100;
	iHighHLow = 130;
	iLowS = 150;
	iHighS = 255;
	iLowV = 45;
	iHighV = 255;
	return false;
}
bool opencv::brown() {
	color = 8;
	color = 0;
	iLowHHigh = 175;
	iHighHHigh = 180;
	iLowHLow = 0;
	iHighHLow = 180;
	iLowS = 0;
	iHighS = 70;
	iLowV = 50;
	iHighV = 85;
	return true;
}
bool opencv::cyan() {
	color = 5;
	iLowHLow = 75;
	iHighHLow = 95;
	iLowS = 50;
	iHighS = 255;
	iLowV = 100;
	iHighV = 255;                            
	return false;
}
bool opencv::purple() {
	color = 6;
	iLowHLow = 130;
	iHighHLow = 180;
	
	///night H :D
	//iLowHLow = 150;
	//iHighHLow = 170;
	iLowS = 50;
	iHighS = 255;
	iLowV = 0;
	iHighV = 100;
	return false;
}
bool opencv::darkPurple()
{
	color = 9;
	iLowHLow = 100;
	iHighHLow = 130;
	iLowS = 0;
	iHighS = 255;
	iLowV = 25;
	iHighV = 50;
	return false;
}
bool opencv::black()
{
	color = 10;
	iLowHLow = 0;
	iHighHLow = 180;
	iLowS = 0;
	iHighS = 255;
	iLowV = 0;
	iHighV = 28;
	return false;
}
void opencv::playRed() {
	mciSendString("close mp3", NULL, 0, NULL);
	mciSendString("open \"red.wav\" type mpegvideo alias mp3", NULL, 0, NULL);
	mciSendString("play mp3", NULL, 0, NULL);
}
void opencv::playYellow() {
	mciSendString("close mp3", NULL, 0, NULL);
	mciSendString("open \"yellow.wav\" type mpegvideo alias mp3", NULL, 0, NULL);
	mciSendString("play mp3", NULL, 0, NULL);
}
void opencv::playOrange() {
	mciSendString("close mp3", NULL, 0, NULL);
	mciSendString("open \"orange.wav\" type mpegvideo alias mp3", NULL, 0, NULL);
	mciSendString("play mp3", NULL, 0, NULL);
}
void opencv::playGreen() {
	mciSendString("close mp3", NULL, 0, NULL);
	mciSendString("open \"green.wav\" type mpegvideo alias mp3", NULL, 0, NULL);
	mciSendString("play mp3", NULL, 0, NULL);
}
void opencv::playBlue() {
	mciSendString("close mp3", NULL, 0, NULL);
	mciSendString("open \"blue.wav\" type mpegvideo alias mp3", NULL, 0, NULL);
	mciSendString("play mp3", NULL, 0, NULL);
}
void opencv::playCyan() {
	mciSendString("close mp3", NULL, 0, NULL);
	mciSendString("open \"cyan.wav\" type mpegvideo alias mp3", NULL, 0, NULL);
	mciSendString("play mp3", NULL, 0, NULL);
}
void opencv::playLightGreen() {
	mciSendString("close mp3", NULL, 0, NULL);
	mciSendString("open \"light green.wav\" type mpegvideo alias mp3", NULL, 0, NULL);
	mciSendString("play mp3", NULL, 0, NULL);
}
void opencv::playBrown() {
	mciSendString("close mp3", NULL, 0, NULL);
	mciSendString("open \"brown.wav\" type mpegvideo alias mp3", NULL, 0, NULL);
	mciSendString("play mp3", NULL, 0, NULL);
}
void opencv::playPurple() {
	mciSendString("close mp3", NULL, 0, NULL);
	mciSendString("open \"purple.wav\" type mpegvideo alias mp3", NULL, 0, NULL);
	mciSendString("play mp3", NULL, 0, NULL);
}
void opencv::play(int color) {
	if (color == 0)
		playRed();
	else if (color == 1)
		playYellow();
	else if (color == 2)
		playGreen();
	else if (color == 3)
		playBlue();
	else if (color == 4)
		playOrange();
	else if (color == 5)
		playCyan();
	else if (color == 6)
		playPurple();
	else if (color == 7)
		playLightGreen();
	else if (color == 8)
		playBrown();
}

Point2f opencv::detectColoring(Mat *thresholdd) {
	
	IplImage *img = new IplImage(*thresholdd);
	Scalar color = CV_RGB(255, 255, 255);
	CvMemStorage *storage = cvCreateMemStorage(0);
	CvSeq *contours = 0;
	int contAthresh = 1600;

	numCont = cvFindContours(img, storage, &contours, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));
	double max = 0;
	cvSet(img, cvScalar(0, 0, 0));
	int size = 0, maxSize = 0;
	for (CvSeq * test = contours; test != 0; test = test->h_next)
	{

		double area = (cvContourArea(test, CV_WHOLE_SEQ));
		if (area > contAthresh) {
			size++;
			cvDrawContours(img, test, color, color, -1, CV_FILLED, 8);
			if (area >= max)
				max = area;
		}

	}
	numCont = size;
	int getMax = 0;
	for (CvSeq * test = contours; test != 0; test = test->h_next)
	{
		getMax++;
		double area = (cvContourArea(test, CV_WHOLE_SEQ));
		if (area > contAthresh) {
			if (area >= max - 30) {
				CvMoments* moments = new CvMoments();
				cvMoments( test, moments);
				maxArea = max;
				///  Get the mass centers:
				return Point2f((float)(moments->m10 / moments->m00), (float)(moments->m01 / moments->m00));
			}
		}
	}
}
void opencv::check(int *i) {
	end = false;
	//std::thread intialized(gwez);
	// open the default camera, use something different from 0 otherwise;
	// Check VideoCapture documentation.
	if (!cap.open(1)) {
		std::cout << "error on open camera..." << std::endl;
		return;
	}
	int counter = 0;
	while (counter < 1)
	{
		maxArea = 0;
		if (*i == -1)
			gwezz = false;
		else if (*i == 0)
			gwezz = red();
		else if (*i == 1)
			gwezz = yellow();
		else if (*i == 2)
			gwezz = green();
		else if (*i == 3)
			gwezz = blue();
		else if (*i == 4)
			gwezz = orange();
		else if (*i == 5)
			gwezz = cyan();
		else if (*i == 6)
			gwezz = purple();
		else if (*i == 7)
			gwezz = lightGreen();
		else if (*i == 8)
			gwezz = brown();
		else if (*i == 9)
			gwezz = darkPurple();
		else if (*i == 10)
			gwezz = black();

		Mat threshhold_img;
		cap >> frame;
		cvtColor(frame, hsv, COLOR_BGR2HSV);
		inRange(hsv, Scalar(iLowHLow, iLowS, iLowV), Scalar(iHighHLow, iHighS, iHighV), low);
	dowrBaro:
		if (gwezz) {
			inRange(hsv, Scalar(iLowHHigh, iLowS, iLowV), Scalar(iHighHHigh, iHighS, iHighV), high);
			threshhold_img = low | high;
		}
		else threshhold_img = low;

		//Reduce noise
		blur(threshhold_img, threshhold_img, Size(3, 3));

		erode(threshhold_img, threshhold_img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(threshhold_img, threshhold_img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//morphological closing (fill small holes in the foreground)
		dilate(threshhold_img, threshhold_img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(threshhold_img, threshhold_img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		/*CvMat *gwezz = new CvMat(threshhold_img);
		Rect xx(10, 10, 120, 120);
		Mat crp(gwezz);
		Mat xxx = crp(xx);
		crp.copyTo(xxx);
		threshhold_img = xxx;
		*/

		Point2f x = detectColoring(&threshhold_img);
		y.x = x.x;
		y.y = x.y;

		/*if (color == 0) {
			std::cout << "red center : " << y.x << " " << y.y << " ,  " << "count : " << numCont << "  , surface : " << maxArea << std::endl;
		}
		else if (color == 1) {
			std::cout << "yellow center : " << y.x << " " << y.y << "  , " << "count : " << numCont << std::endl;
		}
		else if (color == 2) {
			std::cout << "green center : " << y.x << " " << y.y << " ,  " << "count : " << numCont << std::endl;;
		}
		else if (color == 3) {
			std::cout << "blue center : " << y.x << " " << y.y << "  , " << "count : " << numCont << std::endl;;
		}
		else if (color == 4) {
			std::cout << "orange center : " << y.x << " " << y.y << "  , " << "count : " << numCont << std::endl;;
		}*/

		if (test) {
			//std::cout << "H : " << iLowHLow << " " << iHighHLow << "    S : " << iLowS << " " << iHighS << "    V : " << iLowV << " " << iHighV << std::endl;
			if (threshhold_img.empty()) break;
			imshow("this is your threshhold_img", threshhold_img);
			if (frame.empty()) break; // end of video stream
			imshow("this is you, smile! :)", frame);
		}
		
		if (waitKey(1) == 27) break;
		if (!test)
			counter++;
	}
	//cap.close();
	end = true;
	destroyAllWindows();
	return;
}

opencv::~opencv()
{
}
