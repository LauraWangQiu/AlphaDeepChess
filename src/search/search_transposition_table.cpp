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

#include "search.hpp"
#include "search_utils.hpp"
#include "move_generator.hpp"
#include "evaluation.hpp"
#include "move_ordering.hpp"
#include "move_list.hpp"
#include "transposition_table.hpp"
#include "history.hpp"

/**
 * @brief search_stop
 * 
 * @note this variable is thread safe
 * 
 * stop the search process
 * 
 */
std::atomic<bool> searchStop;

static void iterative_deepening(SearchResults& searchResults, Board& board, int max_depth, SearchContext& context);

template<SearchType searchType>
static int alpha_beta_search(Board& board, int depth, int ply, int alpha, int beta, SearchContext& context);

template<SearchType searchType>
static int quiescence_search(Board& board, int ply, int alpha, int beta);

static bool get_entry_in_transposition_table(uint64_t zobrist, int depth, int alpha, int beta, int& eval, Move& move);

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
    SearchContext context;

    const ChessColor side_to_move = board.state().side_to_move();

    context.bestEvalFound = side_to_move == ChessColor::WHITE ? -INF_EVAL : +INF_EVAL;
    context.bestMoveFound = Move::null();
    searchResults.depthReached = 0;
    searchStop = false;

    iterative_deepening(searchResults, board, max_depth, context);

    if (!context.bestMoveFound.is_valid()) {
        // if none move found choose one
        MoveList moves;
        generate_legal_moves<ALL_MOVES>(moves, board);
        context.bestMoveFound = moves[0];
        context.bestEvalFound = 0;
        const int depth = 1;
        insert_new_result(searchResults, depth, context.bestEvalFound, context.bestMoveFound);
    }

    searchStop = true;

    //notify the reader thread that search has stopped
    searchResults.data_available_cv.notify_one();
}

void iterative_deepening(SearchResults& searchResults, Board& board, int max_depth, SearchContext& context)
{
    const ChessColor side_to_move = board.state().side_to_move();

    History::clear();

    for (int depth = 1; depth <= max_depth; depth++) {

        context.bestMoveInIteration = Move::null();
        context.bestEvalInIteration = side_to_move == ChessColor::WHITE ? -INF_EVAL : +INF_EVAL;

        side_to_move == ChessColor::WHITE
            ? alpha_beta_search<MAXIMIZE_WHITE>(board, depth, 0, -INF_EVAL, +INF_EVAL, context)
            : alpha_beta_search<MINIMIZE_BLACK>(board, depth, 0, -INF_EVAL, +INF_EVAL, context);

        if (searchStop) {
            break;
        }

        context.bestMoveFound = context.bestMoveInIteration;
        context.bestEvalFound = context.bestEvalInIteration;

        assert(context.bestMoveFound.is_valid());

        insert_new_result(searchResults, depth, context.bestEvalFound, context.bestMoveFound);

        if (abs(context.bestEvalFound) > MATE_THRESHOLD) {
            break;   // We found a checkmate, we stop because we cant find a shorter checkMate
        }
    }
}

/**
  * @brief alpha_beta_search
  * 
  * Alpha beta search.
  * 
  * @tparam searchType [MAXIMIZE_WHITE, MINIMIZE_BLACK]
  * 
  * @param[in] board chess position.
  * @param[in] depth current depth in the tree
  * @param[in] ply   current ply in the tree
  * @param[in] alpha maximum value that the maximizing player(white) can guarantee
  * @param[in] beta  minimum value that the minimizing player(black) can guarantee
  * 
  * @return best score possible for black (minimum score), for white (maximum score)
  * 
  */
template<SearchType searchType>
static int alpha_beta_search(Board& board, int depth, int ply, int alpha, int beta, SearchContext& context)
{
    constexpr bool MAXIMIZING_WHITE = searchType == MAXIMIZE_WHITE;
    constexpr bool MINIMIZING_BLACK = searchType == MINIMIZE_BLACK;

    const uint64_t zobrist_key = board.state().get_zobrist_key();

    // check transposition table
    if (ply == 0) {
        int eval_tt;
        Move move_tt;
        if (get_entry_in_transposition_table(zobrist_key, depth, alpha, beta, eval_tt, move_tt)) {
            context.bestEvalInIteration = eval_tt;
            context.bestMoveInIteration = move_tt;
            return eval_tt;
        }
    }

    MoveList moves;
    bool isCheck;
    generate_legal_moves<ALL_MOVES>(moves, board, &isCheck);
    const bool isCheckMate = isCheck && moves.size() == 0;
    const bool isStaleMate = !isCheck && moves.size() == 0;

    if (isCheckMate) {
        // we substract ply so checkMate in less moves has a higher score
        if constexpr (MAXIMIZE_WHITE) {
            return -(MATE_IN_ONE_SCORE - ply);
        }
        else if constexpr (MINIMIZE_BLACK) {
            return MATE_IN_ONE_SCORE - ply;
        }
    }
    else if (isStaleMate || History::threefold_repetition_detected(zobrist_key)) {
        return 0;
    }
    else if (depth == 0) {
        return quiescence_search<searchType>(board, ply, alpha, beta);
    }

    TranspositionTable::NodeType node_tt = TranspositionTable::NodeType::EXACT;
    Move best_move_for_tt;
    constexpr int worst_evaluation = MAXIMIZING_WHITE ? -INF_EVAL : +INF_EVAL;
    int best_eval_for_tt = worst_evaluation;
    int final_node_evaluation = worst_evaluation;

    const GameState game_state = board.state();

    order_moves(moves, board);

    for (int i = 0; i < moves.size(); i++) {

        if (searchStop) {
            return 0;
        }

        constexpr SearchType nextSearchType = MAXIMIZING_WHITE ? MINIMIZE_BLACK : MAXIMIZE_WHITE;

        History::push_position(zobrist_key);
        board.make_move(moves[i]);
        int eval = alpha_beta_search<nextSearchType>(board, depth - 1, ply + 1, alpha, beta, context);
        board.unmake_move(moves[i], game_state);
        History::pop_position();

        if constexpr (MAXIMIZING_WHITE) {

            if (eval > best_eval_for_tt) {
                best_eval_for_tt = eval;
                best_move_for_tt = moves[i];
            }

            if (ply == 0 && eval > context.bestEvalInIteration) {
                context.bestEvalInIteration = eval;
                context.bestMoveInIteration = moves[i];   // if we are in the root node update the best move
            }

            final_node_evaluation = std::max(final_node_evaluation, eval);
            alpha = std::max(alpha, eval);

            if (final_node_evaluation >= beta) {
                node_tt = TranspositionTable::NodeType::LOWER_BOUND;
                break;   // beta cutoff
            }
        }
        else if (MINIMIZING_BLACK) {

            if (eval < best_eval_for_tt) {
                best_eval_for_tt = eval;
                best_move_for_tt = moves[i];
            }

            if (ply == 0 && eval < context.bestEvalInIteration) {
                context.bestEvalInIteration = eval;
                context.bestMoveInIteration = moves[i];   // if we are in the root node update the best move
            }

            final_node_evaluation = std::min(final_node_evaluation, eval);
            beta = std::min(beta, eval);

            if (final_node_evaluation <= alpha) {
                node_tt = TranspositionTable::NodeType::UPPER_BOUND;
                break;   // alpha cutoff
            }
        }
    }

    if (best_move_for_tt.is_valid()) {
        TranspositionTable::store_entry(zobrist_key, best_eval_for_tt, best_move_for_tt, node_tt, depth);
    }

    return final_node_evaluation;
}

/**
  * @brief quiescence_search
  * 
  * Alpha beta search only considering the capture moves, this is called when we reach the maximum depth
  * and it is paramaunt in order to avoid the 'horizon effect', for example if we stop the search in the
  * middle of a piece exchange the evaluation is corrupted.
  * 
  * The search continues until there are no more capture moves, to also implement cutoffs we first calculate
  * the static evaluation of the position (stand_pat)
  * 
  * @tparam searchType [MAXIMIZE_WHITE, MINIMIZE_BLACK]
  * 
  * @param[in] board chess position.
  * @param[in] ply   current ply in the tree
  * @param[in] alpha maximum value that the maximizing player(white) can guarantee
  * @param[in] beta  minimum value that the minimizing player(black) can guarantee
  * 
  * @return best score possible for black (minimum score possible), for white (maximum score possible)
  * 
  */
template<SearchType searchType>
static int quiescence_search(Board& board, int ply, int alpha, int beta)
{
    constexpr bool MAXIMIZING_WHITE = searchType == MAXIMIZE_WHITE;
    constexpr bool MINIMIZING_BLACK = searchType == MINIMIZE_BLACK;

    const uint64_t zobrist_key = board.state().get_zobrist_key();

    if (History::threefold_repetition_detected(zobrist_key)) {
        return 0;
    }

    int static_evaluation = evaluate_position(board);

    if (ply >= MAX_PLY) {
        return static_evaluation;
    }

    if constexpr (MAXIMIZING_WHITE) {
        if (static_evaluation >= beta) {
            return beta;   // beta cutoff
        }
        alpha = std::max(alpha, static_evaluation);
    }
    else if constexpr (MINIMIZING_BLACK) {
        if (static_evaluation <= alpha) {
            return alpha;   // Alpha cutoff
        }
        beta = std::min(beta, static_evaluation);
    }

    MoveList capture_moves;
    generate_legal_moves<ONLY_CAPTURES>(capture_moves, board);

    if (capture_moves.size() == 0) {
        return static_evaluation;   // No captures: return static evaluation
    }

    order_moves(capture_moves, board);

    const GameState game_state = board.state();
    int final_node_evaluation = static_evaluation;

    for (int i = 0; i < capture_moves.size(); i++) {

        if (searchStop) {
            return 0;
        }

        constexpr SearchType nextSearchType = MAXIMIZING_WHITE ? MINIMIZE_BLACK : MAXIMIZE_WHITE;

        History::push_position(zobrist_key);
        board.make_move(capture_moves[i]);
        int eval = quiescence_search<nextSearchType>(board, ply + 1, alpha, beta);
        board.unmake_move(capture_moves[i], game_state);
        History::pop_position();

        if constexpr (MAXIMIZING_WHITE) {
            final_node_evaluation = std::max(final_node_evaluation, eval);
            alpha = std::max(alpha, eval);

            if (final_node_evaluation >= beta) {
                break;   // Beta cutoff
            }
        }
        else if constexpr (MINIMIZING_BLACK) {
            final_node_evaluation = std::min(final_node_evaluation, eval);
            beta = std::min(beta, eval);

            if (final_node_evaluation <= alpha) {
                break;   // Alpha cutoff
            }
        }
    }

    return final_node_evaluation;
}

static bool get_entry_in_transposition_table(uint64_t zobrist, int depth, int alpha, int beta, int& eval, Move& move)
{
    const TranspositionTable::Entry entry = TranspositionTable::get_entry(zobrist);

    eval = entry.evaluation;
    move = entry.move;

    if (!entry.is_valid() || entry.node_type == TranspositionTable::NodeType::PERFT) {
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

/**
  * @brief search_stop
  * 
  * @note this method is thread safe
  * 
  * stop the search process
  * 
  */
void search_stop() { searchStop.store(true); }

/**
    * @brief is_search_running
    * 
    * @note this method is thread safe
    * 
    * @return True if the search is running (stop is false)
    * 
    */
bool is_search_running() { return !searchStop.load(); }