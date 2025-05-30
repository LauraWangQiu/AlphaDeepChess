#pragma once

/**
 * @file coordinates.hpp
 * @brief Coordinates types and utilities declaration.
 *
 */

#include "square.hpp"
#include <cassert>


/**
 * @brief is_valid_coord 
 * 
 * Returns if the coordinate inside the 8x8 board.
 * 
 * @param[in] row The row coordinate
 * @param[in] col The column coordinate
 * 
 * @return
 * - TRUE if row >= ROW_1 && row <= ROW_8 && col >= COL_A && col <= COL_H.
 * - FALSE if row or col are invalid.
 * 
 */
constexpr inline bool is_valid_coord(Row row, Col col)
{
    return row >= ROW_1 && row <= ROW_8 && col >= COL_A && col <= COL_H;
}

/**
 * @brief get_direction 
 * 
 * Calculates the direction to go from origin square to end square.
 * 
 * @note if the two coordinates are not aligned vertically or horizontally 
 * the direction to go will be an approximation.
 * 
 * @param[in] origin_row The origin row coordinate
 * @param[in] origin_col The origin column coordinate
 * @param[in] end_row The end row coordinate
 * @param[in] end_col The end column coordinate
 * 
 * @return
 * - Direction::EAST,
 * - Direction::SOUTH_EAST,
 * - Direction::SOUTH,
 * - Direction::SOUTH_WEST,
 * - Direction::WEST,
 * - Direction::NORTH_WEST,
 * - Direction::NORTH,
 * - Direction::NORTH_EAST
 * 
 */
Direction get_direction(Row origin_row, Col origin_col, Row end_row, Col end_col);


/**
 * @brief squares_in_same_diagonal 
 * 
 * Returns if the squares are inside the same diagonal
 * 
 * @param[in] sq1 first square
 * @param[in] sq2 second square
 * 
 * @return
 * - TRUE if sq1.diagonal() == sq2.diagonal().
 * - FALSE sq1.diagonal() != sq2.diagonal().
 * 
 */
constexpr inline bool squares_in_same_diagonal(Square sq1, Square sq2)
{
    assert(sq1.is_valid() && sq2.is_valid());
    return sq1.diagonal() == sq2.diagonal();
}

/**
 * @brief squares_in_same_antidiagonal 
 * 
 * Returns if the squares are inside the same antidiagonal
 * 
 * @param[in] sq1 first square
 * @param[in] sq2 second square
 * 
 * @return
 * - TRUE if sq1.antidiagonal() == sq2.antidiagonal().
 * - FALSE sq1.antidiagonal() != sq2.antidiagonal().
 * 
 */
constexpr inline bool squares_in_same_antidiagonal(Square sq1, Square sq2)
{
    assert(sq1.is_valid() && sq2.is_valid());
    return sq1.antidiagonal() == sq2.antidiagonal();
}

/**
 * @brief get_direction_mask 
 * 
 * Returns the mask of the common direction of two squares
 * 
 * @param[in] sq1 first square
 * @param[in] sq2 second square
 * 
 * @return
 * -
 * - row mask of sq1,sq2 if sq1.row() == sq2.row()
 * - col mask of sq1,sq2 if sq1.col() == sq2.col()
 * - diagonal_mask of sq1,sq2 if squares_in_same_diagonal(sq1, sq2).
 * - antidiagonal_mask of sq1,sq2 if squares_in_same_antidiagonal(sq1, sq2).
 * - 0ULL in other case
 */
uint64_t get_direction_mask(Square sq1, Square sq2);
