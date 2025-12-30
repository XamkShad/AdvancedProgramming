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
	case 'g': 
		current_battery -= 5;
		map->reveal_tiles(this, 2);
		update(map);
		break;
	}

	if (map->is_walkable(pos_y + dy, pos_x + dx)) {
		current_oxygen -= 10;
		move(dx, dy);
		update(map);		
	}
}

void Player::update(Map* map){
	if (current_oxygen <= 0) {
		current_health -= 10;	
	}
	
	if (current_oxygen <= 0) current_oxygen = 0;
	if (current_health <= 0) current_health = 0;	
}

void Player::reset_minus_life(Map* map) {
	current_lives--;

	current_oxygen = 100;
	current_battery = 100;
	current_health = 100;

	set_position(map->spawn.player_x, map->spawn.player_y);
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