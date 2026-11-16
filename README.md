# 20-Questions Binary Decision Tree Game (Python)

A classic **20-Questions** guessing game implemented in Python using a 20-level binary decision tree. The player thinks of a famous personality (historical figure, scientist, athlete, leader, entertainer, etc.), answers 20 yes/no questions, and the binary decision tree evaluates and predicts who the player is thinking of.

If the game guesses incorrectly or encounters someone new, it learns the person and persists their 20-bit trait mask directly into `data/personalities.csv`.

---

## 📂 Project Structure

```
20_Questions/
├── main.py                     # CLI entry point and argument parsing
├── README.md                   # Documentation and run instructions
├── .gitignore                  # Python & OS ignore rules
├── data/
│   ├── questions.csv           # 20 level questions in CSV format
│   └── personalities.csv       # Known personalities with 20-bit path masks
└── src/
    └── twenty_questions/
        ├── __init__.py         # Package exports
        ├── decision_tree.py    # Binary tree definitions, resolution & CSV persistence
        └── game_engine.py      # Interactive game loop and user I/O
```

---

## 🌳 Binary Decision Tree Mechanics

- **Tree Depth / Levels:** 20 levels.
- **Node Structure:** Each level represents an identifying yes/no question.
  - Answering **Yes (`y`)** traverses the left child (binary `1`).
  - Answering **No (`n`)** traverses the right child (binary `0`).
- **Binary Path Key:** 20 answers form an exact 20-bit path key ($2^{20} = 1,048,576$ possible leaf states).
- **Personality Resolution:**
  - Evaluates exact bitmask matches against known personalities stored in `data/personalities.csv`.
  - In cases where answers vary slightly, a **Hamming distance** nearest-neighbor matcher finds the closest personality match and reports the alignment score (e.g., `19/20 traits match`).
- **Learning & Persistence:**
  - If the game cannot guess or guesses incorrectly, it asks who the player was thinking of.
  - The new person and their 20-bit answer bitmask are automatically saved to `data/personalities.csv` and loaded for future games.

---

## 🚀 Running the Game

### Prerequisites
- Python 3.8+ (no external third-party dependencies required; uses Python standard library)

### Commands

1. **Play the game:**
   ```bash
   python3 main.py
   ```

2. **Run automated verification tests:**
   ```bash
   python3 main.py --test
   ```

3. **Specify custom CSV datasets (optional):**
   ```bash
   python3 main.py path/to/questions.csv path/to/personalities.csv
   ```

---

## 📝 Customization

- **Questions:** Edit [`data/questions.csv`](file:///Users/mihirtiwari/Documents/20_Questions/data/questions.csv) to modify any of the 20 questions.
- **Personalities:** Pre-seed or inspect known entries in [`data/personalities.csv`](file:///Users/mihirtiwari/Documents/20_Questions/data/personalities.csv).
