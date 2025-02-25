/**
 * @file search_basic.cpp
 * @brief search services.
 *
 * chess search basic implementation. 
 * 
 */

#include <iostream>
#include "search.hpp"
#include "move_generator.hpp"
#include "evaluation.hpp"
#include "move_ordering.hpp"
#include "move_list.hpp"

const int INF = std::numeric_limits<int>::max();
const int32_t INMEDIATE_MATE_SCORE = 32000;
const int32_t MATE_THRESHOLD = INMEDIATE_MATE_SCORE - 10000U;

std::atomic<bool> stop;
Move bestMoveFound;
int bestEvalFound;
bool hasSearchedAtLeastOneMove;
Move bestMoveInIteration;
int bestEvalInIteration;

/**
 * @brief search_stop
 * 
 * stop the search process, this method is thread safe
 * 
 */
void search_stop() { stop.store(true); }


static void iterative_deepening(Board& board, int max_depth);
static int alpha_beta(Board& board, int depth, int ply, int alpha, int beta);
static int alpha_beta_captures(Board& board, int alpha, int beta);

/**
 * @brief search_best_move
 * 
 * Calculate the best legal move in the chess position.
 * 
 * @note This method is thread safe
 * 
 * @param[in] board chess position.
 * @param[in] max_depth maximum depth of search, default value is INFINITE_DEPTH
 * 
 * @return best move found in the position.
 * 
 */
Move search_best_move(Board board, int32_t max_depth)
{

    bestEvalInIteration = 0;
    bestMoveInIteration = Move::null();
    bestMoveFound = bestMoveInIteration;
    bestEvalFound = bestEvalInIteration;

    iterative_deepening(board, max_depth);

    // not enough time to find a move, select random move
    if (!bestMoveFound.is_valid()) {
        MoveList moves;
        bool isMate, isStaleMate;
        generate_legal_moves(moves, board, isMate, isStaleMate);
        bestMoveFound = moves[0];
        std::cout << "random move selected " << bestMoveFound.to_string() << "\n";
    }
    return bestMoveFound;
}

void iterative_deepening(Board& board, int max_depth)
{

    stop = false;

    for (int depth = 1; depth <= max_depth; depth++) {
        hasSearchedAtLeastOneMove = false;


        alpha_beta(board, depth, 0, -INF, +INF);

        if (stop) {
            if (hasSearchedAtLeastOneMove) {
                bestMoveFound = bestMoveInIteration;
                bestEvalFound = bestEvalInIteration;
            }

            break;
        }
        else {

            bestMoveFound = bestMoveInIteration;
            bestEvalFound = bestEvalInIteration;

            if (abs(bestEvalFound) > MATE_THRESHOLD) {
                break;   // We found a checkmate
            }
        }

        std::cout << "info depth " << depth << " score " << bestEvalFound << " bestMove "
                  << bestMoveFound.to_string() << "\n";
    }
}

int alpha_beta(Board& board, int depth, int ply, int alpha, int beta)
{

    if (stop) return 0;

    if (ply > 0) {
        // mate in less moves has better score than in more moves
        alpha = std::max(alpha, -MATE_IN_ONE_SCORE + ply);
        beta = std::min(beta, MATE_IN_ONE_SCORE - ply);
        if (alpha >= beta) {
            //numCutoffs++; // debug
            return alpha;
        }
    }

    if (depth == 0) {
        return alpha_beta_captures(board, alpha, beta);
    }

    MoveList moves;
    bool isCheckMate, isStaleMate = false;
    generate_legal_moves(moves, board, isCheckMate, isStaleMate);

    if (isCheckMate) {
        int mateScore = MATE_IN_ONE_SCORE - ply;
        return -mateScore;
    }
    else if (isStaleMate) {
        return 0;
    }
    order_moves(moves, board);

    Move bestMoveInPosition;
    const GameState game_state = board.state();

    for (int i = 0; i < moves.size(); i++) {

        board.make_move(moves[i]);
        int evaluation = -alpha_beta(board, depth - 1, ply + 1, -beta, -alpha);
        board.unmake_move(moves[i], game_state);

        if (stop) {
            return 0;
        }

        if (evaluation >= beta) {
            return beta;
        }

        if (evaluation > alpha) {
            bestMoveInPosition = moves[i];
            alpha = evaluation;
            if (ply == 0) {
                bestMoveInIteration = moves[i];
                bestEvalInIteration = evaluation;
                hasSearchedAtLeastOneMove = true;
            }
        }
    }

    return alpha;
}

int alpha_beta_captures(Board& board, int alpha, int beta)
{
    if (stop) {
        return 0;
    }

    int evaluation = evaluate_position(board);

    if (evaluation >= beta) {
        return beta;
    }
    if (evaluation > alpha) {
        alpha = evaluation;
    }

    MoveList moves;
    bool isCheckMate, isStaleMate = false;
    generate_legal_moves(moves, board, isCheckMate, isStaleMate);

    order_moves(moves, board);

    Move bestMoveInPosition;
    const GameState game_state = board.state();

    for (int i = 0; i < moves.size(); i++) {
        if (!board.move_is_capture(moves[i])) continue;

        board.make_move(moves[i]);
        int evaluation = -alpha_beta_captures(board, -beta, -alpha);
        board.unmake_move(moves[i], game_state);

        if (stop) {
            return 0;
        }

        if (evaluation >= beta) {
            return beta;
        }
        if (evaluation > alpha) {
            alpha = evaluation;
        }
    }

    return alpha;
}