#include <iostream>
using namespace std;
#include "board/board.hpp"

int main(int argc, char** argv) {
    cout << "Hello world!\n";

    Board* board = new Board();
    board->foo();
    delete board;
    
    return 0;
}
