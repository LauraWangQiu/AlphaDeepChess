/**
 * @file history.cpp
 * @brief search history utilities implementation.
 * 
 */

#include "history.hpp"
#include <cassert>

// max number of positions in the array
int History::next_position_index = 0;

// circular array with the hash of the last positions
uint64_t History::last_positions[HISTORY_MAX_SIZE] = {0ULL};

/**
  * @brief push_position(uint64_t)
  * 
  * Insert new position in the game history 
  * 
  * @param[in] position_hash zobrist hash of the position
  *   
  */
void History::push_position(uint64_t position_hash)
{
    last_positions[next_position_index] = position_hash;
    // update index : (next_position_index + 1) % HISTORY_MAX_SIZE
    next_position_index = (next_position_index + 1) & (HISTORY_MAX_SIZE - 1);
}

/**
 * @brief pop_position()
 * 
 * Remove last inserted position in the game history 
 *       
 */
void History::pop_position()
{
    // update index : (next_position_index - 1) % HISTORY_MAX_SIZE
    next_position_index = (next_position_index + HISTORY_MAX_SIZE - 1) & (HISTORY_MAX_SIZE - 1);
    last_positions[next_position_index] = 0ULL;
}

/**
 * @brief threefold_repetition_detected(uint64_t)
 * 
 * @note we consider repetition with 2 instead of 3 reps for performance reasons
 * @return True if the position is repeated two times in the history
 *       
 */
bool History::threefold_repetition_detected(uint64_t position_hash)
{
    assert(position_hash != 0ULL);

    int repetitions = 0;
    for (uint64_t position : last_positions) {
        if (position == position_hash) {
            if (++repetitions >= 2) return true;
        }
    }

    return false;
}

/**
 * @brief clear()
 * 
 * remove all position in the history
 *       
 */
void History::clear()
{
    for (int i = 0; i < HISTORY_MAX_SIZE; i++) {
        last_positions[i] = 0ULL;
    }
    next_position_index = 0;
}