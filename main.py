#!/usr/bin/env python3
import sys
import os

# Add src to module lookup path
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "src"))

from twenty_questions.game_engine import GameEngine


def main():
    questions_path = "data/questions.csv"
    personalities_path = "data/personalities.csv"
    run_test = False

    args = sys.argv[1:]
    non_flag_args = []
    for arg in args:
        if arg == "--test":
            run_test = True
        else:
            non_flag_args.append(arg)

    if len(non_flag_args) >= 1:
        questions_path = non_flag_args[0]
    if len(non_flag_args) >= 2:
        personalities_path = non_flag_args[1]

    game = GameEngine(questions_path, personalities_path)
    if not game.initialize():
        print("Initialization failed. Exiting.", file=sys.stderr)
        sys.exit(1)

    if run_test:
        game.run_automated_test()
        sys.exit(0)

    play_again = "y"
    while play_again.lower() == "y":
        game.play()

        try:
            choice = input("\nDo you want to play another round? (y/n): ").strip()
        except EOFError:
            break

        if not choice:
            break
        play_again = choice[0]

    print("\nThank you for playing 20-Questions! Goodbye.")


if __name__ == "__main__":
    main()
