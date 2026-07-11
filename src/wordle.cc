#include <iostream>
#include <limits>

#include "display.h"
#include "words_implication.h"

using namespace std;

int main()
{
    Words words;

    // LOADING BAR (1 SEC.)
    loadingAnimation(2);
    info();

    int option = 0;
    while (option != 4)
    {
        clearScreen();
        cout << "Enter Option" << endl;
        cout << "[1]. Play Game" << endl;
        cout << "[2]. Player Stats" << endl;
        cout << "[3]. Debug" << endl;
        cout << "[4]. Exit" << endl;
        cin >> option;

        switch (option)
        {
        case 1:
            words.play_game();
            break;
        case 2:
            words.show_stats();
            break;
        case 3:
            words.debug();
            break;
        case 4:
            clearScreen();
            cout << "Goodbye!" << endl;
            break;
        default:
            clearScreen();
            cout << "Invalid Option." << endl;
            cout << "\nPress Enter to continue: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    }

    return 0;
}
