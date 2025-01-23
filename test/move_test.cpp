#include "test_utils.hpp"
#include "move.hpp"

static void move_constructor_test();
static void move_square_from_to_test();
static void move_type_test();
static void move_promotion_piece_test();
static void move_is_valid_test();
static void move_null_test();
static void move_castle_white_king_test();
static void move_castle_white_queen_test();
static void move_castle_black_king_test();
static void move_castle_black_queen_test();
static void move_to_string_test();

void move_test()
{
    std::cout << "---------move test---------\n\n";

    move_constructor_test();
    move_square_from_to_test();
    move_type_test();
    move_promotion_piece_test();
    move_is_valid_test();
    move_null_test();
    move_castle_white_king_test();
    move_castle_white_queen_test();
    move_castle_black_king_test();
    move_castle_black_queen_test();
    move_to_string_test();
}


static void move_constructor_test()
{
    const std::string test_name = "move_constructor_test";

    Move move;

    if (move.is_valid() != false) {
        PRINT_TEST_FAILED(test_name, "move.is_valid() != false");
    }

    move = Move(Square::SQ_B2, Square::SQ_B4);

    if (move.square_from() != Square::SQ_B2) {
        PRINT_TEST_FAILED(test_name, "move.square_from() != Square::SQ_B2");
    }
    if (move.square_to() != Square::SQ_B4) {
        PRINT_TEST_FAILED(test_name, "move.square_to() != Square::SQ_B4");
    }
    if (move.type() != MoveType::NORMAL) {
        PRINT_TEST_FAILED(test_name, "move.type() != MoveType::NORMAL");
    }


    move = Move(Square::SQ_D2, Square::SQ_D1, MoveType::PROMOTION, PieceType::QUEEN);

    if (move.square_from() != Square::SQ_D2) {
        PRINT_TEST_FAILED(test_name, "move.square_from() != Square::SQ_D2");
    }
    if (move.square_to() != Square::SQ_D1) {
        PRINT_TEST_FAILED(test_name, "move.square_to() != Square::SQ_D1");
    }
    if (move.type() != MoveType::PROMOTION) {
        PRINT_TEST_FAILED(test_name, "move.type() != MoveType::PROMOTION");
    }
    if (move.promotion_piece() != PieceType::QUEEN) {
        PRINT_TEST_FAILED(test_name, "move.promotion_piece() != PieceType::QUEEN");
    }

    move = Move(Square::SQ_D5, Square::SQ_E6, MoveType::EN_PASSANT);

    if (move.square_from() != Square::SQ_D5) {
        PRINT_TEST_FAILED(test_name, "move.square_from() != Square::SQ_D5");
    }
    if (move.square_to() != Square::SQ_E6) {
        PRINT_TEST_FAILED(test_name, "move.square_to() != Square::SQ_E6");
    }
    if (move.type() != MoveType::EN_PASSANT) {
        PRINT_TEST_FAILED(test_name, "move.type() != MoveType::EN_PASSANT");
    }
}

static void move_square_from_to_test()
{
    const std::string test_name = "move_square_from_to_test";

    Move move = Move(Square::SQ_B2, Square::SQ_B4);

    if (move.square_from() != Square::SQ_B2) {
        PRINT_TEST_FAILED(test_name, "move.square_from() != Square::SQ_B2");
    }
    if (move.square_to() != Square::SQ_B4) {
        PRINT_TEST_FAILED(test_name, "move.square_to() != Square::SQ_B4");
    }

    move = Move(Square::SQ_D2, Square::SQ_D1, MoveType::PROMOTION, PieceType::QUEEN);

    if (move.square_from() != Square::SQ_D2) {
        PRINT_TEST_FAILED(test_name, "move.square_from() != Square::SQ_D2");
    }
    if (move.square_to() != Square::SQ_D1) {
        PRINT_TEST_FAILED(test_name, "move.square_to() != Square::SQ_D1");
    }

    move = Move(Square::SQ_D5, Square::SQ_E6, MoveType::EN_PASSANT);

    if (move.square_from() != Square::SQ_D5) {
        PRINT_TEST_FAILED(test_name, "move.square_from() != Square::SQ_D5");
    }
    if (move.square_to() != Square::SQ_E6) {
        PRINT_TEST_FAILED(test_name, "move.square_to() != Square::SQ_E6");
    }

    move = Move::castle_black_king();

    if (move.square_from() != Square::SQ_E8) {
        PRINT_TEST_FAILED(test_name, "move.square_from() != Square::SQ_E8");
    }
    if (move.square_to() != Square::SQ_G8) {
        PRINT_TEST_FAILED(test_name, "move.square_to() != Square::SQ_G8");
    }

    move = Move::castle_black_queen();

    if (move.square_from() != Square::SQ_E8) {
        PRINT_TEST_FAILED(test_name, "move.square_from() != Square::SQ_E8");
    }
    if (move.square_to() != Square::SQ_C8) {
        PRINT_TEST_FAILED(test_name, "move.square_to() != Square::SQ_C8");
    }

    move = Move::castle_white_king();

    if (move.square_from() != Square::SQ_E1) {
        PRINT_TEST_FAILED(test_name, "move.square_from() != Square::SQ_E1");
    }
    if (move.square_to() != Square::SQ_G1) {
        PRINT_TEST_FAILED(test_name, "move.square_to() != Square::SQ_G1");
    }

    move = Move::castle_white_queen();

    if (move.square_from() != Square::SQ_E1) {
        PRINT_TEST_FAILED(test_name, "move.square_from() != Square::SQ_E1");
    }
    if (move.square_to() != Square::SQ_C1) {
        PRINT_TEST_FAILED(test_name, "move.square_to() != Square::SQ_C1");
    }
}

static void move_type_test()
{
    const std::string test_name = "move_type_test";

    Move move = Move(Square::SQ_B2, Square::SQ_B4);

    if (move.type() != MoveType::NORMAL) {
        PRINT_TEST_FAILED(test_name, "move(B2,B4).type() != MoveType::NORMAL");
    }

    move = Move(Square::SQ_D2, Square::SQ_D1, MoveType::PROMOTION, PieceType::QUEEN);

    if (move.type() != MoveType::PROMOTION) {
        PRINT_TEST_FAILED(test_name, "move(SQ_D2,SQ_D1, PROMOTION, QUEEN).type() != PROMOTION");
    }

    move = Move(Square::SQ_D5, Square::SQ_E6, MoveType::EN_PASSANT);

    if (move.type() != MoveType::EN_PASSANT) {
        PRINT_TEST_FAILED(test_name, "move(SQ_D5,SQ_E6, EN_PASSANT).type() != EN_PASSANT");
    }

    if (Move::castle_black_king().type() != MoveType::CASTLING) {
        PRINT_TEST_FAILED(test_name, "castle_black_king().type() != MoveType::CASTLING");
    }

    if (Move::castle_black_queen().type() != MoveType::CASTLING) {
        PRINT_TEST_FAILED(test_name, "castle_black_queen().type() != MoveType::CASTLING");
    }

    if (Move::castle_white_king().type() != MoveType::CASTLING) {
        PRINT_TEST_FAILED(test_name, "castle_white_king().type() != MoveType::CASTLING");
    }

    if (Move::castle_white_queen().type() != MoveType::CASTLING) {
        PRINT_TEST_FAILED(test_name, "castle_white_queen().type() != MoveType::CASTLING");
    }
}

static void move_promotion_piece_test()
{
    const std::string test_name = "move_promotion_piece_test";

    Move move = Move(Square::SQ_D2, Square::SQ_D1, MoveType::PROMOTION, PieceType::QUEEN);

    if (move.promotion_piece() != PieceType::QUEEN) {
        PRINT_TEST_FAILED(test_name, "move.promotion_piece() != PieceType::QUEEN");
    }

    move = Move(Square::SQ_D2, Square::SQ_D1, MoveType::PROMOTION, PieceType::ROOK);

    if (move.promotion_piece() != PieceType::ROOK) {
        PRINT_TEST_FAILED(test_name, "move.promotion_piece() != PieceType::ROOK");
    }

    move = Move(Square::SQ_D2, Square::SQ_D1, MoveType::PROMOTION, PieceType::BISHOP);

    if (move.promotion_piece() != PieceType::BISHOP) {
        PRINT_TEST_FAILED(test_name, "move.promotion_piece() != PieceType::BISHOP");
    }

    move = Move(Square::SQ_D2, Square::SQ_D1, MoveType::PROMOTION, PieceType::KNIGHT);

    if (move.promotion_piece() != PieceType::KNIGHT) {
        PRINT_TEST_FAILED(test_name, "move.promotion_piece() != PieceType::KNIGHT");
    }
}

static void move_is_valid_test()
{
    const std::string test_name = "move_is_valid_test";

    if (Move::null().is_valid() != false) {
        PRINT_TEST_FAILED(test_name, "Move::null().is_valid() != false");
    }

    if (Move(Square::SQ_A5, Square::SQ_A5).is_valid() != false) {
        PRINT_TEST_FAILED(test_name, "Move(SQ_A5,SQ_A5).is_valid() != false");
    }

    if (Move(Square::SQ_A5, Square::SQ_A6).is_valid() != true) {
        PRINT_TEST_FAILED(test_name, "Move(SQ_A5,SQ_A6).is_valid() != true");
    }
}

static void move_null_test()
{
    const std::string test_name = "move_null_test";

    if (Move::null() != Move(0)) {
        PRINT_TEST_FAILED(test_name, "Move::null() != Move(0)");
    }
}

static void move_castle_white_king_test()
{
    const std::string test_name = "move_castle_white_king_test";

    if (Move::castle_white_king() != Move(Square::SQ_E1, Square::SQ_G1, MoveType::CASTLING)) {
        PRINT_TEST_FAILED(test_name, "castle_white_king()");
    }
}

static void move_castle_white_queen_test()
{
    const std::string test_name = "move_castle_white_queen_test";

    if (Move::castle_white_queen() != Move(Square::SQ_E1, Square::SQ_C1, MoveType::CASTLING)) {
        PRINT_TEST_FAILED(test_name, "castle_white_queen()");
    }
}

static void move_castle_black_king_test()
{
    const std::string test_name = "move_castle_black_king_test";

    if (Move::castle_black_king() != Move(Square::SQ_E8, Square::SQ_G8, MoveType::CASTLING)) {
        PRINT_TEST_FAILED(test_name, "castle_black_king()");
    }
}

static void move_castle_black_queen_test()
{
    const std::string test_name = "move_castle_black_queen_test";

    if (Move::castle_black_queen() != Move(Square::SQ_E8, Square::SQ_C8, MoveType::CASTLING)) {
        PRINT_TEST_FAILED(test_name, "castle_black_queen()");
    }
}

static void move_to_string_test()
{
    const std::string test_name = "move_to_string_test";

    if (Move(Square::SQ_B2, Square::SQ_B4).to_string() != "b2b4") {
        PRINT_TEST_FAILED(test_name, "Move(SQ_B2, SQ_B4).to_string() != b2b4");
    }

    Move move = Move(Square::SQ_D2, Square::SQ_D1, MoveType::PROMOTION, PieceType::QUEEN);

    if (move.to_string() != "d2d1q") {
        PRINT_TEST_FAILED(test_name, "Move(SQ_D2, SQ_D1, PROMOTION, QUEEN).to_string() != d2d1q");
    }

    move = Move(Square::SQ_D5, Square::SQ_E6, MoveType::EN_PASSANT);

    if (move.to_string() != "d5e6") {
        PRINT_TEST_FAILED(test_name, "Move(SQ_D5, SQ_E6, EN_PASSANT).to_string() != d5e6");
    }

    if (Move::castle_black_king().to_string() != "e8g8") {
        PRINT_TEST_FAILED(test_name, "Move::castle_black_king().to_string() != e8g8");
    }

    if (Move::castle_black_queen().to_string() != "e8c8") {
        PRINT_TEST_FAILED(test_name, "Move::castle_black_queen().to_string() != e8c8");
    }

    if (Move::castle_white_king().to_string() != "e1g1") {
        PRINT_TEST_FAILED(test_name, "Move::castle_white_king().to_string() != e1g1");
    }

    if (Move::castle_white_queen().to_string() != "e1c1") {
        PRINT_TEST_FAILED(test_name, "Move::castle_white_queen().to_string() != e1c1");
    }

    if (Move::null().to_string() != "invalid") {
        PRINT_TEST_FAILED(test_name, "Move::null().to_string() != invalid");
    }
}
