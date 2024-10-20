/**
 * @file perft.cpp
 * @brief perft services.
 *
 * perft test types and utilities declaration.
 * 
 * https://www.chessprogramming.org/Perft
 * 
 */

#include "perft.hpp"
#include "board.hpp"
#include "move_generator.hpp"

#include <chrono>

/**
 * @brief perft_recursive
 * 
 * Count all the nodes of the position until the maximum depth.
 * 
 * @param[in,out] board The output stream
 * @param[in] depth maximum depth to reach.
 * 
 * @return counted nodes.
 * 
 */
static uint64_t perft_recursive(Board& board, uint32_t depth);

/**
 * @brief perft
 * 
 * Debugging function to walk the move generation tree 
 * of strictly legal moves to count all the leaf nodes of a certain depth,
 * which can be compared to predetermined values and used to isolate bugs. In perft,
 * nodes are only counted at the end after the last makemove. Thus "higher" terminal nodes
 * (e.g. mate or stalemate) are not counted, instead the number of move paths of a certain depth.
 * 
 * @note Perft ignores draws by repetition, by the fifty-move rule and by insufficient material.
 * By recording the amount of time taken for each iteration, it's possible to compare the 
 * performance of different move generators or the same generator on different machines.
 * 
 * @param[in] FEN position to study.
 * @param[in] depth depth to reach.
 * @param[out] time ms passed to complete the perft test.
 * @param[out] nodes counted nodes.
 */
void perft(const std::string &FEN, uint64_t depth, int64_t& time, uint64_t& nodes)
{
    Board board;
    board.load_fen(FEN);

    auto start = std::chrono::high_resolution_clock::now();

    uint64_t n_moves = perft_recursive(board, depth);

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    time = duration.count();
    nodes = n_moves;
}

/**
 * @brief perft_recursive
 * 
 * Count all the nodes of the position until the maximum depth.
 * 
 * @param[in,out] board The output stream
 * @param[in] depth maximum depth to reach.
 * 
 * @return counted nodes.
 * 
 */
static uint64_t perft_recursive(Board& board, uint32_t depth)
{
    if (depth == 0)
    {
        return 1;
    }

    uint64_t nodes = 0;
    MoveList moves;
    generate_legal_moves(moves,board);

    const GameState game_state = board.state();

    for (int i = 0; i < moves.size(); i++)
    {
        board.make_move(moves[i]);
        nodes += perft_recursive(board, depth - 1);
        board.unmake_move(moves[i], game_state);
    }

    return nodes;
}