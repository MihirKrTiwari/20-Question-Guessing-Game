from typing import Optional, List, Tuple


class TreeNode:
    """Node representing a state in the 20-Questions Decision Tree."""

    def __init__(self, level: int, text: str):
        self.level: int = level
        self.text: str = text  # Question if level < 20, or Person Name/Guess if leaf
        self.yes_child: Optional["TreeNode"] = None
        self.no_child: Optional["TreeNode"] = None

    def is_leaf(self) -> bool:
        return self.yes_child is None and self.no_child is None


def parse_mask(s: str) -> int:
    """Parse binary string like '0b1101...' or decimal integer."""
    trimmed = s.strip()
    if trimmed.startswith(("0b", "0B")):
        val = 0
        for char in trimmed[2:]:
            if char in ("0", "1"):
                val = (val << 1) | int(char)
        return val
    return int(trimmed)


def hamming_distance(a: int, b: int) -> int:
    """Compute Hamming distance (number of bit differences)."""
    diff = a ^ b
    dist = 0
    while diff:
        dist += diff & 1
        diff >>= 1
    return dist


class DecisionTree:
    def __init__(self):
        self.root: Optional[TreeNode] = None
        self.level_questions: List[str] = []
        self.known_personalities: List[Tuple[int, str]] = []

    def load_questions(self, csv_file_path: str) -> bool:
        """Loads question templates for each level from CSV."""
        try:
            with open(csv_file_path, "r", encoding="utf-8") as f:
                self.level_questions = []
                for line in f:
                    line = line.strip()
                    if not line or line.startswith("#") or line.startswith("//"):
                        continue
                    parts = line.split(",", 1)
                    if len(parts) == 2:
                        self.level_questions.append(parts[1].strip())
            return len(self.level_questions) > 0
        except OSError as e:
            print(f"Error: Could not open questions CSV file: {csv_file_path} ({e})")
            return False

    def load_personalities(self, csv_file_path: str) -> bool:
        """Loads specific pre-seeded personalities mapped to answer bitmasks."""
        try:
            with open(csv_file_path, "r", encoding="utf-8") as f:
                self.known_personalities = []
                for line in f:
                    line = line.strip()
                    if not line or line.startswith("#") or line.startswith("//"):
                        continue
                    parts = [p.strip() for p in line.split(",")]
                    if len(parts) >= 2:
                        mask = parse_mask(parts[0])
                        label = parts[1]
                        if len(parts) >= 3 and parts[2]:
                            label += f" ({parts[2]})"
                        self.known_personalities.append((mask, label))
            return True
        except OSError as e:
            print(f"Warning: Could not open personalities CSV: {csv_file_path} ({e})")
            return False

    def register_person(self, bitmask: int, name: str) -> None:
        """Register a known person for a given 20-bit path in memory."""
        self.known_personalities.append((bitmask, name))

    def save_personality_to_file(self, csv_file_path: str, bitmask: int, name: str, description: str = "") -> bool:
        """Appends a newly learned personality to the personalities CSV file."""
        try:
            # Format bitmask as 20-bit binary representation: 0b0101...
            mask_str = f"0b{bitmask:020b}"
            line = f"{mask_str},{name}"
            if description:
                line += f",{description}"
            line += "\n"
            with open(csv_file_path, "a", encoding="utf-8") as f:
                f.write(line)
            return True
        except OSError as e:
            print(f"Error saving new personality to {csv_file_path}: {e}")
            return False

    def build_tree(self, max_depth: int = 20) -> None:
        """Builds the 20-level binary decision tree structure."""
        self.root = self._build_subtree(0, max_depth, 0)

    def _build_subtree(self, current_level: int, max_depth: int, current_path: int) -> TreeNode:
        if current_level >= max_depth:
            identity = self.resolve_person(current_path)
            return TreeNode(current_level, identity)

        q = (
            self.level_questions[current_level]
            if current_level < len(self.level_questions)
            else f"Question at level {current_level + 1}"
        )
        return TreeNode(current_level, q)

    def get_question(self, level: int) -> str:
        """Get question at specific level."""
        if 0 <= level < len(self.level_questions):
            return self.level_questions[level]
        return ""

    def resolve_person(self, answer_path: int) -> str:
        """Evaluates a path of yes/no choices and returns the identified person."""
        if not self.known_personalities:
            return f"Unknown Mystery Personality (Path: {answer_path})"

        min_distance = 100
        closest_person = ""

        for mask, name in self.known_personalities:
            dist = hamming_distance(answer_path, mask)
            if dist < min_distance:
                min_distance = dist
                closest_person = name

        if min_distance == 0:
            return closest_person
        else:
            return f"{closest_person} [Closest Match - {20 - min_distance}/20 traits match]"
