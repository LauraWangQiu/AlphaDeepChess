/**
 * @file search_basic.cpp
 * @brief search services.
 *
 * chess search basic implementation. 
 * 
 */

#include "search.hpp"

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
Move search_best_move(Board& board, uint32_t max_depth, std::atomic<bool>& stop)
{
    Move best_move = Move::null();
    while(!stop)
    {
        
    }

    return best_move;
}