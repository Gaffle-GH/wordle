#ifndef COLORS_H
#define COLORS_H

#include <string>

constexpr int RESET = 0;
constexpr int BOLD = 1;
constexpr int RED = 31;
constexpr int GREEN = 32;
constexpr int YELLOW = 33;
constexpr int BLUE = 34;
constexpr int MAGENTA = 35;
constexpr int CYAN = 36;
constexpr int WHITE = 37;

inline std::string color(int code)
{
    return "\033[" + std::to_string(code) + "m";
}

#endif
