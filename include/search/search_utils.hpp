#pragma once

/**
 * @file search:utils.hpp
 * @brief search utilities declaration.
 * 
 */

#include "board.hpp"
#include "move.hpp"
#include <cstdint>
#include <limits>
#include <atomic>
#include <condition_variable>
#include <mutex>

// value that represents an infinite search depth.
constexpr uint32_t INF_DEPTH = 1024;

// infinite evaluation score
constexpr int INF_EVAL = std::numeric_limits<int>::max();

// score for mate in one
constexpr int INMEDIATE_MATE_SCORE = 32000;

// score from which it is consider mate evaluation
constexpr int MATE_THRESHOLD = INMEDIATE_MATE_SCORE - 1000U;

// max ply to be reached
constexpr int MAX_PLY = 32;

/**
 * @brief SearchType
 * 
 * @note used in the templates for search functions
 * 
 * -MAXIMIZE_WHITE = 0,
 * -MINIMIZE_BLACK = 1
 * 
 */
enum SearchType : int
{
    MAXIMIZE_WHITE = 0,
    MINIMIZE_BLACK = 1
};

/**
 * @brief SearchContext
 * 
 * struct containing updated information about the search
 * 
 */
struct SearchContext
{
    int bestEvalFound;
    int bestEvalInIteration;
    Move bestMoveFound;
    Move bestMoveInIteration;
    Board& board;

    SearchContext(Board& board)
        : bestEvalFound(0), bestEvalInIteration(0), bestMoveFound(), bestMoveInIteration(), board(board)
    { }
};

struct SearchResult
{
    std::atomic<uint32_t> depth;
    std::atomic<int> evaluation;
    std::atomic<uint16_t> bestMove_data;
};

struct SearchResults
{
    std::mutex mtx_data_available_cv;
    std::condition_variable data_available_cv;
    std::atomic<uint32_t> depthReached;
    SearchResult results[INF_DEPTH];
};

/**
 * @brief insert_new_result(SearchResults&,int,int,Move)
 * 
 * @note this method is thread safe
 * 
 * insert new result in the search results
 * 
 * @param[in, out] searchResults
 * @param[in] depth calculated depth
 * @param[in] evaluation best evaluation result
 * @param[in] move best move result
 * 
 */
inline void insert_new_result(SearchResults& results, int depth, int evaluation, Move move)
{
    assert(results.depthReached < INF_DEPTH);
    results.results[results.depthReached].depth = depth;
    results.results[results.depthReached].evaluation = evaluation;
    results.results[results.depthReached].bestMove_data = move.raw_data();
    results.depthReached++;

    results.data_available_cv.notify_one();
}