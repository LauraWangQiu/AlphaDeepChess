/**
 * @file move_generator_magic_bitboards.cpp
 * @brief move generator services.
 *
 * chess move generator magic bitboards implementation. 
 * 
 * https://www.chessprogramming.org/Magic_Bitboards
 * 
 */

#include "move_generator.hpp"
#include "precomputed_move_data.hpp"
#include "move_generator_info.hpp"
#include "coordinates.hpp"

/**
  * @brief generate_legal_moves
  * 
  * Calculate all the legal moves in the chess position.
  * 
  * @param[out] moves move list.
  * @param[in] board chess position.
  * @param[out] isMate (optional) return true if the position is check mate.
  * @param[out] isStaleMate (optional) return true if the position is stalemate.
  * 
  */
void generate_legal_moves(MoveList& moves, const Board& board, bool* isMate, bool* isStaleMate) { }
