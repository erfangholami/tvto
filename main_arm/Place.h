#pragma once
class place
{
public:
	place(int posX, int posY, int posW, int code, int color, bool direction);
	~place();

	int color;
	int code;
	int x, y, area;
	int positionX, positionY, positionW;
	bool direction, complete, found;
};

