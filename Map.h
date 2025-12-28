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
	char** tiles;
	int width;
	int height;

	Enemy** enemies;
	int enemy_count;

public:
	SpawnInfo spawn;

	Map(const std::string& filepath);
	~Map();

	bool is_walkable(int y, int x) const;
	char tile_at(int y, int x) const;
	void update(Player* player);
	void render() const;
};