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
 * @brief order the move list by priority from best to worst
 *  
 * @note the first move in the list is the move with the best prospects of being the best move in the position.
 * 
 * @param[in,out] moves move list.
 * @param[in] board chess position.
 * @param[in] ply actual search depth ply.
 */
void order_moves(MoveList& moves, const Board& board, uint32_t ply);