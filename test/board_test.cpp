#include <iostream>

#include "board.hpp"

static void initialization_test();
static void clean_test();
static void put_remove_test();
static void fen_test();
static void make_move_test();

void board_test()
{

    std::cout << "---------board test---------\n\n";

    initialization_test();
    clean_test();
    put_remove_test();
    fen_test();
    make_move_test();
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

static void fen_test()
{
    std::cout << "Fen test :\n\n";

    Board board;
    const auto StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    const auto EnPassantFEN = "rnbqkb1r/2pp2pn/1p6/pP1PppPp/8/2N5/P1P1PP1P/R1BQKBNR w KQkq f6 0 8";
    const auto PromotionFEN = "r3kb1r/pbpqn1P1/1pn4p/5Q2/2P5/2N5/PP1BN1pP/R3KB1R w KQkq - 2 13";
    const auto KiwipeteFEN = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
    const auto NoCastleFEN = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b - - 0 1";
    const auto CastleFEN1 = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b Kk - 0 1";
    const auto CastleFEN2 = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b Qq - 0 1";

    board.load_fen(StartFEN);
    if (board.fen() != StartFEN) {
        std::cout << "TEST FAILED : board_test : fen() != StartFEN\n";
    }

    board.load_fen(EnPassantFEN);
    if (board.fen() != EnPassantFEN) {
        std::cout << "TEST FAILED : board_test : fen() != EnPassantFEN\n";
    }

    board.load_fen(PromotionFEN);
    if (board.fen() != PromotionFEN) {
        std::cout << "TEST FAILED : board_test : fen() != PromotionFEN\n";
    }

    board.load_fen(KiwipeteFEN);
    if (board.fen() != KiwipeteFEN) {
        std::cout << "TEST FAILED : board_test : fen() != KiwipeteFEN\n";
    }
    board.load_fen(NoCastleFEN);
    if (board.fen() != NoCastleFEN) {
        std::cout << "TEST FAILED : board_test : fen() != NoCastleFEN\n";
    }
    board.load_fen(CastleFEN1);
    if (board.fen() != CastleFEN1) {
        std::cout << "TEST FAILED : board_test : fen() != CastleFEN1\n";
    }
    board.load_fen(CastleFEN2);
    if (board.fen() != CastleFEN2) {
        std::cout << "TEST FAILED : board_test : fen() != CastleFEN2\n";
    }
}


static void make_move_test()
{
    std::cout << "Make move test :\n\n";

    constexpr auto StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    constexpr auto EndFEN = "rn3r2/pbppq1p1/1p2pN2/8/3P2NP/6P1/PPP1BP1R/2KR2k1 b - - 6 18";

    Board board;
    board.load_fen(StartFEN);

    board.make_move(Move(Square::SQ_D2, Square::SQ_D4));
    board.make_move(Move(Square::SQ_F7, Square::SQ_F5));
    //rnbqkbnr/ppppp1pp/8/5p2/3P4/8/PPP1PPPP/RNBQKBNR w KQkq - 0 2
    board.make_move(Move(Square::SQ_E2, Square::SQ_E4));
    board.make_move(Move(Square::SQ_F5, Square::SQ_E4));
    //rnbqkbnr/ppppp1pp/8/8/3Pp3/8/PPP2PPP/RNBQKBNR w KQkq - 0 3
    board.make_move(Move(Square::SQ_B1, Square::SQ_C3));
    board.make_move(Move(Square::SQ_G8, Square::SQ_F6));
    //rnbqkb1r/ppppp1pp/5n2/8/3Pp3/2N5/PPP2PPP/R1BQKBNR w KQkq - 2 4
    board.make_move(Move(Square::SQ_C1, Square::SQ_G5));
    board.make_move(Move(Square::SQ_E7, Square::SQ_E6));
    //rnbqkb1r/pppp2pp/4pn2/6B1/3Pp3/2N5/PPP2PPP/R2QKBNR w KQkq - 0 5
    board.make_move(Move(Square::SQ_C3, Square::SQ_E4));
    board.make_move(Move(Square::SQ_F8, Square::SQ_E7));
    //rnbqk2r/ppppb1pp/4pn2/6B1/3PN3/8/PPP2PPP/R2QKBNR w KQkq - 1 6
    board.make_move(Move(Square::SQ_G5, Square::SQ_F6));
    board.make_move(Move(Square::SQ_E7, Square::SQ_F6));
    //rnbqk2r/pppp2pp/4pb2/8/3PN3/8/PPP2PPP/R2QKBNR w KQkq - 0 7
    board.make_move(Move(Square::SQ_G1, Square::SQ_F3));
    board.make_move(Move::castle_black_king());
    //rnbq1rk1/pppp2pp/4pb2/8/3PN3/5N2/PPP2PPP/R2QKB1R w KQ - 2 8
    board.make_move(Move(Square::SQ_F1, Square::SQ_D3));
    board.make_move(Move(Square::SQ_B7, Square::SQ_B6));
    //rnbq1rk1/p1pp2pp/1p2pb2/8/3PN3/3B1N2/PPP2PPP/R2QK2R w KQ - 0 9
    board.make_move(Move(Square::SQ_F3, Square::SQ_E5));
    board.make_move(Move(Square::SQ_C8, Square::SQ_B7));
    //rn1q1rk1/pbpp2pp/1p2pb2/4N3/3PN3/3B4/PPP2PPP/R2QK2R w KQ - 2 10
    board.make_move(Move(Square::SQ_D1, Square::SQ_H5));
    board.make_move(Move(Square::SQ_D8, Square::SQ_E7));
    //rn3rk1/pbppq1pp/1p2pb2/4N2Q/3PN3/3B4/PPP2PPP/R3K2R w KQ - 4 11
    board.make_move(Move(Square::SQ_H5, Square::SQ_H7));
    board.make_move(Move(Square::SQ_G8, Square::SQ_H7));
    //rn3r2/pbppq1pk/1p2pb2/4N3/3PN3/3B4/PPP2PPP/R3K2R w KQ - 0 12
    board.make_move(Move(Square::SQ_E4, Square::SQ_F6));
    board.make_move(Move(Square::SQ_H7, Square::SQ_H6));
    //rn3r2/pbppq1p1/1p2pN1k/4N3/3P4/3B4/PPP2PPP/R3K2R w KQ - 1 13
    board.make_move(Move(Square::SQ_E5, Square::SQ_G4));
    board.make_move(Move(Square::SQ_H6, Square::SQ_G5));
    //rn3r2/pbppq1p1/1p2pN2/6k1/3P2N1/3B4/PPP2PPP/R3K2R w KQ - 3 14
    board.make_move(Move(Square::SQ_H2, Square::SQ_H4));
    board.make_move(Move(Square::SQ_G5, Square::SQ_F4));
    //rn3r2/pbppq1p1/1p2pN2/8/3P1kNP/3B4/PPP2PP1/R3K2R w KQ - 1 15
    board.make_move(Move(Square::SQ_G2, Square::SQ_G3));
    board.make_move(Move(Square::SQ_F4, Square::SQ_F3));
    //rn3r2/pbppq1p1/1p2pN2/8/3P2NP/3B1kP1/PPP2P2/R3K2R w KQ - 1 16
    board.make_move(Move(Square::SQ_D3, Square::SQ_E2));
    board.make_move(Move(Square::SQ_F3, Square::SQ_G2));
    //rn3r2/pbppq1p1/1p2pN2/8/3P2NP/6P1/PPP1BPk1/R3K2R w KQ - 3 17
    board.make_move(Move(Square::SQ_H1, Square::SQ_H2));
    board.make_move(Move(Square::SQ_G2, Square::SQ_G1));
    //rn3r2/pbppq1p1/1p2pN2/8/3P2NP/6P1/PPP1BP1R/R3K1k1 w Q - 5 18
    board.make_move(Move::castle_white_queen());

    if (board.fen() != EndFEN) {
        std::cout << "TEST FAILED : board_test : fen() != EndFEN\n";
    }
}
