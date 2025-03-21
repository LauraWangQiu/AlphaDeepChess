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
    static inline void insert_position(uint64_t position_hash)
    {
        last_positions[next_position_index] = position_hash;
        next_position_index = next_position_index < LAST_POSITIONS_SIZE - 1 ? next_position_index + 1 : 0;
    }

    static inline void remove_last_position()
    {
        next_position_index = next_position_index > 0 ? next_position_index - 1 : LAST_POSITIONS_SIZE - 1;

        last_positions[next_position_index] = 0ULL;
    }

    static inline bool threefold_repetition_detected(uint64_t position_hash)
    {
        int repetitions = 0;
        for (uint64_t position : last_positions) {
            if (position == position_hash) {
                repetitions++;
            }
        }

        return repetitions >= 3;
    }

    static inline void clear()
    {
        for (int i = 0; i < LAST_POSITIONS_SIZE; i++) {
            last_positions[i] = 0ULL;
        }
        next_position_index = 0;
    }

    History() = delete;
    ~History() = delete;

private:
    // max number of positions in the array
    static constexpr int LAST_POSITIONS_SIZE = 10;

    // position array circular index
    static int next_position_index;

    // circular array with the hash of the last positions
    static uint64_t last_positions[LAST_POSITIONS_SIZE];
};

int History::next_position_index = 0;
uint64_t History::last_positions[History::LAST_POSITIONS_SIZE] = {0};