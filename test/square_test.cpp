#include <iostream>

#include "square.hpp"

void square_test()
{
    std::cout << "---------square test---------\n\n";

    Square square;

    if (square.is_valid() != false) {
        std::cout << "TEST FAILED : square_test : square.is_valid() != false\n";
    }
    if (square.to_string() != "invalid") {
        std::cout << "TEST FAILED : square_test : to_string() != invalid\n";
    }

    square = Square(Square::SQ_D4);

    if (square.is_valid() != true) {
        std::cout << "TEST FAILED : square_test : square.is_valid() != true\n";
    }
    if (square.value() != 27U) {
        std::cout << "TEST FAILED : square_test : value() != 27U\n";
    }
    if (square.row() != ROW_4) {
        std::cout << "TEST FAILED : square_test : square.row() != ROW_4\n";
    }
    if (square.col() != COL_D) {
        std::cout << "TEST FAILED : square_test : square.col() != COL_D\n";
    }
    if (square.mask() != (1UL << 27U)) {
        std::cout << "TEST FAILED : square_test : square.mask() != (1UL << 27U)\n";
    }
    if (square.to_string() != "d4") {
        std::cout << "TEST FAILED : square_test : to_string() != d4\n";
    }
    square.to_direction(NORTH);
    if (square != Square::SQ_D5) {
        std::cout << "TEST FAILED : square_test : to_direction(NORTH) != Square::SQ_D5\n";
    }
    square.to_direction(NORTH_EAST);
    if (square != Square::SQ_E6) {
        std::cout << "TEST FAILED : square_test : to_direction(NORTH_EAST) != Square::SQ_E6\n";
    }
    square.to_direction(EAST);
    if (square != Square::SQ_F6) {
        std::cout << "TEST FAILED : square_test : to_direction(EAST) != Square::SQ_F6\n";
    }
    square.to_direction(SOUTH_EAST);
    if (square != Square::SQ_G5) {
        std::cout << "TEST FAILED : square_test : to_direction(SOUTH_EAST) != Square::SQ_G5\n";
    }
    square.to_direction(SOUTH);
    if (square != Square::SQ_G4) {
        std::cout << "TEST FAILED : square_test : to_direction(SOUTH) != Square::SQ_G4\n";
    }
    square.to_direction(SOUTH_WEST);
    if (square != Square::SQ_F3) {
        std::cout << "TEST FAILED : square_test : to_direction(SOUTH_WEST) != Square::SQ_F3\n";
    }
    square.to_direction(WEST);
    if (square != Square::SQ_E3) {
        std::cout << "TEST FAILED : square_test : to_direction(WEST) != Square::SQ_E3\n";
    }
    square.to_direction(NORTH_WEST);
    if (square != Square::SQ_D4) {
        std::cout << "TEST FAILED : square_test : to_direction(NORTH_WEST) != Square::SQ_D4\n";
    }

    square = Square(Square::SQ_A1);

    if (square.is_valid() != true) {
        std::cout << "TEST FAILED : square_test : square.is_valid() != true\n";
    }
    if (square.value() != 0U) {
        std::cout << "TEST FAILED : square_test : value() != 0U\n";
    }
    if (square.row() != ROW_1) {
        std::cout << "TEST FAILED : square_test : square.row() != ROW_1\n";
    }
    if (square.col() != COL_A) {
        std::cout << "TEST FAILED : square_test : square.col() != COL_A\n";
    }
    if (square.mask() != (1UL << 0U)) {
        std::cout << "TEST FAILED : square_test : square.mask() != (1UL << 0U)\n";
    }
    if (square.to_string() != "a1") {
        std::cout << "TEST FAILED : square_test : to_string() != a1\n";
    }
    if (square.north() != Square::SQ_A2) {
        std::cout << "TEST FAILED : square_test : north() != Square::SQ_A2\n";
    }
    if (square.northEast() != Square::SQ_B2) {
        std::cout << "TEST FAILED : square_test : northEast() != Square::SQ_B2\n";
    }
    if (square.east() != Square::SQ_B1) {
        std::cout << "TEST FAILED : square_test : east() != Square::SQ_B1\n";
    }
    if (square.southEast() != Square::SQ_INVALID) {
        std::cout << "TEST FAILED : square_test : southEast() != Square::SQ_INVALID\n";
    }
    if (square.south() != Square::SQ_INVALID) {
        std::cout << "TEST FAILED : square_test : south() != Square::SQ_INVALID\n";
    }
    if (square.southWest() != Square::SQ_INVALID) {
        std::cout << "TEST FAILED : square_test : southWest() != Square::SQ_INVALID\n";
    }
    if (square.west() != Square::SQ_INVALID) {
        std::cout << "TEST FAILED : square_test : west() != Square::SQ_INVALID\n";
    }
    if (square.northWest() != Square::SQ_INVALID) {
        std::cout << "TEST FAILED : square_test : northWest() != Square::SQ_INVALID\n";
    }

    square = Square(Square::SQ_H8);

    if (square.is_valid() != true) {
        std::cout << "TEST FAILED : square_test : square.is_valid() != true\n";
    }
    if (square.value() != 63U) {
        std::cout << "TEST FAILED : square_test : value() != 63U\n";
    }
    if (square.row() != ROW_8) {
        std::cout << "TEST FAILED : square_test : square.row() != ROW_8\n";
    }
    if (square.col() != COL_H) {
        std::cout << "TEST FAILED : square_test : square.col() != COL_H\n";
    }
    if (square.mask() != (1UL << 63U)) {
        std::cout << "TEST FAILED : square_test : square.mask() != (1UL << 63U)\n";
    }
    if (square.to_string() != "h8") {
        std::cout << "TEST FAILED : square_test : to_string() != h8\n";
    }
    if (square.north() != Square::SQ_INVALID) {
        std::cout << "TEST FAILED : square_test : north() != Square::SQ_INVALID\n";
    }
    if (square.northEast() != Square::SQ_INVALID) {
        std::cout << "TEST FAILED : square_test : northEast() != Square::SQ_INVALID\n";
    }
    if (square.east() != Square::SQ_INVALID) {
        std::cout << "TEST FAILED : square_test : east() != Square::SQ_INVALID\n";
    }
    if (square.southEast() != Square::SQ_INVALID) {
        std::cout << "TEST FAILED : square_test : southEast() != Square::SQ_INVALID\n";
    }
    if (square.south() != Square::SQ_H7) {
        std::cout << "TEST FAILED : square_test : south() != Square::SQ_H7\n";
    }
    if (square.southWest() != Square::SQ_G7) {
        std::cout << "TEST FAILED : square_test : southWest() != Square::SQ_G7\n";
    }
    if (square.west() != Square::SQ_G8) {
        std::cout << "TEST FAILED : square_test : west() != Square::SQ_G8\n";
    }
    if (square.northWest() != Square::SQ_INVALID) {
        std::cout << "TEST FAILED : square_test : northWest() != Square::SQ_INVALID\n";
    }
}
