#include <iostream>
#include <string>

void game_over();

class Player() {
public:
	int max_health;
	int max_oxygen;
	int max_lives;

	int current_health;
	int current_oxygen;
	int current_lives;

	int pos_x;
	int pos_y;

	Player(int hp = 100, int ox = 100, int lv = 3) : max_health(hp), max_oxygen(ox), max_lives(lv) {
		reset();
	}

	~Player() {
		game_over();
	}

	void reset() {
		current_health = max_health;
		current_oxygen = max_oxygen;
		current_lives = max_lives;
	}

	void move(int delta_x, int delta_y) {
		pos_x += delta_x;
		pos_y += delta_y;
	}
};

Player global_player;