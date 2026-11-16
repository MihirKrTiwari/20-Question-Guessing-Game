# 20-Questions Binary Tree Game (C++)

A classic **20-Questions** guessing game implemented in C++ using a 20-level binary decision tree. The player thinks of a famous personality (historical, scientist, athlete, leader, entertainer, etc.), answers 20 yes/no questions, and the binary tree evaluates and resolves the person.

---

## 📂 Project Structure

```
20_Questions/
├── Makefile                # Build automation (compile, clean, test, run, run-py)
├── README.md               # Documentation and execution instructions
├── .gitignore              # Ignores build artifacts and binaries
├── data/
│   ├── questions.csv       # 20 level questions in CSV format
│   └── personalities.csv   # Pre-seeded personalities with 20-bit path masks
├── main.py                 # Python entry point and CLI argument handling
├── decision_tree.py        # Python binary tree definitions and persistence
├── game_engine.py          # Python interactive game loop interface
├── src/
│   ├── main.cpp            # Entry point and CLI argument handling
│   ├── DecisionTree.h      # Binary tree definitions & node structures
│   ├── DecisionTree.cpp    # Tree traversal and path-resolution logic
│   ├── GameEngine.h        # Interactive game loop interface
│   └── GameEngine.cpp      # User I/O and interactive question flow
└── build/                  # Compiled object files and binary target
```

---

## 🌳 Binary Decision Tree Mechanics

- **Tree Depth / Levels:** 20 levels.
- **Node Structure:** Each internal node represents a question corresponding to that level.
  - Answering **Yes (`y`)** traverses the **left child** branch (encoded as binary `1`).
  - Answering **No (`n`)** traverses the **right child** branch (encoded as binary `0`).
- **Binary Path Key:** The 20 answers form an exact 20-bit path key ($2^{20} = 1,048,576$ possible leaf states).
- **Personality Resolution:**
  - Evaluates exact bitmask matches against known famous figures stored in `data/personalities.csv`.
  - In cases where answers vary slightly, a **Hamming distance** nearest-neighbor matcher finds the closest personality match and reports the trait alignment score (e.g. `19/20 traits match`).
  - If the player was thinking of someone new, the game allows entering the correct name and learns the path dynamically during the session!

---

## 🚀 Building & Running

### Running with Python (No compiler required)

1. **Run the game:**
   ```bash
   python3 main.py
   # Or via make:
   make run-py
   ```

2. **Run automated verification tests:**
   ```bash
   python3 main.py --test
   # Or via make:
   make test-py
   ```

### Building & Running with C++

### Prerequisites
- A modern C++ compiler supporting C++17 (`clang++` or `g++`)
- `make`

### Commands

1. **Build the project:**
   ```bash
   make
   ```

2. **Run the game:**
   ```bash
   make run
   # Or directly:
   ./build/twenty_questions
   ```

3. **Run automated verification tests:**
   ```bash
   make test
   ```

4. **Clean build artifacts:**
   ```bash
   make clean
   ```

---

## 📝 Customization

- **Questions:** Edit [`data/questions.csv`](file:///Users/mihirtiwari/Documents/20_Questions/data/questions.csv) to modify the 20 questions.
- **Personalities:** Add new entries to [`data/personalities.csv`](file:///Users/mihirtiwari/Documents/20_Questions/data/personalities.csv) with their 20-bit binary mask (e.g., `0b11001001111110110111,Name,Description`).
