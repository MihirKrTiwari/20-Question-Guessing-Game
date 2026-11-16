CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Isrc

SRC_DIR = src
BUILD_DIR = build
BIN = $(BUILD_DIR)/twenty_questions

SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/DecisionTree.cpp $(SRC_DIR)/GameEngine.cpp
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

all: $(BIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

clean:
	rm -rf $(BUILD_DIR)

test: $(BIN)
	./$(BIN) --test

run: $(BIN)
	./$(BIN)

run-py:
	python3 main.py

test-py:
	python3 main.py --test

.PHONY: all clean test run run-py test-py
