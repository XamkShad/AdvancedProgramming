#pragma once
#include <string>

class Enemy;
class Player;
class Item;

// Player spawn info for the levels. //
struct SpawnInfo {
	int player_x;
	int player_y;
};

class Map {
private:
	char** original_map; // The original, loaded map which remains untouched. //
	char** game_map; // The game map, which can be modified. //
	char** fog_of_war_map; // Fog of war map, making everything hidden. //
	int width; // Map width, EZ. //
	int height; // And map height. //

	Enemy** enemies; // Enemy array. //
	int enemy_count; // Enemy count, read from the original_map. //

	Item** items; // Item array. //
	int item_count; // Item count, read from the original_map. //

	int current_items; // Current items collected. //
	int all_map_items; // Remaining items in the map, undiscovered. //
public:
	SpawnInfo spawn;

	Map(const std::string& filepath);
	~Map();

	bool is_walkable(int y, int x) const;
	char char_at(int y, int x) const;
	void update(Player* player);
	void reveal_tiles(Player* player, int reveal_radius) const;
	void render(Player* player) const;

	void reset(Player* player);

	int collected_items() const;	
	int hidden_items() const;
};