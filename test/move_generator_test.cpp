#include <iostream>

#include "move_generator.hpp"

void move_generator_test() {
    MoveList movelist;
    Board board;
    generate_legal_moves(movelist, board);
    std::cout << "test\n";
}
