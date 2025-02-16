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
static void square_diagonal_test();
static void square_antidiagonal_test();

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
    square_diagonal_test();
    square_antidiagonal_test();
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

static void square_diagonal_test()
{
    const std::string test_name = "square_diagonal_test";

    static Diagonal SQUARE_TO_DIAGONAL[64] = {
        DIAGONAL_7_A1_H8,  DIAGONAL_6_B1_H7,  DIAGONAL_5_C1_H6,  DIAGONAL_4_D1_H5,
        DIAGONAL_3_E1_H4,  DIAGONAL_2_F1_H3,  DIAGONAL_1_G1_H2,  DIAGONAL_0_H1_H1,
        DIAGONAL_8_A2_G8,  DIAGONAL_7_A1_H8,  DIAGONAL_6_B1_H7,  DIAGONAL_5_C1_H6,
        DIAGONAL_4_D1_H5,  DIAGONAL_3_E1_H4,  DIAGONAL_2_F1_H3,  DIAGONAL_1_G1_H2,
        DIAGONAL_9_A3_F8,  DIAGONAL_8_A2_G8,  DIAGONAL_7_A1_H8,  DIAGONAL_6_B1_H7,
        DIAGONAL_5_C1_H6,  DIAGONAL_4_D1_H5,  DIAGONAL_3_E1_H4,  DIAGONAL_2_F1_H3,
        DIAGONAL_10_A4_E8, DIAGONAL_9_A3_F8,  DIAGONAL_8_A2_G8,  DIAGONAL_7_A1_H8,
        DIAGONAL_6_B1_H7,  DIAGONAL_5_C1_H6,  DIAGONAL_4_D1_H5,  DIAGONAL_3_E1_H4,
        DIAGONAL_11_A5_D8, DIAGONAL_10_A4_E8, DIAGONAL_9_A3_F8,  DIAGONAL_8_A2_G8,
        DIAGONAL_7_A1_H8,  DIAGONAL_6_B1_H7,  DIAGONAL_5_C1_H6,  DIAGONAL_4_D1_H5,
        DIAGONAL_12_A6_C8, DIAGONAL_11_A5_D8, DIAGONAL_10_A4_E8, DIAGONAL_9_A3_F8,
        DIAGONAL_8_A2_G8,  DIAGONAL_7_A1_H8,  DIAGONAL_6_B1_H7,  DIAGONAL_5_C1_H6,
        DIAGONAL_13_A7_B8, DIAGONAL_12_A6_C8, DIAGONAL_11_A5_D8, DIAGONAL_10_A4_E8,
        DIAGONAL_9_A3_F8,  DIAGONAL_8_A2_G8,  DIAGONAL_7_A1_H8,  DIAGONAL_6_B1_H7,
        DIAGONAL_14_A8_A8, DIAGONAL_13_A7_B8, DIAGONAL_12_A6_C8, DIAGONAL_11_A5_D8,
        DIAGONAL_10_A4_E8, DIAGONAL_9_A3_F8,  DIAGONAL_8_A2_G8,  DIAGONAL_7_A1_H8};

    for (Square sq = Square::SQ_A1; sq.is_valid(); sq++) {
        if (sq.diagonal() != SQUARE_TO_DIAGONAL[sq]) {
            PRINT_TEST_FAILED(test_name, "sq.diagonal() != SQUARE_TO_DIAGONAL[sq]");
        }
    }
}

static void square_antidiagonal_test()
{
    const std::string test_name = "square_antidiagonal_test";

    static AntiDiagonal SQUARE_TO_ANTIDIAGONAL[64] = {
        ANTIDIAGONAL_0_A1_A1,  ANTIDIAGONAL_1_B1_A2,  ANTIDIAGONAL_2_C1_A3,  ANTIDIAGONAL_3_D1_A4,
        ANTIDIAGONAL_4_E1_A5,  ANTIDIAGONAL_5_F1_A6,  ANTIDIAGONAL_6_G1_A7,  ANTIDIAGONAL_7_H1_A8,
        ANTIDIAGONAL_1_B1_A2,  ANTIDIAGONAL_2_C1_A3,  ANTIDIAGONAL_3_D1_A4,  ANTIDIAGONAL_4_E1_A5,
        ANTIDIAGONAL_5_F1_A6,  ANTIDIAGONAL_6_G1_A7,  ANTIDIAGONAL_7_H1_A8,  ANTIDIAGONAL_8_H2_B8,
        ANTIDIAGONAL_2_C1_A3,  ANTIDIAGONAL_3_D1_A4,  ANTIDIAGONAL_4_E1_A5,  ANTIDIAGONAL_5_F1_A6,
        ANTIDIAGONAL_6_G1_A7,  ANTIDIAGONAL_7_H1_A8,  ANTIDIAGONAL_8_H2_B8,  ANTIDIAGONAL_9_H3_C8,
        ANTIDIAGONAL_3_D1_A4,  ANTIDIAGONAL_4_E1_A5,  ANTIDIAGONAL_5_F1_A6,  ANTIDIAGONAL_6_G1_A7,
        ANTIDIAGONAL_7_H1_A8,  ANTIDIAGONAL_8_H2_B8,  ANTIDIAGONAL_9_H3_C8,  ANTIDIAGONAL_10_H4_D8,
        ANTIDIAGONAL_4_E1_A5,  ANTIDIAGONAL_5_F1_A6,  ANTIDIAGONAL_6_G1_A7,  ANTIDIAGONAL_7_H1_A8,
        ANTIDIAGONAL_8_H2_B8,  ANTIDIAGONAL_9_H3_C8,  ANTIDIAGONAL_10_H4_D8, ANTIDIAGONAL_11_H5_E8,
        ANTIDIAGONAL_5_F1_A6,  ANTIDIAGONAL_6_G1_A7,  ANTIDIAGONAL_7_H1_A8,  ANTIDIAGONAL_8_H2_B8,
        ANTIDIAGONAL_9_H3_C8,  ANTIDIAGONAL_10_H4_D8, ANTIDIAGONAL_11_H5_E8, ANTIDIAGONAL_12_H6_F8,
        ANTIDIAGONAL_6_G1_A7,  ANTIDIAGONAL_7_H1_A8,  ANTIDIAGONAL_8_H2_B8,  ANTIDIAGONAL_9_H3_C8,
        ANTIDIAGONAL_10_H4_D8, ANTIDIAGONAL_11_H5_E8, ANTIDIAGONAL_12_H6_F8, ANTIDIAGONAL_13_H7_G8,
        ANTIDIAGONAL_7_H1_A8,  ANTIDIAGONAL_8_H2_B8,  ANTIDIAGONAL_9_H3_C8,  ANTIDIAGONAL_10_H4_D8,
        ANTIDIAGONAL_11_H5_E8, ANTIDIAGONAL_12_H6_F8, ANTIDIAGONAL_13_H7_G8, ANTIDIAGONAL_14_H8_H8};

    for (Square sq = Square::SQ_A1; sq.is_valid(); sq++) {
        if (sq.antidiagonal() != SQUARE_TO_ANTIDIAGONAL[sq]) {
            PRINT_TEST_FAILED(test_name, "sq.antidiagonal() != SQUARE_TO_ANTIDIAGONAL[sq]");
        }
    }
}