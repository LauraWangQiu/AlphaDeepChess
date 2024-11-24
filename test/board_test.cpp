#include <iostream>

#include "board.hpp"

static void initialization_test();
static void put_remove_test();

void board_test()
{

    std::cout << "---------board test---------\n\n";

    initialization_test();
    put_remove_test();
}

static void initialization_test()
{
    std::cout << "Initialization test :\n\n";

    Board board;
    for (Square sq = Square::SQ_A1; sq <= Square::SQ_INVALID; sq++) {

        if (board.is_empty(sq) != true) {
            std::cout << "TEST FAILED : board_test : is_empty(sq) != true\n";
        }
        if (board.get_piece(sq) != Piece::EMPTY) {
            std::cout << "TEST FAILED : board_test : get_piece(sq) != Piece::EMPTY\n";
        }
    }
    if (board.get_bitboard_all() != 0ULL) {
        std::cout << "TEST FAILED : board_test : get_bitboard_all() != 0ULL\n";
    }
    if (board.get_bitboard_black() != 0ULL) {
        std::cout << "TEST FAILED : board_test : get_bitboard_black() != 0ULL\n";
    }
    if (board.get_bitboard_white() != 0ULL) {
        std::cout << "TEST FAILED : board_test : get_bitboard_white() != 0ULL\n";
    }

    for (Piece p = Piece::W_PAWN; p <= Piece::EMPTY; p = p + 1) {
        if (board.get_bitboard_piece(p) != 0ULL) {
            std::cout << "TEST FAILED : board_test : get_bitboard_piece(p) != 0ULL\n";
        }
    }
    if (board.fen() != "8/8/8/8/8/8/8/8 w - - 0 1") {
        std::cout << "TEST FAILED : board_test : fen() != 8/8/8/8/8/8/8/8 w - - 0 1\n";
    }

    if (board.state().side_to_move() != ChessColor::WHITE) {
        std::cout << "TEST FAILED : board_test : state().side_to_move() != ChessColor::WHITE\n";
    }
    if (board.state().move_number() != 0) {
        std::cout << "TEST FAILED : board_test : state().move_number() != 0\n";
    }
}

static void put_remove_test() { Board board; }
