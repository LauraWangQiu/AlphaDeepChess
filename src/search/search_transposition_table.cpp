/**
 * @file search_basic.cpp
 * @brief search services.
 *
 * chess search basic implementation. 
 * 
 * https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning
 * https://www.chessprogramming.org/Alpha-Beta
 * https://www.chessprogramming.org/Quiescence_Search
 * 
 */
#include <limits>
#include "search.hpp"
#include "move_generator.hpp"
#include "evaluation.hpp"
#include "move_ordering.hpp"
#include "move_list.hpp"
#include "transposition_table.hpp"

const int INF = std::numeric_limits<int>::max();
const int INMEDIATE_MATE_SCORE = 32000;
const int MATE_THRESHOLD = INMEDIATE_MATE_SCORE - 1000U;

static std::atomic<bool> stop;
static Move bestMoveFound;
static int bestEvalFound;
static Move bestMoveInIteration;
static int bestEvalInIteration;

static inline void iterative_deepening(SearchResults& searchResults, Board& board, int max_depth);
static int alpha_beta_maximize_white(Board& board, int depth, int ply, int alpha, int beta);
static int alpha_beta_minimize_black(Board& board, int depth, int ply, int alpha, int beta);
static int quiescence_maximize_white(Board& board, int ply, int alpha, int beta);
static int quiescence_minimize_black(Board& board, int ply, int alpha, int beta);
static void insert_new_result(SearchResults& searchResults, int depth, int evaluation, Move move);
static bool get_entry_in_transposition_table(uint64_t zobrist, int depth, int alpha, int beta, int& eval, Move& move);

/**
  * @brief search_stop
  * 
  * @note this method is thread safe
  * 
  * stop the search process
  * 
  */
void search_stop() { stop.store(true); }

/**
  * @brief is_search_running
  * 
  * @note this method is thread safe
  * 
  * @return True if the search is running (stop is false)
  * 
  */
bool is_search_running() { return !stop.load(); }

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
void search_best_move(SearchResults& searchResults, Board board, int32_t max_depth)
{
    const ChessColor side_to_move = board.state().side_to_move();

    bestEvalFound = side_to_move == ChessColor::WHITE ? -INF : +INF;
    bestMoveFound = Move::null();
    searchResults.depthReached = 0;
    stop = false;

    iterative_deepening(searchResults, board, max_depth);

    stop = true;

    {
        //notify the reader thread that search has stopped
        std::lock_guard<std::mutex> lock(searchResults.mtx_data_available_cv);
        searchResults.data_available_cv.notify_one();
    }
}

void iterative_deepening(SearchResults& searchResults, Board& board, int max_depth)
{

    const ChessColor side_to_move = board.state().side_to_move();

    for (int depth = 1; depth <= max_depth; depth++) {

        bestMoveInIteration = Move::null();
        bestEvalInIteration = side_to_move == ChessColor::WHITE ? -INF : +INF;

        side_to_move == ChessColor::WHITE ? alpha_beta_maximize_white(board, depth, 0, -INF, +INF)
                                          : alpha_beta_minimize_black(board, depth, 0, -INF, +INF);

        if (stop) {
            break;
        }

        bestMoveFound = bestMoveInIteration;
        bestEvalFound = bestEvalInIteration;

        assert(bestMoveFound.is_valid());

        insert_new_result(searchResults, depth, bestEvalFound, bestMoveFound);

        if (abs(bestEvalFound) > MATE_THRESHOLD) {
            break;   // We found a checkmate, we stop because we cant find a shorter checkMate
        }
    }
}

/**
  * @brief alpha_beta_maximize_white
  * 
  * Alpha beta white to move node maximizing the evaluation
  * 
  * @param[in] board chess position.
  * @param[in] depth current depth in the tree
  * @param[in] ply   current ply in the tree (use to track checkMate score)
  * @param[in] alpha maximum value that the maximizing player(white) can guarantee
  * @param[in] beta  minimum value that the minimizing player(black) can guarantee
  * 
  * @return best score possible for white in this node (maximum score possible)
  * 
  */
int alpha_beta_maximize_white(Board& board, int depth, int ply, int alpha, int beta)
{
    const uint64_t zobrist_key = board.state().get_zobrist_key();

    // check transposition table
    if (ply == 0) {
        int eval_tt;
        Move move_tt;
        if (get_entry_in_transposition_table(zobrist_key, depth, alpha, beta, eval_tt, move_tt)) {
            bestEvalInIteration = eval_tt;
            bestMoveInIteration = move_tt;
            return eval_tt;
        }
    }

    MoveList moves;
    bool isCheckMate, isStaleMate = false;
    generate_legal_moves(moves, board, &isCheckMate, &isStaleMate);

    if (isCheckMate) {
        // we substract ply so checkMate in less moves has a higher score
        return -(MATE_IN_ONE_SCORE - ply);   // white is in checkmate
    }
    else if (isStaleMate) {
        return 0;
    }
    else if (depth == 0) {
        return quiescence_maximize_white(board, ply, alpha, beta);
    }

    TranspositionTable::NodeType node_tt = TranspositionTable::NodeType::EXACT;
    Move best_move_in_pos_for_tt;
    int best_eval_in_pos_for_tt = -INF;

    int max_evaluation = -INF;
    const GameState game_state = board.state();

    order_moves(moves, board);

    for (int i = 0; i < moves.size(); i++) {

        if (stop) {
            return 0;
        }

        board.make_move(moves[i]);
        int evaluation = alpha_beta_minimize_black(board, depth - 1, ply + 1, alpha, beta);
        board.unmake_move(moves[i], game_state);

        if (evaluation > best_eval_in_pos_for_tt) {
            // this statement is to store the best move of the position in the transposition table
            best_eval_in_pos_for_tt = evaluation;
            best_move_in_pos_for_tt = moves[i];
        }

        if (ply == 0 && evaluation > bestEvalInIteration) {
            // if we are in the root node update the best move
            bestEvalInIteration = evaluation;
            bestMoveInIteration = moves[i];
        }

        max_evaluation = std::max(max_evaluation, evaluation);
        alpha = std::max(alpha, evaluation);

        if (max_evaluation >= beta) {
            node_tt = TranspositionTable::NodeType::LOWER_BOUND;
            break;   // beta cutoff
        }
    }

    if (best_move_in_pos_for_tt.is_valid()) {
        TranspositionTable::store_entry(zobrist_key, best_eval_in_pos_for_tt, best_move_in_pos_for_tt, node_tt, depth);
    }
    return max_evaluation;
}

/**
  * @brief alpha_beta_minimize_black
  * 
  * Alpha beta black to move node minimizing the evaluation
  * 
  * @param[in] board chess position.
  * @param[in] depth current depth in the tree
  * @param[in] ply   current ply in the tree
  * @param[in] alpha maximum value that the maximizing player(white) can guarantee
  * @param[in] beta  minimum value that the minimizing player(black) can guarantee
  * 
  * @return best score possible for black in this node (minimum score possible)
  * 
  */
int alpha_beta_minimize_black(Board& board, int depth, int ply, int alpha, int beta)
{
    const uint64_t zobrist_key = board.state().get_zobrist_key();

    // check transposition table
    if (ply == 0) {
        int eval_tt;
        Move move_tt;
        if (get_entry_in_transposition_table(zobrist_key, depth, alpha, beta, eval_tt, move_tt)) {
            bestEvalInIteration = eval_tt;
            bestMoveInIteration = move_tt;
            return eval_tt;
        }
    }

    MoveList moves;
    bool isCheckMate, isStaleMate = false;
    generate_legal_moves(moves, board, &isCheckMate, &isStaleMate);

    if (isCheckMate) {
        // we substract ply so checkMate in less moves has a higher score
        return MATE_IN_ONE_SCORE - ply;   // black is in checkmate
    }
    else if (isStaleMate) {
        return 0;
    }
    else if (depth == 0) {
        return quiescence_minimize_black(board, ply, alpha, beta);
    }

    TranspositionTable::NodeType node_tt = TranspositionTable::NodeType::EXACT;
    Move best_move_in_pos_for_tt;
    int best_eval_in_pos_for_tt = +INF;

    int min_evaluation = +INF;
    const GameState game_state = board.state();

    order_moves(moves, board);

    for (int i = 0; i < moves.size(); i++) {

        if (stop) {
            return 0;
        }

        board.make_move(moves[i]);
        int evaluation = alpha_beta_maximize_white(board, depth - 1, ply + 1, alpha, beta);
        board.unmake_move(moves[i], game_state);

        if (evaluation < best_eval_in_pos_for_tt) {
            // this statement is to store the best move of the position in the transposition table
            best_eval_in_pos_for_tt = evaluation;
            best_move_in_pos_for_tt = moves[i];
        }

        if (ply == 0 && evaluation < bestEvalInIteration) {
            // if we are in the root node update the best move
            bestEvalInIteration = evaluation;
            bestMoveInIteration = moves[i];
        }

        min_evaluation = std::min(min_evaluation, evaluation);
        beta = std::min(beta, evaluation);

        if (min_evaluation <= alpha) {
            node_tt = TranspositionTable::NodeType::UPPER_BOUND;
            break;   // alpha cutoff
        }
    }

    if (best_move_in_pos_for_tt.is_valid()) {
        TranspositionTable::store_entry(zobrist_key, best_eval_in_pos_for_tt, best_move_in_pos_for_tt, node_tt, depth);
    }
    return min_evaluation;
}

/**
  * @brief quiescence_maximize_white
  * 
  * Alpha beta search only considering the capture moves, this is called when we reach the maximum depth
  * and it is paramaunt in order to avoid the 'horizon effect', for example if we stop the search in the
  * middle of a piece exchange the evaluation is corrupted.
  * 
  * The search continues until there are no more capture moves, to also implement cutoffs we first calculate
  * the static evaluation of the position (stand_pat)
  * 
  * @param[in] board chess position.
  * @param[in] ply   current ply in the tree
  * @param[in] alpha maximum value that the maximizing player(white) can guarantee
  * @param[in] beta  minimum value that the minimizing player(black) can guarantee
  * 
  * @return best score possible for white in this node (maximum score possible)
  * 
  */
int quiescence_maximize_white(Board& board, int ply, int alpha, int beta)
{
    MoveList moves;
    bool isCheckMate, isStaleMate = false;
    generate_legal_moves(moves, board, &isCheckMate, &isStaleMate);

    if (isCheckMate) {
        return -(MATE_IN_ONE_SCORE - ply);
    }
    else if (isStaleMate) {
        return 0;
    }

    int static_evaluation = evaluate_position(board);

    if (static_evaluation >= beta) {
        return beta;   // beta cutoff
    }
    alpha = std::max(alpha, static_evaluation);

    MoveList capture_moves;
    for (int i = 0; i < moves.size(); i++) {
        if (board.move_is_capture(moves[i])) {
            capture_moves.add(moves[i]);
        }
    }

    if (capture_moves.size() == 0) {
        return static_evaluation;   // No captures: return static evaluation
    }

    order_moves(capture_moves, board);

    const GameState game_state = board.state();
    int max_evaluation = static_evaluation;

    for (int i = 0; i < capture_moves.size(); i++) {
        board.make_move(capture_moves[i]);
        int eval = quiescence_minimize_black(board, ply + 1, alpha, beta);
        board.unmake_move(capture_moves[i], game_state);

        max_evaluation = std::max(max_evaluation, eval);
        alpha = std::max(alpha, eval);

        if (max_evaluation >= beta) {
            break;   // Beta cutoff
        }
    }

    return max_evaluation;
}

/**
  * @brief quiescence_maximize_white
  * 
  * Alpha beta search only considering the capture moves, this is called when we reach the maximum depth
  * and it is paramaunt in order to avoid the 'horizon effect', for example if we stop the search in the
  * middle of a piece exchange the evaluation is corrupted.
  * 
  * The search continues until there are no more capture moves, to also implement cutoffs we first calculate
  * the static evaluation of the position (stand_pat)
  * 
  * @param[in] board chess position.
  * @param[in] ply   current ply in the tree
  * @param[in] alpha maximum value that the maximizing player(white) can guarantee
  * @param[in] beta  minimum value that the minimizing player(black) can guarantee
  * 
  * @return best score possible for black in this node (minimum score possible)
  * 
  */
int quiescence_minimize_black(Board& board, int ply, int alpha, int beta)
{
    MoveList moves;
    bool isCheckMate, isStaleMate = false;
    generate_legal_moves(moves, board, &isCheckMate, &isStaleMate);

    if (isCheckMate) {
        return MATE_IN_ONE_SCORE - ply;
    }
    else if (isStaleMate) {
        return 0;
    }

    int static_evaluation = evaluate_position(board);
    if (static_evaluation <= alpha) {
        return alpha;   // Alpha cutoff
    }
    beta = std::min(beta, static_evaluation);

    MoveList capture_moves;
    for (int i = 0; i < moves.size(); i++) {
        if (board.move_is_capture(moves[i])) {
            capture_moves.add(moves[i]);
        }
    }

    if (capture_moves.size() == 0) {
        return static_evaluation;   // No captures: return static evaluation
    }

    order_moves(capture_moves, board);

    const GameState game_state = board.state();
    int min_evaluation = static_evaluation;

    for (int i = 0; i < capture_moves.size(); i++) {
        board.make_move(capture_moves[i]);
        int eval = quiescence_maximize_white(board, ply + 1, alpha, beta);
        board.unmake_move(capture_moves[i], game_state);

        min_evaluation = std::min(min_evaluation, eval);
        beta = std::min(beta, eval);

        if (min_evaluation <= alpha) {
            break;   // Alpha cutoff
        }
    }

    return min_evaluation;
}

static inline void insert_new_result(SearchResults& searchResults, int depth, int evaluation, Move move)
{
    assert(searchResults.depthReached < INFINITE_DEPTH);
    searchResults.results[searchResults.depthReached].depth = depth;
    searchResults.results[searchResults.depthReached].evaluation = evaluation;
    searchResults.results[searchResults.depthReached].bestMove_data = move.raw_data();
    searchResults.depthReached++;

    {
        std::lock_guard<std::mutex> lock(searchResults.mtx_data_available_cv);
        searchResults.data_available_cv.notify_one();
    }
}

static bool get_entry_in_transposition_table(uint64_t zobrist, int depth, int alpha, int beta, int& eval, Move& move)
{
    const TranspositionTable::Entry entry = TranspositionTable::get_entry(zobrist);

    eval = entry.evaluation;
    move = entry.move;

    if (!entry.is_valid()) {
        return false;
    }
    else if (entry.depth < depth) {
        return false;   // entry with a not valid eval because it was calculated at less depth
    }

    switch (entry.node_type) {
    case TranspositionTable::NodeType::EXACT:
        // entry with exact evaluation found
        return true;
        break;
    case TranspositionTable::NodeType::UPPER_BOUND:
        // entry with upper bound evaluation, only valid if eval less than alpha
        return entry.evaluation <= alpha ? true : false;
        break;
    case TranspositionTable::NodeType::LOWER_BOUND:
        // entry with lower bound evaluation, only valid if eval more than beta
        return entry.evaluation >= beta ? true : false;
        break;
    default: return false; break;
    }
}
