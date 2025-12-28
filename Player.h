#pragma once
#include "Character.h"

class Map;

class Player : public BaseCharacter {
private:
	int max_oxygen;
	int current_oxygen;
	int max_battery;
	int current_battery;
	int max_health;
	int current_health;
	int max_lives;
	int current_lives;

public:
	Player();

	void set_position(int x, int y);
	void handle_input(char input, Map* map);
	void update() override;

	int oxygen() const;
	int battery() const;
	int health() const;
	int lives() const;
};