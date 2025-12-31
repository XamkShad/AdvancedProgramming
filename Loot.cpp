#include "Loot.h"

void Loot::on_collect(Player* player) {
	if (!collected) {
		player->add_score(score_value);
		collected = true;
	}
}