#include "Map.h"
#include "Enemy.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

Map::Map(const std::string& filepath) : tiles(nullptr), width(0), height(0){
	
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

	tiles = new char* [height];
	for (int y = 0; y < height; y++) {
		tiles[y] = new char[width];
	}

	int y = 0;
	while (getline(file, line)) {
		for (int x = 0; x < width; x++) {
			tiles[y][x] = line[x];

			if (line[x] == 'P') {
				spawn.player_x = x;
				spawn.player_y = y;	
			}
		}

		y++;
	}
}

Map::~Map() {
	for (int i = 0; i < height; i++) delete[] tiles[i];
	delete[] tiles;

	for (int i = 0; i < enemy_count; i++) delete[] enemies[i];
	delete[] enemies;
}

bool Map::is_walkable(int y, int x) const {
	if (y < 0 || y >= height || x < 0 || x >= width) return false;

	return tiles[y][x] != 'x';
}

void Map::update(Player* player) {
	for (int i = 0; i < enemy_count; i++) {
		enemies[i]->update(this, player);
	}
}

void Map::render() const {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			std::cout << tiles[y][x];
		}
		std::cout << "\n";
	}
}

