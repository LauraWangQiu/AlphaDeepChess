#include <iostream>

#include "move_list.hpp"

void move_list_test()
{
    std::cout << "---------move_list test---------\n\n";

    MoveList moves;

    if (moves.size() != 0) {
        std::cout << "TEST FAILED : move_list_test : size()!=0\n";
    }
    moves.add(Move::null());
    moves.add(Move::null());
    moves.add(Move::null());

    if (moves.size() != 3) {
        std::cout << "TEST FAILED : move_list_test : size()!=3\n";
    }

    moves.clear();

    if (moves.size() != 0) {
        std::cout << "TEST FAILED : move_list_test : size()!=0\n";
    }

    moves.add(Move(Square::SQ_B2, Square::SQ_B4));
    moves.add(Move(Square::SQ_D2, Square::SQ_D1, MoveType::PROMOTION, PieceType::QUEEN));
    moves.add(Move::castle_white_queen());

    if (moves.get(0) != Move(Square::SQ_B2, Square::SQ_B4)) {
        std::cout << "TEST FAILED : move_list_test : get(0) != Move(Square::SQ_B2, Square::SQ_B4\n";
    }
    if (moves.get(2) != Move::castle_white_queen()) {
        std::cout << "TEST FAILED : move_list_test : get(2) != Move::castle_white_queen()\n";
    }
    if (moves.to_string() != "b2b4:\nd2d1q:\ne1c1:\n") {
        std::cout << "TEST FAILED : move_list_test : to_string()!=b2b4:d2d1q:e1c1\n";
    }
}
