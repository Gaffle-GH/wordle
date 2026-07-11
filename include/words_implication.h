#ifndef WORDS_IMPLICATION_H
#define WORDS_IMPLICATION_H

#include <string>
#include <vector>

class Words
{
public:
    void check_file();
    void play_game();
    void word_count(std::string input_file);
    void import_stats(std::string save_file);
    void show_stats();
    void word();
    void solving(int random_index);
    void debug();

private:
    void remaining_keys();
    void update_key_states(const std::string &guess, const std::vector<char> &result);
    void print_guess_board(const std::vector<std::pair<std::string, std::string>> &guess_history);
    std::vector<std::string> word_index;
    std::vector<char> letter;
    std::vector<char> key_state;
};

#endif
