#include "GameEngine.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cctype>

GameEngine::GameEngine(const std::string& questionsCsv, const std::string& personalitiesCsv)
    : questionsPath(questionsCsv), personalitiesPath(personalitiesCsv) {}

bool GameEngine::initialize() {
    if (!tree.loadQuestions(questionsPath)) {
        std::cerr << "Failed to load questions from: " << questionsPath << std::endl;
        return false;
    }

    if (!tree.loadPersonalities(personalitiesPath)) {
        std::cerr << "Notice: Personalities file not loaded, fallback mode active." << std::endl;
    }

    tree.buildTree(20);
    return true;
}

void GameEngine::printHeader() {
    std::cout << "\n=======================================================\n";
    std::cout << "           20-QUESTIONS BINARY TREE GAME               \n";
    std::cout << "=======================================================\n";
    std::cout << " Think of a famous person (historical, scientist, celebrity,\n";
    std::cout << " world leader, athlete, etc.).\n";
    std::cout << " Answer each of the 20 questions with 'y' (yes) or 'n' (no).\n";
    std::cout << " The game will traverse the 20-level binary decision tree\n";
    std::cout << " and predict who you are thinking of!\n";
    std::cout << "=======================================================\n\n";
}

bool GameEngine::askYesNo(const std::string& question, int questionIndex, int totalQuestions) {
    while (true) {
        std::cout << "[" << std::setw(2) << questionIndex << "/" << totalQuestions << "] " 
                  << question << " (y/n): ";
        std::string input;
        if (!std::getline(std::cin, input)) {
            // End of input / EOF
            return false;
        }

        // Trim input
        size_t first = input.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) continue;
        char c = std::tolower(input[first]);

        if (c == 'y') {
            return true;
        } else if (c == 'n') {
            return false;
        } else {
            std::cout << "   Invalid input. Please enter 'y' for yes or 'n' for no.\n";
        }
    }
}

void GameEngine::play() {
    printHeader();

    std::cout << "Press [Enter] when you are ready with your person in mind...";
    std::string dummy;
    std::getline(std::cin, dummy);

    uint32_t currentPath = 0;
    auto currentNode = tree.getRoot();

    const int totalLevels = 20;

    for (int level = 0; level < totalLevels; ++level) {
        // Fetch question text for the current level
        std::string questionText = tree.getQuestion(level);
        if (questionText.empty()) {
            questionText = "Question at level " + std::to_string(level + 1);
        }
        bool answerYes = askYesNo(questionText, level + 1, totalLevels);

        // Update 20-bit path: 1 for Yes (left child), 0 for No (right child)
        currentPath = (currentPath << 1) | (answerYes ? 1 : 0);

        // Advance tree node on-the-fly
        if (currentNode) {
            if (answerYes) {
                if (!currentNode->yesChild) {
                    currentNode->yesChild = std::make_shared<TreeNode>(
                        level + 1, 
                        (level + 1 < totalLevels) ? "" : ""
                    );
                }
                currentNode = currentNode->yesChild;
            } else {
                if (!currentNode->noChild) {
                    currentNode->noChild = std::make_shared<TreeNode>(
                        level + 1,
                        (level + 1 < totalLevels) ? "" : ""
                    );
                }
                currentNode = currentNode->noChild;
            }
        }
    }

    std::cout << "\n-------------------------------------------------------\n";
    std::cout << " Traversing to leaf node at level 20...\n";
    std::cout << " Binary Path Key: 0x" << std::hex << std::uppercase << currentPath << std::dec << "\n";
    std::cout << "-------------------------------------------------------\n";

    std::string resultPerson = tree.resolvePerson(currentPath);

    std::cout << "\n>>> THE PERSON YOU WERE THINKING OF IS:\n";
    std::cout << "    *** " << resultPerson << " ***\n\n";

    std::cout << "Was this correct? (y/n): ";
    std::string feedback;
    std::getline(std::cin, feedback);
    if (!feedback.empty() && std::tolower(feedback[0]) == 'y') {
        std::cout << "Awesome! Binary Tree search succeeded!\n";
    } else {
        std::cout << "Who were you thinking of? ";
        std::string actualPerson;
        std::getline(std::cin, actualPerson);
        if (!actualPerson.empty()) {
            tree.registerPerson(currentPath, actualPerson);
            std::cout << "Thank you! Learned \"" << actualPerson << "\" for this decision path!\n";
        }
    }
}

void GameEngine::runAutomatedTest() {
    std::cout << "Running automated validation of 20-level binary tree resolution...\n";
    // Alan Turing bitmask from CSV: 0b01000101011111110111
    uint32_t turingMask = 0b01000101011111110111;
    std::string resTuring = tree.resolvePerson(turingMask);
    std::cout << "Testing Alan Turing path: " << resTuring << "\n";

    // Elon Musk bitmask: 0b11001001111110110111
    uint32_t muskMask = 0b11001001111110110111;
    std::string resMusk = tree.resolvePerson(muskMask);
    std::cout << "Testing Elon Musk path: " << resMusk << "\n";

    // Test with 1 bit difference
    uint32_t nearMusk = muskMask ^ 1;
    std::string resNearMusk = tree.resolvePerson(nearMusk);
    std::cout << "Testing 1-bit mutation path: " << resNearMusk << "\n";

    std::cout << "Automated validation complete.\n";
}
