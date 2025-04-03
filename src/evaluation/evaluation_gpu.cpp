/**
 * @file evaluation_dynamic.cpp
 * @brief evaluation services implementation.
 *
 * chess position dynamic evaluation implementation.
 * 
 * https://www.chessprogramming.org/Evaluation
 * https://www.chessprogramming.org/Simplified_Evaluation_Function
 * https://www.chessprogramming.org/Tapered_Eval
 * 
 */
#include "evaluation.hpp"
#include "move_list.hpp"
#include "move_generator.hpp"
#include "precomputed_eval_data.hpp"
#include "bit_utilities.hpp"
#include <julia.h>
#include <iostream>
#include <mutex>

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
int evaluate_position(const Board& board)
{
    std::string fen = board.fen();
    std::string julia_call = "evaluate_position(\"" + fen + "\")";
    jl_value_t *result = jl_eval_string(julia_call.c_str());

    if (jl_exception_occurred()) {
        std::cout << "Error executing evaluate_position: " << jl_typeof_str(jl_exception_occurred()) << ". Returning 0..." << std::endl;
        jl_exception_clear();
        return 0;
    }
    
    int evaluation = jl_unbox_int32(result);
    return evaluation;
}
