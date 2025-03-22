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
     * @brief threefold_repetition_detected(uint64_t)
     * 
     * @return True if lthe position is repeated three times in the history
     *       
     */
    static bool threefold_repetition_detected(uint64_t position_hash);

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
    static constexpr int HISTORY_MAX_SIZE = 8;

    // position array circular index
    static int next_position_index;

    // circular array with the hash of the last positions
    static uint64_t last_positions[HISTORY_MAX_SIZE];
};
