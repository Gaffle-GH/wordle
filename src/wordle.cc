#include <iomanip>
#include <iostream>

#include "words_implication.h"
#include "display.h"

using namespace std;

int main()
{
    Words words;

    // LOADING BAR (1 SEC.)
    loadingAnimation(2);
    info();

    int option;

    clearScreen();
    cout << "Enter Option" << endl;
    cout << "1. Play Game" << endl;
    cout << "[2]. Player Stats" << endl;
    cout << "3. Debug" << endl;
    cout << "4. Exit" << endl;
    cin >> option;

    switch (option)
    {
    case 1:
        // CHECK FILES
        words.check_file();
        break;
    case 2:

        break;
    case 3:
        words.debug();
        break;
    default:
        cout << "Invalid Option. Exiting." << endl;
    }

    return 0;
}
