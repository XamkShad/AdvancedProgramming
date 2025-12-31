#pragma once
#include <string>

class Enemy;
class Player;

struct SpawnInfo {
	int player_x;
	int player_y;
};

class Map {
private:
	char** original_map;
	char** game_map;
	char** fog_of_war_map;
	int width;
	int height;

	Enemy** enemies;
	int enemy_count;

	int current_items;
	int all_map_items;
public:
	SpawnInfo spawn;

	Map(const std::string& filepath);
	~Map();

	bool is_walkable(int y, int x) const;
	char char_at(int y, int x) const;
	void update(Player* player);
	void reveal_tiles(Player* player, int reveal_radius) const;
	void render(Player* player) const;

	int collected_items() const;
	
	int hidden_items() const;
};