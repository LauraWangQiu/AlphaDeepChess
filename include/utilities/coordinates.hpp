#pragma once

/**
 * @file coordinates.hpp
 * @brief Coordinates types and utilities declaration.
 *
 */

#include "row.hpp"
#include "col.hpp"


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
