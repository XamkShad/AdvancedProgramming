#pragma once
#include "Character.h"

class Enemy : public BaseCharacter {
protected:
	bool alive = true;
public:
	virtual void update(Map* map, Player* player) = 0;
	virtual char sign() const = 0;
	virtual ~Enemy(){}
	bool is_alive() const { return alive; }
};