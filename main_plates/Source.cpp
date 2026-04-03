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
#define mx64 2152  
#define mx106 2400
#define mx28_1 2963
#define mx28_2 1022
using namespace std;
motors *motor = new motors(8, 7);
opencv *open = new opencv();
dynamixel *dynam = new dynamixel();
arduino *ardu = new arduino(6,38400);
place *places[4];
int counter = 0;
int speed[8] = { 50 , 50 , 50 , 50 , 50 , 50 , 50 , 50 };
int acceleration[8] = { 5 , 5 , 5 , 5 , 5 , 5 , 5 , 5 };
int ID = 5;
bool dynamixell = true;
bool motorSet = true;
bool threadd = true;
bool gatee = false;
bool moveToObjective = false;
bool stopCentering = false;
bool errorCentering = false;
int color[4] = { 0 , 1 , 7 , 3 };
bool colorFound[4] = { 0 };
/*
void Move(int id, int pos)
{
	switch (id)
	{
	case 2 :
	{
		if (pos > FirstJoint)
		{
			do
			{
				dynam->dxl_write_word(2, 30, pos - 50);
			} while (abs(dynam->dxl_read_word(2, 36) - pos) > 50);
			dynam->dxl_write_word(2, 30, dynam->dxl_read_word(2, 36));
		}
		else
		{
			do
			{
				dynam->dxl_write_word(2, 30, pos + 50);
			} while (abs(dynam->dxl_read_word(2, 36) - pos) > 50);
			dynam->dxl_write_word(2, 30, dynam->dxl_read_word(2, 36));
		}

		break;
	}
	case 3 :
	{
		do
		{
			dynam->dxl_write_word(3, 30, pos - 50);
		} while (abs(dynam->dxl_read_word(3, 36) - pos) > 50);
		dynam->dxl_write_word(2, 30, dynam->dxl_read_word(3, 36));
		break;
	}
	case 4 :
	{
		if (pos > thirdJoint)
		{

		}
		else
		{

		}

		break;
	}
	}
}

*/
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
		dynam->move(position, 12 - num, 600, 5);
	} while (abs(dynam->dxl_read_word(12 - num, 36) - position) > 10);
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
	dynam->move(position, 12 - num, 600, 5);
	ardu->leiser(true, num);

}
void center(int j) {
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
	errorCentering = false;
	int breakk = 0;
	ardu->LED(0, 5);
	ardu->LED(1, 5);
	int i = 0;
	while (!stopCentering) {
		cout << open->y.x << "   " << open->y.y << endl;
		open->check(&j);
		while (!open->end);
		Sleep(50);
		if (open->y.x > 0) {
			int color = (int)open->y.x;
			motor->y = (310 - color);
			if (motor->y > 7)
				motor->y = 7;
			else if (motor->y < -7)
				motor->y = -7;
			else if (motor->y < 10 && motor->y > 0)
				motor->y = 5;
			else if (motor->y > -10 && motor->y < 0)
				motor->y = -5;
			if (color > 300 && color < 320) {
				motor->y = 0;
				motor->move(true);
				return;
			}
			motor->move(true);
		}
		else {
			breakk++;
			if (breakk > 3)
			{
				errorCentering = true;
				break;
			}
		}
	}
}
void centering(int col) {
	while (!stopCentering) {
		center(col);
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
				int mSpeed = 5;
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
		//cout << ardu->leiserInt[0] << endl;
		//Sleep(1);
		ardu->read();
	}
}
void moveByLeiser(bool x, int distance, int lei, int direction = 1 , bool duel = false)
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
		while (ardu->leiserr[lei] == 0 )
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
		if (abs(ardu->leiserInt[lei] - distance) < 5)
			counter++;
		if (x)
		{
			if (ardu->leiserInt[lei] - distance > 400)
				motor->x = 40 * direction;
			else if(ardu->leiserInt[lei] - distance < -400)
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
void greep(bool open) {
	int counterG = 0;
	if (open) {
		do
		{
			counterG++;
			dynam->move(mx28_1 - 2200, 7, 400, 5);
			dynam->move(mx28_2 + 2200, 8, 400, 5);
			if (counterG > 50)
				cout << "open greep :) ..." << endl;
		} while (abs(dynam->dxl_read_word(7, 36) - mx28_1 + 2200) > 50 ||
			abs(dynam->dxl_read_word(8, 36) - mx28_2 - 2200) > 50);
	}
	else {
		do
		{
			counterG++;
			dynam->move(mx28_1, 7, 400, 5);
			dynam->move(mx28_2, 8, 400, 5);
			if (counterG > 50)
				cout << "close greep :) ..." << endl;
		} while (abs(dynam->dxl_read_word(7, 36) - mx28_1) > 50 ||
			abs(dynam->dxl_read_word(8, 36) - mx28_2) > 50);

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
			if(leiNum < 110)
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
void detectPlaces()
{
	dynam->move(525, 5, 200, 5);
	dynam->move(75, 6, 200, 5);

	ardu->LED(0, 5);
	ardu->LED(1, 5);
	motor->moveByDistance(1650, 'w', -9000);
	ardu->LED(0, 0);
	ardu->LED(1, 0);
	leiserRotate(1, 1);

	moveByLeiser3(false, 1700, 1, -1);

	for (int i = 0; i < 4; i++) {
		if (!colorFound[i]) {
			open->check(&color[i]);
			if (open->maxArea > 3000) {
				places[0]->color = color[i];
				open->play(color[i]);
				cout << color[i];
				colorFound[i] = true;
				break;
			}
		}
	}
	ardu->LED(0, 5);
	ardu->LED(1, 5);
	motor->moveByDistance(50, 'y', 50);

	ardu->LED(0, 0);
	ardu->LED(1, 0);
	for (int i = 0; i < 4; i++) {
		if (!colorFound[i]) {
			open->check(&color[i]);
			if (open->maxArea > 3000) {
				places[1]->color = color[i];
				open->play(color[i]);
				cout << color[i];
				colorFound[i] = true;
				break;
			}
		}
	}

	ardu->LED(0, 5);
	ardu->LED(1, 5);
	motor->moveByDistance(50, 'y', 50);

	ardu->LED(0, 0);
	ardu->LED(1, 0);
	for (int i = 0; i < 4; i++) {
		if (!colorFound[i]) {
			open->check(&color[i]);
			if (open->maxArea > 3000) {
				places[2]->color = color[i];
				open->play(color[i]);
				cout << color[i];
				colorFound[i] = true;
				break;
			}
		}
	}

	for (int i = 0; i < 4; i++)
		if (!colorFound[i]) {
			places[3]->color = color[i];
			colorFound[i] = true;
			cout << color[i];
			break;
		}
	dynam->move(208, 11, 600, 5);
	dynam->move(512, 12, 600, 5);

	ardu->LED(0, 5);
	ardu->LED(1, 5);
	ardu->leiser(false, 0);
	ardu->leiser(false, 1);
	motor->moveByDistance(3385, 'w', 9000);

	leiserRotate(0, 1);
	moveByLeiser2(true, 350, 1, 1);

	ardu->LED(0, 0);
	ardu->LED(1, 0);
}
void start() {
	dynam->move(75, 6, 200, 5);
	dynam->move(525, 5, 200, 5);
	// 2 ,0   1 , 1
	gate();
	if (gatee) {
		mciSendString("close mp3", NULL, 0, NULL);
		mciSendString("open \"audio/\musicTime.wav\" type mpegvideo alias mp3", NULL, 0, NULL);
		mciSendString("play mp3", NULL, 0, NULL);
		GWEZ(120, 60, 'x', 1, 270, 70, 1);
		GWEZ(73, -60, 'y', 1, 830, 70, 0);
	}
	else {
		mciSendString("close mp3", NULL, 0, NULL);
		mciSendString("open \"audio/\musicTime.wav\" type mpegvideo alias mp3", NULL, 0, NULL);
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
void rotateArm(int mx1, int mx2 , bool led) {
	if (led) {
		ardu->LED(0, 3);
		ardu->LED(1, 3);
	}
	int counterArm = 0;
	int pos1 = mx106 + mx1 , ans1;
	int pos2 = mx64 + mx2, ans2;
	do
	{
		counterArm++;
		dynam->move(pos1, 10, 400, 5);
		dynam->move(pos2, 9, 300, 5);
		if (pos1 != 0)
			ans1 = ((abs(pos1) / pos1) - 1) / 2 * 65535;
		else if (dynam->dxl_read_word(10, 36) < 0)
			ans1 = -65535;
		else
			ans1 = 0;

		if (pos2 != 0)
			ans2 = ((abs(pos2) / pos2) - 1) / 2 * 65535;
		else if (dynam->dxl_read_word(9, 36) < 0)
			ans2 = -65535;
		else
			ans2 = 0;
		if (counterArm > 30)
			cout << "rotate Arm..." << endl;
	} while (abs(dynam->dxl_read_word(10, 36) - pos1 + ans1) > 50 ||
		abs(dynam->dxl_read_word(9, 36) - pos2 + ans2) > 50);

}
void referrence() {
	ardu->LED(0, 5);
	ardu->LED(1, 5);
	ardu->setSharp(1);
	Sleep(100);
	leiserRotate(1, 0);
	leiserRotate(2, 1);
	int directionX = 1, directionY = -1, direction = -1;
	int counterX = 0 , counterY = 0 , counter = 0;
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
	ardu->leiser(false , 0);
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
	rotateArm(-3220, -3200 , true);
	
	greep(1);
	Sleep(1000);
	greep(0);
	rotateArm(0, 0 , true);
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
	dynam->move(mx64 + 3220 , 9, 200, 5);
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
	rotateArm(-3200, 3200 , false);
	center(places[ans]->color);
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
	mciSendString("open \"audio/\objective found.wav\" type mpegvideo alias mp3", NULL, 0, NULL);
	mciSendString("play mp3", NULL, 0, NULL);

	//sharp();
	mciSendString("close mp3", NULL, 0, NULL);
	if (ans == 0)
		mciSendString("open \"audio/\bach.wav\" type mpegvideo alias mp3", NULL, 0, NULL);
	else if (ans == 1)
		mciSendString("open \"audio/\beet.wav\" type mpegvideo alias mp3", NULL, 0, NULL);
	else if (ans == 2)
		mciSendString("open \"audio/\mozr.wav\" type mpegvideo alias mp3", NULL, 0, NULL);
	else
		mciSendString("open \"audio/\mozr.wav\" type mpegvideo alias mp3", NULL, 0, NULL);
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
		rotateArm(-1400, -3200 ,true);
	else
		rotateArm(-1400, 3200, true);

	greep(false);
	mciSendString("close mp3", NULL, 0, NULL);
	mciSendString("open \"audio/\objective complete.wav\" type mpegvideo alias mp3", NULL, 0, NULL);
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
		mciSendString("open \"audio/\Aleksander Rybak � Fairytail.wav\" type mpegvideo alias mp3", NULL, 0, NULL);
		mciSendString("play mp3", NULL, 0, NULL);

	}
	else
	{
		mciSendString("close mp3", NULL, 0, NULL);
		mciSendString("open \"audio/\musicTime.wav\" type mpegvideo alias mp3", NULL, 0, NULL);
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
int main() {
	places[0] = new place(350, 1725, 3370, 3, 0);
	places[1] = new place(350, 1240, 3370, 0, 0);
	places[2] = new place(350, 745, 3370, 7, 0);
	places[3] = new place(350, 239, 3370, 1, 0);
	thread t(arduRead);
	if (dynam->dxl_initialize(5, 1) == 0) {

		dynamixell = false;
		cout << "error while open dynamixel port..." << endl;
	}
	testAll();
	//referrence();
	//checkDynamixel();
	/*char s;
	cout << "enter s to start..." << endl;
	while (true) {
		cin >> s;
		if (s == 's') {
			start();
			
			detectPlaces();
			for (; counter < 4; counter++)
				fazzz(faz());
			
			break;
		}
		else
			cout << "wrong input :|" << endl;
	}*/
	
	t.join();
}