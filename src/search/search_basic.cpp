/**
 * @file search_basic.cpp
 * @brief search services.
 *
 * chess search basic implementation. 
 * 
 */

#include "search.hpp"
#include "move_generator.hpp"
#include "evaluation.hpp"
#include "move_list.hpp"

#include <limits>

// Constants for infinity
const int INF = std::numeric_limits<int>::max();

int minimax(Board& board, uint32_t depth, bool isMaximizingPlayer);

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
    /*while (!stop) {
    }*/

    int bestValue = -INF;

    MoveList moves;
    generate_legal_moves(moves, board);
    const GameState game_state = board.state();

    for (int i = 0; i < moves.size(); i++) {
        board.make_move(moves[i]);
        int moveValue = minimax(board, max_depth - 1, false);
        board.unmake_move(moves[i], game_state);

        if (moveValue > bestValue) {
            bestValue = moveValue;
            best_move = moves[i];
        }
    }

    return best_move;
}

// Minimax function
int minimax(Board& board, uint32_t depth, bool isMaximizingPlayer)
{
    if (depth == 0) {
        return evaluate_position(board);
    }

    MoveList moves;
    generate_legal_moves(moves, board);
    const GameState game_state = board.state();

    /*if (moves.size() == 0) {
        return evaluate_position(board);
    }*/

    if (isMaximizingPlayer) {
        int maxEval = -INF;

        for (int i = 0; i < moves.size(); i++) {
            board.make_move(moves[i]);
            int eval = minimax(board, depth - 1, false);
            board.unmake_move(moves[i], game_state);
            maxEval = std::max(maxEval, eval);
        }
        return maxEval;
    }
    else {
        int minEval = INF;

        for (int i = 0; i < moves.size(); i++) {
            board.make_move(moves[i]);
            int eval = minimax(board, depth - 1, true);
            board.unmake_move(moves[i], game_state);
            minEval = std::min(minEval, eval);
        }
        return minEval;
    }
}

/*
Move search_best_move(Board& board, uint32_t max_depth, std::atomic<bool>& stop) {
    Move best_move = Move::null();
    int bestValue = -INF;

    MoveList moves;
    generate_legal_moves(moves, board);
    const GameState game_state = board.state();

    for (int i = 0; i < moves.size(); i++) {
        board.make_move(moves[i]);
        int moveValue = alpha_beta(board, max_depth - 1, -INF, INF, false);
        board.unmake_move(moves[i], game_state);

        if (moveValue > bestValue) {
            bestValue = moveValue;
            best_move = moves[i];
        }
    }

    return best_move;
}
*/
/*
int alpha_beta(Board& board, uint32_t depth, int alpha, int beta, bool isMaximizingPlayer) {
    if (depth == 0) {
        return evaluate_position(board);
    }

    MoveList moves;
    generate_legal_moves(moves, board);
    const GameState game_state = board.state();

    if (isMaximizingPlayer) {
        int maxEval = -INF;

        for (int i = 0; i < moves.size(); i++) {
            board.make_move(moves[i]);
            int eval = alpha_beta(board, depth - 1, alpha, beta, false);
            board.unmake_move(moves[i], game_state);

            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) {
                break; // Poda beta
            }
        }

        return maxEval;
    } else {
        int minEval = INF;

        for (int i = 0; i < moves.size(); i++) {
            board.make_move(moves[i]);
            int eval = alpha_beta(board, depth - 1, alpha, beta, true);
            board.unmake_move(moves[i], game_state);

            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) {
                break; // Poda alfa
            }
        }

        return minEval;
    }
}
*/