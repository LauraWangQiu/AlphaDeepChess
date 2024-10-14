#pragma once

#include "piece.hpp"
#include "square.hpp"

/**
 * @file game_state.hpp
 * @brief Game state of a chess game information utilities.
 *
 * 
 */

static constexpr uint32_t SHIFT_SIDE_TO_MOVE = 31U;
static constexpr uint32_t SHIFT_CASTLE_KING_WHITE = 30U;
static constexpr uint32_t SHIFT_CASTLE_QUEEN_WHITE = 29U;
static constexpr uint32_t SHIFT_CASTLE_KING_BLACK = 28U;
static constexpr uint32_t SHIFT_CASTLE_QUEEN_BLACK = 27U;
static constexpr uint32_t SHIFT_EN_PASSANT_SQUARE = 20U;
static constexpr uint32_t SHIFT_MOVE_NUMBER = 1U;
static constexpr uint32_t SHIFT_HALF_MOVE = 0U;

static constexpr uint32_t MASK_SIDE_TO_MOVE = (1U << SHIFT_SIDE_TO_MOVE);
static constexpr uint32_t MASK_CASTLE_KING_WHITE = (1U << SHIFT_CASTLE_KING_WHITE);
static constexpr uint32_t MASK_CASTLE_QUEEN_WHITE = (1U << SHIFT_CASTLE_QUEEN_WHITE);
static constexpr uint32_t MASK_CASTLE_KING_BLACK = (1U << SHIFT_CASTLE_KING_BLACK);
static constexpr uint32_t MASK_CASTLE_QUEEN_BLACK = (1U << SHIFT_CASTLE_QUEEN_BLACK);
static constexpr uint32_t MASK_EN_PASSANT_SQUARE = (0x7fU << SHIFT_EN_PASSANT_SQUARE);
static constexpr uint32_t MASK_MOVE_NUMBER = (0x7ffffU << SHIFT_MOVE_NUMBER);
static constexpr uint32_t MASK_HALF_MOVE = (1U << SHIFT_HALF_MOVE);


/**
 * @brief GameState
 * 
 * Represents the sate of the chess game.
 * 
 * @note game state is stored as a 32-bit number :
 * 
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
    constexpr inline Square en_passsant_square() const;

    /**
     * @brief move_number
     * 
     * get the number of moves of the chess game.
     * 
     * @return 0 <= move_number <= 524287.
     * 
     */
    constexpr inline uint32_t move_number() const;

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
    constexpr inline uint32_t half_move() const;


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
     * @brief set_en_passsant_square
     * 
     * set the square where en passant is avaliable.
     * 
     * @note SQ_INVALID means that en passant is not avaliable.
     * 
     * @param[in] square nen passant square.
     * 
     */
    constexpr inline void set_en_passsant_square(Square square);

    /**
     * @brief set_move_number
     * 
     * set the move number.
     * 
     * @note move number must be between 0-524287.
     * 
     * @param[in] move_number number of moves in the game
     * 
     */
    constexpr inline void set_move_number(uint32_t move_number);

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
    constexpr GameState() : state_register(0U) { }

    /**
     * @brief GameState
     * 
     * GameState class copy constuctor.
     * 
     * @param[in] gs gameState where to copy the initial value.
     * 
     */
    constexpr GameState(GameState& gs) : state_register(gs.state_register) { }

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
    uint32_t state_register;
};

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
    const uint32_t side = (state_register & MASK_SIDE_TO_MOVE) >> SHIFT_SIDE_TO_MOVE;
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
constexpr inline Square GameState::en_passsant_square() const
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
constexpr inline uint32_t GameState::move_number() const
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
constexpr inline uint32_t GameState::half_move() const
{
    const uint32_t move_number = (state_register & MASK_MOVE_NUMBER) >> SHIFT_MOVE_NUMBER;
    const uint32_t half_move_bit = (state_register & MASK_HALF_MOVE) >> SHIFT_HALF_MOVE;
    return (move_number << 1U) + half_move_bit;
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
    state_register |= static_cast<uint32_t>(side) << SHIFT_SIDE_TO_MOVE;
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
    state_register |= static_cast<uint32_t>(avaliable) << SHIFT_CASTLE_KING_WHITE;
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
    state_register |= static_cast<uint32_t>(avaliable) << SHIFT_CASTLE_QUEEN_WHITE;
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
    state_register |= static_cast<uint32_t>(avaliable) << SHIFT_CASTLE_KING_BLACK;
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
    state_register |= static_cast<uint32_t>(avaliable) << SHIFT_CASTLE_QUEEN_BLACK;
}


/**
 * @brief set_en_passsant_square
 * 
 * set the square where en passant is avaliable.
 * 
 * @note SQ_INVALID means that en passant is not avaliable.
 * 
 * @param[in] square nen passant square.
 * 
 */
constexpr inline void GameState::set_en_passsant_square(Square square)
{
    state_register &= ~MASK_EN_PASSANT_SQUARE;
    state_register |= square << SHIFT_EN_PASSANT_SQUARE;
}

/**
 * @brief set_move_number
 * 
 * set the move number.
 * 
 * @note move number must be between 0-524287.
 * 
 * @param[in] move_number number of moves in the game
 * 
 */
constexpr inline void GameState::set_move_number(uint32_t move_number)
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
    state_register |= static_cast<uint32_t>(half_move_bit) << SHIFT_HALF_MOVE;
}