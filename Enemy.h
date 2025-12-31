#pragma once
#include "Character.h"

class Map;
class Player;

class Enemy : public BaseCharacter {
public:
	virtual void update(Map* map, Player* player) = 0;
	virtual char sign() const = 0;
	virtual ~Enemy(){}
};