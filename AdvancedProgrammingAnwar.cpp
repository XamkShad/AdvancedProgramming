#include <string>
#include <iostream>
#include <fstream>
#include "Player.cpp"

using namespace std;

bool is_valid_coordinate(int, int);
void start_splash_screen(void);
void startup_routines(void);
void quit_routines(void);
int select_level();
void load_level(string);
int read_input(char*);
void update_state(char);
void render_screen(void);

char** map = nullptr;
int mapHeight = 0;
int mapWidth = 0;

bool selected_map_valid = false;

Player global_player;

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
	map = new char* [mapHeight];
	for (int y = 0; y < mapHeight; y++) 
	{
		map[y] = new char[mapWidth];
	}
	
	int y = 0;
	while (getline(file, line)) 
	{
		for (int x = 0; x < mapWidth; x++)
		{
			map[y][x] = line[x];

			// Found the player. Mark it down. If not, abort mission. //
			if (line[x] == 'P')
			{
				global_player.pos_y = y;
				global_player.pos_x = x;

				selected_map_valid = true;
			}
		}

		y++;
	}

	file.close();

	render_screen();
}

int read_input(char* input)
{
	cout << "Awaiting for orders, Captain: ";	// simple prompt
	try {
		cin >> *input;
	}
	catch (...) {
		return -1; // failure
	}
	cout << endl;  //new line to reset output/input
	cin.ignore();  //clear cin to avoid messing up following inputs
	if (*input == 'q') return -2; // quitting game...
	return 0; // default return, no errors
}

void update_state(char input)
{
	// PLAYER INPUT AND COLLISION DETECTION //
	int target_pos_y = 0;
	int target_pos_x = 0;

	switch (input) 
	{
		case 'w':
			target_pos_y--; // W & S keys have to be in reverse, because of arrays. Fuck you C++. //
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

	if (is_valid_coordinate(global_player.pos_y + target_pos_y, global_player.pos_x + target_pos_x))
	{
		map[global_player.pos_y][global_player.pos_x] = 'o';
		map[global_player.pos_y + target_pos_y][global_player.pos_x + target_pos_x] = 'P';

		global_player.pos_y += target_pos_y;
		global_player.pos_x += target_pos_x;

		global_player.current_oxygen--;
	}
	// PLAYER INPUT AND COLLISION DETECTION //
}

bool is_valid_coordinate(int y, int x) 
{
	return map[y][x] != 'x';
}

void render_screen(void)
{
	system("cls"); // Let's make sure we don't "render" over old frames. //
	cout << "Holy Diver v0.01" << endl; 
	cout << "Use WASD keys to move. Press 'Q' to quit at all times." << endl << endl;

	cout << "O2 Level: " << global_player.current_oxygen << "%" << endl;
	cout << "Hull Integrity: " << global_player.current_health << "%" << endl;
	cout << "Lives: " << global_player.current_lives << endl << endl;

	for (int y = 0; y < mapHeight; y++) 
	{
		for (int x = 0; x < mapWidth; x++) 
		{
			cout << map[y][x];
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
	global_player = (100, 100, 3);

	load_level(path);
}

void quit_routines(void)
{
	system("cls");

	// Free the memory and all that pointer black magic. //
	for (int i = 0; i < mapHeight; i++) { delete[] map[i]; }
	delete[] map;

	cout << "The depths of the unknown awaits you! Come back soon!" << endl;
}

class Player {
public:
	int max_health = 100;
	int max_oxygen = 100;
	int max_lives = 3;

	int current_health;
	int current_oxygen;
	int current_lives;

	int pos_x;
	int pos_y;

	Player(int hp = 100, int ox = 100, int lv = 3) : max_health(hp), max_oxygen(ox), max_lives(lv) {
		reset();
	}

	~Player() {
		//game_over();
	}

	void reset() {
		current_health = max_health;
		current_oxygen = max_oxygen;
		current_lives = max_lives;
	}

	void move(int delta_x, int delta_y) {
		pos_x += delta_x;
		pos_y += delta_y;
	}

	void game_over() {

	}
};