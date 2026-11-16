#pragma once

#include "DecisionTree.h"
#include <string>

class GameEngine {
public:
    GameEngine(const std::string& questionsCsv, const std::string& personalitiesCsv);

    bool initialize();
    void play();
    void runAutomatedTest();

private:
    DecisionTree tree;
    std::string questionsPath;
    std::string personalitiesPath;

    bool askYesNo(const std::string& question, int questionIndex, int totalQuestions);
    void printHeader();
};
