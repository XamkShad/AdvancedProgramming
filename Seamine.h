#pragma once
#include "Enemy.h"

class Seamine : public Enemy{
public:
	Seamine(int x, int y);

	void update(Map* map, Player* player) override;
	char sign() const override { return 'S'; }
};

