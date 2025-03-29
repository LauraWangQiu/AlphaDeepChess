#include "test_utils.hpp"
#include "move_list.hpp"

static void move_list_constructor_test();
static void move_list_add_test();
static void move_list_clear_test();
static void move_list_size_test();
static void move_list_get_test();
static void move_list_to_string_test();

void move_list_test()
{
    std::cout << "---------move_list test---------\n\n";

    move_list_constructor_test();
    move_list_add_test();
    move_list_clear_test();
    move_list_size_test();
    move_list_get_test();
    move_list_to_string_test();
}


static void move_list_constructor_test()
{
    const std::string test_name = "move_list_constructor_test";

    MoveList moves;

    if (moves.size() != 0) {
        PRINT_TEST_FAILED(test_name, "moves.size() != 0");
    }
}

static void move_list_add_test()
{
    const std::string test_name = "move_list_add_test";
    MoveList moves;

    moves.add(Move(Square::B2, Square::B4));

    if (moves.size() != 1) {
        PRINT_TEST_FAILED(test_name, "moves.size() != 1");
    }
    if (moves.get(0) != Move(Square::B2, Square::B4)) {
        PRINT_TEST_FAILED(test_name, "moves.get(0) != Move(Square::B2, Square::B4)");
    }
}

static void move_list_clear_test()
{
    const std::string test_name = "move_list_clear_test";

    MoveList moves;

    moves.add(Move(Square::B2, Square::B4));

    moves.clear();

    if (moves.size() != 0) {
        PRINT_TEST_FAILED(test_name, "moves.size() != 0");
    }
}
static void move_list_size_test()
{
    const std::string test_name = "move_list_size_test";

    MoveList moves;

    if (moves.size() != 0) {
        PRINT_TEST_FAILED(test_name, "moves.size() != 0");
    }

    moves.add(Move(Square::B2, Square::B4));

    if (moves.size() != 1) {
        PRINT_TEST_FAILED(test_name, "moves.size() != 1");
    }
}
static void move_list_get_test()
{
    const std::string test_name = "move_list_get_test";
    MoveList moves;

    moves.add(Move(Square::B2, Square::B4));

    if (moves.get(0) != Move(Square::B2, Square::B4)) {
        PRINT_TEST_FAILED(test_name, "moves.get(0) != Move(Square::B2, Square::B4)");
    }
}

static void move_list_to_string_test()
{
    const std::string test_name = "move_list_to_string_test";

    MoveList moves;

    moves.add(Move(Square::B2, Square::B4));
    moves.add(Move(Square::D2, Square::D1, MoveType::PROMOTION, PieceType::QUEEN));
    moves.add(Move::castle_white_queen());

    if (moves.to_string() != "b2b4:\nd2d1q:\ne1c1:\n") {
        PRINT_TEST_FAILED(test_name, "to_string() != b2b4:\nd2d1q:\ne1c1:");
    }
}
