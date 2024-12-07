#include "test_utils.hpp"

#include "square.hpp"

static void square_constructor_test();
static void square_row_test();
static void square_col_test();
static void square_to_string_test();
static void square_value_test();
static void square_is_valid_test();
static void square_mask_test();
static void square_to_uint8_test();
static void square_PreIncrement_test();
static void square_PostIncrement_test();
static void square_Predecrement_test();
static void square_PostDecrement_test();
static void square_Assignment_test();
static void square_north_test();
static void square_south_test();
static void square_east_test();
static void square_west_test();
static void square_northEast_test();
static void square_northWest_test();
static void square_southEast_test();
static void square_southWest_test();
static void square_toDirection_test();

void square_test()
{
    std::cout << "---------square test---------\n\n";

    square_constructor_test();
    square_row_test();
    square_col_test();
    square_to_string_test();
    square_value_test();
    square_is_valid_test();
    square_mask_test();
    square_to_uint8_test();
    square_PreIncrement_test();
    square_PostIncrement_test();
    square_Predecrement_test();
    square_PostDecrement_test();
    square_Assignment_test();
    square_north_test();
    square_south_test();
    square_east_test();
    square_west_test();
    square_northEast_test();
    square_northWest_test();
    square_southEast_test();
    square_southWest_test();
    square_toDirection_test();
}


static void square_constructor_test()
{
    const std::string test_name = "square_constructor_test";

    Square square;

    if (square != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_INVALID");
    }

    Square other_sq(Square::SQ_A7);
    square = Square(other_sq);

    if (square != other_sq) {
        PRINT_TEST_FAILED(test_name, "square != other_sq");
    }

    square = Square(48U);

    if (square != Square::SQ_A7) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_A7");
    }

    square = Square(65U);

    if (square != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_INVALID");
    }

    square = Square(ROW_5, COL_C);

    if (square != Square::SQ_C5) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_C5");
    }

    square = Square("e2");

    if (square != Square::SQ_E2) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_E2");
    }

    square = Square("e9");

    if (square != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_INVALID");
    }

    square = Square("j6");

    if (square != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_INVALID");
    }
}

static void square_row_test()
{
    const std::string test_name = "square_row_test";

    Square square(Square::SQ_B5);

    if (square.row() != ROW_5) {
        PRINT_TEST_FAILED(test_name, "square.row() != ROW_5");
    }

    square = Square(Square::SQ_INVALID);

    if (square.row() != ROW_INVALID) {
        PRINT_TEST_FAILED(test_name, "square.row() != ROW_INVALID");
    }
}

static void square_col_test()
{
    const std::string test_name = "square_col_test";

    Square square(Square::SQ_B5);

    if (square.col() != COL_B) {
        PRINT_TEST_FAILED(test_name, "square.col() != COL_B");
    }

    square = Square(Square::SQ_INVALID);

    if (square.col() != COL_INVALID) {
        PRINT_TEST_FAILED(test_name, "square.col() != COL_INVALID");
    }
}

static void square_to_string_test()
{
    const std::string test_name = "square_to_string_test";

    Square square(Square::SQ_B5);

    if (square.to_string() != "b5") {
        PRINT_TEST_FAILED(test_name, "square.to_string() != b5");
    }

    square = Square(Square::SQ_INVALID);

    if (square.to_string() != "invalid") {
        PRINT_TEST_FAILED(test_name, "square.to_string() != invalid");
    }
}

static void square_value_test()
{
    const std::string test_name = "square_value_test";

    Square square(Square::SQ_B5);

    if (square.value() != 33) {
        PRINT_TEST_FAILED(test_name, "square.value() != 33");
    }
}

static void square_is_valid_test()
{
    const std::string test_name = "square_is_valid_test";

    Square square(Square::SQ_B5);

    if (square.is_valid() != true) {
        PRINT_TEST_FAILED(test_name, "square.is_valid() != true");
    }

    square = Square(Square::SQ_INVALID);

    if (square.is_valid() != false) {
        PRINT_TEST_FAILED(test_name, "square.is_valid() != false");
    }
}

static void square_mask_test()
{
    const std::string test_name = "square_mask_test";

    Square square(Square::SQ_B5);

    if (square.mask() != (1ULL << 33ULL)) {
        PRINT_TEST_FAILED(test_name, "square.mask() != (1ULL << 33ULL)");
    }
}

static void square_to_uint8_test()
{
    const std::string test_name = "square_to_uint8_test";

    Square square(Square::SQ_B5);

    if ((uint8_t)square != 33U) {
        PRINT_TEST_FAILED(test_name, "(uint8_t)square != 33U");
    }
}

static void square_PreIncrement_test()
{
    const std::string test_name = "square_PreIncrement_test";

    Square square(Square::SQ_B5);

    if (++square != Square::SQ_C5) {
        PRINT_TEST_FAILED(test_name, "++square != Square::SQ_C5");
    }

    square = Square(Square::SQ_INVALID);

    if (++square != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "++square != Square::SQ_INVALID");
    }
}

static void square_PostIncrement_test()
{
    const std::string test_name = "square_PostIncrement_test";

    Square square(Square::SQ_B5);

    if (square++ != Square::SQ_B5) {
        PRINT_TEST_FAILED(test_name, "square++ != Square::SQ_B5");
    }
    if (square != Square::SQ_C5) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_C5");
    }

    square = Square(Square::SQ_INVALID);

    if (square++ != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "square++ != Square::SQ_INVALID");
    }
    if (square != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_INVALID");
    }
}

static void square_Predecrement_test()
{
    const std::string test_name = "square_Predecrement_test";

    Square square(Square::SQ_B5);

    if (--square != Square::SQ_A5) {
        PRINT_TEST_FAILED(test_name, "--square != Square::SQ_A5");
    }

    square = Square(Square::SQ_A1);

    if (--square != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "--square != Square::SQ_INVALID");
    }
}

static void square_PostDecrement_test()
{
    const std::string test_name = "square_PostDecrement_test";

    Square square(Square::SQ_B5);

    if (square-- != Square::SQ_B5) {
        PRINT_TEST_FAILED(test_name, "square-- != Square::SQ_B5");
    }
    if (square != Square::SQ_A5) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_A5");
    }

    square = Square(Square::SQ_A1);

    if (square-- != Square::SQ_A1) {
        PRINT_TEST_FAILED(test_name, "square-- != Square::SQ_A1");
    }
    if (square != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_INVALID");
    }
}

static void square_Assignment_test()
{
    const std::string test_name = "square_Assignment_test";

    Square square;
    Square other_sq(Square::SQ_A6);

    square = other_sq;
    if (square != other_sq) {
        PRINT_TEST_FAILED(test_name, "square != other_sq");
    }
}

static void square_north_test()
{
    const std::string test_name = "square_north_test";

    Square square(Square::SQ_A6);

    if (square.north() != Square::SQ_A7) {
        PRINT_TEST_FAILED(test_name, "square.north() != Square::SQ_A7");
    }

    square = Square(Square::SQ_A8);

    if (square.north() != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "square.north() != Square::SQ_INVALID");
    }
}

static void square_south_test()
{
    const std::string test_name = "square_south_test";

    Square square(Square::SQ_A6);

    if (square.south() != Square::SQ_A5) {
        PRINT_TEST_FAILED(test_name, "square.south() != Square::SQ_A5");
    }

    square = Square(Square::SQ_A1);

    if (square.south() != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "square.south() != Square::SQ_INVALID");
    }
}

static void square_east_test()
{
    const std::string test_name = "square_east_test";

    Square square(Square::SQ_A6);

    if (square.east() != Square::SQ_B6) {
        PRINT_TEST_FAILED(test_name, "square.east() != Square::SQ_B6");
    }

    square = Square(Square::SQ_H8);

    if (square.east() != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "square.east() != Square::SQ_INVALID");
    }
}

static void square_west_test()
{
    const std::string test_name = "square_west_test";

    Square square(Square::SQ_H6);

    if (square.west() != Square::SQ_G6) {
        PRINT_TEST_FAILED(test_name, "square.west() != Square::SQ_G6");
    }

    square = Square(Square::SQ_A8);

    if (square.west() != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "square.west() != Square::SQ_INVALID");
    }
}

static void square_northEast_test()
{
    const std::string test_name = "square_northEast_test";

    Square square(Square::SQ_A6);

    if (square.northEast() != Square::SQ_B7) {
        PRINT_TEST_FAILED(test_name, "square.northEast() != Square::SQ_B7");
    }

    square = Square(Square::SQ_H7);

    if (square.northEast() != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "square.northEast() != Square::SQ_INVALID");
    }

    square = Square(Square::SQ_G8);

    if (square.northEast() != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "square.northEast() != Square::SQ_INVALID");
    }
}

static void square_northWest_test()
{
    const std::string test_name = "square_northWest_test";

    Square square(Square::SQ_B6);

    if (square.northWest() != Square::SQ_A7) {
        PRINT_TEST_FAILED(test_name, "square.northWest() != Square::SQ_A7");
    }

    square = Square(Square::SQ_A7);

    if (square.northWest() != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "square.northWest() != Square::SQ_INVALID");
    }

    square = Square(Square::SQ_G8);

    if (square.northWest() != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "square.northWest() != Square::SQ_INVALID");
    }
}

static void square_southEast_test()
{
    const std::string test_name = "square_southEast_test";

    Square square(Square::SQ_A6);

    if (square.southEast() != Square::SQ_B5) {
        PRINT_TEST_FAILED(test_name, "square.southEast() != Square::SQ_B5");
    }

    square = Square(Square::SQ_H7);

    if (square.southEast() != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "square.southEast() != Square::SQ_INVALID");
    }

    square = Square(Square::SQ_G1);

    if (square.southEast() != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "square.southEast() != Square::SQ_INVALID");
    }
}

static void square_southWest_test()
{
    const std::string test_name = "square_southWest_test";

    Square square(Square::SQ_B6);

    if (square.southWest() != Square::SQ_A5) {
        PRINT_TEST_FAILED(test_name, "square.southWest() != Square::SQ_A5");
    }

    square = Square(Square::SQ_A7);

    if (square.southWest() != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "square.southWest() != Square::SQ_INVALID");
    }

    square = Square(Square::SQ_B1);

    if (square.southWest() != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "square.southWest() != Square::SQ_INVALID");
    }
}

static void square_toDirection_test()
{
    const std::string test_name = "square_toDirection_test";

    Square square;

    // H8

    square = Square(Square::SQ_H8);
    square.to_direction(Direction::NORTH);

    if (square != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_INVALID");
    }

    square = Square(Square::SQ_H8);
    square.to_direction(Direction::NORTH_EAST);

    if (square != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_INVALID");
    }

    square = Square(Square::SQ_H8);
    square.to_direction(Direction::EAST);

    if (square != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_INVALID");
    }

    square = Square(Square::SQ_H8);
    square.to_direction(Direction::SOUTH_EAST);

    if (square != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_INVALID");
    }

    square = Square(Square::SQ_H8);
    square.to_direction(Direction::SOUTH);

    if (square != Square::SQ_H7) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_H7");
    }

    square = Square(Square::SQ_H8);
    square.to_direction(Direction::SOUTH_WEST);

    if (square != Square::SQ_G7) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_G7");
    }

    square = Square(Square::SQ_H8);
    square.to_direction(Direction::WEST);

    if (square != Square::SQ_G8) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_G8");
    }

    square = Square(Square::SQ_H8);
    square.to_direction(Direction::NORTH_WEST);

    if (square != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_INVALID");
    }

    square = Square(Square::SQ_H8);
    square.to_direction(Direction::NORTH_EAST);

    if (square != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_INVALID");
    }

    // A1

    square = Square(Square::SQ_A1);
    square.to_direction(Direction::NORTH);

    if (square != Square::SQ_A2) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_A2");
    }

    square = Square(Square::SQ_A1);
    square.to_direction(Direction::NORTH_EAST);

    if (square != Square::SQ_B2) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_B2");
    }

    square = Square(Square::SQ_A1);
    square.to_direction(Direction::EAST);

    if (square != Square::SQ_B1) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_B1");
    }

    square = Square(Square::SQ_A1);
    square.to_direction(Direction::SOUTH_EAST);

    if (square != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_INVALID");
    }

    square = Square(Square::SQ_A1);
    square.to_direction(Direction::SOUTH);

    if (square != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_INVALID");
    }

    square = Square(Square::SQ_A1);
    square.to_direction(Direction::SOUTH_WEST);

    if (square != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_INVALID");
    }

    square = Square(Square::SQ_A1);
    square.to_direction(Direction::WEST);

    if (square != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_INVALID");
    }

    square = Square(Square::SQ_A1);
    square.to_direction(Direction::NORTH_WEST);

    if (square != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_INVALID");
    }

    // d4

    square = Square(Square::SQ_D4);
    square.to_direction(NORTH);

    if (square != Square::SQ_D5) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_D5");
    }

    square.to_direction(NORTH_EAST);

    if (square != Square::SQ_E6) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_E6");
    }

    square.to_direction(EAST);

    if (square != Square::SQ_F6) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_F6");
    }

    square.to_direction(SOUTH_EAST);

    if (square != Square::SQ_G5) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_G5");
    }

    square.to_direction(SOUTH);

    if (square != Square::SQ_G4) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_G4");
    }

    square.to_direction(SOUTH_WEST);

    if (square != Square::SQ_F3) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_F3");
    }

    square.to_direction(WEST);

    if (square != Square::SQ_E3) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_E3");
    }

    square.to_direction(NORTH_WEST);

    if (square != Square::SQ_D4) {
        PRINT_TEST_FAILED(test_name, "square != Square::SQ_D4");
    }
}