#pragma once
#include "ReadWrite.h"
#include "dynamixel.h"
#include <thread>
#include <cmath>
#include <iostream>

using namespace std;

class arduino
{
public:

	arduino(int comm, int baud);
	void read();
	void send();
	void leiser(bool, int);
	void LED(int , int);
	int checksum();
	void setSharp(int sharpNum);
	~arduino();
	int sharpNum[2] = { 0 };
	int x;
	int sharpL, sharpR;
	int led[2] = { 0 };
	BYTE Read[15];
	BYTE Send[4] = { 0 };
	bool leiserr[2] = { 0 };
	bool error[2] = { 0 };
	bool ir[2] = { 0 };
	int sharp[8] = { 0 };
	int leiserInt[2] = { 0 };
	BYTE a = 0;
	ReadWrite *ard;
	dynamixel *dynam;


};

