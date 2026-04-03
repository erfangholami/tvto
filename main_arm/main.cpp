#define _CRT_NONSTDC_NO_WARNINGS 
#pragma comment(lib , "winmm.lib")
#include <Windows.h>
#include <iostream>  
#include "motors.h"
#include "dynamixel.h"
#include "opencv.h"
#include "arduino.h"
#include "Place.h"
#include <thread>
#include "ReadWrite.h"
#include <math.h>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2\nonfree\features2d.hpp>
#include <opencv2\objdetect\objdetect.hpp>
#include <opencv2\calib3d\calib3d.hpp>
#include "opencv2\opencv_modules.hpp"
#include "opencv2\opencv.hpp"
#include <iostream>
#include <Windows.h>
#include <Windows.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/types_c.h"
#include "opencv2/imgproc/imgproc_c.h"
#include "If_DBRP.h"
#include "opencv2\opencv.hpp"
#include <thread>
#include <iostream>
#include "opencv2\aruco.hpp"

using namespace cv;
using namespace std;


Mat sceneP = imread("C:\\Users\\Erfan\\Pictures\\Screenshots\\555.png");
Mat outImg;
#define mx64 2152  
#define mx106 2400
#define mx28_1 2963
#define mx28_2 1022


#define PI 3.14159265
#define BASE 1980
#define firJoint 2048 
#define secJoint 2048
#define thrjoint 2048
#define rotater 2048
#define firGreep 1975
#define secGreep 1830
#define Greep1 300
#define Greep2 779

#define FGreep1 426
#define FGreep2 610
#define Rotator 1950
#define BalaBar 3500

motors *motor = new motors(10, 3);
opencv *open = new opencv();
dynamixel *dynam = new dynamixel();
arduino *ardu = new arduino(4, 38400);
place *places[5];
int counter = 0;
int lastPosY = 180;
int lastPosX = 2500;
int speed[8] = { 50 , 50 , 50 , 50 , 50 , 50 , 50 , 50 };
int acceleration[8] = { 5 , 5 , 5 , 5 , 5 , 5 , 5 , 5 };
int ID = 5;
bool dynamixell = false;
bool motorSet = true;
bool threadd = true;
bool gatee = false;
bool notFound = true;
bool moveToObjective = false;
bool stopCentering = false;
bool errorCentering = false;
char *mmmmmm = "X";
vector<int> realColor;
int color[4] = { 0 , 1 , 7 , 3 };
bool colorFound[4] = { 0 };
void center(int j);
void setSA()
{
	for (int i = 1; i < 8; i++)
	{
		dynam->dxl_write_word(i, 73, 0);
		dynam->dxl_write_word(i, 32, 10);
	}
}
void testArmKey()
{
	while (true)
	{
		if (GetAsyncKeyState(0x1B) != 0)
			break;
		else if (GetAsyncKeyState(0x31) != 0)
		{
			int pos = dynam->dxl_read_word(1, 36);
			if (pos > 4080)
				cout << "Motor 1 in Max mode " << endl;
			else
			{
				dynam->dxl_write_word(1, 30, pos + 100);
				cout << "base : " << dynam->dxl_read_word(1, 36) << endl;
			}
		}
		else if (GetAsyncKeyState(0x61) != 0)
		{
			int pos = dynam->dxl_read_word(1, 36);
			if (pos < 16)
				cout << "Motor 1 in Min mode " << endl;
			else
			{
				dynam->dxl_write_word(1, 30, pos - 100);
				cout << "base : " << dynam->dxl_read_word(1, 36) << endl;
			}
		}

		else if (GetAsyncKeyState(0x32) != 0)
		{
			int pos = dynam->dxl_read_word(2, 36);
			if (pos > 4080)
				cout << "Motor 2 in Max mode " << endl;
			else
			{
				dynam->dxl_write_word(2, 30, pos + 100);
				cout << "firstJoint 1111 : " << dynam->dxl_read_word(2, 36) << endl;
			}
		}
		else if (GetAsyncKeyState(0x62) != 0)
		{
			int pos = dynam->dxl_read_word(2, 36);
			if (pos < 16)
				cout << "Motor 2 in Min mode " << endl;
			else
			{
				dynam->dxl_write_word(2, 30, pos - 100);
				cout << "firstJoint : " << dynam->dxl_read_word(2, 36) << endl;
			}
		}

		else if (GetAsyncKeyState(0x33) != 0)
		{
			int pos = dynam->dxl_read_word(3, 36);
			if (pos > 4080)
				cout << "Motor 3 in Max mode " << endl;
			else
			{
				dynam->dxl_write_word(3, 30, pos + 40);
				cout << "secondJoint : " << dynam->dxl_read_word(3, 36) << endl;
			}
		}
		else if (GetAsyncKeyState(0x63) != 0)
		{
			int pos = dynam->dxl_read_word(3, 36);
			if (pos < 16)
				cout << "Motor 3 in Min mode " << endl;
			else
			{
				dynam->dxl_write_word(3, 30, pos - 40);
				cout << "secondJoint : " << dynam->dxl_read_word(3, 36) << endl;
			}
		}

		else if (GetAsyncKeyState(0x34) != 0)
		{
			int pos = dynam->dxl_read_word(4, 36);
			if (pos > 4080)
				cout << "Motor 4 in Max mode " << endl;
			else
			{
				dynam->dxl_write_word(4, 30, pos + 40);
				cout << "thirdJoint : " << dynam->dxl_read_word(4, 36) << endl;
			}
		}
		else if (GetAsyncKeyState(0x64) != 0)
		{
			int pos = dynam->dxl_read_word(4, 36);
			if (pos < 16)
				cout << "Motor 4 in Min mode " << endl;
			else
			{
				dynam->dxl_write_word(4, 30, pos - 40);
				cout << "thidJoint : " << dynam->dxl_read_word(4, 36) << endl;
			}
		}

		else if (GetAsyncKeyState(0x35) != 0)
		{
			int pos = dynam->dxl_read_word(5, 36);
			if (pos > 4080)
				cout << "Motor 5 in Max mode " << endl;
			else
			{
				dynam->dxl_write_word(5, 30, pos + 40);
				cout << "router : " << dynam->dxl_read_word(5, 36) << endl;
			}
		}
		else if (GetAsyncKeyState(0x65) != 0)
		{
			int pos = dynam->dxl_read_word(5, 36);
			if (pos < 16)
				cout << "Motor 5 in Min mode " << endl;
			else
			{
				dynam->dxl_write_word(5, 30, pos - 40);
				cout << "router : " << dynam->dxl_read_word(5, 36) << endl;
			}
		}

		else if (GetAsyncKeyState(0x36) != 0)
		{
			int pos = dynam->dxl_read_word(6, 36);
			if (pos > 4080)
				cout << "Motor 6 in Max mode " << endl;
			else
			{
				dynam->dxl_write_word(6, 30, pos + 30);
				cout << "firstGreeper : " << dynam->dxl_read_word(6, 36) << endl;
			}
		}
		else if (GetAsyncKeyState(0x66) != 0)
		{
			int pos = dynam->dxl_read_word(6, 36);
			if (pos < 16)
				cout << "Motor 6 in Min mode " << endl;
			else
			{
				dynam->dxl_write_word(6, 30, pos - 30);
				cout << "firstGreeper : " << dynam->dxl_read_word(6, 36) << endl;
			}
		}

		else if (GetAsyncKeyState(0x37) != 0)
		{
			int pos = dynam->dxl_read_word(7, 36);
			if (pos > 4080)
				cout << "Motor 7 in Max mode " << endl;
			else
			{
				dynam->dxl_write_word(7, 30, pos + 30);
				cout << "secondGreeper : " << dynam->dxl_read_word(7, 36) << endl;
			}
		}
		else if (GetAsyncKeyState(0x67) != 0)
		{
			int pos = dynam->dxl_read_word(7, 36);
			if (pos < 16)
				cout << "Motor 7 in Min mode " << endl;
			else
			{
				dynam->dxl_write_word(7, 30, pos - 30);
				cout << "secondGreeper : " << dynam->dxl_read_word(7, 36) << endl;
			}
		}
		else if (GetAsyncKeyState(0x26) != 0)
		{
			dynam->syncWrite(6, 7, 30, firGreep - 452, secGreep + 452);
		}
		else if (GetAsyncKeyState(0x28) != 0)
		{
			dynam->syncWrite(6, 7, 30, firGreep, secGreep);
		}
		else;
	}
}
void setPos(int i)
{
	if (i == 1)
	{
		dynam->dxl_write_word(1, 30, 2000);
		dynam->dxl_write_word(2, 30, 1953);
		dynam->dxl_write_word(3, 30, 2393);
		dynam->dxl_write_word(4, 30, 1300);
	}
	else if (i == 2)
	{
		dynam->dxl_write_word(1, 30, 2984);
		dynam->dxl_write_word(2, 30, 1953);
		dynam->dxl_write_word(3, 30, 3259);
		dynam->dxl_write_word(4, 30, 1300);
	}
	else if (i == 3)
	{
		dynam->dxl_write_word(1, 30, 2000);
		dynam->dxl_write_word(2, 30, 1752);
		dynam->dxl_write_word(3, 30, 2891);
		dynam->dxl_write_word(4, 30, 1263);
	}
	else if (i == 4)
	{
		dynam->dxl_write_word(1, 30, 2000);
		dynam->dxl_write_word(2, 30, 1329);
		dynam->dxl_write_word(3, 30, 3263);
		dynam->dxl_write_word(4, 30, 837);
	}
	else if (i == 5)
	{
		dynam->dxl_write_word(1, 30, 2000);
		dynam->dxl_write_word(2, 30, 2476);
		dynam->dxl_write_word(3, 30, 1830);
		dynam->dxl_write_word(4, 30, 2245);
	}
	else if (i == 6)
	{

		dynam->syncWrite(6, 7, 73, 15, 15);
		dynam->syncWrite(6, 7, 32, 100, 100);
		do
		{
			dynam->dxl_write_word(5, 30, 2282);
			dynam->syncWrite(6, 7, 30, 2048, 2048);
		} while (abs(dynam->dxl_read_word(6, 36) - 2048) > 50 & (abs(dynam->dxl_read_word(7, 36) - 2048) > 50) & (abs(dynam->dxl_read_word(5, 36) - 2282) > 50));

		dynam->dxl_write_word(2, 32, 10);
		dynam->dxl_write_word(2, 73, 0);
		do
		{
			dynam->dxl_write_word(2, 30, 2048);
		} while (abs(dynam->dxl_read_word(2, 36) - 2048) > 50);

		dynam->dxl_write_word(1, 32, 100);
		dynam->dxl_write_word(1, 73, 5);
		do
		{
			dynam->dxl_write_word(1, 30, 2982);
		} while (abs(dynam->dxl_read_word(1, 36) - 2982) > 50);

		dynam->dxl_write_word(2, 32, 10);
		dynam->dxl_write_word(2, 73, 0);
		dynam->dxl_write_word(3, 32, 8);
		dynam->dxl_write_word(3, 73, 5);
		dynam->dxl_write_word(4, 32, 8);
		dynam->dxl_write_word(4, 73, 5);
		do
		{
			dynam->dxl_write_word(2, 30, 2784);
			dynam->dxl_write_word(3, 30, 3730);
			dynam->dxl_write_word(4, 30, 720);
		} while (abs(dynam->dxl_read_word(2, 36) - 2784) > 50);



	}
	else;
}
void Move(int id, int pos)
{
	switch (id)
	{
	case 2:
	{
		if (pos > firJoint)
		{
			do
			{
				dynam->dxl_write_word(2, 30, pos - 50);
			} while (abs(dynam->dxl_read_word(2, 36) - pos) > 50);
		}
		else
		{
			do
			{
				dynam->dxl_write_word(2, 30, pos + 50);
			} while (abs(dynam->dxl_read_word(2, 36) - pos) > 50);
		}
		dynam->dxl_write_word(2, 30, dynam->dxl_read_word(2, 36));
		break;
	}
	case 3:
	{
		do
		{
			dynam->dxl_write_word(3, 30, pos - 50);
		} while (abs(dynam->dxl_read_word(3, 36) - pos) > 50);
		dynam->dxl_write_word(3, 30, dynam->dxl_read_word(3, 36));
		break;
	}

	case 4:
	{
		do
		{
			dynam->dxl_write_word(4, 30, pos + 50);
		} while (abs(dynam->dxl_read_word(4, 36) - pos) > 50);
		dynam->dxl_write_word(4, 30, dynam->dxl_read_word(4, 36));
		break;
	}
	}
}
void load()
{
	while (true)
	{
		cout << "      " << dynam->dxl_read_word(3, 40) << endl;
	}
}
void KhaloRebwar(int pos)
{
	for (int i = 1; i < 8; i++)
		dynam->dxl_write_word(i, 32, 40);
	if (pos == 0)
	{
		dynam->syncWrite(6, 7, 30, firGreep, secGreep);
		dynam->dxl_write_word(5, 30, 185);
		dynam->dxl_write_word(4, 30, 1440);
		Sleep(1000);
		Move(3, 2960);
		Move(2, 2750);
		dynam->dxl_write_word(1, 30, 963);
	}
	else if (pos == 1)
	{
		dynam->dxl_write_word(6, 30, firGreep);
		dynam->dxl_write_word(7, 30, secGreep);
		dynam->dxl_write_word(5, 30, 185);
		dynam->dxl_write_word(4, 30, 1700);
		Move(3, 2960);
		Move(2, 2750);
		dynam->dxl_write_word(1, 30, 963);
		Move(3, 2805);
		Move(4, 2200);
		dynam->dxl_write_word(1, 30, 1981);
		Move(2, 2048);
		dynam->dxl_write_word(5, 30, 1201);
		dynam->syncWrite(6, 7, 30, 1297, 2597);
	}
	else if (pos == 2)
	{
		dynam->dxl_write_word(6, 30, firGreep);
		dynam->dxl_write_word(7, 30, secGreep);
		dynam->dxl_write_word(5, 30, 185);
		Move(4, 1700);
		Move(3, 2960);
		Move(2, 2750);
		dynam->dxl_write_word(1, 30, 963);
		Move(3, 2805);
		dynam->dxl_write_word(1, 30, 1981);
		Move(4, 1584);
		Move(2, 2048);
		dynam->dxl_write_word(5, 30, 1201);
		dynam->syncWrite(6, 7, 30, 1297, 2597);
	}
	else if (pos == 3)
	{
		Move(4, 2098);
		Move(2, 1723);
		do
		{
			dynam->syncWrite(6, 7, 30, firGreep, secGreep);
		} while (abs(dynam->dxl_read_word(6, 36) - firGreep) > 50 || abs(dynam->dxl_read_word(7, 36) - secGreep) > 50);
		Move(2, 2048);
		Move(4, 2400);
	}
	else if (pos == 4)
	{
		dynam->dxl_write_word(6, 30, firGreep);
		dynam->dxl_write_word(7, 30, secGreep);
		dynam->dxl_write_word(5, 30, 185);
		dynam->dxl_write_word(4, 30, 1700);
		Move(3, 2960);
		Move(2, 2750);
		dynam->dxl_write_word(1, 30, 963);
		Move(3, 2805);
		Move(4, 2200);
		dynam->dxl_write_word(1, 30, 1981);
		Move(2, 2048);
		dynam->dxl_write_word(5, 30, 1201);
	}
	else if (pos == 5)
	{
		dynam->syncWrite(6, 7, 30, firGreep, secGreep);
		dynam->dxl_write_word(5, 30, 185);
		dynam->dxl_write_word(4, 30, 1900);
		Sleep(1000);
		Move(3, 2960);
		Move(2, 2750);
		Move(4, 1380);
		dynam->dxl_write_word(1, 30, 963);
	}
}
void leiserRotate(int direction, int num) {
	ardu->leiser(false, num);

	int position = 0;
	if (direction == 0)
		position = 820 - (612 * num);
	else if (direction == 1)
		position = 512;
	else if (direction == 2)
		position = 208 + (612 * num);
	do {
		dynam->move(position, 14 + num, 600, 5);
	} while (abs(dynam->dxl_read_word(14 + num, 36) - position) > 10);
	ardu->leiser(true, num);

}
void leiserRotate2(int direction, int num) {
	ardu->leiser(false, num);

	int position = 0;
	if (direction == 0)
		position = 820 - (612 * num);
	else if (direction == 1)
		position = 512;
	else if (direction == 2)
		position = 208 + (612 * num);
	dynam->move(position, 14 + num, 600, 5);
	ardu->leiser(true, num);

}
void center(int j, int cam = 0) {
	/*int direction = 1;
	int x = 0;
	open->check(&j);
	while (!open->end);
	x = open->y.x;
	//Sleep(200);
	open->check(&j);
	while (!open->end);
	if (x > open->y.x) {
	direction = -1;
	cout << "if" << endl;
	}
	cout << x << "  " << open->y.x << endl;
	cout << direction << endl;*/
	int camDir;
	if (cam == 0)
		camDir = -1;
	else
		camDir = 1;
	errorCentering = false;
	int breakk = 0;
	//ardu->LED(0, 5);
	//ardu->LED(1, 5);
	int i = 0;
	stopCentering = false;
	int color1 = 0;
	int color2 = 0;
	while (!stopCentering) {

		open->check(&j, cam);
		cout << open->y.x << "   " << open->y.y << endl;
		while (!open->end);
		Sleep(50);
		if (open->y.x > 0 && open->y.x < 640) {
			color1 = (int)open->y.x;
			color2 = (int)open->y.y;
			motor->y = camDir * (310 - color1);
			motor->x = camDir * (220 - color2);
			if (motor->y > 50)
				motor->y = 6;
			else if (motor->y < -50)
				motor->y = -6;
			else if (motor->y < 50 && motor->y > 0)
				motor->y = 4;
			else if (motor->y > -50 && motor->y < 0)
				motor->y = -4;
			if (color1 > 300 && color1 < 320) {
				motor->y = 0;
			}
			if (motor->x > 50)
				motor->x = 6;
			else if (motor->x < -50)
				motor->x = -6;
			else if (motor->x < 50 && motor->x > 0)
				motor->x = 4;
			else if (motor->x > -50 && motor->x < 0)
				motor->x = -4;
			if (color2 > 225 && color2 < 255) {
				motor->x = 0;
			}
			if (color2 > 225 && color2 < 255 && color1 > 300 && color1 < 320)
			{
				motor->stop();
				stopCentering = true;
			}
			motor->move(true);
		}
		else {
			if (color1 == 0 && color2 == 0)
			{
				breakk++;
				if (breakk > 3)
				{
					errorCentering = true;
					break;
				}
			}
			else
			{
				if (color1 < 100)
					motor->moveByDistance(3, 'y', camDir * 5);
				if (color1 > 540)
					motor->moveByDistance(3, 'y', camDir * (-5));
				if (color2 < 100)
					motor->moveByDistance(3, 'x', camDir * 5);
				if (color1 > 380)
					motor->moveByDistance(3, 'y', camDir * (-5));
				breakk++;
				if (breakk > 3)
				{
					errorCentering = true;
					break;
				}
			}
		}
	}
	motor->stop();
}
void centering(int col) {
	while (!stopCentering) {
		center(col, 0);
		moveToObjective = true;
	}
}
void gate()
{
	int j = 2;
	open->check(&j);
	int green = open->maxArea;
	j = 0;
	open->check(&j);
	int red = open->maxArea;
	cout << "red and green : " << red << "   " << green << endl;
	if (red > green) {
		gatee = false;
		return;
	}
	gatee = true;
}
void printHelp() {
	cout << "enter s to set speed..." << endl << "and p to set postion" << endl;
	cout << "and a for acceleration..." << endl << "you can set your dynamixel ID by enter its num... :)" << endl;
	cout << "q => Ax-18     w => Ax-18" << endl;
	cout << "e => Mx-28     r => Mx-28" << endl;
	cout << "t => Mx-64     y => Mx-106" << endl;
	cout << "u => Ax-12     i => Ax-12" << endl;
	cout << " *** " << endl << "Enter H for see this explanation again... :)" << endl << " *** " << endl;
}
void dynamic() {
	cout << "Gwez" << endl;
	while (true) {
		ardu->read();
		if (threadd) {
			if (motorSet) {
				bool setSpeed = false;
				int mSpeed = 50;
				while (true) {
					if (GetAsyncKeyState(0x27) != 0) {
						if (!setSpeed) {
							motor->y = -mSpeed;
							motor->x = 0;
							motor->w = 0;
							motor->move(true);
						}
					}
					else if (GetAsyncKeyState(0x25) != 0) {
						if (!setSpeed) {
							motor->y = mSpeed;
							motor->x = 0;
							motor->w = 0;
							motor->move(true);
						}
					}
					else if (GetAsyncKeyState(0x26) != 0) {
						if (!setSpeed) {
							motor->y = 0;
							motor->x = mSpeed;
							motor->w = 0;
							motor->move(true);
						}
						else {
							mSpeed++;
							cout << "Your current speed : " << mSpeed << endl;
						}
					}
					else if (GetAsyncKeyState(0x28) != 0) {
						if (!setSpeed) {
							motor->y = 0;
							motor->x = -mSpeed;
							motor->w = 0;
							motor->move(true);
						}
						else {
							if (mSpeed > 0) {
								mSpeed--;
								cout << "Your current speed : " << mSpeed << endl;
							}
							else
								cout << "Speed is minimun , you can't reduce it..." << endl;
						}
					}
					else if (GetAsyncKeyState(0x64) != 0) {
						if (!setSpeed) {
							motor->y = 0;
							motor->x = 0;
							motor->w = mSpeed * 200;
							motor->move(true);
						}
					}
					else if (GetAsyncKeyState(0x66) != 0) {
						if (!setSpeed) {
							motor->y = 0;
							motor->x = 0;
							motor->w = -mSpeed * 200;
							motor->move(true);
						}
					}
					else
						motor->stop();
					if (GetAsyncKeyState(0x53) != 0) {
						Sleep(400);
						setSpeed = true;
						cout << "your speed : " << mSpeed << endl;
					}
					if (GetAsyncKeyState(0x41) != 0)
						setSpeed = false;
				}
			}
			else if (dynamixell) {
				int what = 1;
				bool help = 0;
				for (int i = 5; i < 13; i++)
					dynam->dxl_write_word(i, 32, 50);
				printHelp();
				while (true) {
					if (GetAsyncKeyState(0x26) != 0) {
						if (what == 1) {
							cout << " speed : " << speed[ID - 5] << endl;
							if (speed[ID - 5] < 1010)
								speed[ID - 5] += 10;
							dynam->dxl_write_word(ID, 32, speed[ID - 5]);
						}
						else if (what == 0) {
							int position = dynam->dxl_read_word(ID, 36);
							cout << " position : " << position << endl;
							dynam->dxl_write_word(ID, 30, position + (speed[ID - 5] / 5));
						}
						else {
							cout << " acceleration : " << acceleration[ID - 5] << endl;
							if (acceleration[ID - 5] < 20)
								acceleration[ID - 5]++;
							else
								cout << "acceleration is max..." << endl;
							dynam->dxl_write_word(ID, 73, acceleration[ID - 5]);
						}
						help = true;
					}
					if (GetAsyncKeyState(0x28) != 0) {
						if (what == 1) {
							cout << " speed : " << speed[ID - 5] << endl;
							if (speed[ID - 5] > 10)
								speed[ID - 5] -= 10;
							dynam->dxl_write_word(ID, 32, speed[ID - 5]);
						}
						else if (what == 0) {
							int position = dynam->dxl_read_word(ID, 36);
							cout << " position : " << position << endl;
							dynam->dxl_write_word(ID, 30, position - (speed[ID - 5] / 5));
						}
						else {
							cout << " acceleration : " << acceleration[ID - 5] << endl;
							if (acceleration[ID - 5] > 0)
								acceleration[ID - 5]--;
							else
								cout << "acceleration is min..." << endl;
							dynam->dxl_write_word(ID, 73, acceleration[ID - 5]);
						}
						help = true;
					}
					if (GetAsyncKeyState(0x51) != 0) {
						Sleep(300);
						ID = 5;
						help = true;
						cout << "ID set to Ax-18" << endl;
					}
					if (GetAsyncKeyState(0x57) != 0) {
						Sleep(300);
						ID = 6;
						help = true;
						cout << "ID set to Ax-18" << endl;
					}if (GetAsyncKeyState(0x45) != 0) {
						Sleep(300);
						ID = 7;
						help = true;
						cout << "ID set to Mx-28" << endl;
					}if (GetAsyncKeyState(0x52) != 0) {
						Sleep(300);
						ID = 8;
						help = true;
						cout << "ID set to Mx-28" << endl;
					}if (GetAsyncKeyState(0x54) != 0) {
						Sleep(300);
						ID = 9;
						help = true;
						cout << "ID set to Mx-64" << endl;
					}if (GetAsyncKeyState(0x59) != 0) {
						Sleep(300);
						ID = 10;
						help = true;
						cout << "ID set to Mx-106" << endl;
					}
					if (GetAsyncKeyState(0x55) != 0) {
						Sleep(300);
						ID = 11;
						help = true;
						cout << "ID set to Ax-12" << endl;
					}
					if (GetAsyncKeyState(0x49) != 0) {
						Sleep(300);
						ID = 12;
						help = true;
						cout << "ID set to Ax-12" << endl;
					}
					if (GetAsyncKeyState(0x50) != 0) {
						Sleep(300);
						what = 0;
						cout << "ok , now you can use up and down arrow to set your dynamixel's position..." << endl;
					}
					if (GetAsyncKeyState(0x53) != 0) {
						Sleep(300);
						what = 1;
						cout << "ok , now you can use up and down arrow to set your dynamixel's speed..." << endl;
					}
					if (GetAsyncKeyState(0x41) != 0) {
						Sleep(300);
						what = 2;
						cout << "ok , now you can use up and down arrow to set your dynamixel's acceleration..." << endl;
					}
					if (GetAsyncKeyState(0x48) != 0) {
						if (help) {
							printHelp();
							help = false;
						}
					}
				}
			}
		}
	}
}
void arduRead()
{
	cout << "gwez" << endl;
	while (true)
	{
		//cout << "                    leisers are : " << ardu->leiserInt[0] << "   " << ardu->leiserInt[1] <<  endl;
		//Sleep(1);
		ardu->read();
	}
}
void moveByLeiser(bool x, int distance, int lei, int direction = 1, bool duel = false)
{
	ardu->LED(0, 5);
	ardu->LED(1, 5);
	ardu->leiser(false, lei);
	if (duel)
	{
		while (ardu->leiserr[0] == 0 || ardu->leiserr[1] == 0)
		{
			ardu->leiser(true, 0);
			ardu->leiser(true, 1);
		}
		if (ardu->leiserInt[0] > (ardu->leiserInt[1] + 50))
			lei = 1;
		else
			lei = 0;
	}
	else
	{
		while (ardu->leiserr[lei] == 0)
		{
			ardu->leiser(true, lei);
		}
	}
	if (abs(ardu->leiserInt[lei] - distance) > 100) {
		int speed = (abs(ardu->leiserInt[lei] - distance) / (ardu->leiserInt[lei] - distance))
			* direction * ((x * 2) - 1) * 40;
		if (x)
			motor->moveByDistance((abs(ardu->leiserInt[lei] - distance) - 100) / 10, 'x', speed);
		else
			motor->moveByDistance((abs(ardu->leiserInt[lei] - distance) - 100) / 10, 'y', speed);
	}


	motor->x = 0;
	motor->y = 0;
	motor->w = 0;
	int counter = 0;
	int k = 0;
	do
	{
		//cout << ardu->leiserInt[lei] << endl;
		if (ardu->leiserInt[lei] == 0)
			ardu->leiser(true, lei);
		if (duel) {
			if (ardu->leiserInt[0] > (ardu->leiserInt[1] + 50))
				lei = 1;
			else
				lei = 0;
		}
		if (ardu->leiserInt[lei] > 4000)
			continue;
		if (abs(ardu->leiserInt[lei] - distance) < 12)
			counter++;
		if (x)
		{
			if (ardu->leiserInt[lei] - distance > 400)
				motor->x = 30 * direction;
			else if (ardu->leiserInt[lei] - distance < -400)
				motor->x = -30 * direction;
			else if (ardu->leiserInt[lei] - distance > 250)
				motor->x = 30 * direction;
			else if (ardu->leiserInt[lei] - distance < -250)
				motor->x = -30 * direction;
			else if (ardu->leiserInt[lei] - distance > 50)
				motor->x = 7 * direction;
			else if (ardu->leiserInt[lei] - distance < -50)
				motor->x = -7 * direction;
			else if (ardu->leiserInt[lei] - distance > 12)
				motor->x = 7 * direction;
			else if (ardu->leiserInt[lei] - distance < -12)
				motor->x = -7 * direction;
			else {
				motor->stop();
			}

		}
		else
		{
			if (ardu->leiserInt[lei] - distance > 400)
				motor->y = -30 * direction;
			else if (ardu->leiserInt[lei] - distance < -400)
				motor->y = 30 * direction;
			else if (ardu->leiserInt[lei] - distance > 250)
				motor->y = -30 * direction;
			else if (ardu->leiserInt[lei] - distance < -250)
				motor->y = 30 * direction;
			else if (ardu->leiserInt[lei] - distance > 50)
				motor->y = -7 * direction;
			else if (ardu->leiserInt[lei] - distance < -50)
				motor->y = 7 * direction;
			else if (ardu->leiserInt[lei] - distance > 12)
				motor->y = -7 * direction;
			else if (ardu->leiserInt[lei] - distance < -12)
				motor->y = 7 * direction;
			else
				motor->stop();
		}
		motor->move(true);
	} while (counter < 30);
	motor->stop();
	ardu->LED(0, 0);
	ardu->LED(1, 0);
}
void moveByLeiser2(bool x, int distance, int lei, int direction = 1, bool duel = false)
{
	ardu->LED(0, 5);
	ardu->LED(1, 5);
	ardu->leiser(false, lei);
	if (duel)
	{
		while (ardu->leiserr[0] == 0 || ardu->leiserr[1] == 0)
		{
			ardu->leiser(true, 0);
			ardu->leiser(true, 1);
		}
		if (ardu->leiserInt[0] > (ardu->leiserInt[1] + 50))
			lei = 1;
		else
			lei = 0;
	}
	else
	{
		while (ardu->leiserr[lei] == 0)
		{
			ardu->leiser(true, lei);
		}
	}
	if (abs(ardu->leiserInt[lei] - distance) > 100) {
		int speed = (abs(ardu->leiserInt[lei] - distance) / (ardu->leiserInt[lei] - distance))
			* direction * ((x * 2) - 1) * 60;
		if (x)
			motor->moveByDistance((abs(ardu->leiserInt[lei] - distance) - 70) / 10, 'x', speed);
		else
			motor->moveByDistance((abs(ardu->leiserInt[lei] - distance) - 70) / 10, 'y', speed);
	}

	motor->x = 0;
	motor->y = 0;
	motor->w = 0;
	int counter = 0;
	int k = 0;
	do
	{
		if (ardu->leiserInt[lei] == 0)
			ardu->leiser(true, lei);
		if (duel) {
			if (ardu->leiserInt[0] > (ardu->leiserInt[1] + 50))
				lei = 1;
			else
				lei = 0;
		}
		if (ardu->leiserInt[lei] > 4000)
			continue;
		if (abs(ardu->leiserInt[lei] - distance) < 30)
			counter++;
		if (x)
		{
			if (ardu->leiserInt[lei] - distance > 400)
				motor->x = 40 * direction;
			else if (ardu->leiserInt[lei] - distance < -400)
				motor->x = -40 * direction;
			else if (ardu->leiserInt[lei] - distance > 250)
				motor->x = 25 * direction;
			else if (ardu->leiserInt[lei] - distance < -250)
				motor->x = -25 * direction;
			else if (ardu->leiserInt[lei] - distance > 50)
				motor->x = 5 * direction;
			else if (ardu->leiserInt[lei] - distance < -50)
				motor->x = -5 * direction;
			else if (ardu->leiserInt[lei] - distance > 5)
				motor->x = 4 * direction;
			else if (ardu->leiserInt[lei] - distance < -5)
				motor->x = -4 * direction;
			else {
				motor->stop();
			}

		}
		else
		{
			if (ardu->leiserInt[lei] - distance > 400)
				motor->y = -40 * direction;
			else if (ardu->leiserInt[lei] - distance < -400)
				motor->y = 40 * direction;
			else if (ardu->leiserInt[lei] - distance > 250)
				motor->y = -20 * direction;
			else if (ardu->leiserInt[lei] - distance < -250)
				motor->y = 20 * direction;
			else if (ardu->leiserInt[lei] - distance > 50)
				motor->y = -5 * direction;
			else if (ardu->leiserInt[lei] - distance < -50)
				motor->y = 5 * direction;
			else if (ardu->leiserInt[lei] - distance > 5)
				motor->y = -4 * direction;
			else if (ardu->leiserInt[lei] - distance < -5)
				motor->y = 4 * direction;
			else
				motor->stop();
		}
		motor->move(true);
	} while (counter < 30);
	motor->stop();
	ardu->LED(0, 0);
	ardu->LED(1, 0);
}
void moveByLeiser3(bool x, int distance, int lei, int direction = 1, bool duel = false)
{
	ardu->LED(0, 5);
	ardu->LED(1, 5);
	ardu->leiser(false, lei);

	{
		while (ardu->leiserr[lei] == 0)
		{
			ardu->leiser(true, lei);
		}
		int speed = (abs(ardu->leiserInt[lei] - distance) / (ardu->leiserInt[lei] - distance))
			* direction * ((x * 2) - 1) * 60;
		if (x)
			motor->moveByDistance((abs(ardu->leiserInt[lei] - distance) - 100) / 10, 'x', speed);
		else
			motor->moveByDistance((abs(ardu->leiserInt[lei] - distance) - 100) / 10, 'y', speed);
	}
	ardu->LED(0, 0);
	ardu->LED(1, 0);
}
void GWEZ(int cm, int speed, char distanceDir, bool leiNum, int leiInt, int d, bool forward)
{
	ardu->LED(1, 5);
	ardu->LED(0, 5);
	//rotate leiser 
	if (distanceDir == 'x')
	{
		if (forward)
		{

			leiserRotate2(0, leiNum);
		}
		else
		{
			leiserRotate2(2, leiNum);
		}
	}
	else
	{
		if (leiNum == 0)
		{
			leiserRotate2(1, 0);
		}
		else
		{
			leiserRotate2(1, 1);
		}
	}
	//end rotete leiser
	//move to goal position
	motor->moveByDistance(cm, distanceDir, speed);
	//check goal position with leiser
	if (abs(ardu->leiserInt[leiNum] - leiInt) > d)
	{
		if (distanceDir == 'x')
		{
			moveByLeiser2(true, leiInt, leiNum, 2 * forward - 1);
		}
		else
		{
			moveByLeiser2(false, leiInt, leiNum, (-2 * leiNum) + 1);
		}
	}
	//end check
	ardu->LED(1, 0);
	ardu->LED(0, 0);
}
void greep(bool gree) {
	dynam->dxl_write_word(10, 32, 100);
	dynam->dxl_write_word(11, 32, 100);
	if (!gree) {
		do
		{
			dynam->dxl_write_word(10, 30, FGreep1 + 590);
			dynam->dxl_write_word(11, 30, FGreep2 - 600);
		} while (abs(dynam->dxl_read_word(11, 36) - (FGreep2 - 600)) > 100);
	}
	else {
		do
		{
			dynam->move(FGreep1, 10, 500, 5);
			dynam->move(FGreep2, 11, 500, 5);
		} while (abs(dynam->dxl_read_word(10, 36) - FGreep1) > 100 ||
			abs(dynam->dxl_read_word(11, 36) - FGreep2) > 100);

	}

}
void sharp()
{

	int sharpTimer = 0;
	int direction = -1;
	if (ardu->sharpNum[0] == 6 || ardu->sharpNum[0] == 0)
		direction = 1;
	while (ardu->sharp[ardu->sharpNum[0]] == 0)
	{
		cout << "Error on sharp..." << endl;
	}
	int leiNum = 0;
	if (ardu->sharpNum[0] == 0)
	{
		leiserRotate(1, 0);
		leiNum = ardu->leiserInt[0];
		if (leiNum < 110)
		{
			motor->moveByDistance(5, 'y', 15);
		}
	}
	else if (ardu->sharpNum[0] == 2)
	{
		leiserRotate(2, 0);
		leiNum = ardu->leiserInt[0];
		if (leiNum < 390)
			motor->moveByDistance(5, 'x', 15);

	}
	else if (ardu->sharpNum[0] == 4)
	{
		leiserRotate(1, 1);
		leiNum = ardu->leiserInt[1];
		if (leiNum < 110)
		{
			motor->moveByDistance(5, 'y', -15);
		}
	}
	else
	{
		leiserRotate(0, 1);
		if (ardu->error[1])
			motor->moveByDistance(6, 'x', -15);
		else
		{
			leiNum = ardu->leiserInt[1];
			if (leiNum < 110)
				motor->moveByDistance(5, 'x', -15);
		}
	}

	int counter = 0;
	motor->x = 0;
	int k = ardu->sharp[ardu->sharpNum[0]] + ardu->sharp[ardu->sharpNum[1]];
	motor->x = 0;
	motor->y = 0;
	motor->w = 0;

	if (k > 1000)
		k = 3;
	else if (k > 600)
		k = 2;
	else
		k = 1;
	motor->y = 0;
	if (abs(ardu->sharp[ardu->sharpNum[0]] - ardu->sharp[ardu->sharpNum[1]]) < 7 * k) {
		std::cout << "sharp :                    " << ardu->sharp[ardu->sharpNum[0]] <<
			"   " << ardu->sharp[ardu->sharpNum[1]] << endl;
		return;
	}
	/*do
	{
	motor->x = 7;
	motor->w = 0;
	motor->move(true);
	} while (ardu->sharp[6] < 450 || ardu->sharp[7] < 450);
	motor->stop();*/
	do
	{
		//sharpTimer++;
		k = ardu->sharp[ardu->sharpNum[0]] + ardu->sharp[ardu->sharpNum[1]];
		if (k > 1000)
			k = 3;
		else if (k > 600)
			k = 2;
		else
			k = 1;

		int m = ardu->sharp[ardu->sharpNum[0]] - ardu->sharp[ardu->sharpNum[1]];
		if (m > 50)
		{
			motor->w = -1000 * direction;
			counter = 0;
		}
		else if (m > 20) {
			motor->w = -850 * direction;
			counter = 0;
		}
		else if (m > 3) {
			motor->w = -650 * direction;
			counter = 0;
		}
		else if (m < -50)
		{
			motor->w = 1000 * direction;
			counter = 0;
		}
		else if (m < -20) {
			motor->w = 850 * direction;
			counter = 0;
		}
		else if (m < -3) {
			motor->w = 650 * direction;
			counter = 0;
		}
		motor->move(true);
		if (abs(ardu->sharp[ardu->sharpNum[0]] - ardu->sharp[ardu->sharpNum[1]]) < 3 * k)
		{
			//stop = true;
			counter++;
		}
		//if (sharpTimer > 50)
	} while (counter < 30);
	motor->stop();
	cout << "sharp :                    " << ardu->sharp[ardu->sharpNum[0]] <<
		"   " << ardu->sharp[ardu->sharpNum[1]] << endl;


	if (ardu->sharpNum[0] == 0)
	{
		leiserRotate(1, 0);
		moveByLeiser(false, leiNum, 0);
	}
	else if (ardu->sharpNum[0] == 2)
	{
		leiserRotate(2, 1);
		moveByLeiser(true, leiNum, 1, -1);

	}
	else if (ardu->sharpNum[0] == 4)
	{
		leiserRotate(1, 1);
		moveByLeiser(false, leiNum, 1, -1);
	}
	/*else
	{
	leiserRotate(0, 1);
	moveByLeiser(true, 120, 1);
	}*/
}
void start() {
	dynam->move(75, 6, 200, 5);
	dynam->move(525, 5, 200, 5);
	// 2 ,0   1 , 1
	gate();
	if (gatee) {
		mciSendString("close mp3", NULL, 0, NULL);
		mciSendString("open \"musicTime.wav\" type mpegvideo alias mp3", NULL, 0, NULL);
		mciSendString("play mp3", NULL, 0, NULL);
		GWEZ(120, 60, 'x', 1, 270, 70, 1);
		GWEZ(73, -60, 'y', 1, 830, 70, 0);
	}
	else {
		mciSendString("close mp3", NULL, 0, NULL);
		mciSendString("open \"musicTime.wav\" type mpegvideo alias mp3", NULL, 0, NULL);
		mciSendString("play mp3", NULL, 0, NULL);
		GWEZ(74, -60, 'y', 1, 860, 30, 0);
		GWEZ(120, 60, 'x', 0, 1650, 70, 0);
	}
	motor->moveByDistance(4, 'y', -20);
}
void testAll() {
	while (true) {
		cout << "(Camera , Dynamixel , Motors)..." << endl;
		char x;
		cin >> x;
		if (x == 'd') {
			if (dynamixell)
			{
				char firstEnter;
				cout << "for set 106 dynamixel enter 1" << endl;
				cout << "for set 64 dynamixel enter 2" << endl;
				cout << "for set first 28 dynamixel enter 3" << endl;
				cout << "for set second 28 dynamixel enter 4" << endl;
				cout << "for set first 18 dynamixel enter 5" << endl;
				cout << "for set second 18dynamixel enter 6" << endl;
				cout << "for set first 12 dynamixel enter 7" << endl;
				cout << "for set second 12 dynamixel enter 8" << endl;
				cin >> firstEnter;
				switch (firstEnter)
				{
				case '1':
				{
					int acc, speed, position;
					cout << "Enter acceleration :  " << endl;
					cin >> acc;
					cout << "enter speed : " << endl;
					cin >> speed;
					cout << "Enter position from -28672 to 28672 : " << endl;
					cin >> position;
					dynam->dxl_write_word(10, 73, acc);
					dynam->dxl_write_word(10, 32, speed);
					dynam->dxl_write_word(10, 30, position);
					break;
				}
				case '2':
				{
					int acc, speed, position;
					cout << "Enter acceleration :  " << endl;
					cin >> acc;
					cout << "enter speed : " << endl;
					cin >> speed;
					cout << "Enter position from 0 to 4095 (360 degree)" << endl;
					cin >> position;
					dynam->dxl_write_word(9, 73, acc);
					dynam->dxl_write_word(9, 32, speed);
					dynam->dxl_write_word(9, 30, position);
					break;
				}
				case '3':
				{
					int acc, speed, position;
					cout << "Enter acceleration :  " << endl;
					cin >> acc;
					cout << "enter speed : " << endl;
					cin >> speed;
					cout << "Enter position from -28672 to 28672 : " << endl;
					cin >> position;
					dynam->dxl_write_word(7, 73, acc);
					dynam->dxl_write_word(7, 32, speed);
					dynam->dxl_write_word(7, 30, position);
					break;
				}
				case '4':
				{
					int acc, speed, position;
					cout << "Enter acceleration :  " << endl;
					cin >> acc;
					cout << "enter speed : " << endl;
					cin >> speed;
					cout << "Enter position from -28672 to 28672 : " << endl;
					cin >> position;
					dynam->dxl_write_word(8, 73, acc);
					dynam->dxl_write_word(8, 32, speed);
					dynam->dxl_write_word(8, 30, position);
					break;
				}
				case '5':
				{
					int acc, speed, position;
					cout << "Enter acceleration :  " << endl;
					cin >> acc;
					cout << "enter speed : " << endl;
					cin >> speed;
					cout << "Enter position from 0 to 1023 (300 degree )" << endl;
					cin >> position;
					dynam->dxl_write_word(5, 73, acc);
					dynam->dxl_write_word(5, 32, speed);
					dynam->dxl_write_word(5, 30, position);
					break;
				}
				case '6':
				{
					int acc, speed, position;
					cout << "Enter acceleration :  " << endl;
					cin >> acc;
					cout << "enter speed : " << endl;
					cin >> speed;
					cout << "Enter position from 0 to 1023 (300 degree )" << endl;
					cin >> position;
					dynam->dxl_write_word(6, 73, acc);
					dynam->dxl_write_word(6, 32, speed);
					dynam->dxl_write_word(6, 30, position);
					break;
				}
				case '7':
				{
					int acc, speed, position;
					cout << "Enter acceleration :  " << endl;
					cin >> acc;
					cout << "enter speed : " << endl;
					cin >> speed;
					cout << "Enter position from 0 to 1023 (300 degree )" << endl;
					cin >> position;
					dynam->dxl_write_word(11, 73, acc);
					dynam->dxl_write_word(11, 32, speed);
					dynam->dxl_write_word(11, 30, position);
					break;
				}
				case '8':
				{
					int acc, speed, position;
					cout << "Enter acceleration :  " << endl;
					cin >> acc;
					cout << "enter speed : " << endl;
					cin >> speed;
					cout << "Enter position from 0 to 1023 (300 degree )" << endl;
					cin >> position;
					dynam->dxl_write_word(12, 73, acc);
					dynam->dxl_write_word(12, 32, speed);
					dynam->dxl_write_word(12, 30, position);
					break;
				}
				default:
					break;
				}
			}
			motorSet = false;
		}
		else if (x == 'm') {
			motorSet = true;
			cout << "manual or Leiser?! " << endl;
			char w;
			cin >> w;
			if (w != 'l') {
				int cm, speed;
				char component;
				do {
					cout << "Enter your component (x, y, w)" << endl;
					cin >> component;
				} while (component != 'x' && component != 'y' && component != 'w');
				cout << "ok, now enter speed..." << endl;
				cin >> speed;
				cout << "and distance... (cm) :)" << endl;
				cin >> cm;
				motor->moveByDistance(cm, component, speed);
			}
			else {
				cout << "Enter distance..." << endl;
				int distance;
				cin >> distance;
				moveByLeiser(true, distance, 0);
			}
		}
		else if (x == 'c')
			open->gwez();
		else if (x == 'e')
			return;
		else if (x == 't')
			threadd = true;
		else
			cout << "wrong input..." << endl;
	}
}
void rotateArm(int mx1, int mx2, bool led) {
	if (led) {
		ardu->LED(0, 3);
		ardu->LED(1, 3);
	}
	int counterArm = 0;
	int pos1 = BalaBar + mx1, ans1;
	int pos2 = Rotator + mx2, ans2;
	do
	{
		counterArm++;
		dynam->move(pos1, 8, 300, 5);
		dynam->move(pos2, 9, 100, 5);
		if (pos1 != 0)
			ans1 = ((abs(pos1) / pos1) - 1) / 2 * 65535;
		else if (dynam->dxl_read_word(8, 36) < 0)
			ans1 = -65535;
		else
			ans1 = 0;

		if (pos2 != 0)
			ans2 = ((abs(pos2) / pos2) - 1) / 2 * 65535;
		else if (dynam->dxl_read_word(9, 36) < 0)
			ans2 = -65535;
		else
			ans2 = 0;
	} while (abs(dynam->dxl_read_word(8, 36) - pos1 + ans1) > 100 ||
		abs(dynam->dxl_read_word(9, 36) - pos2 + ans2) > 100);
}
void referrence() {
	ardu->LED(0, 5);
	ardu->LED(1, 5);
	ardu->setSharp(1);
	Sleep(100);
	leiserRotate(1, 0);
	leiserRotate(2, 1);
	int directionX = 1, directionY = -1, direction = -1;
	int counterX = 0, counterY = 0, counter = 0;
	do
	{
		if (abs(ardu->sharp[ardu->sharpNum[0]] - ardu->sharp[ardu->sharpNum[1]]) < 3)
		{
			//stop = true;
			counter++;
		}
		if (abs(ardu->leiserInt[0] - 1350) < 5)
			counterX++;
		if (abs(ardu->leiserInt[1] - 450) < 5)
			counterY++;
		if (ardu->leiserInt[1] < 600) {
			int m = ardu->sharp[ardu->sharpNum[0]] - ardu->sharp[ardu->sharpNum[1]];
			if (m > 50)
			{
				motor->w = -1000 * direction;
				counter = 0;
			}
			else if (m > 20) {
				motor->w = -850 * direction;
				counter = 0;
			}
			else if (m > 3) {
				motor->w = -650 * direction;
				counter = 0;
			}
			else if (m < -50)
			{
				motor->w = 1000 * direction;
				counter = 0;
			}
			else if (m < -20) {
				motor->w = 850 * direction;
				counter = 0;
			}
			else if (m < -3) {
				motor->w = 650 * direction;
				counter = 0;
			}
		}
		if (ardu->leiserInt[0] - 1380 > 250)
			motor->y = 25 * directionY;
		else if (ardu->leiserInt[0] - 1380 < -250)
			motor->y = -25 * directionY;
		else if (ardu->leiserInt[0] - 1380 > 50)
			motor->y = 5 * directionY;
		else if (ardu->leiserInt[0] - 1380 < -50)
			motor->y = -5 * directionY;
		else if (ardu->leiserInt[0] - 1380 > 5)
			motor->y = 4 * directionY;
		else if (ardu->leiserInt[0] - 1380 < -5)
			motor->y = -4 * directionY;
		else
			motor->y = 0;

		if (ardu->leiserInt[1] - 450 > 250)
			motor->x = -20 * directionX;
		else if (ardu->leiserInt[1] - 450 < -250)
			motor->x = 20 * directionX;
		else if (ardu->leiserInt[1] - 450 > 50)
			motor->x = -5 * directionX;
		else if (ardu->leiserInt[1] - 450 < -50)
			motor->x = 5 * directionX;
		else if (ardu->leiserInt[1] - 450 > 5)
			motor->x = -4 * directionX;
		else if (ardu->leiserInt[1] - 450 < -5)
			motor->x = 4 * directionX;
		else
			motor->x = 0;
		motor->move(true);
	} while (counterX < 30 || counterY < 30 || counter < 30);
	motor->stop();

	ardu->LED(0, 0);
	ardu->LED(1, 0);
	ardu->leiser(false, 0);
	ardu->leiser(false, 1);
}
void checkDynamixel()
{
	//camera Up/Down
	do
	{
		dynam->move(60, 6, 100, 5);
	} while (abs(dynam->dxl_read_word(6, 36) - 60)> 50);

	do
	{
		dynam->move(266, 6, 100, 5);
	} while (abs(dynam->dxl_read_word(6, 36) - 266) > 50);
	//camera Left/Right
	do
	{
		dynam->move(300, 5, 100, 5);
	} while (abs(dynam->dxl_read_word(5, 36) - 300)> 50);

	do
	{
		dynam->move(700, 5, 100, 5);
	} while (abs(dynam->dxl_read_word(5, 36) - 700) > 50);

	do
	{
		dynam->move(525, 5, 100, 5);
	} while (abs(dynam->dxl_read_word(5, 36) - 525)> 50);

	do
	{
		dynam->move(75, 6, 100, 5);
	} while (abs(dynam->dxl_read_word(6, 36) - 75) > 50);
	rotateArm(-3220, -3200, true);

	greep(1);
	Sleep(1000);
	greep(0);
	rotateArm(0, 0, true);
	leiserRotate(1, 0);
	leiserRotate(0, 0);
	leiserRotate(2, 0);

	leiserRotate(1, 1);
	leiserRotate(2, 0);
	leiserRotate(1, 1);
}
int faz() {
	ardu->setSharp(3);

	cout << "start faz..." << endl;
nothing:
	cout << "seraching..." << endl;
	int ans = -1;
	dynam->move(525, 5, 100, 5);
	dynam->move(160, 6, 100, 5);

	//camera
	do
	{
		dynam->move(525, 5, 100, 5);
		dynam->move(160, 6, 100, 5);
		cout << "camera rotate..." << endl;
	} while (abs(dynam->dxl_read_word(5, 36) - 525) > 50 ||
		abs(dynam->dxl_read_word(6, 36) - 160 > 50));
	ardu->LED(3, 0);
	ardu->LED(3, 1);
	dynam->move(mx106 - 3200, 10, 300, 5);
	dynam->move(mx64 + 3220, 9, 200, 5);
	greep(0);
	int detectCount = 0;
	int direction = 0;
	while (true) {
		detectCount++;
		cout << "detecting..." << endl;
		for (int i = 0; i < 4; i++) {
			if (!places[i]->complete) {
				open->check(&places[i]->color);
				if (open->maxArea > 2000) {
					ans = i;
				}
			}
		}
		if (detectCount > 6 && ans == -1) {
			dynam->move(525, 5, 100, 5);
			direction = 0;
		}
		else if (detectCount > 4 && ans == -1) {
			dynam->move(400, 5, 100, 5);
			direction = 1;
		}
		else if (detectCount > 2 && ans == -1) {
			dynam->move(650, 5, 100, 5);
			direction = -1;
		}
		if (ans != -1) {
			cout << "successful detecting with color  " << ans << "   :)" << endl;
			open->play(places[ans]->color);
			if (direction == 1) {
				cout << 1;
				motor->moveByDistance(30, 'y', -30);
				do
				{
					dynam->move(525, 5, 100, 5);
					dynam->move(160, 6, 100, 5);
					cout << "camera rotate..." << endl;
				} while (abs(dynam->dxl_read_word(5, 36) - 525) > 50 ||
					abs(dynam->dxl_read_word(6, 36) - 160 > 50));
			}
			else if (direction == -1) {
				cout << -1;
				motor->moveByDistance(30, 'y', 30);
				do
				{
					dynam->move(525, 5, 100, 5);
					dynam->move(160, 6, 100, 5);
					cout << "camera rotate..." << endl;
				} while (abs(dynam->dxl_read_word(5, 36) - 525) > 50 ||
					abs(dynam->dxl_read_word(6, 36) - 160 > 50));
			}
			detectCount = 0;
			break;
		}
	}
	rotateArm(-3200, 3200, false);
	center(places[ans]->color, 0);
	//thread p(centering, places[ans]->color);
	//while (!moveToObjective);
	if (errorCentering)
		goto nothing;
	//motor->moveByDistance(10, 'x', 30);
	//center(places[ans]->color);

	ardu->LED(0, 5);
	ardu->LED(1, 5);
	motor->moveByDistance(29, 'x', 50);
	ardu->LED(0, 0);
	ardu->LED(1, 0);
	stopCentering = true;
	//p.join();

	rotateArm(-3400, 3200, false);
	greep(1);
	dynam->move(mx106 - 6000, 10, 300, 5);
	stopCentering = false;
	moveToObjective = false;



	/*open->check(&places[ans]->color);
	if (open->maxArea < 2000) {
	cout << "no object! try again..." << endl;
	goto nothing;
	}*/
	mciSendString("close mp3", NULL, 0, NULL);
	mciSendString("open \"objective found.wav\" type mpegvideo alias mp3", NULL, 0, NULL);
	mciSendString("play mp3", NULL, 0, NULL);

	//sharp();
	mciSendString("close mp3", NULL, 0, NULL);
	if (ans == 0)
		mciSendString("open \"bach.wav\" type mpegvideo alias mp3", NULL, 0, NULL);
	else if (ans == 1)
		mciSendString("open \"beet.wav\" type mpegvideo alias mp3", NULL, 0, NULL);
	else if (ans == 2)
		mciSendString("open \"mozr.wav\" type mpegvideo alias mp3", NULL, 0, NULL);
	else
		mciSendString("open \"mozr.wav\" type mpegvideo alias mp3", NULL, 0, NULL);
	mciSendString("play mp3", NULL, 0, NULL);

	leiserRotate(0, 0);
	leiserRotate(0, 1);
	if (ardu->error[0] || ardu->error[1])
		motor->moveByDistance(5, 'x', -4);
	for (int i = 0; i < 30; i++) {
		Sleep(400);
		int l0 = ardu->leiserInt[0], l1 = ardu->leiserInt[1];
		if (abs(l0 - l1) <= 5)
			break;
		else {
			if (l0 > l1)
				motor->moveByDistance(30, 'w', 850);
			else
				motor->moveByDistance(30, 'w', -850);
		}
	}

	ardu->LED(0, 3);
	ardu->LED(1, 3);
	dynam->move(mx64, 9, 200, 5);
	dynam->move(mx106, 10, 200, 5);


	ardu->LED(0, 0);
	ardu->LED(1, 0);


	//sharp();
	return ans;
}
void fazzz(int i) {
	ardu->LED(0, 5);
	ardu->LED(1, 5);
	motor->moveByDistance(22, 'x', -60);
	ardu->LED(0, 0);
	ardu->LED(1, 0);

	ardu->LED(0, 5);
	ardu->LED(1, 5);
	ardu->leiser(false, 0);
	ardu->leiser(false, 1);
	motor->moveByDistance(places[i]->positionW, 'w', -9000 * (2 * places[i]->direction - 1));
	ardu->LED(0, 0);
	ardu->LED(1, 0);

	leiserRotate(1, 1);
	moveByLeiser(false, places[i]->positionY, 1, -1);
	//center(places[i]->color);
	if (i == 3)
		rotateArm(-2400, -3200, true);
	else
		rotateArm(-2400, 3200, true);
	leiserRotate(2, 0);
	if (i == 0)
		moveByLeiser(true, 1380, 0, -1);
	else if (i == 1) {
		leiserRotate(2, 1);
		moveByLeiser(true, 910, 1, -1);
	}
	else
		moveByLeiser(true, 910, 0, -1);
	ardu->leiser(false, 0);
	ardu->leiser(false, 1);
	dynam->move(225, 11, 800, 5);
	dynam->move(800, 12, 800, 5);
	ardu->LED(1, 0);
	ardu->LED(0, 0);
	ardu->leiser(true, 0);
	ardu->leiser(true, 1);
	/*Sleep(300);
	if (places[i]->color == 7)
	Sleep(300);*/
	ardu->LED(0, 5);
	ardu->LED(1, 5);
	for (int j = 0; j < 3; j++) {
		Sleep(1000);
		int l0 = 140, l1 = 140;
		if (ardu->leiserInt[0] - l0 < 400)
			l0 = ardu->leiserInt[0];
		if (!(ardu->leiserInt[1] - l1 > 500))
			l1 = ardu->leiserInt[1];
		if (l0 - l1 > 50) {
			motor->moveByDistance(2, 'y', 3);
		}
		else if (l0 - l1 < -50) {
			motor->moveByDistance(2, 'y', -3);
		}
		cout << l0 << " " << ardu->leiserInt[0] << "     " << l1 << " " << ardu->leiserInt[1] << endl;
	}
	ardu->LED(0, 0);
	ardu->LED(1, 0);
	if (i == 3)
		rotateArm(-1400, -3200, true);
	else
		rotateArm(-1400, 3200, true);

	greep(false);
	mciSendString("close mp3", NULL, 0, NULL);
	mciSendString("open \"objective complete.wav\" type mpegvideo alias mp3", NULL, 0, NULL);
	mciSendString("play mp3", NULL, 0, NULL);
	places[i]->complete = true;

	ardu->LED(0, 5);
	ardu->LED(1, 5);
	motor->moveByDistance(20, 'x', -60);
	ardu->LED(0, 0);
	ardu->LED(1, 0);
	if (counter == 3)
	{
		mciSendString("close mp3", NULL, 0, NULL);
		mciSendString("open \"Aleksander Rybak – Fairytail.wav\" type mpegvideo alias mp3", NULL, 0, NULL);
		mciSendString("play mp3", NULL, 0, NULL);

	}
	else
	{
		mciSendString("close mp3", NULL, 0, NULL);
		mciSendString("open \"musicTime.wav\" type mpegvideo alias mp3", NULL, 0, NULL);
		mciSendString("play mp3", NULL, 0, NULL);
	}
	ardu->leiser(false, 0);
	ardu->leiser(false, 1);
	ardu->LED(0, 5);
	ardu->LED(1, 5);



	dynam->move(mx106, 10, 200, 5);
	dynam->move(mx64, 9, 200, 5);

	if (counter != 3)
	{
		ardu->leiser(false, 0);
		ardu->leiser(false, 1);
		motor->moveByDistance(places[i]->positionW, 'w', 9000 * (2 * places[i]->direction - 1));
		Sleep(250);
		if (i == 0)
			motor->moveByDistance(107, 'y', -60);
		else if (i == 1)
			motor->moveByDistance(57, 'y', -60);
		else if (i == 3)
			motor->moveByDistance(43, 'y', 60);
		leiserRotate(0, 1);
		moveByLeiser(true, 365, 1);
	}
	else
	{
		leiserRotate(1, 1);
		moveByLeiser2(false, 100, 1, -1);
		leiserRotate(0, 1);
		moveByLeiser2(true, 720, 1);
	}
	ardu->LED(0, 0);
	ardu->LED(1, 0);
	rotateArm(0, 0, false);
}
void end() {

	ardu->leiser(false, 0);
	leiserRotate(0, 1);
	dynam->move(266, 6, 200, 5);
	dynam->move(512, 5, 200, 5);

	leiserRotate(1, 0);
	moveByLeiser(true, 1500, 1);
	dynam->move(820, 11, 200, 5);

	if (gatee) {
		moveByLeiser(false, 720, 0, 1);
		leiserRotate(2, 1);
		moveByLeiser(true, 485, 1, -1);
	}
	else {
		leiserRotate(1, 0);
		moveByLeiser(false, 150, 0, 1);

		sharp();

		leiserRotate(2, 1);
		moveByLeiser(true, 500, 1, -1);
	}
	referrence();
	ardu->leiser(false, 0);
	ardu->leiser(false, 1);
	ardu->LED(0, 0);
	ardu->LED(1, 0);

}
int setCode(String x)
{
	if (x == "0")
		return 0;
	else if (x == "1")
		return 1;
	else if (x == "2")
		return 2;
	else if (x == "3")
		return 3;
	else if (x == "4")
		return 4;
	else if (x == "5")
		return 5;
	else if (x == "6")
		return 6;
	else if (x == "7")
		return 7;
	else if (x == "8")
		return 8;
	else
		return -1;
}
String setStr(int x)
{
	if (x == 0)
		return "0";
	else if (x == 1)
		return "1";
	else if (x == 2)
		return "2";
	else if (x == 3)
		return "3";
	else if (x == 4)
		return "4";
	else if (x == 5)
		return "5";
	else if (x == 6)
		return "6";
	else if (x == 7)
		return "7";
	else if (x == 8)
		return "8";
	else
		return "-1";
}
int setColor(int code)
{
	if (code == 0)
		return -1;
	else if (code == 1)
		return 1;
	else if (code == 2)
		return 3;
	else if (code == 3)
		return 0;
	else if (code == 4)
		return 9;
	else if (code == 5)
		return 4;
	else if (code == 6)
		return 2;
	else if (code == 7)
		return 6;
	else if (code == 8)
		return 10;
	else if (code == 9)
		return -1;
	else if (code == 10)
		return -1;
	else if (code == 11)
		return -1;
	else if (code == 12)
		return -1;
	else if (code == 13)
		return -1;
	else if (code == 14)
		return -1;
	else if (code == 15)
		return -1;
	else
		return -1;
}
void detectPlaces()
{
	KhaloRebwar(1);
	leiserRotate(1, 0);
	ardu->leiser(1, 0);
	moveByLeiser(false, 130, 0);
	leiserRotate(0, 1);
	moveByLeiser(1, 2580, 1);
	Sleep(1000);
	String parent = "UNKNOWN";
	while (parent == "UNKNOWN")
		parent = (String)open->detectCode();
	cout << parent << endl;
	places[0]->code = setCode(parent);
	places[0]->color = setColor(places[0]->code);
	motor->moveByDistance(35, 'y', 20);
	Sleep(1000);
	while (parent == "UNKNOWN" | parent == setStr(places[0]->code))
		parent = (String)open->detectCode();
	cout << parent << endl;
	places[1]->code = setCode(parent);
	places[1]->color = setColor(places[1]->code);
	motor->moveByDistance(40, 'y', 20);
	Sleep(1000);
	while (parent == "UNKNOWN" | parent == setStr(places[1]->code))
		parent = (String)open->detectCode();
	cout << parent << endl;
	places[2]->code = setCode(parent);
	places[2]->color = setColor(places[2]->code);
	motor->moveByDistance(35, 'y', 20);
	Sleep(1000);
	while (parent == "UNKNOWN" | parent == setStr(places[2]->code))
		parent = (String)open->detectCode();
	cout << parent << endl;
	places[3]->code = setCode(parent);
	places[3]->color = setColor(places[3]->code);
	motor->moveByDistance(23, 'y', 20);
	Sleep(1000);
	while (parent == "UNKNOWN" | parent == setStr(places[3]->code))
		parent = (String)open->detectCode();
	cout << parent << endl;
	places[4]->code = setCode(parent);
	places[4]->color = setColor(places[4]->code);
	leiserRotate(1, 0);
	moveByLeiser(0, 750, 0);
	leiserRotate(0, 1);
	moveByLeiser(1, 2150, 1);
	dynam->syncWrite(6, 7, 30, firGreep, secGreep);
	dynam->dxl_write_word(5, 30, 185);
	dynam->dxl_write_word(4, 30, 1700);
	Move(3, 2960);
	Move(2, 2750);
	dynam->dxl_write_word(1, 30, 963);
	motor->moveByDistance(3400, 'w', 6000);
}
void realOpenCv()
{
	while (notFound)
	{
		for (int i = 0; i < 5; i++)
		{
			if (!notFound)
				break;
			if (places[i]->found == false)
			{
				cout << "check   :   " << i << endl;
				open->check(&(places[i]->color));
				if (open->maxArea > 1000)
				{
					realColor[0] = places[i]->color;
					notFound = false;
					cout << "detect color : " << realColor[0] << endl;
					leiserRotate(2, 0);
					leiserRotate(1, 1);
					cout << "leisers 0-1 for set last pos is : " << ardu->leiserInt[0] << "   " << ardu->leiserInt[1] << endl;
					while (ardu->leiserInt[0] == 0 | ardu->leiserInt[1] == 0)
					{
						Sleep(30);
					}
					Sleep(1500);

					cout << "centering..." << endl;
					center(realColor[0], 0);
					cout << "centerd done " << endl;


					KhaloRebwar(3);
					KhaloRebwar(0);


					lastPosX = ardu->leiserInt[0];
					lastPosY = ardu->leiserInt[1];

					places[i]->found = true;
					leiserRotate(1, 1);
					moveByLeiser(0, 750, 1, -1);

					motor->moveByDistance(3400, 'w', 6000);
					KhaloRebwar(4);
					leiserRotate(1, 0);
					moveByLeiser(0, places[i]->positionY, 0);
					leiserRotate(0, 1);
					moveByLeiser(1, places[i]->positionX, 1);
					dynam->syncWrite(6, 7, 30, 1297, 2597);

					Sleep(1000);
					moveByLeiser(1, 2200, 1);
					moveByLeiser(0, 750, 0);
					KhaloRebwar(0);
					Sleep(2000);
					motor->moveByDistance(3400, 'w', 6000);
					places[i]->complete = true;
					return;
				}
			}
		}
	}
}
void senario()
{

	//faz2

	cout << "enter x" << endl;
	int m;
	cin >> m;
	int maxY = 0, maxCol = -1, id = -1;
	vector<vector<int>> T;
	for (int j = 0; j < 5; j++)
	{

		KhaloRebwar(2);
		leiserRotate(1, 1);
		moveByLeiser(0, 180, 1, -1);
		maxCol = -1;
		id = -1;
		maxY = 0;
		for (int i = 0; i < 5; i++)
		{
			places[i]->area = 0;
			places[i]->x = 0;
			places[i]->y = 0;
		}
		for (int i = 0; i < 5; i++)
		{
			if (places[i]->found == false)
			{
				cout << "check   :   " << i << endl;
				open->check(&(places[i]->color));
				if (open->maxArea > 1000)
				{
					places[i]->area = open->maxArea;
					places[i]->x = open->y.x;
					places[i]->y = open->y.y;
				}
			}
		}
		for (int i = 0; i < 5; i++)
		{
			if (places[i]->found == false)
			{
				if (places[i]->y > maxY)
				{
					maxY = places[i]->y;
					maxCol = places[i]->color;
					id = i;
				}
			}
		}
		if (maxCol > -1)
		{
			cout << "Centering places : " << id << " with color : " << maxCol << endl;
			center(maxCol, 0);
			cout << "picking up..." << endl;
			KhaloRebwar(3);
			places[id]->found = true;
			motor->moveByDistance(30, 'y', -30);
			KhaloRebwar(0);
		}
		/*for (int i = 0; i < 5; i++)
		{
		cout << "reading : " << i << endl;
		//open->maxArea = -1;
		if (!places[i]->found)
		{
		open->check(&places[i]->color);
		if (open->maxArea > 1000)
		{
		places[i]->area = open->maxArea;
		places[i]->x = open->y.x;
		places[i]->y = open->y.y;


		}
		}
		}

		for (int i = 0; i < 5; i++)
		{
		if (!places[i]->found) {
		if (places[i]->y > maxY)
		{
		maxY = places[i]->y;
		maxCol = places[i]->color;
		id = i;
		}
		}
		}
		if (maxCol > -1)
		{
		cout << "place : " << id << "   found.with num : " << places[id]->code << endl;
		cout << "area : " << places[id]->area << endl;
		center(maxCol);
		KhaloRebwar(3);
		places[id]->found = true;
		motor->moveByDistance(30, 'y', -30);
		KhaloRebwar(0);
		cout << "enter x :" << endl;
		cin >> m;
		}
		else
		{
		cout << "NOT FOUND HERE !" << endl;
		leiserRotate(1, 1);
		moveByLeiser(0, 1300, 1,-1);
		for (int i = 0; i < 5; i++)
		{
		if (!places[i]->found) {
		open->check(&places[i]->color);
		if (open->maxArea > 1000)
		{
		places[i]->area = open->maxArea;
		places[i]->x = open->y.x;
		places[i]->y = open->y.y;
		}
		}
		}
		for (int i = 0; i < 5; i++)
		{
		if (!places[i]->found) {
		if (places[i]->y > maxY)
		{
		maxY = places[i]->y;
		maxCol = places[i]->color;
		id = i;
		}
		}
		}
		if (maxCol > -1)
		{
		cout << "place : " << id << "   found.with num : " << places[id]->code << endl;
		cout << "area : " << places[id]->area << endl;
		center(maxCol);
		KhaloRebwar(3);
		places[id]->found = true;
		motor->moveByDistance(30, 'y', 30);
		KhaloRebwar(0);
		cout << "enter x :" << endl;
		cin >> m;
		}
		else
		{
		cout << "NOT FOUND HERE !" << endl;
		leiserRotate(1, 1);
		moveByLeiser(0, 750, 1,-1);
		leiserRotate(2, 0);
		moveByLeiser(1, 850, 0, -1);
		for (int i = 0; i < 5; i++)
		{
		if (!places[i]->found) {
		open->check(&places[i]->color);
		if (open->maxArea > 1000)
		{
		places[i]->area = open->maxArea;
		places[i]->x = open->y.x;
		places[i]->y = open->y.y;
		}
		}
		}
		for (int i = 0; i < 5; i++)
		{
		if (!places[i]->found) {
		if (places[i]->y > maxY)
		{
		maxY = places[i]->y;
		maxCol = places[i]->color;
		id = i;
		}
		}
		}
		if (maxCol > -1)
		{
		cout << "place : " << id << "   found.with num : " << places[id]->code << endl;
		cout << "area : " << places[id]->area << endl;
		center(maxCol);
		KhaloRebwar(3);
		places[id]->found = true;
		motor->moveByDistance(30, 'x', 30);
		KhaloRebwar(0);
		cout << "enter x :" << endl;
		cin >> m;
		}
		}
		}*/
		leiserRotate(1, 1);
		moveByLeiser(0, 750, 1, -1);
		motor->moveByDistance(3400, 'w', -6000);
		cout << "enter x :" << endl;
		cin >> m;
		KhaloRebwar(4);
		leiserRotate(1, 0);
		moveByLeiser(0, places[id]->positionY, 0);
		leiserRotate(0, 1);
		moveByLeiser(1, places[id]->positionX, 1);
		dynam->syncWrite(6, 7, 30, 1297, 2597);
		places[id]->complete = true;
		Sleep(3000);
		moveByLeiser(1, 2250, 1);
		moveByLeiser(0, 750, 0);
		KhaloRebwar(0);
		Sleep(2000);
		motor->moveByDistance(3400, 'w', 6000);
		open->cap.~VideoCapture();
		cout << "enter x :" << endl;
		cin >> m;
	}
}
void senarioo()
{
	detectPlaces();
	for (int i = 0; i < 5; i++)
	{
		KhaloRebwar(2);
		leiserRotate(1, 1);
		moveByLeiser(0, lastPosY, 1, -1);
		notFound = true;
		realColor[0] = -1;
		leiserRotate(1, 1);
		thread *p = new thread(realOpenCv);
		while (realColor[0] == -1 && ardu->leiserInt[1] < 1400)
		{
			motor->moveByDistance(5, 'y', -6);
		}
		motor->stop();
		int id = -1;
		if (realColor[0] == -1)
		{
			cout << "not found" << endl;
		}
		else {
			for (int i = 0; i < 5; i++)
			{
				if (realColor[0] == places[i]->color)
				{
					id = i;
					break;
				}
			}
		}
		while (!places[id]->complete);
	}
}
void senariooo()
{
	detectPlaces();
	for (int i = 0; i < 5; i++)
	{
		KhaloRebwar(2);
		leiserRotate(1, 1);
		leiserRotate(2, 0);
		cout << "Last Y : " << lastPosY << endl;
		cout << "last X : " << lastPosX << endl;
		moveByLeiser(0, lastPosY, 1, -1);
		moveByLeiser(1, lastPosX, 0, -1);
		notFound = true;
		realColor[0] = -1;
		leiserRotate(1, 1);
		thread *p = new thread(realOpenCv);
		cout << "leiser for enter state are : " << ardu->leiserInt[0] << "   " << ardu->leiserInt[1] << endl;
		while (ardu->leiserInt[0] == 0)
		{
			Sleep(20);
		}
		cout << "leiser for enter state are : " << ardu->leiserInt[0] << "   " << ardu->leiserInt[1] << endl;

		if (ardu->leiserInt[0] > 2200)
		{

			while (realColor[0] == -1 && ardu->leiserInt[1] < 1350)
			{
				motor->moveByDistance(5, 'y', -10);
				cout << "                  11" << endl;
			}
			Sleep(1000);
			while (realColor[0] == -1 && ardu->leiserInt[0] > 1700)
			{
				motor->moveByDistance(5, 'x', -10);
				cout << "                  12" << endl;
			}
			Sleep(1000);
			while (realColor[0] == -1 && ardu->leiserInt[1] > 200)
			{
				motor->moveByDistance(5, 'y', 10);
				cout << "                  13" << endl;
			}
			Sleep(1000);
			while (realColor[0] == -1 && ardu->leiserInt[0] > 1100)
			{
				motor->moveByDistance(5, 'x', -10);
				cout << "                  14" << endl;
			}
			Sleep(1000);
			while (realColor[0] == -1 && ardu->leiserInt[1] < 1350)
			{
				motor->moveByDistance(5, 'y', -10);
				cout << "                  15" << endl;
			}
		}
		else if (ardu->leiserInt[0] > 1770)
		{

			while (realColor[0] == -1 && ardu->leiserInt[0] > 1700)
			{
				motor->moveByDistance(5, 'x', -10);
				cout << "                    21" << endl;
			}
			Sleep(1000);
			while (realColor[0] == -1 && ardu->leiserInt[1] > 200)
			{
				motor->moveByDistance(5, 'y', 10);
				cout << "                    22" << endl;
			}
			Sleep(1000);
			while (realColor[0] == -1 && ardu->leiserInt[0] > 1100)
			{
				motor->moveByDistance(5, 'x', -10);
				cout << "                    23" << endl;
			}
			Sleep(1000);
			while (realColor[0] == -1 && ardu->leiserInt[1] < 1350)
			{
				motor->moveByDistance(5, 'y', -10);
				cout << "                    24" << endl;
			}
		}
		else if (ardu->leiserInt[0] > 1650)
		{
			while (realColor[0] == -1 && ardu->leiserInt[1] > 200)
			{
				motor->moveByDistance(5, 'y', 10);
				cout << "                    31" << endl;
			}
			Sleep(300);
			while (realColor[0] == -1 && ardu->leiserInt[0] > 1100)
			{
				motor->moveByDistance(5, 'x', -10);
				cout << "                    32" << endl;
			}
			Sleep(300);
			while (realColor[0] == -1 && ardu->leiserInt[1] < 1350)
			{
				motor->moveByDistance(5, 'y', -10);
				cout << "                    33" << endl;
			}
		}
		else if (ardu->leiserInt[0] > 1100)
		{
			while (realColor[0] == -1 && ardu->leiserInt[0] > 1100)
			{
				motor->moveByDistance(5, 'x', -10);
				cout << "                    41" << endl;
			}
			Sleep(300);
			while (realColor[0] == -1 && ardu->leiserInt[1] < 1350)
			{
				motor->moveByDistance(5, 'y', -10);
				cout << "                    42" << endl;

			}
		}
		else
		{
			while (realColor[0] == -1 && ardu->leiserInt[1] < 1350)
			{
				motor->moveByDistance(5, 'y', -10);
				cout << "                    51" << endl;

			}
		}
		/*while (realColor[0] == -1 && ardu->leiserInt[1] < 1400)
		{
		motor->moveByDistance(5, 'y', -6);
		}*/
		motor->stop();
		int id = -1;
		if (realColor[0] == -1)
		{
			cout << "not found" << endl;
		}
		else {
			for (int i = 0; i < 5; i++)
			{
				if (realColor[0] == places[i]->color)
				{
					id = i;
					break;
				}
			}
		}
		while (!places[id]->complete)
		{
			Sleep(500);
		}
	}
}
void findObject ( Mat objectP, int minHessian, Scalar color );

int main() 
{
	places[0] = new place(2650, 120, 3370, -1, -1, 0);
	places[1] = new place(2650, 450, 3370, -1, -1, 0);
	places[2] = new place(2650, 800, 3370, -1, -1, 0);
	places[3] = new place(2650, 1200, 3370, -1, -1, 0);
	places[4] = new place(2650, 1430, 3370, -1, -1, 0);
	/*
	places[0] = new place(2650, 120, 3370, 1, 1, 0);
	places[1] = new place(2650, 450, 3370, 6, 2, 0);
	places[2] = new place(2650, 800, 3370, 3, 0, 0);
	places[3] = new place(2650, 1200, 3370, 7, 6, 0);
	places[4] = new place(2650, 1530, 3370, 5, 4, 0);*/
	realColor.push_back(-1);
	//thread t(arduRead);

	if (dynam->dxl_initialize(5, 1) == 0)
	{
		dynamixell = false;
		cout << "error while open dynamixel port..." << endl;
	}
	//testAll	


			
	//VideoCapture cap(0);
	//cap >> sceneP;
	
/*	Mat Gray(sceneP.size(), CV_8U);
	cvtColor(sceneP, Gray, CV_BGR2GRAY);
	Mat Bin(Gray.size(),Gray.type());
	threshold(Gray, Bin, 100, 1000, cv::THRESH_BINARY);

	sceneP = Bin;*/
	//imshow("Binary", Bin);
	//VideoCapture cap(1);
	//cap >> sceneP;
	//cvtColor(sceneP, sceneP, COLOR_BGR2GRAY);
	//IplImage m = (IplImage)sceneP;
	//cvThreshold(&m, &m, 100, 255, CV_THRESH_BINARY);
	//Sleep(2000);
	Mat find = imread("C:\\Users\\Erfan\\Desktop\\Patt\\1.jpg");
	/*Mat gray(find.size(), CV_8U);
	cvtColor(find, gray, CV_BGR2GRAY);
	Mat bin(gray.size(), gray.type());
	threshold(gray, bin, 100, 255, cv::THRESH_BINARY);
	find = bin;*/
	//Mat label = imread("label1.jpg");
	//Mat label2 = imread("label2.jpg");
	outImg = sceneP;


	if(find.empty() && sceneP.empty())
	{
		cout << "Could not load the image!" << endl;		
		exit(0);
	}
	else
	{
		cout << "Images loaded succesfully" << endl;
	}

	//findObject( label, 2500, Scalar(255,0,0) );
	//findObject( label2, 1500, Scalar(0,255,0) );{
	findObject(find, 1500, Scalar(0, 0, 255));
			

	namedWindow("Match");
	imshow("Match",outImg);


	waitKey(0);

	return 1;
}

void findObject( Mat objectP, int minHessian, Scalar color )
{
	//vector of keypoints	
	vector<cv::KeyPoint> keypointsO;
	vector<cv::KeyPoint> keypointsS;
	cout << "Looking for object...\n";
	//Start the timer
	double duration;
	duration = static_cast<double>(cv::getTickCount());



	SurfFeatureDetector surf(minHessian);
	surf.detect(objectP,keypointsO);
	surf.detect(sceneP,keypointsS);


	//-- Step 2: Calculate descriptors (feature vectors)
  SurfDescriptorExtractor extractor;

  Mat descriptors_object, descriptors_scene;

  extractor.compute( objectP, keypointsO, descriptors_object );
  extractor.compute( sceneP, keypointsS, descriptors_scene );
  
 

  //-- Step 3: Matching descriptor vectors using FLANN matcher
  FlannBasedMatcher matcher;  
  //BFMatcher matcher(NORM_L1);
  std::vector< DMatch > matches;
  matcher.match( descriptors_object, descriptors_scene, matches);

  double max_dist = 0; double min_dist = 150;
  double dist;

  //Quick calculation of min and max distances between keypoints
  for(int i=0; i<descriptors_object.rows; i++)
  {
	dist = matches[i].distance;
	if( dist < min_dist ) min_dist = dist;
	if( dist > max_dist ) max_dist = dist;
  }

 /* cout << "-- Max Dist: " << max_dist << endl;
  cout << "-- Min Dist: " << min_dist << endl;*/

  vector< DMatch > good_matches;

  for(int i = 0; i < descriptors_object.rows; i++)
  {
	  if( matches[i].distance < 3*min_dist) 
		  good_matches.push_back( matches[i] );
  }

	//drawMatches(objectP,keypointsO,sceneP,keypointsS,matches,outImg,Scalar::all(-1), Scalar::all(-1),vector<char>(),DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	/*drawKeypoints(objectP,keypointsO,objectP,Scalar(0,0,255),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	namedWindow("SURF");
	imshow("SURF",objectP);*/

	//-- Localize the object

  if( good_matches.size() >=8 && good_matches.size() <= 30)
  {
	cout << "OBJECT FOUND!" << endl;
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;
	
	for( unsigned int i = 0; i < good_matches.size(); i++ )
	{
		//-- Get the keypoints from the good matches
		obj.push_back( keypointsO[ good_matches[i].queryIdx ].pt );
	    scene.push_back( keypointsS[ good_matches[i].trainIdx ].pt );
	}
	
	Mat H = findHomography( obj, scene, CV_LMEDS);

  

	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( objectP.cols, 0 );
	obj_corners[2] = cvPoint( objectP.cols, objectP.rows ); obj_corners[3] = cvPoint( 0, objectP.rows );
	std::vector<Point2f> scene_corners(4);

	perspectiveTransform( obj_corners, scene_corners, H);
	
	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
  
	line( outImg, scene_corners[0] , scene_corners[1], Scalar(255,255,0), 2 ); //TOP line
	line( outImg, scene_corners[1] , scene_corners[2], color, 2 );
	line( outImg, scene_corners[2] , scene_corners[3], color, 2 );
	line( outImg, scene_corners[3] , scene_corners[0] , color, 2 );
  }
  else cout << "OBJECT NOT FOUND!" << endl;
	
	duration = static_cast<double>(cv::getTickCount())-duration;
	duration = (duration/cv::getTickFrequency()) * 1000;
	
	
	 
	//cout <<  << endl;
	cout << "Good matches found: " << good_matches.size() << endl;
	cout << "Algorithm duration: " << duration << endl << "--------------------------------------" << endl;


	// drawing the results
	namedWindow("matches");
	Mat img_matches;
	drawMatches(objectP, keypointsO, sceneP, keypointsS, good_matches, img_matches);
	imshow("matches", img_matches);
	waitKey(100);

}