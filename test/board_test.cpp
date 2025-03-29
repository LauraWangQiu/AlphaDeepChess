#include "board.hpp"
#include "test_utils.hpp"
#include <stack>

static void board_get_piece_test();
static void board_is_empty_test();
static void board_get_bitboard_all_test();
static void board_get_bitboard_white_test();
static void board_get_bitboard_black_test();
static void board_get_bitboard_piece_test();
static void board_put_piece_test();
static void board_remove_piece_test();
static void board_set_side_to_move_test();
static void board_clean_test();
static void board_make_unmake_castling_move_test();
static void board_make_unmake_enPassant_move_test();
static void board_make_unmake_normal_move_test();
static void board_make_unmake_promotion_move_test();
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
    board_set_side_to_move_test();
    board_remove_piece_test();
    board_clean_test();
    board_make_unmake_castling_move_test();
    board_make_unmake_enPassant_move_test();
    board_make_unmake_normal_move_test();
    board_make_unmake_promotion_move_test();
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

    if (board.get_piece(Square::A1) != Piece::W_ROOK) {
        PRINT_TEST_FAILED(test_name, "get_piece(A1)!=Piece::W_ROOK");
    }
    if (board.get_piece(Square::A2) != Piece::W_PAWN) {
        PRINT_TEST_FAILED(test_name, "get_piece(A2)!=Piece::W_PAWN");
    }
    if (board.get_piece(Square::A3) != Piece::EMPTY) {
        PRINT_TEST_FAILED(test_name, "get_piece(A3)!=Piece::EMPTY");
    }
    if (board.get_piece(Square::A4) != Piece::EMPTY) {
        PRINT_TEST_FAILED(test_name, "get_piece(A4)!=Piece::EMPTY");
    }
    if (board.get_piece(Square::A5) != Piece::EMPTY) {
        PRINT_TEST_FAILED(test_name, "get_piece(A5)!=Piece::EMPTY");
    }
    if (board.get_piece(Square::A6) != Piece::EMPTY) {
        PRINT_TEST_FAILED(test_name, "get_piece(A6)!=Piece::EMPTY");
    }
    if (board.get_piece(Square::A7) != Piece::B_PAWN) {
        PRINT_TEST_FAILED(test_name, "get_piece(A7)!=Piece::B_PAWN");
    }
    if (board.get_piece(Square::A8) != Piece::B_ROOK) {
        PRINT_TEST_FAILED(test_name, "get_piece(A8)!=Piece::B_ROOK");
    }
}

static void board_is_empty_test()
{
    const std::string test_name = "board_is_empty_test";

    constexpr auto StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    Board board;
    board.load_fen(StartFEN);

    if (board.is_empty(Square::A1) != false) {
        PRINT_TEST_FAILED(test_name, "is_empty(A1)!=false");
    }
    if (board.is_empty(Square::A2) != false) {
        PRINT_TEST_FAILED(test_name, "is_empty(A2)!=false");
    }
    if (board.is_empty(Square::A3) != true) {
        PRINT_TEST_FAILED(test_name, "is_empty(A3)!=true");
    }
    if (board.is_empty(Square::A4) != true) {
        PRINT_TEST_FAILED(test_name, "is_empty(A4)!=true");
    }
    if (board.is_empty(Square::A5) != true) {
        PRINT_TEST_FAILED(test_name, "is_empty(A5)!=true");
    }
    if (board.is_empty(Square::A6) != true) {
        PRINT_TEST_FAILED(test_name, "is_empty(A6)!=true");
    }
    if (board.is_empty(Square::A7) != false) {
        PRINT_TEST_FAILED(test_name, "is_empty(A7)!=false");
    }
    if (board.is_empty(Square::A8) != false) {
        PRINT_TEST_FAILED(test_name, "is_empty(A8)!=false");
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

    board.put_piece(Piece::W_QUEEN, Square::A1);
    board.put_piece(Piece::B_QUEEN, Square::H8);
    board.put_piece(Piece::W_KNIGHT, Square::A8);
    board.put_piece(Piece::B_PAWN, Square::H1);
    board.put_piece(Piece::B_ROOK, Square::D4);

    if (board.get_piece(Square::A1) != Piece::W_QUEEN) {
        PRINT_TEST_FAILED(test_name, "get_piece(A1)!=Piece::W_QUEEN");
    }
    if (board.get_piece(Square::H8) != Piece::B_QUEEN) {
        PRINT_TEST_FAILED(test_name, "get_piece(H8)!=Piece::B_QUEEN");
    }
    if (board.get_piece(Square::A8) != Piece::W_KNIGHT) {
        PRINT_TEST_FAILED(test_name, "get_piece(A8)!=Piece::W_KNIGHT");
    }
    if (board.get_piece(Square::H1) != Piece::B_PAWN) {
        PRINT_TEST_FAILED(test_name, "get_piece(H1)!=Piece::B_PAWN");
    }
    if (board.get_piece(Square::D4) != Piece::B_ROOK) {
        PRINT_TEST_FAILED(test_name, "get_piece(D4)!=Piece::B_ROOK");
    }

    board.put_piece(Piece::EMPTY, Square::A1);

    if (board.get_piece(Square::A1) != Piece::EMPTY) {
        PRINT_TEST_FAILED(test_name, "get_piece(A1)!=Piece::EMPTY");
    }
}

static void board_remove_piece_test()
{
    const std::string test_name = "board_put_piece_test";

    Board board;

    board.put_piece(Piece::W_QUEEN, Square::A1);
    board.put_piece(Piece::B_QUEEN, Square::H8);
    board.put_piece(Piece::W_KNIGHT, Square::A8);
    board.put_piece(Piece::B_PAWN, Square::H1);
    board.put_piece(Piece::B_ROOK, Square::D4);

    board.remove_piece(Square::A1);
    board.remove_piece(Square::H8);
    board.remove_piece(Square::A8);
    board.remove_piece(Square::H1);
    board.remove_piece(Square::D4);

    if (board.get_piece(Square::A1) != Piece::EMPTY) {
        PRINT_TEST_FAILED(test_name, "get_piece(A1)!=Piece::EMPTY");
    }
    if (board.get_piece(Square::H8) != Piece::EMPTY) {
        PRINT_TEST_FAILED(test_name, "get_piece(H8)!=Piece::EMPTY");
    }
    if (board.get_piece(Square::A8) != Piece::EMPTY) {
        PRINT_TEST_FAILED(test_name, "get_piece(A8)!=Piece::EMPTY");
    }
    if (board.get_piece(Square::H1) != Piece::EMPTY) {
        PRINT_TEST_FAILED(test_name, "get_piece(H1)!=Piece::EMPTY");
    }
    if (board.get_piece(Square::D4) != Piece::EMPTY) {
        PRINT_TEST_FAILED(test_name, "get_piece(D4)!=Piece::EMPTY");
    }
}

static void board_set_side_to_move_test()
{
    const std::string test_name = "board_set_side_to_move_test";
    constexpr auto StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    Board board;
    board.load_fen(StartFEN);

    board.set_side_to_move(ChessColor::BLACK);

    if (board.state().side_to_move() != ChessColor::BLACK) {
        PRINT_TEST_FAILED(test_name, "board.state().side_to_move() != ChessColor::BLACK");
    }

    board.set_side_to_move(ChessColor::WHITE);

    if (board.state().side_to_move() != ChessColor::WHITE) {
        PRINT_TEST_FAILED(test_name, "board.state().side_to_move() != ChessColor::WHITE");
    }
}

static void board_clean_test()
{
    const std::string test_name = "board_clean_test";

    constexpr auto StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    Board board;

    board.load_fen(StartFEN);
    board.clean();

    for (Square sq = Square::A1; sq.is_valid(); sq++) {
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
    if (board.state().en_passant_square() != Square::INVALID) {
        PRINT_TEST_FAILED(test_name, "state().en_passant_square() != INVALID");
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
    std::stack<GameState> game_states;

    Move moves[num_moves] = {
        Move(Square::D2, Square::D4),   // 1. d4
        Move(Square::F7, Square::F5),   // 1... f5
        Move(Square::E2, Square::E4),   // 2. e4
        Move(Square::F5, Square::E4),   // 2... fxe4
        Move(Square::B1, Square::C3),   // 3. Nc3
        Move(Square::G8, Square::F6),   // 3... Nf6
        Move(Square::C1, Square::G5),   // 4. Bg5
        Move(Square::E7, Square::E6),   // 4... e6
        Move(Square::C3, Square::E4),   // 5. Nxe4
        Move(Square::F8, Square::E7),   // 5... Be7
        Move(Square::G5, Square::F6),   // 6. Bxf6
        Move(Square::E7, Square::F6),   // 6... Bxf6
        Move(Square::G1, Square::F3),   // 7. Nf3
        Move::castle_black_king(),            // 7... O-O
        Move(Square::F1, Square::D3),   // 8. Bd3
        Move(Square::B7, Square::B6),   // 8... b6
        Move(Square::F3, Square::E5),   // 9. Ne5
        Move(Square::C8, Square::B7),   // 9... Bb7
        Move(Square::D1, Square::H5),   // 10. Qh5
        Move(Square::D8, Square::E7),   // 10... Qe7
        Move(Square::H5, Square::H7),   // 11. Qxh7+
        Move(Square::G8, Square::H7),   // 11... Kxh7
        Move(Square::E4, Square::F6),   // 12. Nxf6+
        Move(Square::H7, Square::H6),   // 12... Kh6
        Move(Square::E5, Square::G4),   // 13. Ng4+
        Move(Square::H6, Square::G5),   // 13... Kg5
        Move(Square::H2, Square::H4),   // 14. h4+
        Move(Square::G5, Square::F4),   // 14... Kf4
        Move(Square::G2, Square::G3),   // 15. g3+
        Move(Square::F4, Square::F3),   // 15... Kf3
        Move(Square::D3, Square::E2),   // 16. Be2+
        Move(Square::F3, Square::G2),   // 16... Kg2
        Move(Square::H1, Square::H2),   // 17. Rh2+
        Move(Square::G2, Square::G1),   // 17... Kg1
        Move::castle_white_queen()            // 18. O-O-O
    };

    Board board;
    board.load_fen(StartFEN);
    game_states.push(board.state());

    for (uint32_t i = 0; i < num_moves; i++) {

        board.make_move(moves[i]);
        if (i < num_moves - 1) {
            game_states.push(board.state());
        }
    }

    if (board.fen() != EndFEN) {
        PRINT_TEST_FAILED(test_name, "fen() != EndFEN");
    }

    for (int32_t i = num_moves - 1; !game_states.empty(); i--) {

        board.unmake_move(moves[i], game_states.top());
        game_states.pop();
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
    for (Square sq = Square::A1; sq < Square::INVALID; sq++) {

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
    if (board.state().en_passant_square() != Square::INVALID) {
        PRINT_TEST_FAILED(test_name, "state().en_passant_square() != INVALID");
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

static void board_make_unmake_castling_move_test()
{
    const std::string test_name = "board_make_unmake_castling_move_test";

    Board board;
    const auto start_castling_white_fen = "r3k2r/8/8/3Pp3/8/8/8/R3K2R w KQkq e6 0 2";
    const auto end_fen_king_castle_white = "r3k2r/8/8/3Pp3/8/8/8/R4RK1 b kq - 1 2";
    const auto end_fen_queen_castle_white = "r3k2r/8/8/3Pp3/8/8/8/2KR3R b kq - 1 2";

    board.load_fen(start_castling_white_fen);
    const GameState start_castling_white_fen_state = board.state();

    // castle white king
    board.make_move(Move::castle_white_king());
    if (board.fen() != end_fen_king_castle_white) {
        PRINT_TEST_FAILED(test_name, "board.fen() != end_fen_king_castle_white");
    }
    board.unmake_move(Move::castle_white_king(), start_castling_white_fen_state);
    if (board.fen() != start_castling_white_fen) {
        PRINT_TEST_FAILED(test_name, "board.fen() != start_castling_white_fen");
    }

    // castle white queen
    board.make_move(Move::castle_white_queen());
    if (board.fen() != end_fen_queen_castle_white) {
        PRINT_TEST_FAILED(test_name, "board.fen() != end_fen_queen_castle_white");
    }
    board.unmake_move(Move::castle_white_queen(), start_castling_white_fen_state);
    if (board.fen() != start_castling_white_fen) {
        PRINT_TEST_FAILED(test_name, "board.fen() != start_castling_white_fen");
    }

    const auto start_castling_black_fen = "r3k2r/8/8/8/3Pp3/8/8/R3K2R b KQkq d3 0 1";
    const auto end_fen_king_castle_black = "r4rk1/8/8/8/3Pp3/8/8/R3K2R w KQ - 1 2";
    const auto end_fen_queen_castle_black = "2kr3r/8/8/8/3Pp3/8/8/R3K2R w KQ - 1 2";

    board.load_fen(start_castling_black_fen);
    const GameState start_castling_black_fen_state = board.state();

    // castle black king
    board.make_move(Move::castle_black_king());
    if (board.fen() != end_fen_king_castle_black) {
        PRINT_TEST_FAILED(test_name, "board.fen() != end_fen_king_castle_black");
    }
    board.unmake_move(Move::castle_black_king(), start_castling_black_fen_state);
    if (board.fen() != start_castling_black_fen) {
        PRINT_TEST_FAILED(test_name, "board.fen() != start_castling_black_fen");
    }

    // castle black queen
    board.make_move(Move::castle_black_queen());
    if (board.fen() != end_fen_queen_castle_black) {
        PRINT_TEST_FAILED(test_name, "board.fen() != end_fen_queen_castle_black");
    }
    board.unmake_move(Move::castle_black_queen(), start_castling_black_fen_state);
    if (board.fen() != start_castling_black_fen) {
        PRINT_TEST_FAILED(test_name, "board.fen() != start_castling_black_fen");
    }
}

static void board_make_unmake_enPassant_move_test()
{
    const std::string test_name = "board_make_unmake_enPassant_move_test";

    Board board;

    const auto start_enPassant_white_fen = "r3k2r/8/8/3Pp3/8/8/8/R3K2R w KQkq e6 0 3";
    const auto end_fen_enPassant_white = "r3k2r/8/4P3/8/8/8/8/R3K2R b KQkq - 0 3";

    board.load_fen(start_enPassant_white_fen);
    const GameState start_enPassant_white_fen_state = board.state();
    const Move enPassant_white_move(Square::D5, Square::E6, MoveType::EN_PASSANT);

    // enPassant white
    board.make_move(enPassant_white_move);

    if (board.fen() != end_fen_enPassant_white) {
        PRINT_TEST_FAILED(test_name, "board.fen() != end_fen_enPassant_white");
    }
    board.unmake_move(enPassant_white_move, start_enPassant_white_fen_state);
    if (board.fen() != start_enPassant_white_fen) {
        PRINT_TEST_FAILED(test_name, "board.fen() != start_enPassant_white_fen");
    }

    // enPassant black

    const auto start_enPassant_black_fen = "r3k2r/8/8/8/3Pp3/8/8/R3K2R b KQkq d3 0 2";
    const auto end_fen_enPassant_black = "r3k2r/8/8/8/8/3p4/8/R3K2R w KQkq - 0 3";

    board.load_fen(start_enPassant_black_fen);

    const GameState start_enPassant_black_fen_state = board.state();
    const Move enPassant_black_move(Square::E4, Square::D3, MoveType::EN_PASSANT);

    board.make_move(enPassant_black_move);

    if (board.fen() != end_fen_enPassant_black) {
        PRINT_TEST_FAILED(test_name, "board.fen() != end_fen_enPassant_black");
    }
    board.unmake_move(enPassant_black_move, start_enPassant_black_fen_state);
    if (board.fen() != start_enPassant_black_fen) {
        PRINT_TEST_FAILED(test_name, "board.fen() != start_enPassant_black_fen");
    }
}

static void board_make_unmake_normal_move_test()
{
    const std::string test_name = "board_make_unmake_normal_move_test";

    Board board;
    const auto start_white_fen = "r3k2r/1p6/8/3b1pP1/3B2p1/8/P4P2/R3K2R w KQkq f6 0 2";
    const auto start_black_fen = "r3k2r/1p3p2/8/3b2P1/3B1Pp1/8/P7/R3K2R b KQkq f3 0 1";

    //super normal move white
    board.load_fen(start_white_fen);
    const GameState start_white_fen_state = board.state();
    const Move normal_move_white(Square::H1, Square::H7);
    const auto end_normal_white_fen = "r3k2r/1p5R/8/3b1pP1/3B2p1/8/P4P2/R3K3 b Qkq - 1 2";

    board.make_move(normal_move_white);
    if (board.fen() != end_normal_white_fen) {
        PRINT_TEST_FAILED(test_name, "board.fen() != end_normal_white_fen");
    }
    board.unmake_move(normal_move_white, start_white_fen_state);
    if (board.fen() != start_white_fen) {
        PRINT_TEST_FAILED(test_name, "board.fen() != start_white_fen");
    }

    //super normal move black
    board.load_fen(start_black_fen);
    const GameState start_black_fen_state = board.state();
    const Move normal_move_black(Square::H8, Square::H2);
    const auto end_normal_black_fen = "r3k3/1p3p2/8/3b2P1/3B1Pp1/8/P6r/R3K2R w KQq - 1 2";

    board.make_move(normal_move_black);
    if (board.fen() != end_normal_black_fen) {
        PRINT_TEST_FAILED(test_name, "board.fen() != end_normal_black_fen");
    }
    board.unmake_move(normal_move_black, start_black_fen_state);
    if (board.fen() != start_black_fen) {
        PRINT_TEST_FAILED(test_name, "board.fen() != start_black_fen");
    }

    //move with capture white
    board.load_fen(start_white_fen);
    const Move capture_move_white(Square::D4, Square::H8);
    const auto end_capture_white_fen = "r3k2B/1p6/8/3b1pP1/6p1/8/P4P2/R3K2R b KQq - 0 2";

    board.make_move(capture_move_white);
    if (board.fen() != end_capture_white_fen) {
        PRINT_TEST_FAILED(test_name, "board.fen() != end_capture_white_fen");
    }
    board.unmake_move(capture_move_white, start_white_fen_state);
    if (board.fen() != start_white_fen) {
        PRINT_TEST_FAILED(test_name, "board.fen() != start_white_fen");
    }

    //move with capture black
    board.load_fen(start_black_fen);
    const Move capture_move_black(Square::D5, Square::H1);
    const auto end_capture_black_fen = "r3k2r/1p3p2/8/6P1/3B1Pp1/8/P7/R3K2b w Qkq - 0 2";

    board.make_move(capture_move_black);
    if (board.fen() != end_capture_black_fen) {
        PRINT_TEST_FAILED(test_name, "board.fen() != end_capture_black_fen");
    }
    board.unmake_move(capture_move_black, start_black_fen_state);
    if (board.fen() != start_black_fen) {
        PRINT_TEST_FAILED(test_name, "board.fen() != start_black_fen");
    }

    //double pawn push white
    board.load_fen(start_white_fen);
    const Move double_push_move_white(Square::A2, Square::A4);
    const auto end_double_push_white_fen = "r3k2r/1p6/8/3b1pP1/P2B2p1/8/5P2/R3K2R b KQkq - 0 2";

    board.make_move(double_push_move_white);
    if (board.fen() != end_double_push_white_fen) {
        PRINT_TEST_FAILED(test_name, "board.fen() != end_double_push_white_fen");
    }
    board.unmake_move(double_push_move_white, start_white_fen_state);
    if (board.fen() != start_white_fen) {
        PRINT_TEST_FAILED(test_name, "board.fen() != start_white_fen");
    }

    //double pawn push black
    board.load_fen(start_black_fen);
    const Move double_push_move_black(Square::B7, Square::B5);
    const auto end_double_push_black_fen = "r3k2r/5p2/8/1p1b2P1/3B1Pp1/8/P7/R3K2R w KQkq - 0 2";

    board.make_move(double_push_move_black);
    if (board.fen() != end_double_push_black_fen) {
        PRINT_TEST_FAILED(test_name, "board.fen() != end_double_push_black_fen");
    }
    board.unmake_move(double_push_move_black, start_black_fen_state);
    if (board.fen() != start_black_fen) {
        PRINT_TEST_FAILED(test_name, "board.fen() != start_black_fen");
    }
}

static void board_make_unmake_promotion_move_test()
{
    const std::string test_name = "board_make_unmake_promotion_move_test";
    Board board;

    //promotion knight move white
    const auto start_promo_white_fen = "4n3/3P3k/8/Pp6/1p6/8/P2p3K/4N3 w - b6 0 2";
    const auto end_promo_knight_white_fen = "3Nn3/7k/8/Pp6/1p6/8/P2p3K/4N3 b - - 0 2";

    const Move promo_move_knight_white(Square::D7, Square::D8, MoveType::PROMOTION,
                                       PieceType::KNIGHT);

    board.load_fen(start_promo_white_fen);
    const GameState start_white_fen_state = board.state();

    board.make_move(promo_move_knight_white);
    if (board.fen() != end_promo_knight_white_fen) {
        PRINT_TEST_FAILED(test_name, "board.fen() != start_promo_white_fen");
    }
    board.unmake_move(promo_move_knight_white, start_white_fen_state);
    if (board.fen() != start_promo_white_fen) {
        PRINT_TEST_FAILED(test_name, "board.fen() != start_promo_white_fen");
    }

    //promotion queen move black
    const auto start_promo_black_fen = "4n3/1p1P3k/8/P7/Pp6/8/3p3K/4N3 b - a3 0 1";
    const auto end_promo_queen_black_fen = "4n3/1p1P3k/8/P7/Pp6/8/7K/3qN3 w - - 0 2";

    const Move promo_move_queen_black(Square::D2, Square::D1, MoveType::PROMOTION,
                                      PieceType::QUEEN);

    board.load_fen(start_promo_black_fen);
    const GameState start_black_fen_state = board.state();

    board.make_move(promo_move_queen_black);
    if (board.fen() != end_promo_queen_black_fen) {
        PRINT_TEST_FAILED(test_name, "board.fen() != end_promo_queen_black_fen");
    }
    board.unmake_move(promo_move_queen_black, start_black_fen_state);
    if (board.fen() != start_promo_black_fen) {
        PRINT_TEST_FAILED(test_name, "board.fen() != start_promo_black_fen");
    }

    //promotion bishop with capture white
    const auto end_promo_bishop_white_fen = "4B3/7k/8/Pp6/1p6/8/P2p3K/4N3 b - - 0 2";
    const Move promo_move_bishop_white(Square::D7, Square::E8, MoveType::PROMOTION,
                                       PieceType::BISHOP);

    board.load_fen(start_promo_white_fen);

    board.make_move(promo_move_bishop_white);
    if (board.fen() != end_promo_bishop_white_fen) {
        PRINT_TEST_FAILED(test_name, "board.fen() != end_promo_bishop_white_fen");
    }
    board.unmake_move(promo_move_bishop_white, start_white_fen_state);
    if (board.fen() != start_promo_white_fen) {
        PRINT_TEST_FAILED(test_name, "board.fen() != start_promo_white_fen");
    }

    //promotion rook with capture
    const auto end_promo_rook_black_fen = "4n3/1p1P3k/8/P7/Pp6/8/7K/4r3 w - - 0 2";
    const Move promo_move_rook_black(Square::D2, Square::E1, MoveType::PROMOTION,
                                     PieceType::ROOK);

    board.load_fen(start_promo_black_fen);

    board.make_move(promo_move_rook_black);
    if (board.fen() != end_promo_rook_black_fen) {
        PRINT_TEST_FAILED(test_name, "board.fen() != end_promo_rook_black_fen");
    }
    board.unmake_move(promo_move_rook_black, start_black_fen_state);
    if (board.fen() != start_promo_black_fen) {
        PRINT_TEST_FAILED(test_name, "board.fen() != start_promo_black_fen");
    }
}