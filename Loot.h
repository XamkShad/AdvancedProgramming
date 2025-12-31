#pragma once
#include "Item.h"

class Loot : public Item
{
	int score_value;
	
public:
	Loot(int x, int y, int value = 10) : Item(x, y), score_value(value) {}

	void on_collect(Player* player) override;

	char sign() const override { return 'L'; }
};

