#include "test_utils.hpp"
#include "coordinates.hpp"

static void is_valid_coord_test();
static void get_direction_test();

void coordinates_test()
{
    std::cout << "---------coordinates test---------\n\n";

    is_valid_coord_test();
    get_direction_test();
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