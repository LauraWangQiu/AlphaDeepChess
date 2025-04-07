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
#include "precomputed_move_data.hpp"

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
    constexpr inline Piece get_piece(Square square) const
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
    constexpr inline bool is_empty(Square square) const
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
    constexpr inline uint64_t get_bitboard_all() const { return bitboard_all; }

    /**
     * @brief get_bitboard_color 
     * 
     * Get bitboard for each piece color.
     *      
     * @note color must be valid 
     * 
     * @return bitboard_color[color]
     * 
     */
    constexpr inline uint64_t get_bitboard_color(ChessColor color) const
    {
        assert(is_valid_color(color));
        return bitboard_color[static_cast<int>(color)];
    }

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
    constexpr inline uint64_t get_bitboard_piece(Piece piece) const
    {
        assert(is_valid_piece(piece));
        return bitboard_piece[static_cast<int>(piece)];
    }

    /**
     * @brief get thenumber of pieces in the board
     * 
     * @return num_pieces
     */
    constexpr inline int get_num_pieces() const { return game_state.num_pieces(); }

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
    constexpr inline void put_piece(Piece piece, Square square);

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
    constexpr inline void remove_piece(Square square);

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
    constexpr inline void set_side_to_move(ChessColor color)
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
     * @brief returns the bitboard with 1 in the squares that all pieces of this type attacks on the position
     * 
     * @note piece should be valid and not Empty
     * 
     * @param[in] piece piece to get its attack bitboard
     * 
     * @return (uint64_t) attacks_bb[piece]
     * 
     */
    constexpr inline uint64_t get_attacks_bb(Piece piece) const { return game_state.get_attacks_bb(piece); }

    /**
     * @brief returns the bitboard with 1 in the squares that all pieces of this type attacks on the position
     * 
     * @param[in] color color side to get its attack bitboard
     * 
     * @return (uint64_t) attacks_bb[color]
     * 
     */
    constexpr inline uint64_t get_attacks_bb(ChessColor color) const { return game_state.get_attacks_bb(color); }

    /**
     * @brief set the bitboard with 1 in the squares that all pieces of this type attacks on the position
     * 
     * @note piece should be valid and not Empty
     * 
     * @param[in] piece piece to set its attack bitboard
     * @param[in] attacks attacks bitboard
     * 
     */
    constexpr inline void set_attacks_bb(Piece piece, uint64_t attacks) { game_state.set_attacks_bb(piece, attacks); }

    /**
     * @brief set the bitboard with 1 in the squares that all pieces of this type attacks on the position
     * 
     * @param[in] color color side to get its attack bitboard
     * @param[in] attacks attacks bitboard
     * 
     */
    constexpr inline void set_attacks_bb(ChessColor color, uint64_t attacks)
    {
        game_state.set_attacks_bb(color, attacks);
    }

    /**
     * @brief number of pieces of the specified type in the board
     * 
     * @note piece should be valid and not Empty
     * 
     * @param[in] piece selected piece
     * 
     * @return piece_counter[piece]
     * 
     */
    constexpr inline uint8_t get_piece_counter(Piece piece) const { return game_state.get_piece_counter(piece); }

    /**
     * @brief recalculates all the attack bitboards for each piece in the position
     */
    void update_attacks_bb();

    /**
     * @brief state
     * 
     * Returns game state of the position
     * 
     * @return game_state
     * 
     */
    constexpr inline const GameState state() const { return game_state; }

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
     * @brief bitboard_all
     * 
     *  Bitboard containing all pieces on the board.
     */
    uint64_t bitboard_all;

    /**
     * @brief bitboard_piece[ChessColor]
     * 
     *  Array of bitboards for each piece color.
     * 
     * @note Index must be 0 (White) or 1 (Black)
     * 
     */
    uint64_t bitboard_color[2];

    /**
     * @brief bitboard_piece[Piece]
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

    /**
     * @brief recalculates all the number of pieces
     */
    void update_piece_counter();

    /**
     * @brief check that piece counter is correct (for debug)
     */
    bool assert_that_piece_counter_is_correct();
};

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
constexpr inline void Board::put_piece(Piece piece, Square square)
{
    assert(is_valid_piece(piece));
    assert(square.is_valid());

    if (piece == Piece::EMPTY) {
        remove_piece(square);
        return;
    }

    const ChessColor piece_color = get_color(piece);
    const Piece previous_piece = array_piece[square];
    const ChessColor previous_piece_color = get_color(previous_piece);

    const uint64_t mask = square.mask();

    // first remove the previous piece
    bitboard_piece[static_cast<int>(previous_piece)] &= ~mask;
    bitboard_color[static_cast<int>(previous_piece_color)] &= ~mask;

    // place the new piece
    bitboard_piece[static_cast<int>(piece)] |= mask;
    array_piece[square] = piece;
    bitboard_color[static_cast<int>(piece_color)] |= mask;

    bitboard_all |= mask;
}

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
constexpr inline void Board::remove_piece(Square square)
{
    assert(square.is_valid());

    const uint64_t mask = square.mask();

    bitboard_piece[static_cast<int>(get_piece(square))] &= ~mask;
    array_piece[square] = Piece::EMPTY;
    bitboard_all &= ~mask;
    bitboard_color[static_cast<int>(ChessColor::WHITE)] &= ~mask;
    bitboard_color[static_cast<int>(ChessColor::BLACK)] &= ~mask;
}