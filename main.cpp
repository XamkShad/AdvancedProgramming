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

// AI USAGE: Yes indeed, I did use AI. I used ChatGPT for //
// math, that was difficult for me to wrap my head around. //
// I have commented ChatGPT for the code that is from ChatGPT. //

// What isn't commented, was how separating things was ChatGPTs idea. //
// Seperating the code into Map.cpp, Game.cpp, etc... //
// I mean, I would've figured that out at some point, right? :) //
// I also asked it to explain pointers, I still don't get it 100%. But //
// oh well. I try not to use code from AI, only ask it to explain concepts. //
// The math stuff is convenient though. //
// When I did get serious errors I couldn't figure out the source of, //
// I just put the error code into ChatGPT and it would explain it. //
// It would give me a better idea to look for the source. //
// Other than that, I take full credit. I have been programming a while //
// in Unity with C#. C++ is sort of like C#'s annoying cousin. :) //