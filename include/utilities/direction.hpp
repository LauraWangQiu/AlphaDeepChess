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