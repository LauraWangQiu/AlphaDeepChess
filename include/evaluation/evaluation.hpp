#pragma once

/**
 * @file evaluation.hpp
 * @brief evaluation services.
 *
 * chess position evaluation declarations. 
 * 
 */

#include "board.hpp"

/**
 * @brief OPENING = 0  
 * @brief MIDDLEGAME = 1
 * @brief ENDGAME = 2
 */
enum class GamePhase {
    OPENING,
    MIDDLEGAME,
    ENDGAME
};

/** 
 * @brief evaluate_position
 *
 * Evaluate chess position.
 *  
 * @note None.
 * 
 * @param[in] board board to evaluate.
 *
 * @returns
 *  - (0) if position is evaluated as equal.
 *  - (+) if position is evaluated as white is better.
 *  - (-) if position is evaluated as black is better.
 */
float evaluate_position(const Board& board);