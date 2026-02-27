// standard headers
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>  // for all_of, find
#include <filesystem> // for file existence check

#include "color.h"
#include "display.h"

using namespace std;

class Words
{
public:
    // CHECK FILE
    void check_file();
    void word_count(string filepath);

    // PICKS WORD
    void word(int count_5);

private:
    vector<string> word_index;
    vector<char> letter;
};

// CHECKS FILE EXISTENCE AND CALLS WORD COUNT.
void Words::check_file()
{
    clearScreen();
    string filepath = "src/words.txt";

    // Checks if the file exists in the src directory
    // Ignore Error: 'filesystem' is not a member of 'std'
    if (filesystem::exists(filepath))
    {
        cout << "File 'words.txt' exists in src directory." << endl;
        return (word_count(filepath));
    }
    else
    {
        cout << "File 'words.txt' does not exist in src directory." << endl;
        exit(1);
    }
}

// RETURNS NUM OF WORDS IN FILE, CREATES NEW FILE THAT CONTAINS 5-LETTERS ONLY FOR GAME FUNCTIONALITY.
void Words::word_count(string filepath)
{
    ifstream file(filepath);
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
    Words::word(count_5);
}

// PLANS TO MAKE THIS FUNCTION INTO MULTIPLE FUNCTIONS, BUT FOR NOW IT'S ALL IN ONE
void Words::word(int count_5)
{
    ifstream file("src/5_letter.txt");
    vector<string> word_index(count_5);
    // avoid variable length arrays (VLA) which are a non-standard C++ extension
    // use a std::vector or std::string to hold the characters instead
    vector<char> letter(word_index.size());

    for (size_t i = 0; i < static_cast<size_t>(count_5); i++)
    {
        string word;
        file >> word;
        word_index[i] = word;
    }

    // Generate a random index to select a random word from the list of 5-letter words
    srand(time(NULL));
    int random_index = rand() % count_5;
    cout << "Random index: " << random_index << endl;
    cout << "Random word: " << word_index[random_index] << endl;

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
            // input stream closed (EOF or error); exit the loop gracefully
            break;
        }

        // CHECKS WORD IS VALID, HAS 5 LETTERS, AND EXISTS IN THE LIST
        bool valid_length = (guess.length() == 5);
        bool in_dictionary = valid_length &&
                             (find(word_index.begin(), word_index.end(), guess) != word_index.end());

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

        cout << "\nAttempt " << (attempts + 1) << " of 6";
        if (guess == word_index[random_index])
        {
            cout << "\nCongratulations! You've guessed the word!" << endl;
            return;
        }
        else if (attempts == 6)
        {
            cout << "\nThe word was: " << word_index[random_index] << endl;
            return;
        }
    }
}
