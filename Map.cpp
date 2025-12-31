#include "Map.h"
#include "Enemy.h"
#include "Player.h"
#include "Anglerfish.h"
#include "Seamine.h"
#include "Loot.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// In the constructor, the level is loaded and read for player, enemy and item positions. //
Map::Map(const std::string& filepath) : original_map(nullptr), game_map(nullptr), fog_of_war_map(nullptr), width(0), height(0){

	// Open and read from the filepath. //
	std::ifstream file(filepath);

	// Well, shit. //
	if (!file.is_open()) {
		throw std::runtime_error("Map load failed!");
	}

	// Set the width to the line size, and increase height. //
	std::string line;
	while (getline(file, line)) {
		if (height == 0) width = line.size();
		height++;
	}

	// Clear the stream, and go back to the start (0). //
	file.clear();
	file.seekg(0);

	// Set the all 3 layers of the map to be the same size. //
	original_map = new char* [height];
	game_map = new char* [height];
	fog_of_war_map = new char* [height];

	for (int y = 0; y < height; y++) {
		original_map[y] = new char[width];
		game_map[y] = new char[width];
		fog_of_war_map[y] = new char[width];
	}

	// Here, the layers get separated. //
	int y = 0;
	while (getline(file, line)) {
		for (int x = 0; x < width; x++) {
			original_map[y][x] = line[x]; // Original map, is essentially the .map file as-is. //

			if (line[x] == 'X') {				
				game_map[y][x] = 'X';
			}
			else {
				game_map[y][x] = '~';
			}

			fog_of_war_map[y][x] = '#';
		}

		y++;
	}

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			char c = original_map[y][x];

			if (c == 'P') {
				spawn.player_x = x;
				spawn.player_y = y;
			}

			if (c == 'A' || c == 'S') {
				enemy_count++;
			}

			if (c == 'L') {
				item_count++;
			}
		}
	}

	enemies = new Enemy* [enemy_count];
	int enemy_index = 0;

	items = new Item* [item_count];
	int item_index = 0;
	
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			char c = original_map[y][x];

			switch (c) {
			case 'A':
				enemies[enemy_index++] = new Anglerfish(x, y);
				break;
			case 'S':
				enemies[enemy_index++] = new Seamine(x, y);
				break;		
			case 'L':
				items[item_index++] = new Loot(x, y, 50);
				break;
			}		
		}
	}
}

// Destroy and free everything from the memory in destructor. //
Map::~Map() {
	for (int i = 0; i < height; i++) delete[] original_map[i];
	delete[] original_map;

	for (int i = 0; i < height; i++) delete[] game_map[i];
	delete[] game_map;

	for (int i = 0; i < height; i++) delete[] fog_of_war_map[i];
	delete[] fog_of_war_map;

	for (int i = 0; i < enemy_count; i++) delete[] enemies[i];
	delete[] enemies;

	for (int i = 0; i < item_count; i++) delete[] items[i];
	delete[] items;
}

bool Map::is_walkable(int y, int x) const {
	if (y < 0 || y >= height || x < 0 || x >= width) return false;

	return original_map[y][x] != 'X';
}

void Map::update(Player* player) {
	for(int i = 0; i < enemy_count; i++){
		enemies[i]->update(this, player);
	}

	for (int i = 0; i < enemy_count;) {
		if (!enemies[i]->is_alive()) {
			delete enemies[i];

			for (int j = i; j < enemy_count - 1; j++) {
				enemies[j] = enemies[j + 1];
			}

			enemy_count--;
		}
		else {
			i++;
		}
	}

	for (int i = 0; i < item_count; i++) {
		if (!items[i]->is_collected() &&
			items[i]->x() == player->x() && items[i]->y() == player->y()) {
			items[i]->on_collect(player);
		}
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
	int player_x = player->x();
	int player_y = player->y();

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			
			if (x == player_x && y == player_y) {
				std::cout << 'P';
				continue;
			}

			for (int i = 0; i < item_count; i++) {
				if (!items[i]->is_collected() &&
					items[i]->x() == x && items[i]->y() == y) {
					std::cout << items[i]->sign();
					break;
				}
			}

			bool enemy_drawn = false;
			for (int i = 0; i < enemy_count; i++) {
				if (enemies[i]->x() == x && enemies[i]->y() == y) {
					if (fog_of_war_map[y][x] != '#') {
						std::cout << enemies[i]->sign();
					}
					else {
						std::cout << '#';
					}

					enemy_drawn = true;
					break;
				}
			}

			if (enemy_drawn) continue;

			std::cout << fog_of_war_map[y][x];
		}
		std::cout << "\n";
	}
}

char Map::char_at(int y, int x) const {
	return game_map[y][x];
}

void Map::reset(Player* player) {

	// Reset the game map, using the original map file as a source. //
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			char c = original_map[y][x];

			if (c == 'X') {
				game_map[y][x] = 'X';
			}
			else {
				game_map[y][x] = '~';
			}

			if (c == 'P') {
				spawn.player_x = x;
				spawn.player_y = y;
			}

			fog_of_war_map[y][x] = '#'; // Reset fog of war. //
		}
	}

	// Destroy existing enemies. //
	for (int i = 0; i < enemy_count; i++) {
		delete enemies[i];
	}
	enemy_count = 0;

	// Now respawn enemies from original map. //
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {

			switch (original_map[y][x]) {
			case 'S':
				enemies[enemy_count++] = new Seamine(x, y);
				game_map[y][x] = '~';
				break;

			case 'A':
				enemies[enemy_count++] = new Anglerfish(x, y);
				game_map[y][x] = '~';
				break;
			}
		}
	}

	// Let's reset the player. //
	player->reset();
	player->set_position(spawn.player_x, spawn.player_y);

	// Aaaand reveal the tiles around the player. //
	reveal_tiles(player, 1);
}

int Map::collected_items() const {
	return 0;
}

int Map::hidden_items() const {
	return 0;
}

