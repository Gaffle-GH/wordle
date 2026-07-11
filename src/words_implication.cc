#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "color.h"
#include "display.h"
#include "user_stats.h"
#include "words_implication.h"

using namespace std;

namespace
{
bool file_exists(const string &path)
{
    ifstream file(path);
    return file.good();
}
}

void Words::debug()
{
    clearScreen();

    cout << "Debugging Mode: \n";

    cout << "FILE CHECK:" << endl;
    check_file();
    import_stats("bin/stats.txt");

    cout << endl;

    cout << "\nPress Enter to return to main menu: ";
    cin.ignore();
    cin.get();
    clearScreen();

}

void Words::check_file()
{
    string save_file = "bin/stats.txt";
    string input_file = "src/words.txt";

    if (file_exists(save_file))
    {
        cout << "File 'stats.txt' exists in bin directory." << endl;
    }
    else
    {
        cout << "File 'stats.txt' does not exist in bin directory." << endl;
    }

    if (file_exists(input_file))
    {
        cout << "File 'words.txt' exists in src directory." << endl;
    }
    else
    {
        cout << "File 'words.txt' does not exist in src directory." << endl;
        exit(1);
    }
}

void Words::play_game()
{
    clearScreen();
    string input_file = "src/words.txt";

    if (!file_exists(input_file))
    {
        cerr << "File 'words.txt' does not exist in src directory." << endl;
        exit(1);
    }

    if (file_exists("bin/stats.txt"))
    {
        import_stats("bin/stats.txt");
    }

    word_count(input_file);
}

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

void Words::show_stats()
{
    clearScreen();
    import_stats("bin/stats.txt");
    stats(totalGames, wins, totalAttempts);
}

void Words::word_count(string input_file)
{
    clearScreen();
    ifstream file(input_file);
    string word;
    int count_5 = 0;
    int count = 0;

    ofstream clear_file("src/5_letter.txt", ios::trunc);
    clear_file.close();

    while (file >> word)
    {
        count++;
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
    this->word();
}

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

    srand(static_cast<unsigned>(time(nullptr)));
    int random_index = rand() % static_cast<int>(word_index.size());

    solving(random_index);
}

void Words::solving(int random_index)
{
    for (size_t i = 0; i < word_index[random_index].length(); i++)
    {
        letter[i] = word_index[random_index][i];
    }

    key_state.assign(26, 0);
    size_t attempts = 0;
    vector<pair<string, string>> guess_history;

    while (attempts < 6)
    {
        clearScreen();
        print_guess_board(guess_history);

        cout << "\nAttempt " << (attempts + 1) << " of 6\n";

        string guess;
        cout << "Say a word: ";

        if (!(cin >> guess))
        {
            break;
        }

        transform(guess.begin(), guess.end(), guess.begin(), ::tolower);

        bool valid_length = (guess.length() == 5);
        bool in_dictionary = valid_length && (find(word_index.begin(), word_index.end(), guess) != word_index.end());

        if (!valid_length || !in_dictionary)
        {
            clearScreen();
            print_guess_board(guess_history);
            cout << "\nNot a valid word in the database." << endl;
            continue;
        }

        string colored_output;
        vector<char> result(5, ' ');
        vector<char> remaining = letter;

        for (size_t i = 0; i < 5; i++)
        {
            if (guess[i] == letter[i])
            {
                result[i] = 'G';
                auto it = find(remaining.begin(), remaining.end(), letter[i]);
                if (it != remaining.end())
                    remaining.erase(it);
            }
        }

        for (size_t i = 0; i < 5; i++)
        {
            if (result[i] == 'G')
                continue;

            auto it = find(remaining.begin(), remaining.end(), guess[i]);
            if (it != remaining.end())
            {
                result[i] = 'Y';
                remaining.erase(it);
            }
        }

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

        update_key_states(guess, result);
        guess_history.push_back({guess, colored_output});
        attempts++;

        clearScreen();
        print_guess_board(guess_history);

        if (guess == word_index[random_index])
        {
            cout << "\nCongratulations! You've guessed the word!\n" << endl;
            updateStats(true, static_cast<int>(attempts));
            return;
        }

        if (attempts == 6)
        {
            cout << "\nGame Over! The word was: " << word_index[random_index] << endl;
            updateStats(false, static_cast<int>(attempts));
            return;
        }

        cout << "\nAttempt " << (attempts + 1) << " of 6";
    }
}

void Words::print_guess_board(const vector<pair<string, string>> &guess_history)
{
    for (size_t j = 0; j < 6; j++)
    {
        if (j < guess_history.size())
            cout << j + 1 << "." << " : " << guess_history[j].second << "\n";
        else
            cout << j + 1 << "." << " : " << "_____\n";
    }

    remaining_keys();
}

void Words::update_key_states(const string &guess, const vector<char> &result)
{
    for (size_t i = 0; i < 5; i++)
    {
        char c = guess[i];
        if (c < 'a' || c > 'z')
            continue;

        int idx = c - 'a';
        char state = result[i];

        if (state == 'G')
            key_state[idx] = 'G';
        else if (state == 'Y' && key_state[idx] != 'G')
            key_state[idx] = 'Y';
        else if (state == ' ' && key_state[idx] == 0)
            key_state[idx] = 'A';
    }
}

void Words::remaining_keys()
{
    const vector<string> rows = {
        "qwertyuiop",
        "asdfghjkl",
        "zxcvbnm"};

    cout << "\nRemaining Keys:\n";
    for (const auto &row : rows)
    {
        cout << "  ";
        for (char c : row)
        {
            int idx = c - 'a';
            char state = key_state[idx];

            if (state == 'G')
                cout << color(GREEN) << c << color(RESET) << " ";
            else if (state == 'Y')
                cout << color(YELLOW) << c << color(RESET) << " ";
            else if (state == 'A')
                cout << "\033[90m" << c << color(RESET) << " ";
            else
                cout << c << " ";
        }
        cout << "\n";
    }
}
