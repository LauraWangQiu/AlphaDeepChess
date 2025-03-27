#pragma once

/**
 * @file history.hpp
 * @brief search history utilities declaration.
 * 
 */

#include <cstdint>

/**
 * @brief History
 *
 * History class, used for triple repetition detection
 * 
 */
class History
{
public:
    /**
     * @brief push_position(uint64_t)
     * 
     * Insert new position in the game history 
     * 
     * @param[in] position_hash zobrist hash of the position
     *   
     */
    static void push_position(uint64_t position_hash);

    /**
     * @brief pop_position()
     * 
     * Remove last inserted position in the game history 
     *       
     */
    static void pop_position();

    /**
     * @brief calculate if threefold repetition has happened in the history of positions
     * 
     * @note we consider repetition with 2 instead of 3 reps for performance reasons
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
    static bool threefold_repetition_detected(uint8_t fify_move_rule_counter);

    /**
     * @brief clear()
     * 
     * remove all position in the history
     *       
     */
    static void clear();

    /**
     * @brief MAX_SIZE()
     * 
     * @return HISTORY_MAX_SIZE
     *       
     */
    static constexpr int MAX_SIZE() { return HISTORY_MAX_SIZE; }

    History() = delete;
    ~History() = delete;

private:
    // max number of positions in the array (must be power of two)
    static constexpr int HISTORY_MAX_SIZE = 64;
    static_assert((HISTORY_MAX_SIZE & (HISTORY_MAX_SIZE - 1)) == 0, "HISTORY_MAX_SIZE has to be a power of 2");

    // position array circular index
    static int next_position_index;

    // circular array with the hash of the last positions
    static uint64_t positions[HISTORY_MAX_SIZE];
};
