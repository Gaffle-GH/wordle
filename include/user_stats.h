#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "color.h"
#include "display.h"


using namespace std;

int totalGames = 0;
int wins = 0;
int totalAttempts = 0;

void updateStats(bool won, int attempts);
void displayBars(int totalGames, int wins, int attempts);

void updateStats(bool won, int attempts)
{
    if (won == true)
    {
        wins++;
        totalGames = totalGames + 1;
        totalAttempts += attempts;
        displayBars(totalGames, wins, totalAttempts);
    }
    else if (won == false)
    {
        totalGames = totalGames + 1;
        displayBars(totalGames, wins, totalAttempts);
    }
}

void displayBars(int totalGames, int wins, int attempts)
{
    const int barWidth = 20;

    int wins_per_attempt[6] = {0, 0, 0, 0, 0, 0};

    wins_per_attempt[attempts - 1]++;

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
    cout << endl;
}
