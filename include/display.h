#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

#include "color.h"

using namespace std;

atomic<bool> loading(true);

void clearScreen()
{
    cout << "\033[2J\033[1;1H";
}

void loadingBar()
{
    const int barWidth = 30;
    int i = 0;

    while (loading)
    {
        int pos = i % barWidth;

        cout << color(WHITE) << "[" << color(YELLOW) << (pos + 1) << color(WHITE) << " / " << color(YELLOW) << barWidth << color(WHITE) << "] [";
        for (int j = 0; j < barWidth; j++)
        {
            if (j <= pos)
            {
                cout << color(YELLOW) << "#";
            }
            else
            {
                cout << " ";
            }
        }
        cout << color(WHITE) << "]\r";
        cout.flush();

        this_thread::sleep_for(std::chrono::milliseconds(80));
        i++;
    }

    // Complete bar
    cout << color(WHITE) << "[" << color(YELLOW) << barWidth << color(WHITE) << " / " << color(YELLOW) << barWidth << color(WHITE) << "] [";
    for (int j = 0; j < barWidth; j++)
    {
        cout << color(YELLOW) << "*";
    }
    cout << color(WHITE) << "]" << endl;
}

void loadingAnimation(double duration_seconds)
{
    clearScreen();
    thread loader(loadingBar);
    this_thread::sleep_for(std::chrono::duration<double>(duration_seconds));
    loading = false;
    loader.join();
    clearScreen();
}

void info()
{
    cout << "⚠️ Disclaimer: " << endl;
    cout << "This is the 'NYTimes " << color(GREEN) << "W" << color(YELLOW) << "o" << color(WHITE) << "r" << color(GREEN) << "d" << color(YELLOW) << "l" << color(WHITE) << "e'" << " Game But Created in C++ Format!" << endl;
    cout << "This Version of the Game is Created by: Gaffle-GH" << endl;

    cout << "\n⚠️ File Management: " << endl;
    cout << "Game relies on the text file named 'words.txt', which is located in the 'src' directory." << endl;
    cout << "File contains ONE WORD PER LINE, and within the game process it will filter out any word that is not 5 LETTERS LONG or contains SPECIAL CHARACTERS" << endl;

    cout << "\nPress Enter to Continue [] : ";
    cin.ignore();
}

#endif