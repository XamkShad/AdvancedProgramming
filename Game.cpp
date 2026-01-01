#include "Game.h"
#include "Player.h"
#include "Map.h"
#include <iostream>
#include <string>

using namespace std;

// Get the level path from the constructor. //
Game::Game(std::string path) : player(nullptr), map(nullptr), running(true) {
	level_path = path;
}

// Free memory in destructor. //
Game::~Game() {
	delete map;
	delete player;
}

// Start, called once like in Unity. //
void Game::start() {
	splash_screen(); // Just a splash screen. //

	// If the path is empty, abort. //
	if (level_path.empty()) {
		running = false;
		return;
	}

	// If the path is corrupt, also abort. //
	if (!load_level(level_path)) {
		running = false;
		return;
	}

	// Otherwise, let's go. //
	running = true;
}

// COOL ART. //
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

// Load the level. //
bool Game::load_level(const std::string& filepath) {
	try {
		// Create new Map and Player objects. //
		map = new Map(filepath);
		player = new Player();

		// Set the start position for the player. //
		player->set_position(map->spawn.player_x, map->spawn.player_y);
		// Reveal some tiles around the player. //
		map->reveal_tiles(player, 1);

		// Render the first frame. //
		render();
		return true;
	}
	catch (...) {
		return false;
	}
}

// The actual run loop. //
GameResult Game::run() {
	running = true;

	while (running) {
		process_input(); // Process input, goes to the player as well. //
		update(); // Update all the states. //
		render(); // Render the map, and text. //

		// Simple gameover condition. //
		if (player->lives() < 0) {
			return gameover();
		}

		// If the player is just below the upper edge, "resurfaced", //
		// and has collected all the loot, they pass the level. //
		if (player->y() == 1 && player->items() == map->hidden_items()) {
			return next_level();
		}
	}

	return GameResult::QUIT;
}

// Input processing. //
void Game::process_input() {
	// Give the player a cue to give orders. //
	char input;
	std::cout << "\n\n";
	std::cout << "Awaiting for your orders, Captain: ";
	std::cin >> input;

	// If it's 'R', we gotta call the reset method. //
	if (input == 'r' || input == 'R') {
		map->reset(player);
		return;
	}

	// If it's 'Q', the player can quit. //
	if (input == 'q' || input == 'Q') {
		running = false;
		return;
	}

	// Also call the player input method, with the same input from this method. //
	player->handle_input(input, map);
}

// Update game states. //
void Game::update() {
	player->update(map, player);
	map->update(player);

	// If the player health drops below 0, we have to reset //
	// with one life less. See method definitions with F12 for more info. //
	if (player->health() <= 0) {
		player->reset_minus_life(map);
	}
}

// Actual 'rendering', it's a lot of TEXT. Good luck! //
void Game::render() {	
	system("cls"); // Let's make sure we don't "render" over old "frames". //
	cout << "Holy Diver v1.00" << endl;
	cout << "Use WASD keys to move. G key lights up surrounding area." << endl;
	cout << "Press R to reload level. Press 'Q' to quit at all times." << endl << endl;

	cout << "O2 Level: " << player->oxygen() << "%" << endl;
	cout << "Battery: " << player->battery() << "%" << endl;
	cout << "Hull Integrity: " << player->health() << "%" << endl;
	cout << "Lives: " << player->lives() << endl << endl;

	cout << "Score: " << player->score() << endl;

	// If the player hasn't found all loot, just print x/x. //
	// Otherwise, print instructions to resurface to the //
	// player.
	if (player->items() == map->hidden_items()) {
		cout << player->items() << "/" << map->hidden_items() << " items collected. Let's resurface, captain!" << endl << endl;
	}
	else {
		cout << "Items: " << player->items() << "/" << map->hidden_items() << endl << endl;
	}

	map->render(player);
}

// Gameover state over here. //
GameResult Game::gameover() {
	system("cls");

	// Cool message. //
	std::cout << "GAME OVER\n\n";
	std::cout << "You have joined the ever growing army of the drowned...\n";
	std::cout << "[R]etry  [Q]uit\n";

	char c;
	std::cin >> c;

	// Do you wanna play again?? //
	if (c == 'R' || c == 'r') {
		return GameResult::REPLAY;
	}

	// By default, just quit. //
	return GameResult::QUIT;
}

// Next level over here. //
GameResult Game::next_level() {
	system("cls");

	// Again, cool message. //
	std::cout << "LEVEL COMPLETE\n\n";
	std::cout << "You have collected treasure from the depths, but you still crave more...\n";
	std::cout << "[C]ontinue  [Q]uit\n";

	char c;
	std::cin >> c;

	// Want to proceed to the next level?? //
	if (c == 'C' || c == 'c') {
		return GameResult::NEXT_LEVEL;
	}

	// If not, you quit. //
	return GameResult::QUIT;
}