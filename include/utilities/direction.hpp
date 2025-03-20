#pragma once

/**
 * @file direction.hpp
 * @brief Direction type utilities declaration.
 *
 */

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
 * @brief is_orthogonal_direction
 * 
 * Calculate if a direction is orthogonal
 * 
 * @param[in] direction selected direction.
 * 
 * @return True if direction == NORTH || EAST || SOUTH || WEST
 *         False in other case.
 * 
 */
inline constexpr bool is_orthogonal_direction(Direction direction)
{
    return direction == NORTH || direction == EAST || direction == SOUTH || direction == WEST;
}

/**
 * @brief is_diagonal_direction
 * 
 * Calculate if a direction is diagonal
 * 
 * @param[in] direction selected direction.
 * 
 * @return True if direction == NORTH_EAST || SOUTH_EAST || SOUTH_WEST || NORTH_WEST
 *         False in other case.
 * 
 */
inline constexpr bool is_diagonal_direction(Direction direction)
{
    return direction == NORTH_EAST || direction == SOUTH_EAST || direction == SOUTH_WEST || direction == NORTH_WEST;
}