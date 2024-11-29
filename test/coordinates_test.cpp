#include <iostream>

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
    std::cout << "is_valid_coord test :\n\n";


    if (is_valid_coord(ROW_1, COL_A) != true) {
        std::cout << "TEST FAILED : coordinates_test : is_valid_coord(ROW_1, COL_A) != true\n";
    }
    if (is_valid_coord(ROW_8, COL_H) != true) {
        std::cout << "TEST FAILED : coordinates_test : is_valid_coord(ROW_8, COL_H) != true\n";
    }
    if (is_valid_coord((Row)(-1), COL_A) != false) {
        std::cout << "TEST FAILED : coordinates_test : is_valid_coord((Row)(-1), COL_A) != false\n";
    }
    if (is_valid_coord((Row)(8), COL_H) != false) {
        std::cout << "TEST FAILED : coordinates_test : is_valid_coord((Row)(8), COL_H) != false\n";
    }
    if (is_valid_coord(ROW_1, (Col)(-1)) != false) {
        std::cout << "TEST FAILED : coordinates_test : is_valid_coord(ROW_1, (Col)(-1)) != false\n";
    }
    if (is_valid_coord(ROW_8, (Col)(8)) != false) {
        std::cout << "TEST FAILED : coordinates_test : is_valid_coord(ROW_8, (Col)(8)) != false\n";
    }
}

static void get_direction_test()
{

    std::cout << "get_direction test :\n\n";

    if (get_direction(ROW_4, COL_D, ROW_4, COL_H) != Direction::EAST) {
        std::cout << "TEST FAILED : get_direction_test : EAST\n";
    }

    if (get_direction(ROW_4, COL_D, ROW_1, COL_G) != Direction::SOUTH_EAST) {
        std::cout << "TEST FAILED : get_direction_test : SOUTH_EAST\n";
    }

    if (get_direction(ROW_4, COL_D, ROW_1, COL_D) != Direction::SOUTH) {
        std::cout << "TEST FAILED : get_direction_test : SOUTH\n";
    }

    if (get_direction(ROW_4, COL_D, ROW_1, COL_A) != Direction::SOUTH_WEST) {
        std::cout << "TEST FAILED : get_direction_test : SOUTH_WEST\n";
    }

    if (get_direction(ROW_4, COL_D, ROW_4, COL_A) != Direction::WEST) {
        std::cout << "TEST FAILED : get_direction_test : WEST\n";
    }

    if (get_direction(ROW_4, COL_D, ROW_7, COL_A) != Direction::NORTH_WEST) {
        std::cout << "TEST FAILED : get_direction_test : NORTH_WEST\n";
    }

    if (get_direction(ROW_4, COL_D, ROW_7, COL_D) != Direction::NORTH) {
        std::cout << "TEST FAILED : get_direction_test : NORTH\n";
    }

    if (get_direction(ROW_4, COL_D, ROW_7, COL_G) != Direction::NORTH_EAST) {
        std::cout << "TEST FAILED : get_direction_test : NORTH_EAST\n";
    }
}