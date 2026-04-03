#include "arduino.h"

arduino::arduino(int comm, int baud)
{
	ard = new ReadWrite(comm, baud);
	sharpNum[0] = 0;
	sharpNum[1] = 0;
	led[0] = 0;
	led[1] = 0;
	Send[0] = 0;
	Send[1] = 0;
	Send[2] = 0;
	Send[3] = 0;

	leiserr[0] = 0;
	leiserr[1] = 0;
	error[0] = 0;
	error[1] = 0;
	ir[1] = 0;
	ir[0] = 0;
	for (int i = 0; i < 8; i++)
		sharp[i] = 0;
	leiserInt[0] = 0;
	leiserInt[1] = 0;


}
void arduino::read()
{
	BYTE readd[12];
	BYTE a = 0;
	ard->dxl_hal_rx(&readd[0], 1);
	if (readd[0] == 255) {
		ard->dxl_hal_rx(&readd[1], 1);
		if (readd[1] == 255) {
			ard->dxl_hal_rx(&readd[2], 10);
			int sum = 0;
			for (int i = 2; i < 11; i++)
				sum += readd[i];
			if ((sum & 255) == readd[11])
				for (int i = 2; i < 11; i++)
					Read[i - 2] = readd[i];
			ir[0] = Read[8] & 1;
			ir[1] = (Read[8] & 2) >> 1;
			sharpNum[0] = (Read[8] >> 2) & 7;
			sharpNum[1] = (Read[8] >> 5) & 7;
			sharp[(Read[8] >> 2) & 7] = Read[0] + (Read[1] << 8);
			sharp[(Read[8] >> 5) & 7] = Read[2] + (Read[3] << 8);
			//cout << sharp[(Read[8] >> 2) & 7] << "   " << sharp[(Read[8] >> 5) & 7] << endl;
			//cout << (int)Read[14] << endl;
			//for (int i = 0; i <  8; i++)
			//cout << sharp[i] << endl;
			//cout << endl;
			//cout << sharpNum[0] << "  "<< sharpNum[1] << endl;
			if (Read[4] == 0 && Read[5] == 0) {
				leiserr[0] = false;
				leiserInt[0] = 0;
			}
			else if (Read[5] != 0xfe)
			{
				leiserr[0] = true;
				if ((Read[4] + (Read[5] << 8)) < 4000)
					leiserInt[0] = Read[4] + (Read[5] << 8);
				error[0] = false;
			}
			else {
				if (!error[0])
					cout << "Error on first leiser ..." << endl;
				error[0] = true;
			}
			if (Read[6] == 0 && Read[7] == 0) {
				leiserr[1] = false;
				leiserInt[1] = 0;
			}
			else if (Read[7] != 0xfe)
			{
				leiserr[1] = true;
				if ((Read[6] + (Read[7] << 8)) < 4000)
					leiserInt[1] = Read[6] + (Read[7] << 8);
				error[1] = false;
			}
			else {
				if (!error[1])
					cout << "Error on second leiser ..." << endl;
				error[1] = true;
			}
			ard->dxl_hal_clear();

			//cout << leiserInt[0] << "   " << leiserInt[1] << endl;
		}
	}
}
void arduino::send()
{
	Send[0] = 0xFF;
	Send[3] = checksum();
	ard->dxl_hal_tx(Send, 4);
}
void arduino::leiser(bool on, int num) {
	if (on) {
		while ((leiserInt[num] == 0 && error[num] == 0) | error[num] == 1)
		{
			//cout << "waiting for leiser" << leiserInt[num] << endl;
			Send[1] = Send[1] | (int)pow(2, num);
			leiserr[num] = true;
			send();
			Sleep(1);
		}
	}
	else
	{
		while (leiserInt[num] != 0)
		{
			//cout << "turn off leiser" << leiserInt[num] << endl;
			Send[1] = Send[1] & (~(int)pow(2, num));
			leiserr[num] = false;
			send();
			Sleep(1);
		}
	}
}
void arduino::LED(int num, int color)
{
	if (num == 0)
	{
		Send[1] = Send[1] & 227;
		Send[1] = Send[1] | (color << 2);
	}
	if (num == 1)
	{
		Send[1] = Send[1] & 31;
		Send[1] = Send[1] | (color << 5);
	}
	send();
	send();
	send();
	send();
	send();
	send();
	send();
	send();
	send();
	send();
}
void arduino::setSharp(int setNum) {
	Send[2] = 0;
	if (setNum == 0)
		Send[2] = 8;
	else if (setNum == 1)
		Send[2] = 26;
	else if (setNum == 2)
		Send[2] = 44;
	else if (setNum == 3)
		Send[2] = 62;
	send();
	send();
	send();
	send();
	send();
}
arduino::~arduino()
{
}
int arduino::checksum()
{
	return (Send[1] + Send[2]) & 255;
}