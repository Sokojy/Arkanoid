#pragma once

class Brick
{
public:
	bool active;
	short row, col, hit;
	float width, height;

	Brick(short r, short c, short h, short width, short height) :
		row(r), col(c), hit(h), width(width), height(height), active(false) {};

	Brick() : 
		row(), col(), hit(1), width(20), height(20), active(false) {};
};