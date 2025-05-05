#pragma once

/**
 * @file square.hpp
 * @brief Square types and utilities declaration.
 *
 */

#include <string>
#include <cstdint>
#include <ostream>
#include <cassert>

#include "col.hpp"
#include "row.hpp"
#include "direction.hpp"
#include "diagonal.hpp"

/**
 * @brief NUM_SQUARES
 * 
 * number of squares in a chess board.
 * 
 */
constexpr const int NUM_SQUARES = 64;


/**
 * @brief Square
 * 
 * Represents a square on the board as an integer.
 * 
 * @note 
 * 0 is the square A1
 * 7 is the square H1
 * 56 is the square H1
 * 63 is the square H8
 * >=64 is the square INVALID
 * 
 */
class Square
{
public:
    // clang-format off
    /**
     * @brief A1, B1, C1, D1, E1, F1, G1, H1,  
     * @brief A2, B2, C2, D2, E2, F2, G2, H2,  
     * @brief A3, B3, C3, D3, E3, F3, G3, H3,  
     * @brief A4, B4, C4, D4, E4, F4, G4, H4,  
     * @brief A5, B5, C5, D5, E5, F5, G5, H5,  
     * @brief A6, B6, C6, D6, E6, F6, G6, H6,  
     * @brief A7, B7, C7, D7, E7, F7, G7, H7,  
     * @brief A8, B8, C8, D8, E8, F8, G8, H8
     */
    enum Name : uint8_t {
        A1 = 0U, B1, C1, D1, E1, F1, G1, H1,
        A2, B2, C2, D2, E2, F2, G2, H2,
        A3, B3, C3, D3, E3, F3, G3, H3,
        A4, B4, C4, D4, E4, F4, G4, H4,
        A5, B5, C5, D5, E5, F5, G5, H5,
        A6, B6, C6, D6, E6, F6, G6, H6,
        A7, B7, C7, D7, E7, F7, G7, H7,
        A8, B8, C8, D8, E8, F8, G8, H8,
        INVALID = 64U
    };
    // clang-format on

    /**
     * @brief Square
     * 
     * Empty constructor. Initializes the square to Types::Square::INVALID
     * 
     */
    constexpr Square() : sq_value(INVALID) { }

    /**
     * @brief Square(Square& square)
     * 
     * copy constructor.
     * 
     * @param[in] square 
     * 
     */
    constexpr Square(const Square& square) : sq_value(square.value()) { }

    /**
     * @brief Square(uint8_t square)
     * 
     * Constructor that initializes with the int square value
     * 
     * @param[in] square square uint8_t value
     * 
     */
    constexpr Square(uint8_t square) : sq_value(square) { sq_value = is_valid() ? sq_value : (uint8_t)INVALID; }

    /**
     * @brief Square(Row row, Col col)
     * 
     * Constructor that initializes with the specified row and col
     * 
     * @note if row or col are invalid, the square will be INVALID.
     * 
     * @param[in] row selected row.
     * @param[in] col selected col.
     * 
     */
    constexpr Square(Row row, Col col)
        : sq_value(is_valid_row(row) && is_valid_col(col) ? (row << 3U) + col : INVALID)
    { }

    /**
     * @brief Square(Square::Name square)
     * 
     * Constructor that initializes with the Name of the square
     * 
     * @param[in] square 
     * 
     */
    constexpr Square(Square::Name square) : sq_value(static_cast<uint8_t>(square)) { }

    /**
     * @brief Square(const std::string& square_string)
     * 
     * Constructor that initializes the square from a string representation.
     * E.g "e2" is the square number 12.
     * 
     * @note If invalid string representation of square, square is set to INVALID.
     * 
     * @param[in] square_string Square represented in string format (e.g., "e2"). 
     *                          
     */
    constexpr Square(const std::string& square_string);

    /**
     * @brief Square(char col, char row)
     * 
     * Constructor that initializes the square from a character representation.
     * E.g "e2" is the square number 12.
     * 
     * @note If invalid character representation of square, square is set to INVALID.
     * 
     * @param[in] col Column that represents the square as a character ('a' to 'h').
     * @param[in] row Row that represents the square as a character ('1' to '8').
     */
    constexpr Square(char col, char row);

    /**
     * @brief row
     * 
     * calculates the row of the square
     * 
     * @return (sq_value / 8)
     */
    constexpr inline Row row() const { return is_valid() ? static_cast<Row>(sq_value >> 3U) : ROW_INVALID; }

    /**
     * @brief col
     * 
     * calculates the column of the square
     * 
     * @return (sq_value % 8)
     */
    constexpr inline Col col() const { return is_valid() ? static_cast<Col>(sq_value & 7U) : COL_INVALID; }

    /**
     * @brief diagonal
     * 
     * calculates the diagonal of the square
     * 
     * @return (row - col + 7)
     */
    constexpr inline Diagonal diagonal() const
    {
        return is_valid() ? static_cast<Diagonal>(static_cast<int>(row()) - static_cast<int>(col()) + 7)
                          : DIAGONAL_INVALID;
    }

    /**
     * @brief antidiagonal
     * 
     * calculates the antidiagonal of the square
     * 
     * @return (row + col)
     */
    constexpr inline AntiDiagonal antidiagonal() const
    {
        return is_valid() ? static_cast<AntiDiagonal>(static_cast<int>(row()) + static_cast<int>(col()))
                          : ANTIDIAGONAL_INVALID;
    }

    /**
     * @brief to_string
     * 
     * calculate string representation. E.g : e4, d6, h1, e8.
     * 
     * @return 
     *  - std::string representation of the square.
     *  - invalid if square is not valid.
     */
    inline std::string to_string() const
    {
        return is_valid() ? std::string(1, col_to_char(col())) + row_to_char(row()) : "invalid";
    }

    /**
     * @brief operator<<
     * 
     * Overloads the << operator to print the coordinates of the square
     * 
     * @param[out] os The output stream
     * @param[in] square The square to print
     */
    friend std::ostream& operator<<(std::ostream& os, const Square& square)
    {
        assert(square.is_valid());

        os << (char)('a' + square.col()) << (char)('1' + square.row());
        return os;
    }

    /**
     * @brief value 
     * 
     * Calculates the square integer value
     * 
     * @return square value
     * 
     */
    constexpr uint8_t value() const { return sq_value; }

    /**
     * @brief is_valid 
     * 
     * Calculates if the square is valid
     * 
     * @return
     * - TRUE if A1(0) <= square <= H8(63)
     * - FALSE if square >= INVALID(64)
     * 
     */
    constexpr bool is_valid() const { return sq_value < INVALID; }

    /**
     * @brief mask 
     * 
     * Calculates the bit mask that represents the square.
     * C1 = 2 has a mask of 0....00000100.
     * 
     * @return (1UL << sq_value) if square is valid, Else 0
     * 
     */
    constexpr uint64_t mask() const { return is_valid() ? 1ULL << sq_value : 0ULL; }

    /**
     * @brief uint8_t
     * Conversion operator from Square to uint8_t.
     * 
     * @return square value
     */
    constexpr operator uint8_t() const { return sq_value; }

    /**
     * @brief operator++
     * Pre-increment operator overload
     * 
     * @note out of bounds will result in INVALID
     * 
     * @return *this
     */
    constexpr Square& operator++()
    {
        assert(this->is_valid());

        ++sq_value;
        sq_value = is_valid() ? sq_value : (uint8_t)INVALID;
        return *this;
    }

    /**
     * @brief operator++(int)
     * Post-increment operator overload
     * 
     * @note out of bounds will result in INVALID
     * 
     * @return *this
     */
    constexpr Square operator++(int)
    {
        assert(this->is_valid());

        Square temp = *this;
        ++sq_value;
        sq_value = is_valid() ? sq_value : (uint8_t)INVALID;
        return temp;
    }

    /**
     * @brief operator--
     * Pre-decrement operator overload
     * 
     * @note out of bounds will result in INVALID
     * 
     * @return *this
     */
    constexpr Square& operator--()
    {
        assert(this->is_valid());

        --sq_value;
        sq_value = is_valid() ? sq_value : (uint8_t)INVALID;
        return *this;
    }
    /**
     * @brief operator--(int)
     * Post-decrement operator overload
     * 
     * @note out of bounds will result in INVALID
     * 
     * @return *this
     */
    constexpr Square operator--(int)
    {
        assert(this->is_valid());

        Square temp = *this;
        --sq_value;
        sq_value = is_valid() ? sq_value : (uint8_t)INVALID;
        return temp;
    }

    /**
     * @brief operator=
     * 
     * Assignment operator overload.
     * 
     * @param[in] other square where to copy the value.
     * 
     * @return *this
     * 
     */
    constexpr Square& operator=(const Square& other)
    {
        if (this != &other)   // not a self-assignment
        {
            this->sq_value = other.sq_value;
        }
        return *this;
    }

    /**
     * @brief north
     * 
     * Calculates the square in the north direction.
     * White player viewpoint.
     * 
     * @note square should be valid
     * 
     * @return
     *  (sq_value + 8) if row() < ROW_8
     *  (INVALID) if row() >= ROW_8
     * 
     */
    constexpr Square north() const { return row() < ROW_8 ? sq_value + NORTH : INVALID; }

    /**
     * @brief south
     * 
     * Calculates the square in the south direction.
     * White player viewpoint.
     * 
     * @note square should be valid
     * 
     * @return
     *  (sq_value - 8) if row() > ROW_1
     *  (INVALID) if row() <= ROW_1
     * 
     */
    constexpr Square south() const { return row() > ROW_1 ? sq_value + SOUTH : INVALID; }

    /**
     * @brief north
     * 
     * Calculates the square in the east direction.
     * White player viewpoint.
     * 
     * @note square should be valid
     * 
     * @return
     *  (sq_value - 1) if col() < COL_H
     *  (INVALID) if col() >= COL_H
     * 
     */
    constexpr Square east() const { return col() < COL_H ? sq_value + EAST : INVALID; }

    /**
     * @brief west
     * 
     * Calculates the square in the west direction.
     * White player viewpoint.
     * 
     * @note square should be valid
     * 
     * @return
     *  (sq_value + 1) if col() > COL_A
     *  (INVALID) if col() <= COL_A
     * 
     */
    constexpr Square west() const { return col() > COL_A ? sq_value + WEST : INVALID; }

    /**
     * @brief northEast
     * 
     * Calculates the square in the northeast direction.
     * White player viewpoint.
     * 
     * @note square should be valid
     * 
     * @return
     *  (sq_value + 7) if row() < ROW_8 && col() < COL_H
     *  (INVALID) if row() >= ROW_8 || col() >= COL_H
     * 
     */
    constexpr Square northEast() const { return row() < ROW_8 && col() < COL_H ? sq_value + NORTH_EAST : INVALID; }

    /**
     * @brief northWest
     * 
     * Calculates the square in the northwest direction.
     * White player viewpoint.
     * 
     * @note square should be valid
     * 
     * @return
     *  (sq_value + 9) if row() < ROW_8 && col() > COL_A
     *  (INVALID) if row() >= ROW_8 || col() <= COL_A
     * 
     */
    constexpr Square northWest() const { return row() < ROW_8 && col() > COL_A ? sq_value + NORTH_WEST : INVALID; }

    /**
     * @brief southEast
     * 
     * Calculates the square in the southeast direction.
     * White player viewpoint.
     * 
     * @note square should be valid
     * 
     * @return
     *  (sq_value - 9) if row() > ROW_1 && col() < COL_H
     *  (INVALID) if row() <= ROW_1 || col() >= COL_H
     * 
     */
    constexpr Square southEast() const { return row() > ROW_1 && col() < COL_H ? sq_value + SOUTH_EAST : INVALID; }

    /**
     * @brief southWest
     * 
     * Calculates the square in the southwest direction.
     * White player viewpoint.
     * 
     * @note square should be valid
     * 
     * @return
     *  (sq_value - 7) if row() > ROW_1 && col() > COL_A
     *  (INVALID) if row() <= ROW_1 || col() <= COL_A
     * 
     */
    constexpr Square southWest() const { return row() > ROW_1 && col() > COL_A ? sq_value + SOUTH_WEST : INVALID; }

    /**
     * @brief to_direction
     * 
     * Move the square in the specified direction.
     * 
     * @note square should be valid
     * 
     * @param[in] direction
     * 
     */
    constexpr void to_direction(const Direction direction)
    {
        switch (direction) {
        case Direction::NORTH: sq_value = north(); break;
        case Direction::NORTH_EAST: sq_value = northEast(); break;
        case Direction::NORTH_WEST: sq_value = northWest(); break;
        case Direction::EAST: sq_value = east(); break;
        case Direction::WEST: sq_value = west(); break;
        case Direction::SOUTH: sq_value = south(); break;
        case Direction::SOUTH_EAST: sq_value = southEast(); break;
        case Direction::SOUTH_WEST: sq_value = southWest(); break;
        default:   // invalid direction
            break;
        }
    }

private:
    uint8_t sq_value;
};

/**
 * @brief Square(const std::string& square_string)
 * 
 * Constructor that initializes the square from a string representation.
 * E.g "e2" is the square number 12.
 * 
 * @note If invalid string representation of square, square is set to INVALID.
 * 
 * @param[in] square_string Square represented in string format (e.g., "e2"). 
 *                          
 */
constexpr Square::Square(const std::string& square_string) : sq_value(INVALID)
{
    if (square_string.size() == 2) {
        char col = tolower(square_string[0]);
        char row = tolower(square_string[1]);

        if (col >= 'a' && col <= 'h' && row >= '1' && row <= '8') {
            sq_value = (col - 'a') + 8 * (row - '1');
        }
    }
}

/**
 * @brief Square(char col, char row)
 * 
 * Constructor that initializes the square from a character representation.
 * E.g "e2" is the square number 12.
 * 
 * @note If invalid character representation of square, square is set to INVALID.
 * 
 * @param[in] col Column that represents the square as a character ('a' to 'h').
 * @param[in] row Row that represents the square as a character ('1' to '8').
 */
constexpr Square::Square(char col, char row) : sq_value(INVALID)
{

    col = tolower(col);
    row = tolower(row);

    if (col >= 'a' && col <= 'h' && row >= '1' && row <= '8') {
        sq_value = (col - 'a') + 8 * (row - '1');
    }
}