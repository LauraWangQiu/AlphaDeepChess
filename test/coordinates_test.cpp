#include "test_utils.hpp"
#include "coordinates.hpp"

static void is_valid_coord_test();
static void get_direction_test();
static void squares_in_same_diagonal_test();
static void squares_in_same_antidiagonal_test();
static void get_direction_mask_test();

void coordinates_test()
{
    std::cout << "---------coordinates test---------\n\n";

    is_valid_coord_test();
    get_direction_test();
    squares_in_same_diagonal_test();
    squares_in_same_antidiagonal_test();
    get_direction_mask_test();
}

static void is_valid_coord_test()
{
    const std::string test_name = "is_valid_coord_test";

    if (is_valid_coord(ROW_1, COL_A) != true) {
        PRINT_TEST_FAILED(test_name, "is_valid_coord(ROW_1, COL_A) != true");
    }
    if (is_valid_coord(ROW_8, COL_H) != true) {
        PRINT_TEST_FAILED(test_name, "is_valid_coord(ROW_8, COL_H) != true");
    }
    if (is_valid_coord((Row)(-1), COL_A) != false) {
        PRINT_TEST_FAILED(test_name, "is_valid_coord((Row)(-1), COL_A) != false");
    }
    if (is_valid_coord((Row)(8), COL_H) != false) {
        PRINT_TEST_FAILED(test_name, "is_valid_coord((Row)(8), COL_H) != false");
    }
    if (is_valid_coord(ROW_1, (Col)(-1)) != false) {
        PRINT_TEST_FAILED(test_name, "is_valid_coord(ROW_1, (Col)(-1)) != false");
    }
    if (is_valid_coord(ROW_8, (Col)(8)) != false) {
        PRINT_TEST_FAILED(test_name, "is_valid_coord(ROW_8, (Col)(8)) != false");
    }
}

static void get_direction_test()
{
    const std::string test_name = "get_direction_test_test";

    if (get_direction(ROW_4, COL_D, ROW_4, COL_H) != Direction::EAST) {
        PRINT_TEST_FAILED(test_name, "get_direction_test != EAST");
    }

    if (get_direction(ROW_4, COL_D, ROW_1, COL_G) != Direction::SOUTH_EAST) {
        PRINT_TEST_FAILED(test_name, "get_direction_test != SOUTH_EAST");
    }

    if (get_direction(ROW_4, COL_D, ROW_1, COL_D) != Direction::SOUTH) {
        PRINT_TEST_FAILED(test_name, "get_direction_test != SOUTH");
    }

    if (get_direction(ROW_4, COL_D, ROW_1, COL_A) != Direction::SOUTH_WEST) {
        PRINT_TEST_FAILED(test_name, "get_direction_test != SOUTH_WEST");
    }

    if (get_direction(ROW_4, COL_D, ROW_4, COL_A) != Direction::WEST) {
        PRINT_TEST_FAILED(test_name, "get_direction_test != WEST");
    }

    if (get_direction(ROW_4, COL_D, ROW_7, COL_A) != Direction::NORTH_WEST) {
        PRINT_TEST_FAILED(test_name, "get_direction_test != NORTH_WEST");
    }

    if (get_direction(ROW_4, COL_D, ROW_7, COL_D) != Direction::NORTH) {
        PRINT_TEST_FAILED(test_name, "get_direction_test != NORTH");
    }

    if (get_direction(ROW_4, COL_D, ROW_7, COL_G) != Direction::NORTH_EAST) {
        PRINT_TEST_FAILED(test_name, "get_direction_test != NORTH_EAST");
    }
}

static void squares_in_same_diagonal_test()
{
    const std::string test_name = "squares_in_same_diagonal_test";

    if (!squares_in_same_diagonal(Square::D4, Square::A1)) {
        PRINT_TEST_FAILED(test_name, "!squares_in_same_diagonal(Square::D4, Square::A1)");
    }
    if (!squares_in_same_diagonal(Square::D4, Square::H8)) {
        PRINT_TEST_FAILED(test_name, "!squares_in_same_diagonal(Square::D4, Square::H8)");
    }
    if (squares_in_same_diagonal(Square::D4, Square::H1)) {
        PRINT_TEST_FAILED(test_name, "squares_in_same_diagonal(Square::D4, Square::H1)");
    }
    if (squares_in_same_diagonal(Square::D4, Square::A8)) {
        PRINT_TEST_FAILED(test_name, "squares_in_same_diagonal(Square::D4, Square::A8)");
    }
}

static void squares_in_same_antidiagonal_test()
{
    const std::string test_name = "squares_in_same_antidiagonal_test";

    if (!squares_in_same_antidiagonal(Square::D4, Square::G1)) {
        PRINT_TEST_FAILED(test_name, "!squares_in_same_antidiagonal(Square::D4, Square::G1)");
    }
    if (!squares_in_same_antidiagonal(Square::D4, Square::A7)) {
        PRINT_TEST_FAILED(test_name, "!squares_in_same_antidiagonal(Square::D4, Square::A7)");
    }
    if (squares_in_same_antidiagonal(Square::D4, Square::A1)) {
        PRINT_TEST_FAILED(test_name, "squares_in_same_antidiagonal(Square::D4, Square::A1)");
    }
    if (squares_in_same_antidiagonal(Square::D4, Square::H8)) {
        PRINT_TEST_FAILED(test_name, "squares_in_same_antidiagonal(Square::D4, Square::H8)");
    }
}

static void get_direction_mask_test()
{
    const std::string test_name = "get_direction_mask_test";

    if (get_direction_mask(Square::A8, Square::H8) != ROW_8_MASK) {
        PRINT_TEST_FAILED(test_name,
                          "get_direction_mask(Square::A8, Square::H8) != ROW_8_MASK");
    }
    if (get_direction_mask(Square::D7, Square::D5) != COL_D_MASK) {
        PRINT_TEST_FAILED(test_name,
                          "get_direction_mask(Square::D7, Square::D5) != COL_D_MASK");
    }
    if (get_direction_mask(Square::D4, Square::A1) != DIAGONAL_7_MASK) {
        PRINT_TEST_FAILED(test_name,
                          "get_direction_mask(Square::D4, Square::A1) != DIAGONAL_7_MASK");
    }
    if (get_direction_mask(Square::D4, Square::A1) != DIAGONAL_7_MASK) {
        PRINT_TEST_FAILED(test_name,
                          "get_direction_mask(Square::D4, Square::A1) != DIAGONAL_7_MASK");
    }
    if (get_direction_mask(Square::D4, Square::G1) != ANTIDIAGONAL_6_MASK) {
        PRINT_TEST_FAILED(
            test_name, "get_direction_mask(Square::D4, Square::G1) != ANTIDIAGONAL_6_MASK");
    }
    if (get_direction_mask(Square::A8, Square::G6) != 0ULL) {
        PRINT_TEST_FAILED(test_name, "get_direction_mask(Square::A8, Square::G8) != 0ULL");
    }
}
