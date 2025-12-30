#include "Player.h"
#include "Map.h"

Player::Player() :
	max_oxygen(100),
	current_oxygen(100),
	max_battery(100),
	current_battery(100),
	max_health(100),
	current_health(100),
	max_lives(3),
	current_lives(3) { }

void Player::set_position(int x, int y) {
	pos_x = x;
	pos_y = y;
}

void Player::handle_input(char input, Map* map) {
	int dx = 0, dy = 0;

	switch (input) {
	case 'w': dy = -1; break;
	case 's': dy = 1; break;
	case 'a': dx = -1; break;
	case 'd': dx = 1; break;
	}

	if (map->is_walkable(pos_y + dy, pos_x + dx)) {
		move(dx, dy);
		update(map);		
	}
}

void Player::update(Map* map){
	current_oxygen--;
	map->update(this);
}

int Player::oxygen() const {
	return current_oxygen;
}

int Player::battery() const {
	return current_battery;
}

int Player::health() const {
	return current_health;
}

int Player::lives() const {
	return current_lives;
}