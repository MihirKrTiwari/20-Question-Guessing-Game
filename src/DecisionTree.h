#pragma once

#include <string>
#include <vector>
#include <memory>

// Node representing a state in the 20-Questions Decision Tree
struct TreeNode {
    int level;               // 0 to 19 for questions, 20 for leaf
    std::string text;        // Question if level < 20, or Person Name/Guess if leaf
    std::shared_ptr<TreeNode> yesChild;
    std::shared_ptr<TreeNode> noChild;

    TreeNode(int lvl, const std::string& t)
        : level(lvl), text(t), yesChild(nullptr), noChild(nullptr) {}

    bool isLeaf() const {
        return yesChild == nullptr && noChild == nullptr;
    }
};

class DecisionTree {
public:
    DecisionTree();

    // Loads question templates for each level from CSV
    bool loadQuestions(const std::string& csvFilePath);

    // Loads specific pre-seeded personalities mapped to answer bitmasks
    bool loadPersonalities(const std::string& csvFilePath);

    // Builds the 20-level binary decision tree structure
    void buildTree(int maxDepth = 20);

    // Get root node for traversal
    std::shared_ptr<TreeNode> getRoot() const { return root; }

    // Get question at specific level
    std::string getQuestion(int level) const {
        if (level >= 0 && level < static_cast<int>(levelQuestions.size())) {
            return levelQuestions[level];
        }
        return "";
    }

    // Evaluates a path of yes/no choices and returns the identified person
    std::string resolvePerson(uint32_t answerPath) const;

    // Register a known person for a given 20-bit path
    void registerPerson(uint32_t bitmask, const std::string& name);

private:
    std::shared_ptr<TreeNode> root;
    std::vector<std::string> levelQuestions;
    std::vector<std::pair<uint32_t, std::string>> knownPersonalities;

    // Lazy/recursive node builder
    std::shared_ptr<TreeNode> buildSubtree(int currentLevel, int maxDepth, uint32_t currentPath);
};
