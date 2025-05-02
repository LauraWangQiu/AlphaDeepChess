#pragma once

/**
 * @file search_utils.hpp
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
#ifdef _MSC_VER
#include <xmmintrin.h> // For _mm_prefetch
#endif

/**
 * @brief Value that represents an infinite search depth.
 *
 * This constant is used to define the maximum depth for a search operation.
 */
constexpr uint32_t INF_DEPTH = 1024;

/**
 * @brief Infinite evaluation score.
 *
 * This constant represents the maximum possible evaluation score in the search algorithm.
 */
constexpr int INF_EVAL = std::numeric_limits<int>::max();

/**
 * @brief Score for mate in one.
 *
 * This constant represents the evaluation score assigned to a position where a mate in one move is possible.
 */
constexpr int MATE_IN_ONE_SCORE = 3200000;

/**
 * @brief Score threshold for mate evaluation.
 *
 * This constant defines the score from which a position is considered a mate evaluation.
 */
constexpr int MATE_THRESHOLD = MATE_IN_ONE_SCORE - 1000U;

/**
 * @brief Maximum ply to be reached in the search.
 *
 * This constant defines the maximum number of plies (half-moves) that the search algorithm will consider.
 */
constexpr int MAX_PLY = 64;

/**
 * @brief Initial aspiration window margin.
 *
 * This constant defines the initial margin for the aspiration window in centipawns (50 cp).
 */
constexpr int ASPIRATION_MARGIN = 50;

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
 * Struct containing updated information about the search.
 */
struct SearchContext
{
    /**
     * @brief Best evaluation found so far.
     *
     * Stores the best evaluation score found during the search.
     */
    int bestEvalFound;

    /**
     * @brief Best evaluation found in the current iteration.
     *
     * Stores the best evaluation score found during the current search iteration.
     */
    int bestEvalInIteration;

    /**
     * @brief Best move found so far.
     *
     * Stores the best move found during the search.
     */
    Move bestMoveFound;

    /**
     * @brief Best move found in the current iteration.
     *
     * Stores the best move found during the current search iteration.
     */
    Move bestMoveInIteration;

    /**
     * @brief Reference to the chessboard.
     *
     * Provides access to the current state of the chessboard during the search.
     */
    Board& board;

    /**
     * @brief Constructor for SearchContext.
     *
     * Initializes the search context with default values.
     *
     * @param[in] board Reference to the chessboard.
     */
    SearchContext(Board& board)
        : bestEvalFound(0), bestEvalInIteration(0), bestMoveFound(), bestMoveInIteration(), board(board)
    { }
};

/**
 * @brief SearchResult
 * 
 * Struct containing the search result for a specific depth.
 */
struct SearchResult
{
    /**
     * @brief Depth at which the search result was calculated.
     *
     * Represents the depth of the search tree where this result was obtained.
     */
    std::atomic<uint32_t> depth;

    /**
     * @brief Evaluation score of the position.
     *
     * The evaluation score of the position at the given depth.
     */
    std::atomic<int> evaluation;

    /**
     * @brief Encoded best move data.
     *
     * Stores the best move found at the given depth as a 16-bit encoded value.
     */
    std::atomic<uint16_t> bestMove_data;
};

/**
 * @brief SearchResults
 * 
 * Struct containing the search results for all depths.
 */
struct SearchResults
{
    /**
     * @brief Mutex for synchronizing access to the search results.
     *
     * Ensures thread-safe access to the search results.
     */
    std::mutex mtx_data_available_cv;

    /**
     * @brief Condition variable for notifying when new data is available.
     *
     * Used to signal threads waiting for updates to the search results.
     */
    std::condition_variable data_available_cv;

    /**
     * @brief Encoded ponder move data.
     *
     * Stores the move to be played if the opponent makes the expected move.
     */
    std::atomic<uint16_t> ponderMove_data;

    /**
     * @brief Depth reached in the search.
     *
     * Indicates the maximum depth reached during the search.
     */
    std::atomic<uint32_t> depthReached;

    /**
     * @brief Array of search results for all depths.
     *
     * Stores the search results for each depth up to `INF_DEPTH`.
     */
    SearchResult results[INF_DEPTH];
};

/**
 * @brief insert_new_result(SearchResults&,int,int,Move)
 * 
 * @note This method is thread safe.
 * 
 * Insert new result in the search results.
 * 
 * @param[in, out] results
 * @param[in] depth calculated depth
 * @param[in] evaluation best evaluation result
 * @param[in] move best move result
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


/**
 * @brief Tells the CPU to load data from memory into the cache.
 * 
 * @param[in] addr Memory address to prefetch.
 */
inline void prefetch(const void* addr)
{
#ifdef NO_PREFETCH
    (void)addr; // suppress unused param warning
#else
    #if defined(_MSC_VER)
        _mm_prefetch((char const*)addr, _MM_HINT_T0);
    #else
        __builtin_prefetch(addr);
    #endif
#endif
}
