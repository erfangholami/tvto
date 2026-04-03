#include "motors.h"
#include <iostream>
#include <math.h>

# define M_PI          3.141592653589793238462643383279502884L /* pi */

using namespace std;

//open port
motors::motors(int com1, int com2) {
	front = new ReadWrite(com1, 38400);
	back = new ReadWrite(com2, 38400);
	setAcceleration(5, front);
	setAcceleration(5, back);
}
void motors::setSpeed(int speed, bool isFirst, ReadWrite *port) {
	unsigned char *sendd = new unsigned char[3];
	sendd[0] = 0;
	if (isFirst)
		sendd[1] = 0x31;
	else
		sendd[1] = 0x32;
	sendd[2] = speed;
	port->dxl_hal_tx(sendd, 3);
}
void motors::setAcceleration(int acc, ReadWrite *port) {
	unsigned char *sendd = new unsigned char[3];
	sendd[0] = 0;
	sendd[1] = 0x33;
	sendd[2] = acc;
	port->dxl_hal_tx(sendd, 3);
}
void motors::resetEncoder(ReadWrite *port) {
	unsigned char *sendd = new unsigned char[2];
	sendd[0] = 0;
	sendd[1] = 0x35;
	port->dxl_hal_tx(sendd, 2);
}
int motors::getAcceleration(ReadWrite *port) {
	unsigned char *sendd = new unsigned char[2];
	sendd[0] = 0;
	sendd[1] = 0x2A;
	port->dxl_hal_tx(sendd, 2);
	Sleep(20);
	unsigned char *readd = new unsigned char[1];
	port->dxl_hal_rx(readd, 1);

	return readd[0];
}
int motors::getEncoder(bool isFirst, ReadWrite *port) {

	unsigned char *sendd = new unsigned char[2];
	sendd[0] = 0x00;
	if (isFirst)
		sendd[1] = 0x23;
	else
		sendd[1] = 0x24;
	port->dxl_hal_tx(sendd, 2);
	Sleep(25);
	unsigned char *readd = new unsigned char[4];

	port->dxl_hal_rx(&readd[0], 4);
	int x = (readd[0] << 24) + (readd[1] << 16) + (readd[2] << 8) + readd[3];
	return ((int)x);
}
void motors::moveByDistance(int cm, char component, int speed) {
	int encoder = 0;
	resetEncoder(front);
	if (component != 'w')
		encoder = cm * 998 / 35;
	else
		encoder = cm;
	if (component == 'x')
	{
		x = speed;
		y = 0;
		w = 0;
	}
	else if (component == 'y')
	{
		x = 0;
		y = speed;
		w = 0;
	}
	else
	{
		x = 0;
		y = 0;
		w = speed;
	}
	bool isFirst = true;
	move(false);
	if (speeds[1] != 128) {
		if (speeds[1] > 128)
			encoder = -encoder;
	}
	else {
		if (speeds[0] > 128)
			encoder = -encoder;
		isFirst = false;
	}
	resetEncoder(front);
	while (pulseCounter(encoder, isFirst, front))
		move(true);
	stop();
}
void motors::move(bool move) {
	float Lx = 0.165;
	float Ly = 0.19;

	speeds[0] = (int)(x - y - (w *(Lx + Ly) * M_PI / 180));
	speeds[1] = (int)(x + y + (w *(Lx + Ly) * M_PI / 180));
	speeds[2] = (int)(x + y - (w *(Lx + Ly) * M_PI / 180));
	speeds[3] = (int)(x - y + (w *(Lx + Ly) * M_PI / 180));
	for (int i = 0; i < 4; i++)
		if (speeds[i] > 127)
			speeds[i] = 127;
		else if (speeds[i] < -128)
			speeds[i] = -128;

	speeds[0] += 128;
	speeds[1] += 128;
	speeds[2] += 128;
	speeds[3] += 128;
	if (move) {
		setSpeed(-speeds[0], false, front);
		setSpeed(-speeds[1], true, front);
		setSpeed(-speeds[2], false, back);
		setSpeed(-speeds[3], true, back);
	}
}
void motors::stop() {
	x = 0;
	y = 0;
	w = 0;

	setSpeed(128, false, front);
	setSpeed(128, true, front);
	setSpeed(128, false, back);
	setSpeed(128, true, back);
}
motors::~motors()
{
}
bool motors::pulseCounter( int counter, bool isFirst, ReadWrite *port) {
	if (counter > 0)
		if (counter > getEncoder(isFirst, port))
			return true;
		else
			return false;
	else
		if (counter < getEncoder(isFirst, port))
			return true;
		else
			return false;
}
void motors::multiMove(int X, int Y, int W, int Encoder)
{
	x = X;
	y = Y;
	w = W;
	int encoder = Encoder;
	resetEncoder(front);
		
	bool isFirst = true;
	move(false);
	if (speeds[1] != 128) {
		if (speeds[1] > 128)
			encoder = -encoder;
	}
	else {
		if (speeds[0] > 128)
			encoder = -encoder;
		isFirst = false;
	}
	resetEncoder(front);
	while (pulseCounter(encoder, isFirst, front))
		move(true);
	stop();
}