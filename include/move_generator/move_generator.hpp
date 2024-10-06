#pragma once
/**
 * @file move_generator.hpp
 * @brief move generator services.
 *
 * chess move generator declarations. 
 * 
 */

#include "board.hpp"
#include "move.hpp"

void generate_legal_moves(MoveList &moves, const Board &board);