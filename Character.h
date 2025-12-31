#include "Map.h"
#pragma once

class BaseCharacter {
protected:
	int pos_x;
	int pos_y;

	int max_health = 100;
	int current_health;

	int damage = 0;
public:
	BaseCharacter();
	virtual ~BaseCharacter();

	virtual void update(Map* map, Player* player) = 0;
	virtual void move(int delta_x, int delta_y);
	virtual void take_damage(int dmg);

	int x() const;
	int y() const;
};