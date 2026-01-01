#include "Game.h"
#include <string>
#include <iostream>

using namespace std;

int main() {
    bool running = true;
    int level = 0;
    int max_level = 2;

    while (running) {
        std::string level_path = "maps/level_" + std::to_string(level) + ".map";
        Game game(level_path);

        game.start();
        GameResult result = game.run();

        switch (result) {
        case GameResult::REPLAY:
            // Same level, so just break the switch so we can //
            // Create a new game essentially. //
            break;

        case GameResult::NEXT_LEVEL:
            if (level < max_level) {
                level++;
            }
            else {
                running = false;

                system("cls");
                std::cout << "You have completed Holy Diver." << endl;
                std::cout << "You are a true Diver." << endl;
            }
            
            break;

        case GameResult::QUIT:
            running = false;
            break;
        }
    }

    return 0;
}

// AI USAGE: 