#include <iostream>

#include "board.hpp"

static void initialization_test();
static void clean_test();
static void put_remove_test();

void board_test()
{

    std::cout << "---------board test---------\n\n";

    initialization_test();
    clean_test();
    put_remove_test();
}

static void initialization_test()
{
    std::cout << "Initialization test :\n\n";

    Board board;
    for (Square sq = Square::SQ_A1; sq < Square::SQ_INVALID; sq++) {

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
    if (board.state().move_number() != 1) {
        std::cout << "TEST FAILED : board_test : state().move_number() != 1\n";
    }
    if (board.state().fifty_move_rule_counter() != 0) {
        std::cout << "TEST FAILED : board_test : state().fifty_move_rule_counter() != 0\n";
    }
    if (board.state().last_captured_piece() != PieceType::EMPTY) {
        std::cout << "TEST FAILED : board_test : state().last_captured_piece() != EMPTY\n";
    }
    if (board.state().en_passant_square() != Square::SQ_INVALID) {
        std::cout << "TEST FAILED : board_test : state().en_passant_square() != SQ_INVALID\n";
    }
    if (board.state().castle_king_black() != false) {
        std::cout << "TEST FAILED : board_test : state().castle_king_black() != false\n";
    }
    if (board.state().castle_king_white() != false) {
        std::cout << "TEST FAILED : board_test : state().castle_king_white() != false\n";
    }
    if (board.state().castle_queen_black() != false) {
        std::cout << "TEST FAILED : board_test : state().castle_queen_black() != false\n";
    }
    if (board.state().castle_queen_white() != false) {
        std::cout << "TEST FAILED : board_test : state().castle_queen_white() != false\n";
    }
}

static void clean_test()
{
    std::cout << "Clean test :\n\n";

    Board board;
    constexpr auto StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    board.load_fen(StartFEN);
    board.clean();

    for (Square sq = Square::SQ_A1; sq < Square::SQ_INVALID; sq++) {

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
    if (board.state().move_number() != 1) {
        std::cout << "TEST FAILED : board_test : state().move_number() != 1\n";
    }
    if (board.state().fifty_move_rule_counter() != 0) {
        std::cout << "TEST FAILED : board_test : state().fifty_move_rule_counter() != 0\n";
    }
    if (board.state().last_captured_piece() != PieceType::EMPTY) {
        std::cout << "TEST FAILED : board_test : state().last_captured_piece() != EMPTY\n";
    }
    if (board.state().en_passant_square() != Square::SQ_INVALID) {
        std::cout << "TEST FAILED : board_test : state().en_passant_square() != SQ_INVALID\n";
    }
    if (board.state().castle_king_black() != false) {
        std::cout << "TEST FAILED : board_test : state().castle_king_black() != false\n";
    }
    if (board.state().castle_king_white() != false) {
        std::cout << "TEST FAILED : board_test : state().castle_king_white() != false\n";
    }
    if (board.state().castle_queen_black() != false) {
        std::cout << "TEST FAILED : board_test : state().castle_queen_black() != false\n";
    }
    if (board.state().castle_queen_white() != false) {
        std::cout << "TEST FAILED : board_test : state().castle_queen_white() != false\n";
    }
}

static void put_remove_test()
{
    std::cout << "Put remove test :\n\n";

    Board board;
    constexpr auto StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - - 0 1";

    board.put_piece(Piece::W_ROOK, Square::SQ_A1);
    board.put_piece(Piece::W_KNIGHT, Square::SQ_B1);
    board.put_piece(Piece::W_BISHOP, Square::SQ_C1);
    board.put_piece(Piece::W_QUEEN, Square::SQ_D1);
    board.put_piece(Piece::W_KING, Square::SQ_E1);
    board.put_piece(Piece::W_BISHOP, Square::SQ_F1);
    board.put_piece(Piece::W_KNIGHT, Square::SQ_G1);
    board.put_piece(Piece::W_ROOK, Square::SQ_H1);

    for (Col c = COL_A; c < COL_INVALID; c++) {
        board.put_piece(Piece::W_PAWN, Square(ROW_2, c));
        board.put_piece(Piece::B_PAWN, Square(ROW_7, c));
    }

    board.put_piece(Piece::B_ROOK, Square::SQ_A8);
    board.put_piece(Piece::B_KNIGHT, Square::SQ_B8);
    board.put_piece(Piece::B_BISHOP, Square::SQ_C8);
    board.put_piece(Piece::B_QUEEN, Square::SQ_D8);
    board.put_piece(Piece::B_KING, Square::SQ_E8);
    board.put_piece(Piece::B_BISHOP, Square::SQ_F8);
    board.put_piece(Piece::B_KNIGHT, Square::SQ_G8);
    board.put_piece(Piece::B_ROOK, Square::SQ_H8);

    if (board.fen() != StartFEN) {
        std::cout << "TEST FAILED : board_test : fen() != StartFEN\n";
    }

    if (board.get_bitboard_all() != 0xffff00000000ffff) {
        std::cout << "TEST FAILED : board_test : get_bitboard_all() != 0xffff00000000ffff\n";
    }
    if (board.get_bitboard_black() != 0xffff000000000000) {
        std::cout << "TEST FAILED : board_test : get_bitboard_black() != 0xffff000000000000\n";
    }
    if (board.get_bitboard_white() != 0x000000000000ffff) {
        std::cout << "TEST FAILED : board_test : get_bitboard_white() != 0x000000000000ffff\n";
    }
    constexpr static uint64_t start_pos_bitboard[NUM_CHESS_PIECES] = {
        0x000000000000ff00,   // W_PAWN
        0x0000000000000042,   // W_KNIGHT
        0x0000000000000024,   // W_BISHOP
        0x0000000000000081,   // W_ROOK
        0x0000000000000008,   // W_QUEEN
        0x0000000000000010,   // W_KING
        0x00ff000000000000,   // B_PAWN
        0x4200000000000000,   // B_KNIGHT
        0x2400000000000000,   // B_BISHOP
        0x8100000000000000,   // B_ROOK
        0x0800000000000000,   // B_QUEEN
        0x1000000000000000,   // B_KING
        0x0000ffffffff0000    // EMPTY
    };
    for (Piece p = Piece::W_PAWN; p < Piece::EMPTY; p = p + 1) {
        if (board.get_bitboard_piece(p) != start_pos_bitboard[(int)p]) {
            std::cout << "TEST FAILED : board_test : get_bitboard_piece(p) != 0ULL\n";
        }
    }

    for (Col c = COL_A; c < COL_INVALID; c++) {
        board.remove_piece(Square(ROW_1, c));
        board.remove_piece(Square(ROW_2, c));
        board.remove_piece(Square(ROW_7, c));
        board.remove_piece(Square(ROW_8, c));
    }

    for (Square sq = Square::SQ_A1; sq < Square::SQ_INVALID; sq++) {

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
}
