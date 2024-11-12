#pragma once
/**
 * @file move_ordering.hpp
 * @brief move ordering services.
 *
 * chess move ordering declarations. 
 * 
 */

#include "move_list.hpp"
#include "board.hpp"

/**
 * @brief order_moves
 * 
 * Order the move list.
 * 
 * @note the first move in the list is the move with the best prospects of being the best move.
 * 
 * @param[in] board chess position.
 * @param[out] moves move list.
 * 
 */
void order_moves(MoveList& moves, const Board &board);