#pragma once

/**
 * @file board.hpp
 * @brief board services.
 *
 * chess board class and declarations. 
 * 
 */

#include <cstdint>
#include <cstddef>
#include <ostream>
#include <string>
#include <cassert>
#include <cmath>

#include "piece.hpp"
#include "square.hpp"
#include "game_state.hpp"
#include "move.hpp"

/**
 * @brief Board
 *
 * Board containing a chess position.
 * 
 * @note Implemented with the bitboards data structure.
 * 
 */
class Board
{
public:
    /**
     * @brief get_piece 
     * 
     * Get the piece at an specific square.
     * 
     * @note Square should be valid.
     * 
     * @return array_piece[square].
     * 
     */
    inline Piece get_piece(Square square) const
    {
        assert(square.is_valid());
        return array_piece[square];
    }

    /**
     * @brief is_empty 
     * 
     * Calculates if the square in the board is empty.
     * 
     * @note Square should be valid.
     * 
     * @return
     * - TRUE if there is not a piece in the square.
     * - FALSE if there is a piece in the square.
     * 
     */
    inline bool is_empty(Square square) const
    {
        assert(square.is_valid());
        return array_piece[square] == Piece::EMPTY;
    }

    /**
     * @brief bitboard_all_pieces 
     * 
     * Get the bitboard that represents all pieces on the board.
     * 
     * @return bitboard_all
     * 
     */
    inline uint64_t get_bitboard_all() const { return bitboard_all; }

    /**
     * @brief bitboard_white_pieces 
     * 
     * Get the bitboard that represents all white pieces on the board.
     * 
     * @return bitboard_white
     * 
     */
    inline uint64_t get_bitboard_white() const { return bitboard_white; }

    /**
     * @brief bitboard_black_pieces 
     * 
     * Get the bitboard that represents all black pieces on the board.
     * 
     * @return bitboard_black
     * 
     */
    inline uint64_t get_bitboard_black() const { return bitboard_black; }

    /**
     * @brief get_bitboard_piece 
     * 
     * Get the bitboard that represents all pieces of an specific type.
     * 
     * @note piece must be valid.
     * 
     * @param[in] piece Selected piece to get its bitboard.
     * 
     * @pre piece < Piece::NUM_PIECES
     * 
     * @return bitboard_piece[piece]
     * 
     */
    inline uint64_t get_bitboard_piece(Piece piece) const
    {
        assert(is_valid_piece(piece));
        return bitboard_piece[static_cast<int>(piece)];
    }

    /**
     * @brief put_piece
     * 
     * Add piece to the board
     * 
     * @note piece and square must be valid.
     * 
     * @param[in] piece
     * @param[in] square
     * 
     */
    void put_piece(Piece piece, Square square);

    /**
     * @brief remove_piece
     * 
     * Remove piece from the board
     * 
     * @note square must be valid.
     * 
     * @param[in] square
     * 
     */
    void remove_piece(Square square);

    /**
     * @brief set_side_to_move
     * 
     * set the side to move in the chess position
     * 
     * @note color must be valid.
     * 
     * @param[in] color side to move color
     * 
     */
    inline void set_side_to_move(ChessColor color)
    {
        assert(is_valid_color(color));
        game_state.set_side_to_move(color);
    }

    /**
     * @brief clean
     * 
     * Remove all pieces on the board.
     * 
     */
    void clean();

    /**
     * @brief make_move
     * 
     * Make the move in the board.
     * 
     * @note If the move is not valid in the position the game will be corrupted.
     * 
     * @param[in] move chess move.
     * 
     */
    void make_move(Move move);

    /**
     * @brief unmake_move
     * 
     * unmake the move in the board, restoring the previous game state.
     * 
     * @note If the move was not valid in the position the game will be corrupted.
     * 
     * @param[in] move chess move.
     * @param[in] previous_state previous game state.
     * 
     */
    void unmake_move(Move move, GameState previous_state);

    /**
     * @brief move_is_capture
     * 
     * calculate if a move is a capture in the board
     * 
     * 
     * @param[in] move chess move.
     * 
     * @return True if move is En Passant or end square is not empty
     * 
     */
    constexpr bool move_is_capture(Move move) const
    {
        assert(move.is_valid());
        return move.type() == MoveType::EN_PASSANT || (!is_empty(move.square_to()));
    }

    /**
     * @brief load_fen
     * 
     * Set the position represented as fen on the chess board.
     * 
     *  https://www.chess.com/terms/fen-chess
     *  https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
     * 
     * @param[in] fen Chess position in fen string format
     * 
     */
    void load_fen(const std::string& fen);

    /**
     * @brief fen
     * 
     * Returns fen representation of the position
     * 
     *  https://www.chess.com/terms/fen-chess
     *  https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
     * 
     * @return std::string fen
     * 
     */
    std::string fen() const;

    /**
     * @brief state
     * 
     * Returns game state of the position
     * 
     * @return game_state
     * 
     */
    inline const GameState state() const { return game_state; }

    /**
     * @brief operator<<
     * 
     * Overloads the << operator to print the board
     * 
     * @param[out] os The output stream
     * @param[in] board The board to print
     * 
     */
    friend std::ostream& operator<<(std::ostream& os, const Board& board);

    /**
     * @brief Board
     * 
     *  Constructor of Board class.
     */
    Board();

    /**
     * @brief ~Board
     * 
     *  Destructor of Board class.
     */
    ~Board();

private:
    /**
     * @brief bitboard_all
     * 
     *  Bitboard containing all pieces on the board.
     */
    uint64_t bitboard_all;

    /**
     * @brief bitboard_white
     * 
     *  Bitboard containing all white pieces.
     */
    uint64_t bitboard_white;

    /**
     * @brief bitboard_black
     * 
     *  Bitboard containing all black pieces.
     */
    uint64_t bitboard_black;

    /**
     * @brief bitboard_piece
     * 
     *  Array of bitboards for each piece type.
     * 
     * @note Array size of limit NUM_CHESS_PIECES = 13.
     * 
     */
    uint64_t bitboard_piece[NUM_CHESS_PIECES];

    /**
     * @brief array_piece
     * 
     *  Array of pieces in each 64 squares on the board.
     * 
     * @note Array size of limit NUM_SQUARES = 64.
     * 
     */
    Piece array_piece[NUM_SQUARES];

    /**
     * @brief game_state
     * 
     *  Contains all necessary information about the state of the game.
     * 
     * - side_to_move.
     * - castle_king_white.
     * - castle_queen_white.
     * - castle_king_black.
     * - castle_queen_black.
     * - en_passant_square.
     * - move_number.
     * - half_move.
     */
    GameState game_state;


    void make_normal_move(Move normal_move);
    void unmake_normal_move(Move normal_move);
    void make_promotion_move(Move promotion_move);
    void unmake_promotion_move(Move promotion_move);
    void make_castling_move(Move castling_move);
    void unmake_castling_move(Move castling_move);
    void make_enPassant_move(Move enPassant_move);
    void unmake_enPassant_move(Move enPassant_move);

    /**
     * @brief check_and_modify_castle_rights
     * 
     *  Check if the castle rights are correct and modify them if there are errors.
     * 
     */
    void check_and_modify_castle_rights();

    /**
     * @brief check_and_modify_en_passant_rule
     * 
     *  Check if en passant is really possible and modify them if there are errors.
     * 
     */
    void check_and_modify_en_passant_rule();
};
