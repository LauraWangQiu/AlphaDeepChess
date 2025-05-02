#pragma once

/**
 * @file move_generator_info.hpp
 * @brief move generator info services.
 *
 * move generator info class and declarations. 
 * 
 */
#include "board.hpp"
#include "bit_utilities.hpp"
#include <cassert>

/**
 * @brief MoveGeneratorInfo
 *
 * Contains necessary information about the move generation.
 * 
 */
class MoveGeneratorInfo
{
public:
    /**
     * @brief Squares where pinned pieces could move.
     */
    uint64_t pinned_squares_mask;

    /**
     * @brief Squares that enemy pieces attack if the king is removed.
     */
    uint64_t king_danger_squares_mask;

    /**
     * @brief Squares where pieces could move to block a check.
     */
    uint64_t push_squares_mask;

    /**
     * @brief Squares of pieces that could be captured to block a check.
     */
    uint64_t capture_squares_mask;

    /**
     * @brief Square of the white king.
     */
    Square king_white_square;

    /**
     * @brief Square of the black king.
     */
    Square king_black_square;

    /**
     * @brief King square of the side to move.
     */
    Square side_to_move_king_square;

    /**
     * @brief King square of the side waiting (defending).
     */
    Square side_waiting_king_square;

    /**
     * @brief Number of pieces giving check.
     */
    uint8_t number_of_checkers;

    /**
     * @brief Color of the side to move in the game.
     */
    ChessColor side_to_move;

    /**
     * @brief Color of the side waiting.
     */
    ChessColor side_waiting;

    /**
     * @brief Chess position.
     */
    const Board& board;

    /**
     * @brief Move list.
     */
    MoveList& moves;

    /**
     * @brief Bitboard of the pieces of the side to move.
     */
    uint64_t side_to_move_pieces_mask;

    /**
     * @brief Bitboard of the pieces of the side waiting.
     */
    uint64_t side_waiting_pieces_mask;

    /**
     * @brief Row where promotion is available.
     */
    Row row_where_promotion_is_available;

    /**
     * @brief Row where en passant is available.
     */
    Row row_where_en_passant_is_available;

    /**
     * @brief Row where double push is available.
     */
    Row row_where_double_push_is_available;

    /**
     * @brief MoveGeneratorInfo
     * 
     * Constructor of MoveGeneratorInfo class.
     * 
     * Initializes all fields to default values (push and capture masks = 0xffffffff, others to 0).
     * 
     * @param[in] board Actual position.
     * @param[in] moves Array of moves to fill.
     */
    MoveGeneratorInfo(const Board& board, MoveList& moves) : board(board), moves(moves)
    {
        moves.clear();
        side_to_move = board.state().side_to_move();
        side_waiting = opposite_color(side_to_move);
        pinned_squares_mask = 0U;
        king_danger_squares_mask = 0U;
        push_squares_mask = 0xffffffffffffffffU;
        capture_squares_mask = 0xffffffffffffffffU;
        number_of_checkers = 0U;
        king_white_square = lsb(board.get_bitboard_piece(Piece::W_KING));
        king_black_square = lsb(board.get_bitboard_piece(Piece::B_KING));
        side_to_move_king_square = is_white(side_to_move) ? king_white_square : king_black_square;
        side_waiting_king_square = is_white(side_to_move) ? king_black_square : king_white_square;
        side_to_move_pieces_mask = board.get_bitboard_color(side_to_move);
        side_waiting_pieces_mask = board.get_bitboard_color(opposite_color(side_to_move));
        row_where_promotion_is_available = is_white(side_to_move) ? ROW_7 : ROW_2;
        row_where_en_passant_is_available = is_white(side_to_move) ? ROW_5 : ROW_4;
        row_where_double_push_is_available = is_white(side_to_move) ? ROW_2 : ROW_7;
    }

    /**
     * @brief Destructor of MoveGeneratorInfo class.
     */
    ~MoveGeneratorInfo() { }

    /**
     * @brief new_checker_found
     * 
     * Increment the number_of_checkers by 1, update checker_square and update the capture_mask.
     * 
     * @param[in] checker_sq Square of the checker piece.
     * @param[in] new_push_mask Squares where the check could be blocked (pass zero if the checker is not a slider).
     */
    void new_checker_found(Square checker_sq, uint64_t new_push_mask)
    {
        assert(checker_sq.is_valid());
        capture_squares_mask = checker_sq.mask();
        push_squares_mask = new_push_mask;
        number_of_checkers++;
    }
};
