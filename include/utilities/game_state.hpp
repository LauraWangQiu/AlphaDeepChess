#pragma once

#include "piece.hpp"
#include "square.hpp"

/**
 * @file game_state.hpp
 * @brief Game state of a chess game information utilities.
 *
 * 
 */

static constexpr uint64_t SHIFT_TRIPLE_REPETITION_RULE = 41UL;
static constexpr uint64_t SHIFT_FIFTY_MOVE_RULE = 35UL;
static constexpr uint64_t SHIFT_LAST_CAPTURED_PIECE = 32UL;
static constexpr uint64_t SHIFT_SIDE_TO_MOVE = 31UL;
static constexpr uint64_t SHIFT_CASTLE_KING_WHITE = 30UL;
static constexpr uint64_t SHIFT_CASTLE_QUEEN_WHITE = 29UL;
static constexpr uint64_t SHIFT_CASTLE_KING_BLACK = 28UL;
static constexpr uint64_t SHIFT_CASTLE_QUEEN_BLACK = 27UL;
static constexpr uint64_t SHIFT_EN_PASSANT_SQUARE = 20UL;
static constexpr uint64_t SHIFT_MOVE_NUMBER = 1UL;
static constexpr uint64_t SHIFT_HALF_MOVE = 0UL;

static constexpr uint64_t MASK_TRIPLE_REPETITION_RULE = (1UL << SHIFT_TRIPLE_REPETITION_RULE);
static constexpr uint64_t MASK_FIFTY_MOVE_RULE = (1UL << SHIFT_FIFTY_MOVE_RULE);
static constexpr uint64_t MASK_LAST_CAPTURED_PIECE = (1UL << SHIFT_LAST_CAPTURED_PIECE);
static constexpr uint64_t MASK_SIDE_TO_MOVE = (1UL << SHIFT_SIDE_TO_MOVE);
static constexpr uint64_t MASK_CASTLE_KING_WHITE = (1UL << SHIFT_CASTLE_KING_WHITE);
static constexpr uint64_t MASK_CASTLE_QUEEN_WHITE = (1UL << SHIFT_CASTLE_QUEEN_WHITE);
static constexpr uint64_t MASK_CASTLE_KING_BLACK = (1UL << SHIFT_CASTLE_KING_BLACK);
static constexpr uint64_t MASK_CASTLE_QUEEN_BLACK = (1UL << SHIFT_CASTLE_QUEEN_BLACK);
static constexpr uint64_t MASK_EN_PASSANT_SQUARE = (0x7fUL << SHIFT_EN_PASSANT_SQUARE);
static constexpr uint64_t MASK_MOVE_NUMBER = (0x7ffffUL << SHIFT_MOVE_NUMBER);
static constexpr uint64_t MASK_HALF_MOVE = (1UL << SHIFT_HALF_MOVE);


/**
 * @brief GameState
 * 
 * Represents the sate of the chess game.
 * 
 * @note game state is stored as a 64-bit number :
 * 41-42 : triple_repetition_counter : if counter gets to 3 then game is a draw.
 * 35-40 : fifty_move_rule_counter : if counter gets to 50 then game is a draw.
 * 32-34 : last_captured_piece : PieceType::Empty if last move was not a capture.
 * 31 : side_to_move : 0 if white, 1 if black.
 * 30 : castle_king_white : 1 if avaliable, 0 if not.
 * 29 : castle_queen_white : 1 if avaliable, 0 if not.
 * 28 : castle_king_black : 1 if avaliable, 0 if not.
 * 27 : castle_queen_black : 1 if avaliable, 0 if not.
 * 26-20 : en_passant_square : 0-63 if avaliable, >=64 if not avaliable
 * 19-1 : move_number : 0-524287 number of moves in the game.
 * 0 : half_move : 0 if half_move = move_number * 2, 1 if half_move = move_number * 2 + 1.
 * 
 */
class GameState
{

public:
    /**
     * @brief triple_repetition_counter
     * 
     * Counter of the times the position has been repeated,
     * if this counter gets to 3 then game is a draw.
     * 
     * @note if this counter gets to 3 the game is a draw.
     * 
     * @return triple repetition counter.
     */
    constexpr inline uint8_t triple_repetition_counter() const;

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
     * - TRUE if castle is avaliable.
     * - FALSE if castle is not avaliable.
     */
    constexpr inline bool castle_king_white() const;

    /**
     * @brief castle_queen_white
     * 
     * get the avaliability of castle queen side for white.
     * 
     * @return
     * - TRUE if castle is avaliable.
     * - FALSE if castle is not avaliable.
     */
    constexpr inline bool castle_queen_white() const;

    /**
     * @brief castle_king_black
     * 
     * get the avaliability of castle king side for black.
     * 
     * @return
     * - TRUE if castle is avaliable.
     * - FALSE if castle is not avaliable.
     */
    constexpr inline bool castle_king_black() const;

    /**
     * @brief castle_queen_black
     * 
     * get the avaliability of castle queen side for black.
     * 
     * @return
     * - TRUE if castle is avaliable.
     * - FALSE if castle is not avaliable.
     */
    constexpr inline bool castle_queen_black() const;


    /**
     * @brief en_passsant_square
     * 
     * get square where en passant is avaliable.
     * 
     * @return
     * - 0<=square<=63 if en passant is avaliable.
     * - SQ_INVALID if en en passant is no avaliable.
     */
    constexpr inline Square en_passant_square() const;

    /**
     * @brief move_number
     * 
     * get the number of moves of the chess game.
     * 
     * @return 0 <= move_number <= 524287.
     * 
     */
    constexpr inline uint64_t move_number() const;

    /**
     * @brief half_move
     * 
     * get the number of half moves of the chess game.
     * 
     * @return
     * - (move_number * 2) if half_bit = 0(black was the last to make a move).
     * - (move_number * 2 + 1) if half_bit = 1(white was the last to make a move).
     * 
     */
    constexpr inline uint64_t half_move() const;

    /**
     * @brief set_triple_repetition_counter
     * 
     * set number of times the position has been repeated.
     * 
     * @note counter must be <=3, otherwise state will be corrupted.
     * 
     * @param[in] counter triple repetition counter.
     * 
     */
    constexpr inline void set_triple_repetition_counter(uint8_t counter);

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
     * @param[in] avaliable avaliability of castle.
     * 
     */
    constexpr inline void set_castle_king_white(bool avaliable);

    /**
     * @brief set_castle_queen_white
     * 
     * set avaliability of castle queen side for white.
     * 
     * @param[in] avaliable avaliability of castle.
     * 
     */
    constexpr inline void set_castle_queen_white(bool avaliable);

    /**
     * @brief set_castle_king_black
     * 
     * set avaliability of castle king side for black.
     * 
     * @param[in] avaliable avaliability of castle.
     * 
     */
    constexpr inline void set_castle_king_black(bool avaliable);

    /**
     * @brief set_castle_queen_black
     * 
     * set avaliability of castle queen side for black.
     * 
     * @param[in] avaliable avaliability of castle.
     * 
     */
    constexpr inline void set_castle_queen_black(bool avaliable);

    /**
     * @brief set_en_passant_square
     * 
     * set the square where en passant is avaliable.
     * 
     * @note SQ_INVALID means that en passant is not avaliable.
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
     * @note move number must be between 0-524287,
     *  otherwise state will be corrupted.
     * 
     * @param[in] move_number number of moves in the game
     * 
     */
    constexpr inline void set_move_number(uint64_t move_number);

    /**
     * @brief set_half_move
     * 
     * set the half move bit.
     * 
     * @note Usually when white moves put half_move_bit = 1, when black moves put to 0.
     * 
     * @param[in] half_move_bit
     *  - if 1 then half_move counter = move_number * 2 + 1 
     *  - if 0 then half_move counter = move_number * 2
     * 
     */
    constexpr inline void set_half_move(bool half_move_bit);

    /**
     * @brief GameState
     * 
     * GameState class constructor.
     * 
     * @note initializes state_register = 0.
     * 
     */
    constexpr GameState() : state_register(0U) { set_en_passant_square(Square::SQ_INVALID); }

    /**
     * @brief GameState
     * 
     * GameState class copy constuctor.
     * 
     * @param[in] gs gameState where to copy the initial value.
     * 
     */
    constexpr GameState(const GameState& gs) : state_register(gs.state_register) { }

    /**
     * @brief operator==
     * 
     * Equal operator overload.
     * 
     * @param[in] gs gameState to compare with.
     * 
     * @return
     * - TRUE if state_register == gs.state_register.
     * - FALSE state_register != gs.state_register.
     * 
     */
    constexpr bool operator==(const GameState& gs) const
    {
        return state_register == gs.state_register;
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
        return state_register != gs.state_register;
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
};

/**
 * @brief triple_repetition_counter
 * 
 * Counter of the times the position has been repeated,
 * if this counter gets to 3 then game is a draw.
 * 
 * @note if this counter gets to 3 the game is a draw.
 * 
 * @return triple repetition counter.
 */
constexpr inline uint8_t GameState::triple_repetition_counter() const
{
    return (state_register & MASK_TRIPLE_REPETITION_RULE) >> SHIFT_TRIPLE_REPETITION_RULE;
}

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
 * - TRUE if castle is avaliable.
 * - FALSE if castle is not avaliable.
 */
constexpr inline bool GameState::castle_king_white() const
{
    return state_register & MASK_CASTLE_KING_WHITE;
}

/**
 * @brief castle_queen_white
 * 
 * get the avaliability of castle queen side for white.
 * 
 * @return
 * - TRUE if castle is avaliable.
 * - FALSE if castle is not avaliable.
 */
constexpr inline bool GameState::castle_queen_white() const
{
    return state_register & MASK_CASTLE_QUEEN_WHITE;
}

/**
 * @brief castle_king_black
 * 
 * get the avaliability of castle king side for black.
 * 
 * @return
 * - TRUE if castle is avaliable.
 * - FALSE if castle is not avaliable.
 */
constexpr inline bool GameState::castle_king_black() const
{
    return state_register & MASK_CASTLE_KING_BLACK;
}

/**
 * @brief castle_queen_black
 * 
 * get the avaliability of castle queen side for black.
 * 
 * @return
 * - TRUE if castle is avaliable.
 * - FALSE if castle is not avaliable.
 */
constexpr inline bool GameState::castle_queen_black() const
{
    return state_register & MASK_CASTLE_QUEEN_BLACK;
}

/**
 * @brief en_passsant_square
 * 
 * get square where en passant is avaliable.
 * 
 * @return
 * - 0<=square<=63 if en passant is avaliable.
 * - SQ_INVALID if en en passant is no avaliable.
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
 * @return 0 <= move_number <= 524287.
 * 
 */
constexpr inline uint64_t GameState::move_number() const
{
    return (state_register & MASK_MOVE_NUMBER) >> SHIFT_MOVE_NUMBER;
}

/**
 * @brief half_move
 * 
 * get the number of half moves of the chess game.
 * 
 * @return
 * - (move_number * 2) if half_bit = 0(black was the last to make a move).
 * - (move_number * 2 + 1) if half_bit = 1(white was the last to make a move).
 * 
 */
constexpr inline uint64_t GameState::half_move() const
{
    const uint64_t move_number = (state_register & MASK_MOVE_NUMBER) >> SHIFT_MOVE_NUMBER;
    const uint64_t half_move_bit = (state_register & MASK_HALF_MOVE) >> SHIFT_HALF_MOVE;
    return (move_number << 1U) + half_move_bit;
}

/**
 * @brief set_triple_repetition_counterset_triple_repetition_counter
 * 
 * set number of times the position has been repeated.
 * 
 * @note counter must be <=3, otherwise state will be corrupted.
 * 
 * @param[in] counter triple repetition counter.
 * 
 */
constexpr inline void GameState::set_triple_repetition_counter(uint8_t counter)
{
    state_register &= ~MASK_TRIPLE_REPETITION_RULE;
    state_register |= static_cast<uint64_t>(counter) << SHIFT_TRIPLE_REPETITION_RULE;
}
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
    state_register &= ~MASK_SIDE_TO_MOVE;
    state_register |= static_cast<uint64_t>(side) << SHIFT_SIDE_TO_MOVE;
}

/**
 * @brief set_castle_king_white
 * 
 * set avaliability of castle king side for white.
 * 
 * @param[in] avaliable avaliability of castle.
 * 
 */
constexpr inline void GameState::set_castle_king_white(bool avaliable)
{
    state_register &= ~MASK_CASTLE_KING_WHITE;
    state_register |= static_cast<uint64_t>(avaliable) << SHIFT_CASTLE_KING_WHITE;
}

/**
 * @brief set_castle_queen_white
 * 
 * set avaliability of castle queen side for white.
 * 
 * @param[in] avaliable avaliability of castle.
 * 
 */
constexpr inline void GameState::set_castle_queen_white(bool avaliable)
{
    state_register &= ~MASK_CASTLE_QUEEN_WHITE;
    state_register |= static_cast<uint64_t>(avaliable) << SHIFT_CASTLE_QUEEN_WHITE;
}

/**
 * @brief set_castle_king_black
 * 
 * set avaliability of castle king side for black.
 * 
 * @param[in] avaliable avaliability of castle.
 * 
 */
constexpr inline void GameState::set_castle_king_black(bool avaliable)
{
    state_register &= ~MASK_CASTLE_KING_BLACK;
    state_register |= static_cast<uint64_t>(avaliable) << SHIFT_CASTLE_KING_BLACK;
}

/**
 * @brief set_castle_queen_black
 * 
 * set avaliability of castle queen side for black.
 * 
 * @param[in] avaliable avaliability of castle.
 * 
 */
constexpr inline void GameState::set_castle_queen_black(bool avaliable)
{
    state_register &= ~MASK_CASTLE_QUEEN_BLACK;
    state_register |= static_cast<uint64_t>(avaliable) << SHIFT_CASTLE_QUEEN_BLACK;
}


/**
 * @brief set_en_passant_square
 * 
 * set the square where en passant is avaliable.
 * 
 * @note SQ_INVALID means that en passant is not avaliable.
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
 * @note move number must be between 0-524287,
 *  otherwise state will be corrupted.
 * 
 * @param[in] move_number number of moves in the game
 * 
 */
constexpr inline void GameState::set_move_number(uint64_t move_number)
{
    state_register &= ~MASK_MOVE_NUMBER;
    state_register |= move_number << SHIFT_MOVE_NUMBER;
}

/**
 * @brief set_half_move
 * 
 * set the half move bit.
 * 
 * @note Usually when white moves put half_move_bit = 1, when black moves put to 0.
 * 
 * @param[in] half_move_bit
 *  - if 1 then half_move counter = move_number * 2 + 1 
 *  - if 0 then half_move counter = move_number * 2
 * 
 */
constexpr inline void GameState::set_half_move(bool half_move_bit)
{
    state_register &= ~MASK_HALF_MOVE;
    state_register |= static_cast<uint64_t>(half_move_bit) << SHIFT_HALF_MOVE;
}