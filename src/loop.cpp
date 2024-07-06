#include <loop.hpp>
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
    while (!exit) {
        char opt;
        std::cin >> opt;
        switch (opt) {
        case 'q': quit(); break;
        case 'h':
            std::cout << "Options: " << std::endl;
            std::cout << "q - Quit the program" << std::endl;
            break;
        // case '':
        //     break;
        default: break;
        }

        std::cout << board;
    }
}
