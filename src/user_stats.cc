#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

#include "color.h"
#include "display.h"
#include "user_stats.h"

using namespace std;

int totalGames = 0;
int wins = 0;
int totalAttempts = 0;
int wins_per_attempt[6] = {0, 0, 0, 0, 0, 0};

void updateStats(bool won, int attempts)
{
    totalGames += 1;
    if (won)
    {
        wins += 1;
        if (attempts >= 1 && attempts <= 6)
        {
            wins_per_attempt[attempts - 1]++;
            totalAttempts += attempts;
        }
    }

    stats(totalGames, wins, totalAttempts);
}

void stats(int totalGames, int wins, int attempts)
{
    clearScreen();
    const int barWidth = 20;

    cout << "Win Distribution:\n";
    for (int i = 0; i < 6; i++)
    {
        int barLength = (barWidth * wins_per_attempt[i]) / max(1, wins);
        int emptyLength = barWidth - barLength;

        cout << "Attempt " << (i + 1) << "["
             << color(GREEN) << string(barLength, '#') << color(RESET)
             << string(emptyLength, ' ')
             << "] (" << wins_per_attempt[i] << " wins)\n";
    }

    cout << "\nTotal Games Played: " << totalGames << endl;
    cout << "Total Wins: " << wins << endl;
    cout << "Win Rate: " << (totalGames > 0 ? (static_cast<double>(wins) / totalGames) * 100 : 0) << "%\n";
    cout << "Average Guesses (wins): " << (wins > 0 ? static_cast<double>(attempts) / wins : 0) << endl;
    cout << endl;

    cout << "\nPress Enter to return to main menu: ";
    cin.ignore();
    cin.get();
    clearScreen();

    ofstream statsFile("bin/stats.txt");
    if (statsFile.is_open())
    {
        int line_count = 1;
        for (int i = 0; i < 6; i++)
        {
            statsFile << "Win Line " << line_count++ << ": " << wins_per_attempt[i] << endl;
        }

        statsFile << endl;
        statsFile << "Total Games Played: " << totalGames << endl;
        statsFile << "Total Wins: " << wins << endl;
        statsFile << "Win Rate: " << (totalGames > 0 ? (static_cast<double>(wins) / totalGames) * 100 : 0) << "%\n";
    }
    else
    {
        cerr << "Unable to open bin/stats.txt for writing." << endl;
    }
}
