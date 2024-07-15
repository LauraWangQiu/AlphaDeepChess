#include <loop.hpp>
#include <defs.hpp>
#include <iostream>

Loop::Loop() : exit(false), board() { }
Loop::~Loop() { resetConsoleColor(); }

bool Loop::init() {
    // SDL2 initialization?
    // ImGui initialization?

    return true;
}

void Loop::run() {
    debuglog("AlphaDeepChess running...");

    while (!exit) {
        readOpt(opt);
        switch (opt) {
        case 'q': quit(); break;
        case 'v': printVersion(); break;
        case 'p': printBoard(); break;
        // case '': break;
        case 'h': printOptions(); break;
        default: break;
        }
    }
}

void Loop::readOpt(char& opt) const {
#ifdef COLOR_CONSOLE
    std::cout << "\e[0;93m";
#endif
    std::cin >> opt;
    resetConsoleColor();
}

void Loop::printBoard() const {
    debuglog("Printing board...");
    std::cout << board;
}

void Loop::printVersion() const {
    debuglog("Printing version...");
    std::cout << "AlphaDeepChess " << ALPHADEEPCHESS_VERSION << std::endl;
}

void Loop::printOptions() const {
    debuglog("Printing options...");
    std::cout << "Options: " << std::endl;
    std::cout << "q - Quit the program" << std::endl;
    std::cout << "v - Show version" << std::endl;
    std::cout << "p - Print board" << std::endl;
    std::cout << "h - Show options" << std::endl;
}
