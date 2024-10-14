#pragma once

/**
 * @file move_generator.hpp
 * @brief move generator services.
 *
 * chess move generator declarations. 
 * 
 */

#include "board.hpp"
#include "move_list.hpp"

/**
 * @brief generate_legal_moves
 * 
 * Calculate all the legal moves in the chess position.
 * 
 * @param[out] moves move list.
 * @param[in] board chess position.
 * 
 */
void generate_legal_moves(MoveList &moves, const Board &board);