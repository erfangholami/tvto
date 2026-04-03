#include "place.h"

place::place(int posX, int posY, int posW, int Code, int col, bool direct)
{
	complete = false;
	found = false;
	positionX = posX;
	positionY = posY;
	positionW = posW;
	code = Code;
	color = col;
	direction = direct;
	x = 0;
	y = 0;
	area = 0;
}
place::~place()
{
}
