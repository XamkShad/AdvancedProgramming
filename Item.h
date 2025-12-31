#pragma once
#include "Player.h"

class Item
{
protected:
	int pos_x;
	int pos_y;
	bool collected = false;

public:
	Item(int x, int y) : pos_x(x), pos_y(y){}
	virtual ~Item() {}

	virtual void on_collect(Player* player) = 0;

	bool is_collected() const { return collected; }
	int x() const { return pos_x; }
	int y() const { return pos_y; }

	virtual char sign() const = 0;
};

