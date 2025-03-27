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
uint64_t History::positions[HISTORY_MAX_SIZE] = {0ULL};

/**
  * @brief move index forward
  * 
  * @param[in] index index to increment
  * @param[in] increment number of times to move forward the index
  * @param[in] MAX_INDEX max index value, must be power of two
  * 
  * @return (index + increment) % MAX_INDEX
  */
static inline int index_forward(int index, int increment, int MAX_INDEX)
{
    assert((MAX_INDEX & (MAX_INDEX - 1)) == 0);   // MAX_INDEX must be power of two
    return (index + increment) & (MAX_INDEX - 1);
}

/**
  * @brief move index backwards
  * 
  * @param[in] index index to decrement
  * @param[in] decrement number of times to move backwards the index
  * @param[in] MAX_INDEX max index value, must be power of two
  * 
  * @return (index - increment) % MAX_INDEX
  */
static inline int index_backwards(int index, int decrement, int MAX_INDEX)
{
    return (index - decrement) & (MAX_INDEX - 1);
}

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
    positions[next_position_index] = position_hash;
    next_position_index = index_forward(next_position_index, 1, HISTORY_MAX_SIZE);
}

/**
 * @brief pop_position()
 * 
 * Remove last inserted position in the game history 
 *       
 */
void History::pop_position()
{
    next_position_index = index_backwards(next_position_index, 1, HISTORY_MAX_SIZE);
    positions[next_position_index] = 0ULL;
}

/**
 * @brief calculate if threefold repetition has happened in the history of positions
 * 
 * 
 * @param[in] fify_move_rule_counter also called halfmove clock, it indicates The number of
 *   irreversible moves made before the current position. Serves both in the early return condition
 *   and limits how far up the stack one must look for a potential match.
 * 
 * @return bool
 * @retval True if repetition found
 * @retval False if repetition not found
 * 
 */
bool History::threefold_repetition_detected(uint8_t fify_move_rule_counter)
{
    if (fify_move_rule_counter < 4) {
        return false;   // imposible to have triple repetition if halfmove clock < 4
    }
    const int last_index = index_backwards(next_position_index, 1, HISTORY_MAX_SIZE);

    uint64_t last_pos_hash = positions[last_index];

    assert(last_pos_hash != 0ULL);

    for (int decrement = 4; decrement <= fify_move_rule_counter; decrement += 2) {
        int index = index_backwards(last_index, decrement, HISTORY_MAX_SIZE);
        if (last_pos_hash == positions[index]) {
            return true;   // we found a repetition
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
        positions[i] = 0ULL;
    }
    next_position_index = 0;
}