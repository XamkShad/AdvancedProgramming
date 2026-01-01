#pragma once
#include "Player.h"
#include "Map.h"
#include <string>

enum class GameResult {
	QUIT,
	REPLAY,
	NEXT_LEVEL
};

class Game {
private:
	Player* player;
	Map* map;
	bool running;
	std::string level_path;

	void splash_screen();
	bool load_level(const std::string& filepath);

	void process_input();
	void update();
	void render();

public:
	Game(std::string level_path);
	~Game();

	void start();
	GameResult run();
	GameResult gameover();
	GameResult next_level();
};