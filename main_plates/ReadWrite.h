#pragma once

#include <windows.h>
#include <stdio.h>


class ReadWrite
{
public:
	ReadWrite(int, float);
	ReadWrite();
	~ReadWrite();
	int dxl_hal_open(int devIndex, float baudrate);
	void dxl_hal_close();
	void dxl_hal_clear();
	int dxl_hal_tx(unsigned char *pPacket, int numPacket);
	int dxl_hal_rx(unsigned char *pPacket, int numPacket);
	void dxl_hal_set_timeout(int NumRcvByte);
	int dxl_hal_timeout();

#define LATENCY_TIME		(16) //ms	(USB2Serial Latency timer)
#define IN_TRASFER_SIZE		(512) //unsigned char

	HANDLE ghSerial_Handle = INVALID_HANDLE_VALUE; // Serial port handle
	float gfByteTransTime = 0.0f;
	float gfRcvWaitTime = 0.0f;
	LARGE_INTEGER gStartTime;
};

