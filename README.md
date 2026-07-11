# Wordle (C++)

A terminal-based Wordle clone written in C++. Guess the hidden five-letter word in six tries, with color-coded feedback after each guess.

## Features

- Classic Wordle gameplay in the terminal
- Color-coded letter feedback (green = correct position, yellow = wrong position)
- Word validation against a filtered dictionary of five-letter words
- Player statistics with win distribution and win rate
- Persistent stats saved to `bin/stats.txt`
- Debug mode for inspecting file and stats loading

## Requirements

- A C++17-compatible compiler (e.g. `g++`)
- `make`
- A terminal that supports ANSI color codes

## Getting Started

### 1. Clone the repository

```bash
git clone https://github.com/Gaffle-GH/wordle.git
cd wordle
```

### 2. Add a word list

Place a word list file at `src/words.txt` with **one word per line**. The game filters this list to five-letter alphabetic words and writes the result to `src/5_letter.txt`.

> **Note:** `src/words.txt` is not included in the repository. You need to provide your own word list.

### 3. Build and run

```bash
make
make run
```

Or build and run in one step:

```bash
make run
```

Other useful commands:

```bash
make clean    # Remove the compiled binary
make restart  # Clean, rebuild, and run
```

## How to Play

1. Launch the game with `make run`.
2. Choose an option from the main menu:
   - **1. Play Game** — Start a new round
   - **2. Player Stats** — View your statistics
   - **3. Debug** — Inspect file and stats loading
   - **4. Exit** — Quit the program
3. Enter a valid five-letter word from the dictionary.
4. After each guess, letters are highlighted:
   - **Green** — Correct letter in the correct position
   - **Yellow** — Correct letter in the wrong position
   - **White/default** — Letter not in the word
5. You have six attempts to guess the word.

Invalid guesses (wrong length or not in the dictionary) do not count as an attempt.

## Project Structure

```
wordle/
├── bin/
│   ├── wordle          # Compiled executable (generated)
│   └── stats.txt       # Saved player statistics
├── include/
│   ├── color.h         # ANSI terminal color helpers
│   ├── display.h       # Loading animation and screen utilities
│   ├── user_stats.h    # Stats tracking and persistence
│   └── words_implication.h  # Core game logic
├── src/
│   ├── wordle.cc       # Entry point and main menu
│   ├── words.txt       # Full word list (user-provided)
│   └── 5_letter.txt    # Filtered five-letter words (generated)
└── Makefile
```

## Stats

After each game, your results are tracked and can be saved to `bin/stats.txt`, including:

- Win distribution by attempt (1–6)
- Total games played
- Total wins
- Win rate percentage

## Disclaimer

This is an unofficial recreation of the NYTimes Wordle game, built as a C++ learning project. It is not affiliated with or endorsed by The New York Times.
