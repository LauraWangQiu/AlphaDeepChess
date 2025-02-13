#pragma once

/**
 * @file search.hpp
 * @brief search services.
 *
 * chess search declarations. 
 * 
 */

#include "board.hpp"
#include "move.hpp"
#include "move_list.hpp"

#include <atomic>


/**
 * @brief INFINITE_SEARCH_DEPTH_VALUE
 * 
 * Value that represents an infinite search depth.
 * 
 */
constexpr const uint32_t INFINITE_SEARCH_DEPTH_VALUE = 999999999U;

/**
 * @brief search_best_move
 * 
 * Calculate the best legal move in the chess position.
 * 
 * @note The function is prepared to be called in a separated thread, you could use stop signal.
 * 
 * @param[in] board chess position.
 * @param[in] max_depth use INFINITE_SEARCH_DEPTH_VALUE to do an infinite search.
 * @param[in] stop signal to stop the search.
 * 
 * @return 
 *  - move, best move found in the position.
 *  - move.none() if no move was found.
 */
Move search_best_move(Board& board, uint32_t max_depth, MoveList search_moves, std::atomic<bool>& stop);
