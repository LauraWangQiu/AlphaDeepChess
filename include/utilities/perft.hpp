#pragma once

/**
 * @file perft.hpp
 * @brief Perft services.
 *
 * perft test types and utilities declaration.
 * 
 * https://www.chessprogramming.org/Perft
 * 
 */

#include <string>
#include <cstdint>
#include <vector>
#include "move_generator.hpp"

typedef std::vector<std::pair<Move, uint64_t>> MoveNodesList;

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
 * @param[out] moveNodeList legal moves and their number of nodes
 * @param[out] time ms passed to complete the perft test.
 * @param[in] use_tt use of transposition table to speed up process
 * 
 */
void perft(const std::string& FEN, uint64_t depth, MoveNodesList& moveNodeList, int64_t& time, bool use_tt = true);