#include <iostream>

#include "board.hpp"

static void initialization_test();
static void clean_test();
static void put_remove_test();
static void fen_test();
static void make_unmake_move_test1();

void board_test()
{

    std::cout << "---------board test---------\n\n";

    initialization_test();
    clean_test();
    put_remove_test();
    fen_test();
    make_unmake_move_test1();
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

    for (Piece p = Piece::W_PAWN; p < Piece::EMPTY; p = p + 1) {
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

    for (Piece p = Piece::W_PAWN; p < Piece::EMPTY; p = p + 1) {
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

    for (Piece p = Piece::W_PAWN; p < Piece::EMPTY; p = p + 1) {
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


static void make_unmake_move_test1()
{
    std::cout << "Make unmake move test1 :\n\n";

    constexpr auto StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    constexpr auto EndFEN = "rn3r2/pbppq1p1/1p2pN2/8/3P2NP/6P1/PPP1BP1R/2KR2k1 b - - 6 18";

    const uint32_t num_moves = 35U;
    GameState game_states[num_moves + 1];

    Move moves[num_moves] = {
        Move(Square::SQ_D2, Square::SQ_D4),   // 1. d4
        Move(Square::SQ_F7, Square::SQ_F5),   // 1... f5
        Move(Square::SQ_E2, Square::SQ_E4),   // 2. e4
        Move(Square::SQ_F5, Square::SQ_E4),   // 2... fxe4
        Move(Square::SQ_B1, Square::SQ_C3),   // 3. Nc3
        Move(Square::SQ_G8, Square::SQ_F6),   // 3... Nf6
        Move(Square::SQ_C1, Square::SQ_G5),   // 4. Bg5
        Move(Square::SQ_E7, Square::SQ_E6),   // 4... e6
        Move(Square::SQ_C3, Square::SQ_E4),   // 5. Nxe4
        Move(Square::SQ_F8, Square::SQ_E7),   // 5... Be7
        Move(Square::SQ_G5, Square::SQ_F6),   // 6. Bxf6
        Move(Square::SQ_E7, Square::SQ_F6),   // 6... Bxf6
        Move(Square::SQ_G1, Square::SQ_F3),   // 7. Nf3
        Move::castle_black_king(),            // 7... O-O
        Move(Square::SQ_F1, Square::SQ_D3),   // 8. Bd3
        Move(Square::SQ_B7, Square::SQ_B6),   // 8... b6
        Move(Square::SQ_F3, Square::SQ_E5),   // 9. Ne5
        Move(Square::SQ_C8, Square::SQ_B7),   // 9... Bb7
        Move(Square::SQ_D1, Square::SQ_H5),   // 10. Qh5
        Move(Square::SQ_D8, Square::SQ_E7),   // 10... Qe7
        Move(Square::SQ_H5, Square::SQ_H7),   // 11. Qxh7+
        Move(Square::SQ_G8, Square::SQ_H7),   // 11... Kxh7
        Move(Square::SQ_E4, Square::SQ_F6),   // 12. Nxf6+
        Move(Square::SQ_H7, Square::SQ_H6),   // 12... Kh6
        Move(Square::SQ_E5, Square::SQ_G4),   // 13. Ng4+
        Move(Square::SQ_H6, Square::SQ_G5),   // 13... Kg5
        Move(Square::SQ_H2, Square::SQ_H4),   // 14. h4+
        Move(Square::SQ_G5, Square::SQ_F4),   // 14... Kf4
        Move(Square::SQ_G2, Square::SQ_G3),   // 15. g3+
        Move(Square::SQ_F4, Square::SQ_F3),   // 15... Kf3
        Move(Square::SQ_D3, Square::SQ_E2),   // 16. Be2+
        Move(Square::SQ_F3, Square::SQ_G2),   // 16... Kg2
        Move(Square::SQ_H1, Square::SQ_H2),   // 17. Rh2+
        Move(Square::SQ_G2, Square::SQ_G1),   // 17... Kg1
        Move::castle_white_queen()            // 18. O-O-O
    };

    Board board;
    game_states[0] = board.state();

    board.load_fen(StartFEN);

    for (int32_t i = 0; i < (int32_t)num_moves; i++) {

        board.make_move(moves[i]);
        game_states[i + 1] = board.state();
    }

    if (board.fen() != EndFEN) {
        std::cout << "TEST FAILED : board_test : fen() != EndFEN\n";
    }

    for (int32_t i = num_moves - 1; i >= 0; i--) {

        board.unmake_move(moves[i],game_states[i]);
    }

    if (board.fen() != StartFEN) {
        std::cout << "TEST FAILED : board_test : fen() != StartFEN\n";
    }
}
