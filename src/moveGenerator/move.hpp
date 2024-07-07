#pragma once

#include <cstdint>

/*
 *   max number of moves found in a chess position is 218
 *   https://www.chessprogramming.org/Encoding_Moves#MoveIndex
 */
#define MAX_MOVES 256

enum class MoveType { NORMAL = 0, PROMOTION = 1, EN_PASSANT = 2, CASTLING = 3 };

// A move needs 16 bits to be stored
//
// bit  0- 5: destination square (from 0 to 63)
// bit  6-11: origin square (from 0 to 63)
// bit 12-13: promotion piece type
// bit 14-15: special move flag: promotion (1), en passant (2), castling (3)
// En passant bit is set only when a pawn can be captured
// while Move::none() and Move::null() have the same origin and destination square.

class Move {
public:
private:
    std::uint16_t data;
};