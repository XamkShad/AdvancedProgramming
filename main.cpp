#include <string>
#include <iostream>
#include <fstream>
#include <random>

using namespace std;

void start_splash_screen(void);
void startup_routines(void);
void quit_routines(void);
int select_level();
void load_level(string);
int read_input(char*);
void update_state(char);
void render_screen(void);

char** loaded_map = nullptr; // The loaded map, this remains untouched for resetting purposes. //
char** game_map = nullptr; // The game map, this can be overwritten as much as we like. //
char player_over_char = 'o'; // Let's save the char the player is standing over. By default, the player is standing in oxygen. //
int mapHeight = 0;
int mapWidth = 0;

bool selected_map_valid = false;

// RNG Black magic. -ChatGPT //
mt19937 rng_engine(random_device{}());

#pragma region Classes
class BaseCharacter {
public:
	int max_health = 100;
	int damage = 0;

	int current_health;

	int pos_x;
	int pos_y;

	virtual void move(int delta_x, int delta_y) {
		pos_x += delta_x;
		pos_y += delta_y;
	}

	void take_damage(int dmg) {
		current_health -= dmg;
	}
};

class Player : public BaseCharacter {
public:
	int max_oxygen = 100;
	int max_lives = 3;
	int max_battery = 100;

	int current_oxygen;
	int current_battery;
	int current_lives;

	Player() {
		reset();
	}

	~Player() {
		//game_over();
	}

	void reset() {
		current_health = max_health;
		current_oxygen = max_oxygen;
		current_battery = max_battery;
		current_lives = max_lives;
	}

	void game_over() {

	}
};

// Static, stationary enemy. //
class EnemySeamine : BaseCharacter {
public:
};

// Moving enemy. //
class EnemyAnglerfish : public BaseCharacter {
public:
	float move_chance = 75;
	int move_sign_chance = 50;

	bool will_move() {
		uniform_real_distribution<float> dist(0, 100);
		float rng = dist(rng_engine);

		return move_chance > rng;
	}

	void move() {
		uniform_real_distribution<float> dist(0, 100);
		float rng = dist(rng_engine);

		int delta_x = move_sign_chance > rng ? -1 : 1;
		int delta_y = move_sign_chance > rng ? 1 : -1;

		pos_x += delta_x;
		pos_y += delta_y;
	}
};

class Map {
public:
	EnemyAnglerfish* angler_fish;
	int fish_count;

	// Construct map with lil fishies. //
	Map(int count) {
		fish_count = count;
		angler_fish = new EnemyAnglerfish[count];
	}

	// Deconstruct map with lil fishies. //
	~Map() {
		delete[] angler_fish;
	}

	// Check if move is valid. //
	bool is_valid_coordinate(int y, int x)
	{
		return loaded_map[y][x] != 'x';
	}

	void move_fish() {
		for (int i = 0; i < fish_count; i++)
		{
			if (angler_fish[i].will_move()) {
				angler_fish[i].move();
			}
		}
	}
};

#pragma endregion

Player* player = new Player();
Map* map;

int main(void)
{
	start_splash_screen();
	startup_routines();

	if (!selected_map_valid) 
	{
		quit_routines();
		return 0;
	}
	
	char input;

	// IMPORTANT NOTE: do not exit program without cleanup: freeing allocated dynamic memory etc
	while (true) // infinite loop, should end with "break" in case of game over or user quitting etc.
	{
		input = '\0'; // make sure input resetted each cycle
		if (0 > read_input(&input)) break; // exit loop in case input reader returns negative (e.g. user selected "quit")
		update_state(input);
		render_screen();
	}

	quit_routines(); // cleanup, bye-bye messages, game save and whatnot
	return 0;
}

int select_level() 
{
	int level = -1; // Let's assume the player fucks up level selection from the get go. //

	while (true) 
	{
		system("cls");
		cout << "Select level by writing a number between 0-5 and pressing enter." << endl;

		string input;
		cin >> input;

		if (input == "q" || input == "Q") return -1; // Fuck this I want to leave. -Player //

		// Assuming the input is numeric, we are solid. Otherwise, code red all the way. //
		try {
			level = stoi(input);
		}
		catch (...) {
			continue;
		}

		if (level < 0 || level > 5) 
		{
			continue;
		}

		return level;
	}
}

void load_level(string filepath)
{
	ifstream file(filepath);

	// If we can't access the file, it's fucked. Abort mission. //
	if (!file.is_open()) 
	{ 
		system("cls");
		cout << "Failed to load map from " + filepath << endl << "Check map validity." << endl << endl;
		selected_map_valid = false; 
		return; 
	}
	
	string line;

	// Let's assume the map is 0 from the get go. //
	mapHeight = 0;
	mapWidth = 0;

	while (getline(file, line))
	{
		// Since maps can only be rectangular, setting the width as the first line's size will do. //
		if (mapHeight == 0) mapWidth = line.size();

		// For each line, increase the height. //
		mapHeight++;
	}

	file.clear();
	file.seekg(0);

	// Now, for actually assigning the read characters to the pointer of pointers. //
	loaded_map = new char* [mapHeight];
	for (int y = 0; y < mapHeight; y++) 
	{
		loaded_map[y] = new char[mapWidth];
	}
	
	int y = 0;
	while (getline(file, line)) 
	{
		for (int x = 0; x < mapWidth; x++)
		{
			loaded_map[y][x] = line[x];

			// Found the player. Mark it down. If not, abort mission. //
			if (line[x] == 'P')
			{
				player->pos_y = y;
				player->pos_x = x;

				selected_map_valid = true;
			}
		}

		y++;
	}	

	// Let's allocate runtime game map. //
	int angler_fish_count = 0; 
	game_map = new char* [mapHeight];
	for (int y = 0; y < mapHeight; y++) 
	{
		game_map[y] = new char[mapWidth];

		for (int x = 0; x < mapWidth; x++) 
		{
			char index = loaded_map[y][x];

			if (index == 'P') game_map[y][x] = 'P';
			else if (index == 'x') game_map[y][x] = 'x';
			else if (index == 'A') angler_fish_count++;
			else game_map[y][x] = '~';
		}
	}
	
	map = new Map(angler_fish_count);

	file.close();

	render_screen();
}

int read_input(char* input)
{
	cout << "Awaiting for orders, Captain: ";	// Simple prompt. //
	try {
		cin >> *input;
	}
	catch (...) {
		return -1; // Failure condition. //
	}
	cout << endl;
	cin.ignore();  // Clear cin just in case for corrupt input. //
	if (*input == 'q') return -2; // Quit the game. //
	return 0; // Default. //
}

void update_state(char input)
{
	// PLAYER INPUT AND COLLISION DETECTION //
	int target_pos_y = 0;
	int target_pos_x = 0;

	switch (input) 
	{
		case 'w':
			target_pos_y--; // W & S keys have to be in reverse, because of arrays? Fuck you C++. //
		break;
		case 'a':
			target_pos_x--;
		break;
		case 's':
			target_pos_y++;
		break;
		case 'd':
			target_pos_x++;
		break;
	}

	if (map->is_valid_coordinate(player->pos_y + target_pos_y, player->pos_x + target_pos_x))
	{
		game_map[player->pos_y + target_pos_y][player->pos_x + target_pos_x] = 'P'; // Update the P position. //
		game_map[player->pos_y][player->pos_x] = player_over_char; // Set the old position to be the char the player was standing on previously. //
		player_over_char = loaded_map[player->pos_y][player->pos_x]; // Update the player_over_char for the next move. //

		player->move(target_pos_x, target_pos_y);

		player->current_oxygen--;
	}
	// PLAYER INPUT AND COLLISION DETECTION //
}

void render_screen(void)
{
	system("cls"); // Let's make sure we don't "render" over old frames. //
	cout << "Holy Diver v0.01" << endl; 
	cout << "Use WASD keys to move. Press 'Q' to quit at all times." << endl << endl;

	cout << "O2 Level: " << player->current_oxygen << "%" << endl;
	cout << "Battery: " << player->current_battery << "%" << endl;
	cout << "Hull Integrity: " << player->current_health << "%" << endl;
	cout << "Lives: " << player->current_lives << endl << endl;

	for (int y = 0; y < mapHeight; y++) 
	{
		for (int x = 0; x < mapWidth; x++) 
		{
			cout << game_map[y][x];
		}

		cout << endl;
	}

	cout << endl;
}

void start_splash_screen(void)
{
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

void startup_routines(void)
{
	int selectedLevel = select_level();
	if (selectedLevel < 0) return; // Player bailed. //

	// Let's convert the selected level number to file name... //
	string path = "maps/level_" + to_string(selectedLevel) + ".map";

	// Give the player values and shit. //
	//player = (100, 100, 3);

	load_level(path);
}

void quit_routines(void)
{
	system("cls");

	// Free the memory and all that pointer black magic. //
	for (int i = 0; i < mapHeight; i++) { delete[] loaded_map[i]; }
	delete[] loaded_map;

	for (int i = 0; i < mapHeight; i++) { delete[] game_map[i]; }
	delete[] game_map;

	delete player;

	delete map;

	cout << "The depths of the unknown awaits you! Come back soon!" << endl;
}