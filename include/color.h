#ifndef COLORS_H
#define COLORS_H

#include <string>
using namespace std;

const int RESET      = 0;
const int BOLD       = 1;
const int RED        = 31;
const int GREEN      = 32;
const int YELLOW     = 33;
const int BLUE       = 34;
const int MAGENTA    = 35;
const int CYAN       = 36;
const int WHITE      = 37;

string color(int code){
    return "\033[" + to_string(code) + "m";
}

#endif