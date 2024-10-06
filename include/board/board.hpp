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

#include "piece.hpp"
#include "square.hpp"
#include "game_state.hpp"

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
    inline Piece get_piece(Square square) const { return array_piece[square]; }

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
    inline bool is_empty(Square square) const { return array_piece[square] == Piece::EMPTY; }

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
     * @brief clean
     * 
     * Remove all pieces on the board.
     * 
     */
    void clean();

    /**
     * @brief load_fen
     * 
     * Set the position represented as fen on the chess board.
     * 
     *  https://www.chess.com/terms/fen-chess
     *  https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
     * 
     * @param[in] fen Chess position in fne string format
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