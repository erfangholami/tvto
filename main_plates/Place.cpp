#include "place.h"

place::place(int posX , int posY , int posW , int col , bool direct)
{
	complete = false;
	positionX = posX;
	positionY = posY;
	positionW = posW;
	color = col;
	direction = direct;
}
place::~place()
{
}
