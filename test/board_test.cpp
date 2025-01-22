#include <iostream>

#include "board.hpp"
#include "test_utils.hpp"

static void board_get_piece_test();
static void board_is_empty_test();
static void board_get_bitboard_all_test();
static void board_get_bitboard_white_test();
static void board_get_bitboard_black_test();
static void board_get_bitboard_piece_test();
static void board_put_piece_test();
static void board_remove_piece_test();
static void board_clean_test();
static void board_make_unmake_move_test();
static void board_fen_test();
static void board_initialization_test();

void board_test()
{

    std::cout << "---------board test---------\n\n";

    board_get_piece_test();
    board_is_empty_test();
    board_get_bitboard_all_test();
    board_get_bitboard_white_test();
    board_get_bitboard_black_test();
    board_get_bitboard_piece_test();
    board_put_piece_test();
    board_remove_piece_test();
    board_clean_test();
    board_make_unmake_move_test();
    board_fen_test();
    board_initialization_test();
}

static void board_get_piece_test()
{
    const std::string test_name = "board_get_piece_test";

    constexpr auto StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    Board board;
    board.load_fen(StartFEN);

    if (board.get_piece(Square::SQ_A1) != Piece::W_ROOK) {
        PRINT_TEST_FAILED(test_name, "get_piece(SQ_A1)!=Piece::W_ROOK");
    }
    if (board.get_piece(Square::SQ_A2) != Piece::W_PAWN) {
        PRINT_TEST_FAILED(test_name, "get_piece(SQ_A2)!=Piece::W_PAWN");
    }
    if (board.get_piece(Square::SQ_A3) != Piece::EMPTY) {
        PRINT_TEST_FAILED(test_name, "get_piece(SQ_A3)!=Piece::EMPTY");
    }
    if (board.get_piece(Square::SQ_A4) != Piece::EMPTY) {
        PRINT_TEST_FAILED(test_name, "get_piece(SQ_A4)!=Piece::EMPTY");
    }
    if (board.get_piece(Square::SQ_A5) != Piece::EMPTY) {
        PRINT_TEST_FAILED(test_name, "get_piece(SQ_A5)!=Piece::EMPTY");
    }
    if (board.get_piece(Square::SQ_A6) != Piece::EMPTY) {
        PRINT_TEST_FAILED(test_name, "get_piece(SQ_A6)!=Piece::EMPTY");
    }
    if (board.get_piece(Square::SQ_A7) != Piece::B_PAWN) {
        PRINT_TEST_FAILED(test_name, "get_piece(SQ_A7)!=Piece::B_PAWN");
    }
    if (board.get_piece(Square::SQ_A8) != Piece::B_ROOK) {
        PRINT_TEST_FAILED(test_name, "get_piece(SQ_A8)!=Piece::B_ROOK");
    }
}

static void board_is_empty_test()
{
    const std::string test_name = "board_is_empty_test";

    constexpr auto StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    Board board;
    board.load_fen(StartFEN);

    if (board.is_empty(Square::SQ_A1) != false) {
        PRINT_TEST_FAILED(test_name, "is_empty(SQ_A1)!=false");
    }
    if (board.is_empty(Square::SQ_A2) != false) {
        PRINT_TEST_FAILED(test_name, "is_empty(SQ_A2)!=false");
    }
    if (board.is_empty(Square::SQ_A3) != true) {
        PRINT_TEST_FAILED(test_name, "is_empty(SQ_A3)!=true");
    }
    if (board.is_empty(Square::SQ_A4) != true) {
        PRINT_TEST_FAILED(test_name, "is_empty(SQ_A4)!=true");
    }
    if (board.is_empty(Square::SQ_A5) != true) {
        PRINT_TEST_FAILED(test_name, "is_empty(SQ_A5)!=true");
    }
    if (board.is_empty(Square::SQ_A6) != true) {
        PRINT_TEST_FAILED(test_name, "is_empty(SQ_A6)!=true");
    }
    if (board.is_empty(Square::SQ_A7) != false) {
        PRINT_TEST_FAILED(test_name, "is_empty(SQ_A7)!=false");
    }
    if (board.is_empty(Square::SQ_A8) != false) {
        PRINT_TEST_FAILED(test_name, "is_empty(SQ_A8)!=false");
    }
}

static void board_get_bitboard_all_test()
{
    const std::string test_name = "board_get_bitboard_all_test";

    constexpr auto StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    Board board;

    if (board.get_bitboard_all() != 0ULL) {
        PRINT_TEST_FAILED(test_name, "get_bitboard_all()!=0ULL");
    }

    board.load_fen(StartFEN);

    if (board.get_bitboard_all() != 0xffff00000000ffff) {
        PRINT_TEST_FAILED(test_name, "get_bitboard_all()!=0xffff00000000ffff");
    }
}

static void board_get_bitboard_white_test()
{
    const std::string test_name = "board_get_bitboard_white_test";

    constexpr auto StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    Board board;

    if (board.get_bitboard_white() != 0ULL) {
        PRINT_TEST_FAILED(test_name, "get_bitboard_white()!=0ULL");
    }

    board.load_fen(StartFEN);

    if (board.get_bitboard_white() != 0x000000000000ffff) {
        PRINT_TEST_FAILED(test_name, "get_bitboard_white()!=0x000000000000ffff");
    }
}

static void board_get_bitboard_black_test()
{
    const std::string test_name = "board_get_bitboard_black_test";

    constexpr auto StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    Board board;

    if (board.get_bitboard_black() != 0ULL) {
        PRINT_TEST_FAILED(test_name, "get_bitboard_black()!=0ULL");
    }

    board.load_fen(StartFEN);

    if (board.get_bitboard_black() != 0xffff000000000000) {
        PRINT_TEST_FAILED(test_name, "get_bitboard_black()!=0x000000000000ffff");
    }
}

static void board_get_bitboard_piece_test()
{

    const std::string test_name = "board_get_bitboard_piece_test";

    constexpr auto StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    Board board;

    for (Piece p = Piece::W_PAWN; p < Piece::EMPTY; p = p + 1) {
        if (board.get_bitboard_piece(p) != 0) {
            const std::string s = "" + piece_to_char(p);
            PRINT_TEST_FAILED(test_name, "get_bitboard_piece(" + s + ")!=0ULL");
        }
    }

    board.load_fen(StartFEN);

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
            const std::string s = "" + piece_to_char(p);
            PRINT_TEST_FAILED(test_name, "get_bitboard_piece(" + s + ")!=start_pos_bitboard[p]");
        }
    }
}

static void board_put_piece_test()
{
    const std::string test_name = "board_put_piece_test";

    Board board;

    board.put_piece(Piece::W_QUEEN, Square::SQ_A1);
    board.put_piece(Piece::B_QUEEN, Square::SQ_H8);
    board.put_piece(Piece::W_KNIGHT, Square::SQ_A8);
    board.put_piece(Piece::B_PAWN, Square::SQ_H1);
    board.put_piece(Piece::B_ROOK, Square::SQ_D4);

    if (board.get_piece(Square::SQ_A1) != Piece::W_QUEEN) {
        PRINT_TEST_FAILED(test_name, "get_piece(SQ_A1)!=Piece::W_QUEEN");
    }
    if (board.get_piece(Square::SQ_H8) != Piece::B_QUEEN) {
        PRINT_TEST_FAILED(test_name, "get_piece(SQ_H8)!=Piece::B_QUEEN");
    }
    if (board.get_piece(Square::SQ_A8) != Piece::W_KNIGHT) {
        PRINT_TEST_FAILED(test_name, "get_piece(SQ_A8)!=Piece::W_KNIGHT");
    }
    if (board.get_piece(Square::SQ_H1) != Piece::B_PAWN) {
        PRINT_TEST_FAILED(test_name, "get_piece(SQ_H1)!=Piece::B_PAWN");
    }
    if (board.get_piece(Square::SQ_D4) != Piece::B_ROOK) {
        PRINT_TEST_FAILED(test_name, "get_piece(SQ_D4)!=Piece::B_ROOK");
    }

    board.put_piece(Piece::EMPTY, Square::SQ_A1);

    if (board.get_piece(Square::SQ_A1) != Piece::EMPTY) {
        PRINT_TEST_FAILED(test_name, "get_piece(SQ_A1)!=Piece::EMPTY");
    }
}

static void board_remove_piece_test()
{
    const std::string test_name = "board_put_piece_test";

    Board board;

    board.put_piece(Piece::W_QUEEN, Square::SQ_A1);
    board.put_piece(Piece::B_QUEEN, Square::SQ_H8);
    board.put_piece(Piece::W_KNIGHT, Square::SQ_A8);
    board.put_piece(Piece::B_PAWN, Square::SQ_H1);
    board.put_piece(Piece::B_ROOK, Square::SQ_D4);

    board.remove_piece(Square::SQ_A1);
    board.remove_piece(Square::SQ_H8);
    board.remove_piece(Square::SQ_A8);
    board.remove_piece(Square::SQ_H1);
    board.remove_piece(Square::SQ_D4);

    if (board.get_piece(Square::SQ_A1) != Piece::EMPTY) {
        PRINT_TEST_FAILED(test_name, "get_piece(SQ_A1)!=Piece::EMPTY");
    }
    if (board.get_piece(Square::SQ_H8) != Piece::EMPTY) {
        PRINT_TEST_FAILED(test_name, "get_piece(SQ_H8)!=Piece::EMPTY");
    }
    if (board.get_piece(Square::SQ_A8) != Piece::EMPTY) {
        PRINT_TEST_FAILED(test_name, "get_piece(SQ_A8)!=Piece::EMPTY");
    }
    if (board.get_piece(Square::SQ_H1) != Piece::EMPTY) {
        PRINT_TEST_FAILED(test_name, "get_piece(SQ_H1)!=Piece::EMPTY");
    }
    if (board.get_piece(Square::SQ_D4) != Piece::EMPTY) {
        PRINT_TEST_FAILED(test_name, "get_piece(SQ_D4)!=Piece::EMPTY");
    }
}

static void board_clean_test()
{
    const std::string test_name = "board_clean_test";

    constexpr auto StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    Board board;

    board.load_fen(StartFEN);
    board.clean();

    for (Square sq = Square::SQ_A1; sq.is_valid(); sq++) {
        if (board.is_empty(sq) != true) {
            PRINT_TEST_FAILED(test_name, "is_empty(sq)!=true");
        }
    }

    if (board.get_bitboard_all() != 0ULL) {
        PRINT_TEST_FAILED(test_name, "get_bitboard_all() != 0ULL");
    }
    if (board.get_bitboard_black() != 0ULL) {
        PRINT_TEST_FAILED(test_name, "get_bitboard_black() != 0ULL");
    }
    if (board.get_bitboard_white() != 0ULL) {
        PRINT_TEST_FAILED(test_name, "get_bitboard_white() != 0ULL");
    }

    for (Piece p = Piece::W_PAWN; p < Piece::EMPTY; p = p + 1) {
        if (board.get_bitboard_piece(p) != 0ULL) {
            PRINT_TEST_FAILED(test_name, "get_bitboard_piece(p) != 0ULL");
        }
    }

    if (board.fen() != "8/8/8/8/8/8/8/8 w - - 0 1") {
        PRINT_TEST_FAILED(test_name, "fen() != 8/8/8/8/8/8/8/8 w - - 0 1");
    }
    if (board.state().side_to_move() != ChessColor::WHITE) {
        PRINT_TEST_FAILED(test_name, "state().side_to_move() != ChessColor::WHITE");
    }
    if (board.state().move_number() != 1) {
        PRINT_TEST_FAILED(test_name, "state().move_number() != 1");
    }
    if (board.state().fifty_move_rule_counter() != 0) {
        PRINT_TEST_FAILED(test_name, "state().fifty_move_rule_counter() != 0");
    }
    if (board.state().last_captured_piece() != PieceType::EMPTY) {
        PRINT_TEST_FAILED(test_name, "state().last_captured_piece() != EMPTY");
    }
    if (board.state().en_passant_square() != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "state().en_passant_square() != SQ_INVALID");
    }
    if (board.state().castle_king_black() != false) {
        PRINT_TEST_FAILED(test_name, "state().castle_king_black() != false");
    }
    if (board.state().castle_king_white() != false) {
        PRINT_TEST_FAILED(test_name, "state().castle_king_white() != false");
    }
    if (board.state().castle_queen_black() != false) {
        PRINT_TEST_FAILED(test_name, "state().castle_queen_black() != false");
    }
    if (board.state().castle_queen_white() != false) {
        PRINT_TEST_FAILED(test_name, "state().castle_queen_white() != false");
    }
}

static void board_make_unmake_move_test()
{
    const std::string test_name = "board_make_unmake_move_test";

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
        PRINT_TEST_FAILED(test_name, "fen() != EndFEN");
    }

    for (int32_t i = num_moves - 1; i >= 0; i--) {

        board.unmake_move(moves[i], game_states[i]);
    }

    if (board.fen() != StartFEN) {
        PRINT_TEST_FAILED(test_name, "fen() != StartFEN");
    }
}

static void board_fen_test()
{
    const std::string test_name = "board_fen_test";

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
        PRINT_TEST_FAILED(test_name, "fen() != StartFEN");
    }
    board.load_fen(EnPassantFEN);
    if (board.fen() != EnPassantFEN) {
        PRINT_TEST_FAILED(test_name, "fen() != EnPassantFEN");
    }
    board.load_fen(PromotionFEN);
    if (board.fen() != PromotionFEN) {
        PRINT_TEST_FAILED(test_name, "fen() != PromotionFEN");
    }
    board.load_fen(KiwipeteFEN);
    if (board.fen() != KiwipeteFEN) {
        PRINT_TEST_FAILED(test_name, "fen() != KiwipeteFEN");
    }
    board.load_fen(NoCastleFEN);
    if (board.fen() != NoCastleFEN) {
        PRINT_TEST_FAILED(test_name, "fen() != NoCastleFEN");
    }
    board.load_fen(CastleFEN1);
    if (board.fen() != CastleFEN1) {
        PRINT_TEST_FAILED(test_name, "fen() != CastleFEN1");
    }
    board.load_fen(CastleFEN2);
    if (board.fen() != CastleFEN2) {
        PRINT_TEST_FAILED(test_name, "fen() != CastleFEN2");
    }
}

static void board_initialization_test()
{
    const std::string test_name = "board_initialization_test";

    Board board;
    for (Square sq = Square::SQ_A1; sq < Square::SQ_INVALID; sq++) {

        if (board.is_empty(sq) != true) {
            PRINT_TEST_FAILED(test_name, "is_empty(sq) != true");
        }
        if (board.get_piece(sq) != Piece::EMPTY) {
            PRINT_TEST_FAILED(test_name, "get_piece(sq) != Piece::EMPTY");
        }
    }
    if (board.get_bitboard_all() != 0ULL) {
        PRINT_TEST_FAILED(test_name, "get_bitboard_all() != 0ULL");
    }
    if (board.get_bitboard_black() != 0ULL) {
        PRINT_TEST_FAILED(test_name, "get_bitboard_black() != 0ULL");
    }
    if (board.get_bitboard_white() != 0ULL) {
        PRINT_TEST_FAILED(test_name, "get_bitboard_white() != 0ULL");
    }

    for (Piece p = Piece::W_PAWN; p < Piece::EMPTY; p = p + 1) {
        if (board.get_bitboard_piece(p) != 0ULL) {
            PRINT_TEST_FAILED(test_name, "get_bitboard_piece(p) != 0ULL");
        }
    }
    if (board.fen() != "8/8/8/8/8/8/8/8 w - - 0 1") {
        PRINT_TEST_FAILED(test_name, "fen() != 8/8/8/8/8/8/8/8 w - - 0 1");
    }

    if (board.state().side_to_move() != ChessColor::WHITE) {
        PRINT_TEST_FAILED(test_name, "state().side_to_move() != ChessColor::WHITE");
    }
    if (board.state().move_number() != 1) {
        PRINT_TEST_FAILED(test_name, "state().move_number() != 1");
    }
    if (board.state().fifty_move_rule_counter() != 0) {
        PRINT_TEST_FAILED(test_name, "state().fifty_move_rule_counter() != 0");
    }
    if (board.state().last_captured_piece() != PieceType::EMPTY) {
        PRINT_TEST_FAILED(test_name, "state().last_captured_piece() != EMPTY");
    }
    if (board.state().en_passant_square() != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "state().en_passant_square() != SQ_INVALID");
    }
    if (board.state().castle_king_black() != false) {
        PRINT_TEST_FAILED(test_name, "state().castle_king_black() != false");
    }
    if (board.state().castle_king_white() != false) {
        PRINT_TEST_FAILED(test_name, "state().castle_king_white() != false");
    }
    if (board.state().castle_queen_black() != false) {
        PRINT_TEST_FAILED(test_name, "state().castle_queen_black() != false");
    }
    if (board.state().castle_queen_white() != false) {
        PRINT_TEST_FAILED(test_name, "state().castle_queen_white() != false");
    }
}