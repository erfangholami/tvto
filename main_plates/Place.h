#pragma once
class place
{
public:
	place(int posX , int posY , int posW , int color , bool direction);
	~place();

	int color;

	int positionX, positionY , positionW;
	bool direction, complete;
};

