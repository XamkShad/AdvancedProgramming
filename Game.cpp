#include "Game.h"
#include "Player.h"
#include "Map.h"
#include <iostream>
#include <string>

using namespace std;

Game::Game() : player(nullptr), map(nullptr), running(true) {
	player = new Player();
	map = new Map("maps/level_0.map");
}

Game::~Game() {
	delete map;
	delete player;

	system("cls");

	std::cout << "The depths of the sea will be waiting for you!" << "\n";
}

void Game::start() {
	splash_screen();

	string level_path = select_level();

	if (level_path.empty()) {
		running = false;
		return;
	}

	if (!load_level(level_path)) {
		running = false;
		return;
	}

	running = true;
}

void Game::splash_screen() {
	// Badass art from patorjk.com. Don't question the y-tail offset. It only works this way. //
	// The console has a fucking stroke reading this otherwise. // 
	const char* art = R"(

	$$\   $$\           $$\                 $$$$$$$\  $$\                                
	$$ |  $$ |          $$ |                $$  __$$\ \__|                               
	$$ |  $$ | $$$$$$\  $$ |$$\   $$\       $$ |  $$ |$$\ $$\    $$\  $$$$$$\   $$$$$$\  
	$$$$$$$$ |$$  __$$\ $$ |$$ |  $$ |      $$ |  $$ |$$ |\$$\  $$  |$$  __$$\ $$  __$$\ 
	$$  __$$ |$$ /  $$ |$$ |$$ |  $$ |      $$ |  $$ |$$ | \$$\$$  / $$$$$$$$ |$$ |  \__|
	$$ |  $$ |$$ |  $$ |$$ |$$ |  $$ |      $$ |  $$ |$$ |  \$$$  /  $$   ____|$$ |      
	$$ |  $$ |\$$$$$$  |$$ |\$$$$$$$ |      $$$$$$$  |$$ |   \$  /   \$$$$$$$\ $$ |      
	\__|  \__| \______/ \__| \____$$ |      \_______/ \__|    \_/     \_______|\__|      
		                $$\   $$ |                                                   
		                \$$$$$$  |                                                   
		                 \______/                                                    

	)";

	// I see what you did with the name here Anton. Dio would be proud. //

	cout << art << endl << endl;
	cout << "The bottomless abyss calls your name." << endl << endl;
	cout << "Press any key to continue." << endl;
	cin.ignore();
}

std::string Game::select_level() {
	while (true) {
		system("cls");
		cout << "Select level (0-5) or Q to quit: \n";

		string input;
		cin >> input;

		if (input == "q" || input == "Q") return "";

		try {
			int level = stoi(input);
			
			if (level >= 0 && level <= 5) {
				return "maps/level_" + to_string(level) + ".map";
			}			

		}catch(...){}
	}
}

bool Game::load_level(const std::string& filepath) {
	try {
		map = new Map(filepath);
		player = new Player();
		player->set_position(map->spawn.player_x, map->spawn.player_y);
		render();
		return true;
	}
	catch (...) {
		return false;
	}
}

void Game::run() {
	while (running) {
		process_input();
		update();
		render();
	}
}

void Game::process_input() {
	char input;
	std::cout << "\n\n";
	std::cout << "Awaiting for your orders, Captain: ";
	std::cin >> input;

	if (input == 'q') running = false; return;

	player->handle_input(input, map);
}

void Game::update() {
	player->update();
	map->update(player);
}

void Game::render() {	
	system("cls"); // Let's make sure we don't "render" over old "frames". //
	cout << "Holy Diver v1.00" << endl;
	cout << "Use WASD keys to move. Press 'Q' to quit at all times." << endl << endl;

	cout << "O2 Level: " << player->oxygen() << "%" << endl;
	cout << "Battery: " << player->battery() << "%" << endl;
	cout << "Hull Integrity: " << player->health() << "%" << endl;
	cout << "Lives: " << player->lives() << endl << endl;

	map->render();
}