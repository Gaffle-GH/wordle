#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>  // for all_of, find
#include <filesystem> // for file existence check

#include "color.h"
#include "display.h"
#include "user_stats.h"

using namespace std;

class Words
{
public:
    // CHECK FILE
    void check_file();
    void word_count(string input_file);

    // IMPORT STATS
    void import_stats(string save_file);

    // PICKS WORD
    void word();

    // PLAYER SOLVES WORD & COLORING
    void solving(int random_index);

    // DEBUG
    void debug();

private:
    void remaining_keys();
    vector<string> word_index;
    vector<char> letter;
};

// DEBUG
void Words::debug(){
    clearScreen();

    cout << "Debugging Mode: \n";
    
    cout << "FILE CHECK:" << endl;
    check_file(); // CHECK FILES AND LOAD WORDS
    import_stats("bin/stats.txt");

    cout << endl;

    cin.ignore();
    return;
}

// CHECKS FILE EXISTENCE AND CALLS WORD COUNT.
void Words::check_file()
{
    string save_file = "bin/stats.txt";
    string input_file = "src/words.txt";

    if (filesystem::exists(save_file))
    {
        cout << "File 'stats.txt' exists in bin directory." << endl;
        // import_stats(save_file);
    }
    else
    {
        cout << "File 'stats.txt' does not exist in bin directory." << endl;
    }

    // Checks if the file exists in the src directory
    // Ignore Error: 'filesystem' is not a member of 'std'
    if (filesystem::exists(input_file))
    {
        cout << "File 'words.txt' exists in src directory." << endl;
        // word_count(input_file);
    }
    else
    {
        cout << "File 'words.txt' does not exist in src directory." << endl;
        exit(1);
    }
}

// APPLIES STATS
void Words::import_stats(string save_file)
{
    ifstream file(save_file);
    if (!file.is_open())
    {
        cerr << "Unable to open " << save_file << " for reading stats." << endl;
        return;
    }

    string line;
    while (getline(file, line))
    {
        int idx = 0;
        int value = 0;

        if (sscanf(line.c_str(), "Win Line %d: %d", &idx, &value) == 2)
        {
            if (idx >= 1 && idx <= 6)
            {
                wins_per_attempt[idx - 1] = value;
            }
            continue;
        }

        if (sscanf(line.c_str(), "Total Games Played: %d", &value) == 1)
        {
            totalGames = value;
            continue;
        }
        if (sscanf(line.c_str(), "Total Wins: %d", &value) == 1)
        {
            wins = value;
            continue;
        }
    }

    totalAttempts = 0;
    for (int i = 0; i < 6; i++)
    {
        totalAttempts += (i + 1) * wins_per_attempt[i];
    }

    cout << "Loaded stats: " << totalGames << " games, " << wins << " wins, " << totalAttempts << " attempts." << endl;
    cout << "Win distribution: ";
    for (int i = 0; i < 6; i++)
    {
        cout << "[" << (i + 1) << ": " << wins_per_attempt[i] << "] ";
    }
    cout << endl;
}


// RETURNS NUM OF WORDS IN FILE, CREATES NEW FILE THAT CONTAINS 5-LETTERS ONLY FOR GAME FUNCTIONALITY.
void Words::word_count(string input_file)
{
    ifstream file(input_file);
    string word;
    int count_5 = 0;
    int count = 0;

    ofstream clear_file("src/5_letter.txt", ios::trunc);
    clear_file.close();

    while (file >> word)
    {
        count++;
        // Convert to lowercase FIRST
        transform(word.begin(), word.end(), word.begin(), ::tolower);

        if (word.length() == 5 && all_of(word.begin(), word.end(), ::isalpha))
        {
            count_5++;
            ofstream output_file("src/5_letter.txt", ios::app);
            if (output_file.is_open())
            {
                output_file << word << endl;
                output_file.close();
            }
            else
            {
                cerr << "Unable to open file for writing." << endl;
            }
        }
    }

    cout << "Total number of words in 'words.txt': " << count << endl;
    cout << "Total number of 5-letter words in 'words.txt' **Without Special Characters: " << count_5 << endl;
    Words::word();
}

// PLANS TO MAKE THIS FUNCTION INTO MULTIPLE FUNCTIONS, BUT FOR NOW IT'S ALL IN ONE
void Words::word()
{
    ifstream file("src/5_letter.txt");
    if (!file.is_open())
    {
        cerr << "Failed to open 5_letter.txt" << endl;
        exit(1);
    }

    word_index.clear();
    letter.resize(5);

    string w;
    while (file >> w)
    {
        word_index.push_back(w);
    }

    if (word_index.empty())
    {
        cerr << "No words loaded!" << endl;
        exit(1);
    }

    srand(time(NULL));
    int random_index = rand() % word_index.size(); // use actual size, not count_5
    cout << "Random index: " << random_index << endl;
    cout << "Random word: " << word_index[random_index] << endl;

    solving(random_index);
}

// Multiple Functions
void Words::solving(int random_index)
{
    // use size_t for indexing to match the return type of string::length()
    for (size_t i = 0; i < word_index[random_index].length(); i++)
    {
        letter[i] = word_index[random_index][i];
    }

    // SOLVING THE WORD
    size_t attempts = 0;
    vector<pair<string, string>> guess_history; // store (guess, colored_output)

    while (attempts < 6)
    {
        clearScreen();
        // Display history (up to 5 guesses) with blanks for missing slots
        for (size_t j = 0; j < 6; j++)
        {
            if (j < guess_history.size())
            {
                cout << j + 1 << "." << " : " << guess_history[j].second << "\n";
            }
            else
            {
                cout << j + 1 << "." << " : " << "_____\n";
            }
        }

        cout << "\nAttempt " << (attempts + 1) << " of 6\n";

        string guess;
        cout << "Say a word: ";

        if (!(cin >> guess))
        {
            break;
        }

        // CHECKS WORD IS VALID, HAS 5 LETTERS, AND EXISTS IN THE LIST
        bool valid_length = (guess.length() == 5);
        bool in_dictionary = valid_length && (find(word_index.begin(), word_index.end(), guess) != word_index.end());

        if (!valid_length || !in_dictionary)
        {
            cout << "Not a valid word in the database." << endl;
            // do not increment attempts; allow re-try
            continue;
        }

        // we have a valid guess; generate colored feedback
        string colored_output;
        vector<char> result(5, ' '); // 'G', 'Y', or ' '

        // Track which answer letters are still "available" for yellow matching
        vector<char> remaining = letter;

        // Pass 1: Mark greens and remove them from the pool
        for (size_t i = 0; i < 5; i++)
        {
            if (guess[i] == letter[i])
            {
                result[i] = 'G';
                // Remove this letter from the available pool
                auto it = find(remaining.begin(), remaining.end(), letter[i]);
                if (it != remaining.end())
                    remaining.erase(it);
            }
        }

        // Pass 2: Mark yellows using the remaining pool
        for (size_t i = 0; i < 5; i++)
        {
            if (result[i] == 'G')
                continue; // already handled

            auto it = find(remaining.begin(), remaining.end(), guess[i]);
            if (it != remaining.end())
            {
                result[i] = 'Y';
                remaining.erase(it); // consume this letter so duplicates don't over-yellow
            }
        }

        // Build colored output
        for (size_t i = 0; i < 5; i++)
        {
            if (result[i] == 'G')
            {
                colored_output += color(GREEN);
                colored_output += guess[i];
                colored_output += color(RESET);
            }
            else if (result[i] == 'Y')
            {
                colored_output += color(YELLOW);
                colored_output += guess[i];
                colored_output += color(RESET);
            }
            else
            {
                colored_output += guess[i];
            }
        }
        // store this guess in history and increment attempts
        guess_history.push_back({guess, colored_output});
        attempts++;

        // check win/lose AFTER adding to history, then redraw
        clearScreen();

        for (size_t j = 0; j < 6; j++)
        {
            if (j < guess_history.size())
                cout << j + 1 << "." << " : " << guess_history[j].second << "\n";
            else
                cout << j + 1 << "." << " : " << "_____\n";
        }

        if (attempts == 6)
        {
            cout << "\nAttempt 6 of 6";
            cout << "\nThe word was: " << word_index[random_index] << endl;
            updateStats(false, attempts);
        }
        else
        {
            cout << "\nAttempt " << (attempts + 1) << " of 6";
        }

        if (guess == word_index[random_index])
        {
            clearScreen();
            cout << "\nCongratulations! You've guessed the word!\n" << endl;
            updateStats(true, attempts);
            return;
        } else if (attempts == 6) {
            cout << "\nGame Over! The word was: " << word_index[random_index] << endl;
            updateStats(false, attempts);
            return;
        }
    }
}

// REMAINING KEYS FUNCTION
void Words::remaining_keys()
{
    string remaining_output;
    vector<string> letters = {
        "a", "b", "c", "d", "e", "f", "g", "h", "i", "j",
        "k", "l", "m", "n", "o", "p", "q", "r", "s", "t",
        "u", "v", "w", "x", "y", "z"};

    for (const auto &letter : letters)
    {
        // Check if the letter is in the correct position (green)
        if (find(letter.begin(), letter.end(), letter[0]) != letter.end())
        {
            remaining_output += color(GREEN) + letter + color(RESET) + " ";
        }
        // Check if the letter is in the word but in the wrong position (yellow)
        else if (find(letter.begin(), letter.end(), letter[0]) != letter.end())
        {
            remaining_output += color(YELLOW) + letter + color(RESET) + " ";
        }
        // Letter is not in the word (gray)
        else
        {
            remaining_output += color(WHITE) + letter + color(RESET) + " ";
        }
    }
}
