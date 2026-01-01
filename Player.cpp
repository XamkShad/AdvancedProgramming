#include "Player.h"
#include "Map.h"

// Construct player with max and default values. //
Player::Player() :
	max_oxygen(100),
	current_oxygen(100),
	max_battery(100),
	current_battery(100),
	max_health(100),
	current_health(100),
	max_lives(3),
	current_lives(3),
	current_score(0),
	current_items(0){ }

// Sets player position. EZ. //
void Player::set_position(int x, int y) {
	pos_x = x;
	pos_y = y;
}

// Handle input, which is called from the Game.cpp input method. //
void Player::handle_input(char input, Map* map) {
	int dx = 0, dy = 0;

	// Movement for WASD, illumination for G. // 
	switch (input) {
	case 'w': dy = -1; break;
	case 's': dy = 1; break;
	case 'a': dx = -1; break;
	case 'd': dx = 1; break;
	case 'g': 
		current_battery -= 5;
		map->reveal_tiles(this, 2);
		update(map, this);
		break;
	}

	// If G is picked, lets not update twice. //
	if (input == 'g') return;

	// This is if WASD was chosen. //
	if (map->is_walkable(pos_y + dy, pos_x + dx)) {
		current_oxygen -= 1;
		move(dx, dy);
		update(map, this);		
	}
}

// Update the player state, health, oxygen, etc... //
void Player::update(Map* map, Player* player){
	if (current_oxygen <= 0) {
		current_health -= 10;	
	}
	
	// This is just supposed to be a "clamp". //
	if (current_oxygen <= 0) current_oxygen = 0;
	if (current_health <= 0) current_health = 0;	
}

// For when the player dies and loses a life. //
void Player::reset_minus_life(Map* map) {
	current_lives--;

	current_oxygen = 100;
	current_battery = 100;
	current_health = 100;	

	current_score -= 25;

	if (current_score <= 0) {
		current_score = 0;
	}

	set_position(map->spawn.player_x, map->spawn.player_y);
}

// This is when the game reset/reload is called. Total Recall. //
void Player::reset() {
	current_lives = 3;
	current_oxygen = 100;
	current_battery = 100;
	current_health = 100;

	current_score = 0;
	current_items = 0;
}

// Take damage. Simple come, simple go. //
void Player::take_damage(int dmg) {
	current_health -= dmg;
}

void Player::item_collected() {
	current_items++;
}

void Player::add_score(int amount) {
	current_score += amount;
}

// Public getters for the otherwise private values. //
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

int Player::score() const {
	return current_score;
}

int Player::items() const {
	return current_items;
}