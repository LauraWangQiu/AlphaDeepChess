#pragma once

/**
 * @file coordinates.hpp
 * @brief Coordinates types and utilities declaration.
 *
 */

#include "row.hpp"
#include "col.hpp"

/**
 * @brief Direction
 * 
 * Represents all the possible directions to move in the board
 * 
 *   INVALID_DIR = 0,
 *   NORTH = 8,
 *   EAST  = 1,
 *   SOUTH = -8,
 *   WEST  = -1,
 *   NORTH_EAST = 9,
 *   SOUTH_EAST = -7,
 *   SOUTH_WEST = -9,
 *   NORTH_WEST = 7.
 */
enum Direction : int
{
    INVALID_DIR = 0,
    NORTH = 8,
    EAST = 1,
    SOUTH = -8,
    WEST = -1,
    NORTH_EAST = NORTH + EAST,
    SOUTH_EAST = SOUTH + EAST,
    SOUTH_WEST = SOUTH + WEST,
    NORTH_WEST = NORTH + WEST
};

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
 * 
 */
constexpr Direction get_direction(Row origin_row, Col origin_col, Row end_row, Col end_col)
{
    // Calculate row and column differences
    const int row_diff = static_cast<int>(end_row) - static_cast<int>(origin_row);
    const int col_diff = static_cast<int>(end_col) - static_cast<int>(origin_col);

    // Normalize row and column differences to {-1, 0, 1}
    const int normalized_row = (row_diff > 0) - (row_diff < 0);
    const int normalized_col = (col_diff > 0) - (col_diff < 0);

    // Compute the direction based taking advantage of the int value of each direction
    return static_cast<Direction>(8 * normalized_row + normalized_col);
}
