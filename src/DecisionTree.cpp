#include "DecisionTree.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

namespace {
    // Trim whitespace
    std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\r\n");
        return str.substr(first, (last - first + 1));
    }

    // Parse binary string like "0b1101..." or decimal integer
    uint32_t parseMask(const std::string& s) {
        std::string trimmed = trim(s);
        if (trimmed.rfind("0b", 0) == 0 || trimmed.rfind("0B", 0) == 0) {
            uint32_t val = 0;
            for (size_t i = 2; i < trimmed.size(); ++i) {
                if (trimmed[i] == '0' || trimmed[i] == '1') {
                    val = (val << 1) | (trimmed[i] - '0');
                }
            }
            return val;
        }
        return static_cast<uint32_t>(std::stoul(trimmed));
    }

    // Compute Hamming distance (number of bit differences)
    int hammingDistance(uint32_t a, uint32_t b) {
        uint32_t diff = a ^ b;
        int dist = 0;
        while (diff) {
            dist += (diff & 1);
            diff >>= 1;
        }
        return dist;
    }
}

DecisionTree::DecisionTree() : root(nullptr) {}

bool DecisionTree::loadQuestions(const std::string& csvFilePath) {
    std::ifstream file(csvFilePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open questions CSV file: " << csvFilePath << std::endl;
        return false;
    }

    levelQuestions.clear();
    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#' || (line.rfind("//", 0) == 0)) {
            continue;
        }

        // CSV parsing: Level,Question
        std::stringstream ss(line);
        std::string levelStr, questionStr;
        if (std::getline(ss, levelStr, ',') && std::getline(ss, questionStr)) {
            levelQuestions.push_back(trim(questionStr));
        }
    }

    return !levelQuestions.empty();
}

bool DecisionTree::loadPersonalities(const std::string& csvFilePath) {
    std::ifstream file(csvFilePath);
    if (!file.is_open()) {
        std::cerr << "Warning: Could not open personalities CSV: " << csvFilePath << std::endl;
        return false;
    }

    knownPersonalities.clear();
    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#' || (line.rfind("//", 0) == 0)) {
            continue;
        }

        std::stringstream ss(line);
        std::string maskStr, nameStr, descStr;
        if (std::getline(ss, maskStr, ',') && std::getline(ss, nameStr, ',')) {
            std::getline(ss, descStr); // optional desc
            uint32_t mask = parseMask(maskStr);
            std::string label = trim(nameStr);
            if (!descStr.empty()) {
                label += " (" + trim(descStr) + ")";
            }
            knownPersonalities.emplace_back(mask, label);
        }
    }
    return true;
}

void DecisionTree::registerPerson(uint32_t bitmask, const std::string& name) {
    knownPersonalities.emplace_back(bitmask, name);
}

bool DecisionTree::savePersonalityToFile(const std::string& csvFilePath, uint32_t bitmask, const std::string& name, const std::string& description) {
    std::ofstream file(csvFilePath, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open personalities CSV for appending: " << csvFilePath << std::endl;
        return false;
    }

    std::string binStr = "0b";
    for (int i = 19; i >= 0; --i) {
        binStr += ((bitmask >> i) & 1) ? '1' : '0';
    }

    file << binStr << "," << name;
    if (!description.empty()) {
        file << "," << description;
    }
    file << "\n";
    return true;
}

void DecisionTree::buildTree(int maxDepth) {
    root = buildSubtree(0, maxDepth, 0);
}

std::shared_ptr<TreeNode> DecisionTree::buildSubtree(int currentLevel, int maxDepth, uint32_t currentPath) {
    if (currentLevel >= maxDepth) {
        // Leaf node reached at level 20
        std::string identity = resolvePerson(currentPath);
        return std::make_shared<TreeNode>(currentLevel, identity);
    }

    std::string q = (currentLevel < static_cast<int>(levelQuestions.size()))
                        ? levelQuestions[currentLevel]
                        : ("Question at level " + std::to_string(currentLevel + 1));

    auto node = std::make_shared<TreeNode>(currentLevel, q);

    // In a 20-level binary tree, dynamic on-demand traversal or tree expansion
    // Here we can build nodes down to interactive levels, or construct on traversal.
    // For full lazy tree, yesChild and noChild can be instantiated on demand or ahead of time.
    return node;
}

std::string DecisionTree::resolvePerson(uint32_t answerPath) const {
    if (knownPersonalities.empty()) {
        return "Unknown Mystery Personality (Path: " + std::to_string(answerPath) + ")";
    }

    int minDistance = 100;
    std::string closestPerson;

    for (const auto& entry : knownPersonalities) {
        int dist = hammingDistance(answerPath, entry.first);
        if (dist < minDistance) {
            minDistance = dist;
            closestPerson = entry.second;
        }
    }

    if (minDistance == 0) {
        return closestPerson;
    } else {
        std::ostringstream oss;
        oss << closestPerson << " [Closest Match - " << (20 - minDistance) << "/20 traits match]";
        return oss.str();
    }
}
