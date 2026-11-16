#include "GameEngine.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::string questionsPath = "data/questions.csv";
    std::string personalitiesPath = "data/personalities.csv";
    bool runTest = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--test") {
            runTest = true;
        } else if (questionsPath == "data/questions.csv") {
            questionsPath = arg;
        } else if (personalitiesPath == "data/personalities.csv") {
            personalitiesPath = arg;
        }
    }

    GameEngine game(questionsPath, personalitiesPath);
    if (!game.initialize()) {
        std::cerr << "Initialization failed. Exiting." << std::endl;
        return 1;
    }

    if (runTest) {
        game.runAutomatedTest();
        return 0;
    }

    char playAgain = 'y';
    while (std::tolower(playAgain) == 'y') {
        game.play();

        std::cout << "\nDo you want to play another round? (y/n): ";
        std::string choice;
        if (!std::getline(std::cin, choice) || choice.empty()) {
            break;
        }
        playAgain = choice[0];
    }

    std::cout << "\nThank you for playing 20-Questions! Goodbye.\n";
    return 0;
}
