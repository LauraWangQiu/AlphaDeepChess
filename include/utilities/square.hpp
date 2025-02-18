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
 * 0 is the square SQ_A1
 * 7 is the square SQ_H1
 * 56 is the square SQ_H1
 * 63 is the square SQ_H8
 * >=64 is the square SQ_INVALID
 * 
 */
class Square
{


public:
    // clang-format off
    /**
     * @brief SQ_A1, SQ_B1, SQ_C1, SQ_D1, SQ_E1, SQ_F1, SQ_G1, SQ_H1,  
     * @brief SQ_A2, SQ_B2, SQ_C2, SQ_D2, SQ_E2, SQ_F2, SQ_G2, SQ_H2,  
     * @brief SQ_A3, SQ_B3, SQ_C3, SQ_D3, SQ_E3, SQ_F3, SQ_G3, SQ_H3,  
     * @brief SQ_A4, SQ_B4, SQ_C4, SQ_D4, SQ_E4, SQ_F4, SQ_G4, SQ_H4,  
     * @brief SQ_A5, SQ_B5, SQ_C5, SQ_D5, SQ_E5, SQ_F5, SQ_G5, SQ_H5,  
     * @brief SQ_A6, SQ_B6, SQ_C6, SQ_D6, SQ_E6, SQ_F6, SQ_G6, SQ_H6,  
     * @brief SQ_A7, SQ_B7, SQ_C7, SQ_D7, SQ_E7, SQ_F7, SQ_G7, SQ_H7,  
     * @brief SQ_A8, SQ_B8, SQ_C8, SQ_D8, SQ_E8, SQ_F8, SQ_G8, SQ_H8
     */
    enum Name : uint8_t {
        SQ_A1 = 0U, SQ_B1, SQ_C1, SQ_D1, SQ_E1, SQ_F1, SQ_G1, SQ_H1,
        SQ_A2, SQ_B2, SQ_C2, SQ_D2, SQ_E2, SQ_F2, SQ_G2, SQ_H2,
        SQ_A3, SQ_B3, SQ_C3, SQ_D3, SQ_E3, SQ_F3, SQ_G3, SQ_H3,
        SQ_A4, SQ_B4, SQ_C4, SQ_D4, SQ_E4, SQ_F4, SQ_G4, SQ_H4,
        SQ_A5, SQ_B5, SQ_C5, SQ_D5, SQ_E5, SQ_F5, SQ_G5, SQ_H5,
        SQ_A6, SQ_B6, SQ_C6, SQ_D6, SQ_E6, SQ_F6, SQ_G6, SQ_H6,
        SQ_A7, SQ_B7, SQ_C7, SQ_D7, SQ_E7, SQ_F7, SQ_G7, SQ_H7,
        SQ_A8, SQ_B8, SQ_C8, SQ_D8, SQ_E8, SQ_F8, SQ_G8, SQ_H8,
        SQ_INVALID = 64U
    };
    // clang-format on

    /**
     * @brief Square
     * 
     * Empty constructor. Initializes the square to Types::Square::SQ_INVALID
     * 
     */
    constexpr Square() : sq_value(SQ_INVALID) { }

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
    constexpr Square(uint8_t square) : sq_value(square)
    {
        sq_value = is_valid() ? sq_value : (uint8_t)SQ_INVALID;
    }

    /**
     * @brief Square(Row row, Col col)
     * 
     * Constructor that initializes with the specified row and col
     * 
     * @note if row or col are invalid, the square will be SQ_INVALID.
     * 
     * @param[in] row selected row.
     * @param[in] col selected col.
     * 
     */
    constexpr Square(Row row, Col col)
        : sq_value(is_valid_row(row) && is_valid_col(col) ? (row << 3U) + col : SQ_INVALID)
    {

    }

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
     * @note If invalid string representation of square, square is set to SQ_INVALID.
     * 
     * @param[in] square_string Square represented in string format (e.g., "e2"). 
     *                          
     */
    constexpr Square(const std::string& square_string);

    /**
     * @brief row
     * 
     * calculates the row of the square
     * 
     * @return (sq_value / 8)
     */
    constexpr inline Row row() const
    {
        return is_valid() ? static_cast<Row>(sq_value >> 3U) : ROW_INVALID;
    }

    /**
     * @brief col
     * 
     * calculates the column of the square
     * 
     * @return (sq_value % 8)
     */
    constexpr inline Col col() const
    {
        return is_valid() ? static_cast<Col>(sq_value & 7U) : COL_INVALID;
    }

    /**
     * @brief diagonal
     * 
     * calculates the diagonal of the square
     * 
     * @return (row - col + 7)
     */
    constexpr inline Diagonal diagonal() const
    {
        return is_valid()
            ? static_cast<Diagonal>(static_cast<int>(row()) - static_cast<int>(col()) + 7)
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
        return is_valid()
            ? static_cast<AntiDiagonal>(static_cast<int>(row()) + static_cast<int>(col()))
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
     * - TRUE if SQ_A1(0) <= square <= SQ_H8(63)
     * - FALSE if square >= SQ_INVALID(64)
     * 
     */
    constexpr bool is_valid() const { return sq_value < SQ_INVALID; }

    /**
     * @brief mask 
     * 
     * Calculates the bit mask that represents the square.
     * SQ_C1 = 2 has a mask of 0....00000100.
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
     * @note out of bounds will result in SQ_INVALID
     * 
     * @return *this
     */
    constexpr Square& operator++()
    {
        assert(this->is_valid());

        ++sq_value;
        sq_value = is_valid() ? sq_value : (uint8_t)SQ_INVALID;
        return *this;
    }

    /**
     * @brief operator++(int)
     * Post-increment operator overload
     * 
     * @note out of bounds will result in SQ_INVALID
     * 
     * @return *this
     */
    constexpr Square operator++(int)
    {
        assert(this->is_valid());

        Square temp = *this;
        ++sq_value;
        sq_value = is_valid() ? sq_value : (uint8_t)SQ_INVALID;
        return temp;
    }

    /**
     * @brief operator--
     * Pre-decrement operator overload
     * 
     * @note out of bounds will result in SQ_INVALID
     * 
     * @return *this
     */
    constexpr Square& operator--()
    {
        assert(this->is_valid());

        --sq_value;
        sq_value = is_valid() ? sq_value : (uint8_t)SQ_INVALID;
        return *this;
    }
    /**
     * @brief operator--(int)
     * Post-decrement operator overload
     * 
     * @note out of bounds will result in SQ_INVALID
     * 
     * @return *this
     */
    constexpr Square operator--(int)
    {
        assert(this->is_valid());

        Square temp = *this;
        --sq_value;
        sq_value = is_valid() ? sq_value : (uint8_t)SQ_INVALID;
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
     *  (SQ_INVALID) if row() >= ROW_8
     * 
     */
    constexpr Square north() const { return row() < ROW_8 ? sq_value + NORTH : SQ_INVALID; }

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
     *  (SQ_INVALID) if row() <= ROW_1
     * 
     */
    constexpr Square south() const { return row() > ROW_1 ? sq_value + SOUTH : SQ_INVALID; }

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
     *  (SQ_INVALID) if col() >= COL_H
     * 
     */
    constexpr Square east() const { return col() < COL_H ? sq_value + EAST : SQ_INVALID; }

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
     *  (SQ_INVALID) if col() <= COL_A
     * 
     */
    constexpr Square west() const { return col() > COL_A ? sq_value + WEST : SQ_INVALID; }

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
     *  (SQ_INVALID) if row() >= ROW_8 || col() >= COL_H
     * 
     */
    constexpr Square northEast() const
    {
        return row() < ROW_8 && col() < COL_H ? sq_value + NORTH_EAST : SQ_INVALID;
    }

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
     *  (SQ_INVALID) if row() >= ROW_8 || col() <= COL_A
     * 
     */
    constexpr Square northWest() const
    {
        return row() < ROW_8 && col() > COL_A ? sq_value + NORTH_WEST : SQ_INVALID;
    }

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
     *  (SQ_INVALID) if row() <= ROW_1 || col() >= COL_H
     * 
     */
    constexpr Square southEast() const
    {
        return row() > ROW_1 && col() < COL_H ? sq_value + SOUTH_EAST : SQ_INVALID;
    }

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
     *  (SQ_INVALID) if row() <= ROW_1 || col() <= COL_A
     * 
     */
    constexpr Square southWest() const
    {
        return row() > ROW_1 && col() > COL_A ? sq_value + SOUTH_WEST : SQ_INVALID;
    }

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
 * @note If invalid string representation of square, square is set to SQ_INVALID.
 * 
 * @param[in] square_string Square represented in string format (e.g., "e2"). 
 *                          
 */
constexpr Square::Square(const std::string& square_string) : sq_value(SQ_INVALID)
{
    if (square_string.size() == 2) {
        char col = tolower(square_string[0]);
        char row = tolower(square_string[1]);

        if (col >= 'a' && col <= 'h' && row >= '1' && row <= '8') {
            sq_value = (col - 'a') + 8 * (row - '1');
        }
    }
}
