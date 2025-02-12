#pragma once

/**
 * @file move_generator_info.hpp
 * @brief move generator info services.
 *
 * move generator info class and declarations. 
 * 
 */

#include "board.hpp"

#include <cassert>

/**
 * @brief MoveGeneratorInfo
 *
 * Contains necessary information about the move generation
 * 
 */
class MoveGeneratorInfo
{
public:
    uint64_t attacked_squares_mask;      // squares that enemy pieces directly attack
    uint64_t pinned_squares_mask;        // squares where pinned pieces could move
    uint64_t king_danger_squares_mask;   // squares that enemy pieces attack if the king is removed
    uint64_t push_squares_mask;          // squares where pieces could move to block a check
    uint64_t capture_squares_mask;       // squares of pieces that could be capture to block a check
    Square checker_square;               // square of the piece giving check
    Square king_white_square;            // square of the white king
    Square king_black_square;            // square of the black king
    Square side_to_move_king_square;     // king square of the side to move
    Square side_waiting_king_square;     // king square of the side to defending
    uint8_t number_of_checkers;          // number of pieces giving check
    ChessColor side_to_move;             // color of the side to move in the game
    ChessColor side_waiting;             // color of the side waiting
    const Board& board;

    /**
     * @brief MoveGeneratorInfo
     * 
     *  Constructor of MoveGeneratorInfo class.
     * 
     *  @note put all fields to default value (push and capture mask = 0xffffffff, others to 0)
     * 
     *  @param[in] board actual position
     * 
     */
    MoveGeneratorInfo(const Board& board) : board(board)
    {
        side_to_move = board.state().side_to_move();
        side_waiting = side_to_move == ChessColor::WHITE ? ChessColor::BLACK : ChessColor::WHITE;
        attacked_squares_mask = 0U;
        pinned_squares_mask = 0U;
        king_danger_squares_mask = 0U;
        push_squares_mask = 0xffffffffU;
        capture_squares_mask = 0xffffffffU;
        checker_square = Square::SQ_INVALID;
        number_of_checkers = 0U;
        king_white_square = board.get_bitboard_piece(Piece::W_KING);
        king_black_square = board.get_bitboard_piece(Piece::B_KING);
        side_to_move_king_square =
            side_to_move == ChessColor::WHITE ? king_white_square : king_black_square;
        side_waiting_king_square =
            side_to_move == ChessColor::WHITE ? king_black_square : king_white_square;
    }
    ~MoveGeneratorInfo() { }

    /**
     * @brief new_checker_found
     * 
     *  increment the number_of_checkers by 1, update checker_square and update the capture_mask
     *      
     *  @param[in] checker_sq square of the checker piece
     * 
     */
    void new_checker_found(Square checker_sq)
    {
        assert(checker_sq.is_valid());

        if (number_of_checkers == 0) {
            // if this is the first checker the only capture is the checker piece
            capture_squares_mask = checker_sq.mask();
        }
        else {
            capture_squares_mask |= checker_sq.mask();
        }

        number_of_checkers++;
        checker_square = checker_sq;
    }
};