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
#include <condition_variable>
#include <mutex>

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
    std::mutex mtx_data_available_cv;
    std::condition_variable data_available_cv;
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
 * @param[in] wtime white time left, default value is INFINITE_WTIME
 * @param[in] btime black time left, default value is INFINITE_BTIME
 * @param[in] movestogo moves to go, default value is INFINITE_MOVES_TO_GO
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