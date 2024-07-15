#define ALPHADEEPCHESS_VERSION "1.0.0"
#define COLOR_CONSOLE

#define VALUESTR(value) ((value) ? "true" : "false")

#include <iostream>

static void resetConsoleColor() {
#ifdef COLOR_CONSOLE
    std::cout << "\e[0m";
#endif
}

static void debuglog(const char* message) {
#ifdef _DEBUG
#ifdef COLOR_CONSOLE
    std::cout << "\e[0;32m";
#endif
    std::cout << message << std::endl;
    resetConsoleColor();
#endif
}

static void debugerror(const char* message) {
#ifdef _DEBUG
#ifdef COLOR_CONSOLE
    std::cout << "\e[0;31m";
#endif
    std::cerr << message << std::endl;
    resetConsoleColor();
#endif
}
