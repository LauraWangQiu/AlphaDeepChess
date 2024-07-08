#include <loop.hpp>
#include <defs.hpp>
#include <iostream>

Loop::Loop() : exit(false), board(nullptr) { }

Loop::~Loop() { board.reset(); }

bool Loop::init() {
    // SDL2 initialization?
    // ImGui initialization?

    board = std::make_unique<Board>();

    return true;
}

void Loop::run() {
    debuglog("AlphaDeepChess running...");
    while (!exit) {
        char opt;
        std::cin >> opt;
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

void Loop::printBoard() const { std::cout << *board; }

void Loop::printVersion() const { std::cout << "AlphaDeepChess " << ALPHADEEPCHESS_VERSION << std::endl;}

void Loop::printOptions() const {
    std::cout << "Options: " << std::endl;
    std::cout << "q - Quit the program" << std::endl;
    std::cout << "v - Show version" << std::endl;
    std::cout << "p - Print board" << std::endl;
    std::cout << "h - Show options" << std::endl;
}
