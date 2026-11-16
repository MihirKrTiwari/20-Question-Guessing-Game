import sys
from typing import Optional
from twenty_questions.decision_tree import DecisionTree, TreeNode


class GameEngine:
    def __init__(self, questions_csv: str, personalities_csv: str):
        self.questions_path: str = questions_csv
        self.personalities_path: str = personalities_csv
        self.tree: DecisionTree = DecisionTree()

    def initialize(self) -> bool:
        if not self.tree.load_questions(self.questions_path):
            print(f"Failed to load questions from: {self.questions_path}", file=sys.stderr)
            return False

        if not self.tree.load_personalities(self.personalities_path):
            print(f"Notice: Personalities file not loaded, fallback mode active.", file=sys.stderr)

        self.tree.build_tree(20)
        return True

    def print_header(self) -> None:
        print("\n=======================================================")
        print("           20-QUESTIONS BINARY TREE GAME               ")
        print("=======================================================")
        print(" Think of a famous person (historical, scientist, celebrity,")
        print(" world leader, athlete, etc.).")
        print(" Answer each of the 20 questions with 'y' (yes) or 'n' (no).")
        print(" The game will traverse the 20-level binary decision tree")
        print(" and predict who you are thinking of!")
        print("=======================================================\n")

    def ask_yes_no(self, question: str, question_index: int, total_questions: int) -> bool:
        while True:
            try:
                user_input = input(f"[{question_index:2d}/{total_questions}] {question} (y/n): ")
            except EOFError:
                return False

            trimmed = user_input.strip()
            if not trimmed:
                continue

            c = trimmed[0].lower()
            if c == "y":
                return True
            elif c == "n":
                return False
            else:
                print("   Invalid input. Please enter 'y' for yes or 'n' for no.")

    def play(self) -> None:
        self.print_header()

        try:
            input("Press [Enter] when you are ready with your person in mind...")
        except EOFError:
            return

        current_path = 0
        current_node: Optional[TreeNode] = self.tree.root
        total_levels = 20

        for level in range(total_levels):
            question_text = self.tree.get_question(level)
            if not question_text:
                question_text = f"Question at level {level + 1}"

            answer_yes = self.ask_yes_no(question_text, level + 1, total_levels)

            # Update 20-bit path: 1 for Yes (left child), 0 for No (right child)
            current_path = (current_path << 1) | (1 if answer_yes else 0)

            # Advance tree node on-the-fly
            if current_node:
                if answer_yes:
                    if not current_node.yes_child:
                        current_node.yes_child = TreeNode(level + 1, "")
                    current_node = current_node.yes_child
                else:
                    if not current_node.no_child:
                        current_node.no_child = TreeNode(level + 1, "")
                    current_node = current_node.no_child

        print("\n-------------------------------------------------------")
        print(" Traversing to leaf node at level 20...")
        print(f" Binary Path Key: 0x{current_path:X}")
        print("-------------------------------------------------------")

        result_person = self.tree.resolve_person(current_path)

        print("\n>>> THE PERSON YOU WERE THINKING OF IS:")
        print(f"    *** {result_person} ***\n")

        try:
            feedback = input("Was this correct? (y/n): ").strip()
        except EOFError:
            feedback = ""

        if feedback and feedback[0].lower() == "y":
            print("Awesome! Binary Tree search succeeded!")
        else:
            try:
                actual_person = input("Who were you thinking of? ").strip()
            except EOFError:
                actual_person = ""

            if actual_person:
                try:
                    description = input("Optional short description/title: ").strip()
                except EOFError:
                    description = ""

                # Register in active decision tree memory
                stored_label = f"{actual_person} ({description})" if description else actual_person
                self.tree.register_person(current_path, stored_label)

                # Persist to personalities.csv
                saved = self.tree.save_personality_to_file(
                    self.personalities_path, current_path, actual_person, description
                )
                if saved:
                    print(f"Thank you! Learned \"{actual_person}\" and saved to {self.personalities_path}!")
                else:
                    print(f"Thank you! Learned \"{actual_person}\" for this session!")

    def run_automated_test(self) -> None:
        print("Running automated validation of 20-level binary tree resolution...")
        # Alan Turing bitmask from CSV: 0b01000101011111110111
        turing_mask = 0b01000101011111110111
        res_turing = self.tree.resolve_person(turing_mask)
        print(f"Testing Alan Turing path: {res_turing}")

        # Elon Musk bitmask: 0b11001001111110110111
        musk_mask = 0b11001001111110110111
        res_musk = self.tree.resolve_person(musk_mask)
        print(f"Testing Elon Musk path: {res_musk}")

        # Test with 1 bit difference
        near_musk = musk_mask ^ 1
        res_near_musk = self.tree.resolve_person(near_musk)
        print(f"Testing 1-bit mutation path: {res_near_musk}")

        print("Automated validation complete.")
