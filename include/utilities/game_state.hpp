#pragma once

#include "piece.hpp"
#include "square.hpp"
#include <cassert>

/**
 * @file game_state.hpp
 * @brief Game state of a chess game information utilities.
 *
 * 
 */

static constexpr uint64_t SHIFT_NUM_PIECES = 43ULL;
static constexpr uint64_t SHIFT_FIFTY_MOVE_RULE = 35ULL;
static constexpr uint64_t SHIFT_LAST_CAPTURED_PIECE = 32ULL;
static constexpr uint64_t SHIFT_SIDE_TO_MOVE = 31ULL;
static constexpr uint64_t SHIFT_CASTLE_KING_WHITE = 30ULL;
static constexpr uint64_t SHIFT_CASTLE_QUEEN_WHITE = 29ULL;
static constexpr uint64_t SHIFT_CASTLE_KING_BLACK = 28ULL;
static constexpr uint64_t SHIFT_CASTLE_QUEEN_BLACK = 27ULL;
static constexpr uint64_t SHIFT_EN_PASSANT_SQUARE = 20ULL;
static constexpr uint64_t SHIFT_MOVE_NUMBER = 0ULL;

static constexpr uint64_t MASK_NUM_PIECES = (0x7fULL << SHIFT_NUM_PIECES);
static constexpr uint64_t MASK_FIFTY_MOVE_RULE = (0xffULL << SHIFT_FIFTY_MOVE_RULE);
static constexpr uint64_t MASK_LAST_CAPTURED_PIECE = (7ULL << SHIFT_LAST_CAPTURED_PIECE);
static constexpr uint64_t MASK_SIDE_TO_MOVE = (1ULL << SHIFT_SIDE_TO_MOVE);
static constexpr uint64_t MASK_CASTLE_KING_WHITE = (1ULL << SHIFT_CASTLE_KING_WHITE);
static constexpr uint64_t MASK_CASTLE_QUEEN_WHITE = (1ULL << SHIFT_CASTLE_QUEEN_WHITE);
static constexpr uint64_t MASK_CASTLE_KING_BLACK = (1ULL << SHIFT_CASTLE_KING_BLACK);
static constexpr uint64_t MASK_CASTLE_QUEEN_BLACK = (1ULL << SHIFT_CASTLE_QUEEN_BLACK);
static constexpr uint64_t MASK_EN_PASSANT_SQUARE = (0x7fULL << SHIFT_EN_PASSANT_SQUARE);
static constexpr uint64_t MASK_MOVE_NUMBER = (0xfffffULL << SHIFT_MOVE_NUMBER);


/**
 * @brief GameState
 * 
 * Represents the sate of the chess game.
 * 
 * @note game state is stored as a 64-bit number :
 * 43-48 : num_pieces : 0 to 64 pieces
 * 35-42 : fifty_move_rule_counter : if counter gets to 100 then game is a draw.
 * 32-34 : last_captured_piece : PieceType::Empty if last move was not a capture.
 * 31 : side_to_move : 0 if white, 1 if black.
 * 30 : castle_king_white : 1 if available, 0 if not.
 * 29 : castle_queen_white : 1 if available, 0 if not.
 * 28 : castle_king_black : 1 if available, 0 if not.
 * 27 : castle_queen_black : 1 if available, 0 if not.
 * 26-20 : en_passant_square : 0-63 if available, >=64 if not available
 * 19-0 : move_number : 0-1048575 number of moves in the game.
 * 
 * 63-0 zobrist_key : zobrist hash key of the position
 * 
 */
class GameState
{
public:
    /**
     * @brief has_castled_white
     * 
     * get if white has castled.
     * 
     * @note None.
     * 
     * @return
     * - TRUE if white has castled.
     * - FALSE if white has not castled.
     */
    constexpr inline bool has_castled_white() const;

    /**
     * @brief has_castled_black
     * 
     * get if black has castled.
     * @note None.
     * 
     * @return
     * - TRUE if black has castled.
     * - FALSE if black has not castled.
     */
    constexpr inline bool has_castled_black() const;

    /**
     * @brief fifty_move_rule_counter
     * 
     * Counter for the fifty move rule, if 100 moves passed without 
     * a pawn move or a capture then game is a draw.
     * 
     * @note if this counter gets to 100 the game is a draw.
     * 
     * @return 50 move rule counter.
     * 
     */
    constexpr inline uint8_t fifty_move_rule_counter() const;

    /**
     * @brief last_captured_piece
     * 
     * get the last captured piece or PieceType::Empty
     * if last move was not a capture.
     * 
     * @return
     * - PieceType!=Empty if last move was a capture.
     * - PieceType::Empty if last move was not a capture
     */
    constexpr inline PieceType last_captured_piece() const;

    /**
     * @brief side_to_move
     * 
     * get the side to move.
     * 
     * @return
     * - ChessColor::WHITE if is white to move.
     * - ChessColor::Black if is black to move.
     */
    constexpr inline ChessColor side_to_move() const;

    /**
     * @brief castle_king_white
     * 
     * get the avaliability of castle king side for white.
     * 
     * @return
     * - TRUE if castle is available.
     * - FALSE if castle is not available.
     */
    constexpr inline bool castle_king_white() const;

    /**
     * @brief castle_queen_white
     * 
     * get the avaliability of castle queen side for white.
     * 
     * @return
     * - TRUE if castle is available.
     * - FALSE if castle is not available.
     */
    constexpr inline bool castle_queen_white() const;

    /**
     * @brief castle_king_black
     * 
     * get the avaliability of castle king side for black.
     * 
     * @return
     * - TRUE if castle is available.
     * - FALSE if castle is not available.
     */
    constexpr inline bool castle_king_black() const;

    /**
     * @brief castle_queen_black
     * 
     * get the avaliability of castle queen side for black.
     * 
     * @return
     * - TRUE if castle is available.
     * - FALSE if castle is not available.
     */
    constexpr inline bool castle_queen_black() const;


    /**
     * @brief en_passsant_square
     * 
     * get square where en passant is available.
     * 
     * @return
     * - 0<=square<=63 if en passant is available.
     * - INVALID if en en passant is no available.
     */
    constexpr inline Square en_passant_square() const;

    /**
     * @brief move_number
     * 
     * get the number of moves of the chess game.
     * 
     * @return 0 <= move_number <= 1048575.
     * 
     */
    constexpr inline uint64_t move_number() const;

    /**
     * @brief get_zobrist_key
     * 
     * get the zobrist_key.
     * 
     * @return zobrist_key.
     * 
     */
    constexpr inline uint64_t get_zobrist_key() const { return zobrist_key; };

    /**
     * @brief get number of pieces
     *       
     * @return (int) num_pieces.
     * 
     */
    constexpr inline int num_pieces() const;

    /**
     * @brief set_fifty_move_rule_counter
     * 
     * set the moves that have passed since the last pawn move or capture.
     * 
     * @note counter must be <=100, otherwise state will be corrupted.
     * 
     * @param[in] counter fifty rule counter counter.
     * 
     */
    constexpr inline void set_fifty_move_rule_counter(uint8_t counter);

    /**
     * @brief set_last_captured_piece
     * 
     * set the last captured piece or PieceType::Empty
     * if last move was not a capture.
     * 
     * @param[in] piece_type last piece captured.
     * 
     */
    constexpr inline void set_last_captured_piece(PieceType piece_type);
    /**
     * @brief set_side_to_move
     * 
     * set turn to move in the game.
     * 
     * @param[in] side side to move.
     * 
     */
    constexpr inline void set_side_to_move(ChessColor side);

    /**
     * @brief set_castle_king_white
     * 
     * set avaliability of castle king side for white.
     * 
     * @param[in] available avaliability of castle.
     * 
     */
    constexpr inline void set_castle_king_white(bool available);

    /**
     * @brief set_castle_queen_white
     * 
     * set avaliability of castle queen side for white.
     * 
     * @param[in] available avaliability of castle.
     * 
     */
    constexpr inline void set_castle_queen_white(bool available);

    /**
     * @brief set_castle_king_black
     * 
     * set avaliability of castle king side for black.
     * 
     * @param[in] available avaliability of castle.
     * 
     */
    constexpr inline void set_castle_king_black(bool available);

    /**
     * @brief set_castle_queen_black
     * 
     * set avaliability of castle queen side for black.
     * 
     * @param[in] available avaliability of castle.
     * 
     */
    constexpr inline void set_castle_queen_black(bool available);

    /**
     * @brief set_en_passant_square
     * 
     * set the square where en passant is available.
     * 
     * @note INVALID means that en passant is not available.
     * 
     * @param[in] square nen passant square.
     * 
     */
    constexpr inline void set_en_passant_square(Square square);

    /**
     * @brief set_move_number
     * 
     * set the move number.
     * 
     * @note move number must be between 0-1048575,
     *  otherwise state will be corrupted.
     * 
     * @param[in] move_number number of moves in the game
     * 
     */
    constexpr inline void set_move_number(uint64_t move_number);

    /**
     * @brief set the number of pieces in the board
     * 
     * 
     * @note number must be between 0-64, otherwise state will be corrupted.
     * 
     * @param[in] num_pieces number of pieces in the board
     * 
     */
    constexpr inline void set_num_pieces(int num_pieces);

    /**
     * @brief set_zobrist_key
     * 
     * set the zobrist_key.
     * 
     * @param[in] key zobrist key
     * 
     */
    constexpr inline void set_zobrist_key(uint64_t key) { zobrist_key = key; };

    /**
     * @brief xor_zobrist
     * 
     * modify the zobrist_key ( zobrist_key ^= seed ).
     * 
     * @param[in] seed seed hash key modifier
     * 
     */
    constexpr inline void xor_zobrist(uint64_t seed) { zobrist_key ^= seed; };

    /**
     * @brief clean
     * 
     *  Cleans the game state to 0 (initial value)
     *  set_move_number(1ULL);
        set_side_to_move(ChessColor::WHITE);
     *  set_en_passant_square(Square::INVALID);
     *  set_last_captured_piece(PieceType::EMPTY);
     */
    constexpr inline void clean()
    {
        state_register = 0ULL;
        zobrist_key = 0ULL;
        set_move_number(1ULL);
        set_side_to_move(ChessColor::WHITE);
        set_en_passant_square(Square::INVALID);
        set_last_captured_piece(PieceType::EMPTY);
    }

    /**
     * @brief GameState
     * 
     * GameState class constructor.
     * 
     * @note clean state_register.
     *
     * set_move_number(1ULL);
     * set_side_to_move(ChessColor::WHITE);
     * state_register = 0ULL;
     * set_en_passant_square(Square::INVALID);
     * set_last_captured_piece(PieceType::EMPTY);
     * 
     */
    constexpr GameState() : state_register(0ULL), zobrist_key(0ULL) { clean(); }

    /**
     * @brief GameState
     * 
     * GameState class copy constuctor.
     * 
     * @param[in] gs gameState where to copy the initial value.
     * 
     */
    constexpr GameState(const GameState& gs) : state_register(gs.state_register), zobrist_key(gs.zobrist_key) { }

    /**
     * @brief operator==
     * 
     * Equal operator overload.
     * 
     * @param[in] gs gameState to compare with.
     * 
     * @return
     * - TRUE if state_register == gs.state_register && zobrist_key == gs.zobrist_key.
     * - FALSE else.
     * 
     */
    constexpr bool operator==(const GameState& gs) const
    {
        return state_register == gs.state_register && zobrist_key == gs.zobrist_key;
    }

    /**
     * @brief operator!=
     * 
     * Not-equal operator overload.
     * 
     * @param[in] gs gameState to compare with.
     * 
     * @return
     * - TRUE if state_register != gs.state_register.
     * - FALSE state_register == gs.state_register.
     * 
     */
    constexpr bool operator!=(const GameState& gs) const
    {
        return state_register != gs.state_register || zobrist_key != gs.zobrist_key;
    }

    /**
     * @brief operator=
     * 
     * Assignment operator overload.
     * 
     * @param[in] other gameState where to copy the value.
     * 
     * @return *this
     * 
     */
    constexpr GameState& operator=(const GameState& other)
    {
        if (this != &other)   // not a self-assignment
        {
            this->state_register = other.state_register;
            this->zobrist_key = other.zobrist_key;
        }
        return *this;
    }

private:
    /**
     * @brief state_register
     * 
     * Contains all the neccesary information of a chess game state.
     * 
     */
    uint64_t state_register;

    /**
     * @brief zobrist_key
     * 
     * Zobrist hash key
     * 
     */
    uint64_t zobrist_key;
};

/**
 * @brief fifty_move_rule_counter
 * 
 * Counter for the fifty move rule, if 50 moves passed without 
 * a pawn move or a capture then game is a draw.
 * 
 * @note if this counter gets to 50 the game is a draw.
 * 
 * @return 50 move rule counter.
 * 
 */
constexpr inline uint8_t GameState::fifty_move_rule_counter() const
{
    return (state_register & MASK_FIFTY_MOVE_RULE) >> SHIFT_FIFTY_MOVE_RULE;
}

/**
 * @brief last_captured_piece
 * 
 * get the last captured piece or PieceType::Empty
 * if last move was not a capture.
 * 
 * @return
 * - PieceType!=Empty if last move was a capture.
 * - PieceType::Empty if last move was not a capture
 */
constexpr inline PieceType GameState::last_captured_piece() const
{
    const uint64_t piece = (state_register & MASK_LAST_CAPTURED_PIECE) >> SHIFT_LAST_CAPTURED_PIECE;
    return static_cast<PieceType>(piece);
}

/**
 * @brief side_to_move
 * 
 * get the side to move.
 * 
 * @return
 * - ChessColor::WHITE if is white to move.
 * - ChessColor::Black if is black to move.
 */
constexpr inline ChessColor GameState::side_to_move() const
{
    const uint64_t side = (state_register & MASK_SIDE_TO_MOVE) >> SHIFT_SIDE_TO_MOVE;
    return static_cast<ChessColor>(side);
}

/**
 * @brief castle_king_white
 * 
 * get the avaliability of castle king side for white.
 * 
 * @return
 * - TRUE if castle is available.
 * - FALSE if castle is not available.
 */
constexpr inline bool GameState::castle_king_white() const { return state_register & MASK_CASTLE_KING_WHITE; }

/**
 * @brief castle_queen_white
 * 
 * get the avaliability of castle queen side for white.
 * 
 * @return
 * - TRUE if castle is available.
 * - FALSE if castle is not available.
 */
constexpr inline bool GameState::castle_queen_white() const { return state_register & MASK_CASTLE_QUEEN_WHITE; }

/**
 * @brief castle_king_black
 * 
 * get the avaliability of castle king side for black.
 * 
 * @return
 * - TRUE if castle is available.
 * - FALSE if castle is not available.
 */
constexpr inline bool GameState::castle_king_black() const { return state_register & MASK_CASTLE_KING_BLACK; }

/**
 * @brief castle_queen_black
 * 
 * get the avaliability of castle queen side for black.
 * 
 * @return
 * - TRUE if castle is available.
 * - FALSE if castle is not available.
 */
constexpr inline bool GameState::castle_queen_black() const { return state_register & MASK_CASTLE_QUEEN_BLACK; }

/**
 * @brief en_passsant_square
 * 
 * get square where en passant is available.
 * 
 * @return
 * - 0<=square<=63 if en passant is available.
 * - INVALID if en en passant is no available.
 */
constexpr inline Square GameState::en_passant_square() const
{
    return (state_register & MASK_EN_PASSANT_SQUARE) >> SHIFT_EN_PASSANT_SQUARE;
}

/**
 * @brief move_number
 * 
 * get the number of moves of the chess game.
 * 
 * @return 0 <= move_number <= 1048575.
 * 
 */
constexpr inline uint64_t GameState::move_number() const
{
    return (state_register & MASK_MOVE_NUMBER) >> SHIFT_MOVE_NUMBER;
}

/**
 * @brief get number of pieces
 *       
 * @return (int) 0 <= num_pieces <= 64.
 * 
 */
constexpr inline int GameState::num_pieces() const { return (state_register & MASK_NUM_PIECES) >> SHIFT_NUM_PIECES; }

/**
 * @brief set_fifty_move_rule_counter
 * 
 * set the moves that have passed since the last pawn move or capture.
 * 
 * @note counter must be <=50, otherwise state will be corrupted.
 * 
 * @param[in] counter fifty rule counter counter.
 * 
 */
constexpr inline void GameState::set_fifty_move_rule_counter(uint8_t counter)
{
    assert(counter <= 100);
    state_register &= ~MASK_FIFTY_MOVE_RULE;
    state_register |= static_cast<uint64_t>(counter) << SHIFT_FIFTY_MOVE_RULE;
}

/**
 * @brief set_last_captured_piece
 * 
 * set the last captured piece or PieceType::Empty
 * if last move was not a capture.
 * 
 * @param[in] piece_type last piece captured.
 * 
 */
constexpr inline void GameState::set_last_captured_piece(PieceType piece_type)
{
    assert(is_valid_pieceType(piece_type));

    state_register &= ~MASK_LAST_CAPTURED_PIECE;
    state_register |= static_cast<uint64_t>(piece_type) << SHIFT_LAST_CAPTURED_PIECE;
}
/**
 * @brief set_side_to_move
 * 
 * set turn to move in the game.
 * 
 * @param[in] side side to move.
 * 
 */
constexpr inline void GameState::set_side_to_move(ChessColor side)
{
    assert(is_valid_color(side));

    state_register &= ~MASK_SIDE_TO_MOVE;
    state_register |= static_cast<uint64_t>(side) << SHIFT_SIDE_TO_MOVE;
}

/**
 * @brief set_castle_king_white
 * 
 * set avaliability of castle king side for white.
 * 
 * @param[in] available avaliability of castle.
 * 
 */
constexpr inline void GameState::set_castle_king_white(bool available)
{
    state_register &= ~MASK_CASTLE_KING_WHITE;
    state_register |= static_cast<uint64_t>(available) << SHIFT_CASTLE_KING_WHITE;
}

/**
 * @brief set_castle_queen_white
 * 
 * set avaliability of castle queen side for white.
 * 
 * @param[in] available avaliability of castle.
 * 
 */
constexpr inline void GameState::set_castle_queen_white(bool available)
{
    state_register &= ~MASK_CASTLE_QUEEN_WHITE;
    state_register |= static_cast<uint64_t>(available) << SHIFT_CASTLE_QUEEN_WHITE;
}

/**
 * @brief set_castle_king_black
 * 
 * set avaliability of castle king side for black.
 * 
 * @param[in] available avaliability of castle.
 * 
 */
constexpr inline void GameState::set_castle_king_black(bool available)
{
    state_register &= ~MASK_CASTLE_KING_BLACK;
    state_register |= static_cast<uint64_t>(available) << SHIFT_CASTLE_KING_BLACK;
}

/**
 * @brief set_castle_queen_black
 * 
 * set avaliability of castle queen side for black.
 * 
 * @param[in] available avaliability of castle.
 * 
 */
constexpr inline void GameState::set_castle_queen_black(bool available)
{
    state_register &= ~MASK_CASTLE_QUEEN_BLACK;
    state_register |= static_cast<uint64_t>(available) << SHIFT_CASTLE_QUEEN_BLACK;
}


/**
 * @brief set_en_passant_square
 * 
 * set the square where en passant is available.
 * 
 * @note INVALID means that en passant is not available.
 * 
 * @param[in] square nen passant square.
 * 
 */
constexpr inline void GameState::set_en_passant_square(Square square)
{
    state_register &= ~MASK_EN_PASSANT_SQUARE;
    state_register |= square << SHIFT_EN_PASSANT_SQUARE;
}

/**
 * @brief set_move_number
 * 
 * set the move number.
 * 
 * @note move number must be between 0-1048575,
 *  otherwise state will be corrupted.
 * 
 * @param[in] move_number number of moves in the game
 * 
 */
constexpr inline void GameState::set_move_number(uint64_t move_number)
{
    assert(move_number < 1048575);

    state_register &= ~MASK_MOVE_NUMBER;
    state_register |= move_number << SHIFT_MOVE_NUMBER;
}

/**
 * @brief set the number of pieces in the board
 * 
 * 
 * @note number must be between 0-64, otherwise state will be corrupted.
 * 
 * @param[in] num_pieces number of pieces in the board
 * 
 */
constexpr inline void GameState::set_num_pieces(int num_pieces)
{
    state_register &= ~MASK_NUM_PIECES;
    state_register |= static_cast<uint64_t>(num_pieces) << SHIFT_NUM_PIECES;
}