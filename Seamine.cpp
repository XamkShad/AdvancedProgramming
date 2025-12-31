#include "Seamine.h"
#include "Map.h"
#include "Player.h"

Seamine::Seamine(int x, int y) {
	pos_x = x;
	pos_y = y;
}

Seamine::~Seamine() {

}

void Seamine::update(Map* map, Player* player) {
	if (player->x() == pos_x && player->y() == pos_y) {
		player->take_damage(100);
		alive = false;
	}
}