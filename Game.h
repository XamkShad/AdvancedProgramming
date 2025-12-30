#pragma once
#include <string>

class Player;
class Map;

class Game {
private:
	Player* player;
	Map* map;
	bool running;

	void splash_screen();
	std::string select_level();
	bool load_level(const std::string& filepath);

	void process_input();
	void update();
	void render();

public:
	Game();
	~Game();

	void start();
	void run();
	void gameover();
};