#pragma once

/**
 * @file search.hpp
 * @brief search services.
 *
 * chess search declarations. 
 * 
 */
#include "search_utils.hpp"
#include "board.hpp"
#include "move_list.hpp"

/**
 * @brief search(std::atomic<bool>&, SearchResults&, Board&, int32_t)
 * 
 * Search the best legal move in the chess position.
 * 
 * @param[in] stop stop search signal.
 * @param[out] results struct where to store the results.
 * @param[in] board chess position.
 * @param[in] max_depth maximum depth of search, default value is INFINITE_DEPTH
 * 
 */
void search(std::atomic<bool>& stop, SearchResults& results, Board& board, uint32_t max_depth);