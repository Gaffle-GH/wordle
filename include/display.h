#ifndef DISPLAY_H
#define DISPLAY_H

#include <atomic>
#include <chrono>
#include <iostream>
#include <limits>
#include <thread>

#include "color.h"

inline std::atomic<bool> loading{true};

inline void clearScreen()
{
    std::cout << "\033[2J\033[1;1H";
}

inline void loadingBar()
{
    const int barWidth = 30;
    int i = 0;

    while (loading)
    {
        int pos = i % barWidth;

        std::cout << color(WHITE) << "[" << color(YELLOW) << (pos + 1) << color(WHITE) << " / " << color(YELLOW) << barWidth << color(WHITE) << "] [";
        for (int j = 0; j < barWidth; j++)
        {
            if (j <= pos)
            {
                std::cout << color(YELLOW) << "#";
            }
            else
            {
                std::cout << " ";
            }
        }
        std::cout << color(WHITE) << "]\r";
        std::cout.flush();

        std::this_thread::sleep_for(std::chrono::milliseconds(80));
        i++;
    }

    std::cout << color(WHITE) << "[" << color(YELLOW) << barWidth << color(WHITE) << " / " << color(YELLOW) << barWidth << color(WHITE) << "] [";
    for (int j = 0; j < barWidth; j++)
    {
        std::cout << color(YELLOW) << "*";
    }
    std::cout << color(WHITE) << "]" << std::endl;
}

inline void loadingAnimation(double duration_seconds)
{
    loading = true;
    clearScreen();
    std::thread loader(loadingBar);
    std::this_thread::sleep_for(std::chrono::duration<double>(duration_seconds));
    loading = false;
    loader.join();
    clearScreen();
}

inline void info()
{
    clearScreen();
    std::cout << "⚠️ Disclaimer: " << std::endl;
    std::cout << "This is the 'NYTimes " << color(GREEN) << "W" << color(YELLOW) << "o" << color(WHITE) << "r" << color(GREEN) << "d" << color(YELLOW) << "l" << color(WHITE) << "e'" << " Game But Created in C++ Format!" << std::endl;
    std::cout << "This Version of the Game is Created by: Gaffle-GH" << std::endl;

    std::cout << "\n⚠️ File Management: " << std::endl;
    std::cout << "Game relies on the text file named 'words.txt', which is located in the 'src' directory." << std::endl;
    std::cout << "File contains ONE WORD PER LINE, and within the game process it will filter out any word that is not 5 LETTERS LONG or contains SPECIAL CHARACTERS" << std::endl;

    std::cout << "\nPress Enter to Continue [] : ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

#endif
