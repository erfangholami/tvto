#pragma once
#include "ReadWrite.h"

class motors
{
public:
	motors(int, int);
	~motors();
	void setSpeed(int, bool, ReadWrite *);
	void setAcceleration(int, ReadWrite *);
	void resetEncoder(ReadWrite *);
	int getAcceleration(ReadWrite *);
	int getEncoder(bool, ReadWrite *);
	int getSpeed(bool, ReadWrite *);
	void moveByDistance(int cm, char component, int speed);
	void move(bool move);
	void MOVE(int, char, int);
	void stop();
	bool pulseCounter(int counter, bool, ReadWrite *);
	void multiMove(int x, int y, int w, int encoder);
	int speeds[4];
	float x, y, w;
	ReadWrite *front, *back;
};
