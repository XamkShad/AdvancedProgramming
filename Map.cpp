#include "Map.h"
#include "Enemy.h"
#include "Player.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

Map::Map(const std::string& filepath) : original_map(nullptr), game_map(nullptr), fog_of_war_map(nullptr), width(0), height(0){
	
	std::ifstream file(filepath);

	if (!file.is_open()) {
		throw std::runtime_error("Map load failed!");
	}

	std::string line;
	while (getline(file, line)) {
		if (height == 0) width = line.size();
		height++;
	}

	file.clear();
	file.seekg(0);

	original_map = new char* [height];
	game_map = new char* [height];
	fog_of_war_map = new char* [height];

	for (int y = 0; y < height; y++) {
		original_map[y] = new char[width];
		game_map[y] = new char[width];
		fog_of_war_map[y] = new char[width];
	}

	int y = 0;
	while (getline(file, line)) {
		for (int x = 0; x < width; x++) {
			original_map[y][x] = line[x];
			game_map[y][x] = original_map[y][x];

			if (line[x] == 'P') {
				game_map[y][x] = '~';
			}
			else if (line[x] != '~') {				
				game_map[y][x] = 'X';
			}

			fog_of_war_map[y][x] = '#';
		}

		y++;
	}

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (original_map[y][x] == 'P') {
				spawn.player_x = x;
				spawn.player_y = y;
			}
		}
	}
}

Map::~Map() {
	for (int i = 0; i < height; i++) delete[] original_map[i];
	delete[] original_map;

	for (int i = 0; i < height; i++) delete[] game_map[i];
	delete[] game_map;

	for (int i = 0; i < height; i++) delete[] fog_of_war_map[i];
	delete[] fog_of_war_map;

	for (int i = 0; i < enemy_count; i++) delete[] enemies[i];
	delete[] enemies;
}

bool Map::is_walkable(int y, int x) const {
	if (y < 0 || y >= height || x < 0 || x >= width) return false;

	return original_map[y][x] != 'X';
}

void Map::update(Player* player) {
	for (int i = 0; i < enemy_count; i++) {
		enemies[i]->update(this, player);
	}

	int player_x = player->x();
	int player_y = player->y();

	fog_of_war_map[player_y][player_x] = game_map[player_y][player_x];
}

void Map::reveal_tiles(Player* player, int reveal_radius) const{
	int player_x = player->x();
	int player_y = player->y();

	for (int dy = -reveal_radius; dy <= reveal_radius; dy++) {
		for (int dx = -reveal_radius; dx <= reveal_radius; dx++) {
			int nx = player_x + dx;
			int ny = player_y + dy;

			if (nx < 0 || ny < 0 || nx >= width || ny >= height) continue;

			fog_of_war_map[ny][nx] = game_map[ny][nx];
		}
	}
}

void Map::render(Player* player) const {
	int player_row = player->x();
	int player_column = player->y();

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (x == player_row && y == player_column) std::cout << 'P';
			else {
				std::cout << fog_of_war_map[y][x];
			}
		}
		std::cout << "\n";
	}
}

char Map::char_at(int y, int x) const {
	return game_map[y][x];
}

int Map::collected_items() const {
	return 0;
}

int Map::hidden_items() const {
	return 0;
}

