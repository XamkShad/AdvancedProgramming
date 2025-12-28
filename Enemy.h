#pragma once
#include "Character.h"

class Map;
class Player;

class Enemy : BaseCharacter {
public:
	virtual void update(Map* map, Player* player) = 0;
	virtual ~Enemy(){}
};