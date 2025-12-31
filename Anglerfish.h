#pragma once
#include "Enemy.h"

class Anglerfish : public Enemy {
public:
	Anglerfish(int x, int y);
	~Anglerfish();

	void update(Map* map, Player* player) override;
	char sign() const override { return 'A'; }
};

