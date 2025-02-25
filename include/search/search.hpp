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
#include <limits>
#include <atomic>


/**
 * @brief INFINITE_SEARCH_DEPTH_VALUE
 * 
 * Value that represents an infinite search depth.
 * 
 */
constexpr const int32_t INFINITE_DEPTH = std::numeric_limits<int32_t>::max();

/**
 * @brief search_best_move
 * 
 * Calculate the best legal move in the chess position.
 * 
 * @note This method is thread safe
 * 
 * @param[in] board chess position.
 * @param[in] max_depth maximum depth of search, default value is INFINITE_DEPTH
 * 
 * @return best move found in the position.
 * 
 */
Move search_best_move(Board board, int32_t max_depth = INFINITE_DEPTH);

/**
 * @brief search_stop
 * 
 * stop the search process, this method is thread safe
 * 
 */
void search_stop();