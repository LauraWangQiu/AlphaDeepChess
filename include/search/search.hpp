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
constexpr int32_t INFINITE_DEPTH = 1024;

struct SearchResult
{
    std::atomic<int> depth;
    std::atomic<int> evaluation;
    std::atomic<uint16_t> bestMove_data;
};

struct SearchResults
{
    std::atomic<int> depthReached;
    SearchResult results[INFINITE_DEPTH];
};

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
void search_best_move(SearchResults& searchResults, Board board, int32_t max_depth = INFINITE_DEPTH);

/**
 * @brief search_stop
 * 
 * @note this method is thread safe
 * 
 * stop the search process
 * 
 */
void search_stop();

/**
 * @brief is_search_running
 * 
 * @note this method is thread safe
 * 
 * @return True if the search is running (stop is false)
 * 
 */
bool is_search_running();