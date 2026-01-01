#include "Loot.h"

// Call player methods once they stand over me. //
void Loot::on_collect(Player* player) {
	if (!collected) {
		player->add_score(score_value);
		player->item_collected();
		collected = true;
	}
}