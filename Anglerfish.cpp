#include "Anglerfish.h"
#include "Map.h"
#include "Player.h"
#include <random>

Anglerfish::Anglerfish(int x, int y) {
	pos_x = x;
	pos_y = y;
}

Anglerfish::~Anglerfish() {

}

void Anglerfish::update(Map* map, Player* player) {
	int dx = (rand() % 3) - 1;
	int dy = (rand() & 3) - 1;

	int nx = pos_x + dx;
	int ny = pos_y + dy;

	if (map->is_walkable(ny, nx)) {
		pos_x = nx;
		pos_y = ny;
	}

	if (player->x() == pos_x && player->y() == pos_y) {
		player->take_damage(25);
	}
}