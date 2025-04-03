/**
 * @file search_basic.cpp
 * @brief search services.
 *
 * chess search basic implementation. 
 * 
 * https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning
 * https://www.chessprogramming.org/Alpha-Beta
 * https://www.chessprogramming.org/Aspiration_Windows
 * https://www.chessprogramming.org/Quiescence_Search
 */

#include "search.hpp"
#include "move_generator.hpp"
#include "evaluation.hpp"
#include "move_ordering.hpp"
#include "move_list.hpp"
#include "history.hpp"
#include "killer_moves.hpp"

static void iterative_deepening(std::atomic<bool>& stop, SearchResults& results, int max_depth, SearchContext& context);

template<SearchType searchType>
static int alpha_beta_search(std::atomic<bool>& stop, int depth, int ply, int alpha, int beta, SearchContext& context);

template<SearchType searchType>
static int quiescence_search(std::atomic<bool>& stop, int ply, int alpha, int beta, SearchContext& context);

/**
 * @brief search(std::atomic<bool>&, SearchResults&, Board&, int32_t)
 * 
 * Search the best legal move in the chess position.
 * 
 * @param[in] stop stop search signal.
 * @param[out] results struct where to store the results.
 * @param[in] board chess position.
 * @param[in] max_depth maximum depth of search, default value is INFINITE_DEPTH
 * 
 */
void search(std::atomic<bool>& stop, SearchResults& results, Board& board, uint32_t max_depth)
{
    assert(stop.load() == false);
    assert(results.depthReached == 0);

    SearchContext context(board);

    const ChessColor side_to_move = board.state().side_to_move();

    context.bestEvalFound = is_white(side_to_move) ? -INF_EVAL : +INF_EVAL;
    context.bestMoveFound = Move::null();

    iterative_deepening(stop, results, max_depth, context);

    if (!context.bestMoveFound.is_valid()) {
        // if none move found choose one
        MoveList moves;
        generate_legal_moves<ALL_MOVES>(moves, board);
        context.bestMoveFound = moves[0];
        context.bestEvalFound = 0;
        const int depth = 1;
        insert_new_result(results, depth, context.bestEvalFound, context.bestMoveFound);
    }

    stop = true;

    //notify the reader thread that search has stopped
    results.data_available_cv.notify_one();
}

/**
 * @brief iterative_deepening(SearchResults&,Board&,int,SearchContext)
 * 
 * Realize an iterative search, first at depth 1, then depth 2 ... until max_depth.
 * 
 * @note https://www.chessprogramming.org/Iterative_Deepening
 * 
 * @param[in] stop stop search signal
 * @param[out] results struct where to store the results.
 * @param[in] max_depth maximum depth of search, default value is INFINITE_DEPTH
 * @param[in, out] context  board and best moves so far in the search
 * 
 */
static void iterative_deepening(std::atomic<bool>& stop, SearchResults& results, int max_depth, SearchContext& context)
{
    Board& board = context.board;
    const ChessColor side_to_move = board.state().side_to_move();
    KillerMoves::clear();

    int alpha = -INF_EVAL;
    int beta = +INF_EVAL;

    for (int depth = 1; depth <= max_depth; depth++) {
        context.bestMoveInIteration = Move::null();
        context.bestEvalInIteration = is_white(side_to_move) ? -INF_EVAL : +INF_EVAL;

        // if it is not the first iteration, adjust alpha and beta
        if (depth > 1) {
            alpha = context.bestEvalFound - ASPIRATION_MARGIN;
            beta  = context.bestEvalFound + ASPIRATION_MARGIN;
        }

        is_white(side_to_move) ? alpha_beta_search<MAXIMIZE_WHITE>(stop, depth, 0, alpha, beta, context)
                               : alpha_beta_search<MINIMIZE_BLACK>(stop, depth, 0, alpha, beta, context);

        // if the evaluation is out of bounds of the window, redo the search with -INF_EVAL and +INF_EVAL
        if (context.bestEvalInIteration <= alpha || context.bestEvalInIteration >= beta) {
            alpha = -INF_EVAL;
            beta  = +INF_EVAL;

            is_white(side_to_move) ? alpha_beta_search<MAXIMIZE_WHITE>(stop, depth, 0, alpha, beta, context)
                                   : alpha_beta_search<MINIMIZE_BLACK>(stop, depth, 0, alpha, beta, context);
        }

        if (stop) {
            break;
        }

        context.bestMoveFound = context.bestMoveInIteration;
        context.bestEvalFound = context.bestEvalInIteration;

        assert(context.bestMoveFound.is_valid());

        insert_new_result(results, depth, context.bestEvalFound, context.bestMoveFound);

        if (abs(context.bestEvalFound) > MATE_THRESHOLD) {
            break;   // We found a checkmate, we stop because we cant find a shorter checkMate
        }
    }
}

/**
  * @brief alpha_beta_search(Board&, int, int, int, int, SearchContext&)
  * 
  * Alpha beta search.
  * 
  * @tparam searchType [MAXIMIZE_WHITE, MINIMIZE_BLACK]
  * 
  * @param[in] stop  stop search signal.
  * @param[in] depth current depth in the tree
  * @param[in] ply   current ply in the tree
  * @param[in] alpha minumum value that the maximizing player(white) can guarantee
  * @param[in] beta  maximum value that the minimizing player(black) can guarantee
  * @param[in, out] context  board and best moves so far in the search
  * 
  * @return best score possible for black (minimum score), for white (maximum score)
  * 
  */
template<SearchType searchType>
static int alpha_beta_search(std::atomic<bool>& stop, int depth, int ply, int alpha, int beta, SearchContext& context)
{
    constexpr bool MAXIMIZING_WHITE = searchType == MAXIMIZE_WHITE;
    constexpr bool MINIMIZING_BLACK = searchType == MINIMIZE_BLACK;

    Board& board = context.board;
    const uint64_t zobrist_key = board.state().get_zobrist_key();

    if (ply > 0) History::push_position(zobrist_key);

    MoveList moves;
    bool isCheck;
    generate_legal_moves<ALL_MOVES>(moves, board, &isCheck);
    const bool isCheckMate = isCheck && moves.size() == 0;
    const bool isStaleMate = !isCheck && moves.size() == 0;
    const uint8_t fifty_move_rule_counter = board.state().fifty_move_rule_counter();
    const bool fify_move_rule_draw = fifty_move_rule_counter >= 100U;

    if (isCheckMate) {
        // we substract ply so checkMate in less moves has a higher score
        if constexpr (MAXIMIZING_WHITE) {
            return -(MATE_IN_ONE_SCORE - ply);
        }
        else if constexpr (MINIMIZING_BLACK) {
            return MATE_IN_ONE_SCORE - ply;
        }
    }
    else if (isStaleMate) {
        return 0;
    }
    else if (ply > 0 && (fify_move_rule_draw || History::threefold_repetition_detected(fifty_move_rule_counter))) {
        return 0;
    }
    else if (depth == 0) {
        return quiescence_search<searchType>(stop, ply, alpha, beta, context);
    }

    int final_node_evaluation = MAXIMIZING_WHITE ? -INF_EVAL : +INF_EVAL;
    const GameState game_state = board.state();

    order_moves(moves, board, ply);

    for (int i = 0; i < moves.size(); i++) {

        if (stop) {
            return 0;
        }
        constexpr SearchType nextSearchType = MAXIMIZING_WHITE ? MINIMIZE_BLACK : MAXIMIZE_WHITE;

        board.make_move(moves[i]);
        int eval = alpha_beta_search<nextSearchType>(stop, depth - 1, ply + 1, alpha, beta, context);
        board.unmake_move(moves[i], game_state);
        History::pop_position();

        if constexpr (MAXIMIZING_WHITE) {
            if (ply == 0 && eval > context.bestEvalInIteration) {
                // if we are in the root node update the best move
                context.bestEvalInIteration = eval;
                context.bestMoveInIteration = moves[i];
            }

            final_node_evaluation = std::max(final_node_evaluation, eval);
            alpha = std::max(alpha, eval);

            if (final_node_evaluation >= beta) {
                if (!board.move_is_capture(moves[i])) {
                    KillerMoves::store_killer(ply, moves[i]);   // killer move must be quiet and produce a cut off
                }
                break;   // beta cutoff
            }
        }
        else if (MINIMIZING_BLACK) {
            if (ply == 0 && eval < context.bestEvalInIteration) {
                // if we are in the root node update the best move
                context.bestEvalInIteration = eval;
                context.bestMoveInIteration = moves[i];
            }

            final_node_evaluation = std::min(final_node_evaluation, eval);
            beta = std::min(beta, eval);

            if (final_node_evaluation <= alpha) {
                if (!board.move_is_capture(moves[i])) {
                    KillerMoves::store_killer(ply, moves[i]);   // killer move must be quiet and produce a cut off
                }
                break;   // alpha cutoff
            }
        }
    }

    return final_node_evaluation;
}

/**
  * @brief quiescence_search(Board&, int, int, int)
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
  * @param[in] stop  stop search signal
  * @param[in] ply   current ply in the tree
  * @param[in] alpha minimum value that the maximizing player(white) can guarantee
  * @param[in] beta  maximum value that the minimizing player(black) can guarantee
  * @param[in, out] context  board and best moves so far in the search
  * 
  * @return best score possible for black (minimum score possible), for white (maximum score possible)
  * 
  */
template<SearchType searchType>
static int quiescence_search(std::atomic<bool>& stop, int ply, int alpha, int beta, SearchContext& context)
{
    constexpr bool MAXIMIZING_WHITE = searchType == MAXIMIZE_WHITE;
    constexpr bool MINIMIZING_BLACK = searchType == MINIMIZE_BLACK;

    Board& board = context.board;
    const uint64_t zobrist_key = board.state().get_zobrist_key();

    const uint8_t fifty_move_rule_counter = board.state().fifty_move_rule_counter();
    const bool fify_move_rule_draw = fifty_move_rule_counter >= 100U;

    if (History::threefold_repetition_detected(fifty_move_rule_counter) || fify_move_rule_draw) {
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

    order_moves(capture_moves, board, ply);

    const GameState game_state = board.state();
    int final_node_evaluation = static_evaluation;

    for (int i = 0; i < capture_moves.size(); i++) {

        if (stop) {
            return 0;
        }

        constexpr SearchType nextSearchType = MAXIMIZING_WHITE ? MINIMIZE_BLACK : MAXIMIZE_WHITE;

        History::push_position(zobrist_key);
        board.make_move(capture_moves[i]);
        int eval = quiescence_search<nextSearchType>(stop, ply + 1, alpha, beta, context);
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