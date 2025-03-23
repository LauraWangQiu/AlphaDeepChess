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
constexpr int32_t INF_DEPTH = 1024;

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
    SearchResult results[INF_DEPTH];
};

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
void search(std::atomic<bool>& stop, SearchResults& results, Board& board, int32_t max_depth = INF_DEPTH);