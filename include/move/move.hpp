#pragma once

/**
 * @file move.hpp
 * @brief Move types and utilities declaration.
 *
 * 
 */

#include "square.hpp"
#include "piece.hpp"


/**
 * @brief MoveType
 * 
 * Represents a chess move type.
 * 
 * - NORMAL = 0, 
 * - PROMOTION = 1, 
 * - EN_PASSANT = 2, 
 * - CASTLING = 3, 
 */
enum class MoveType
{
    NORMAL = 0,
    PROMOTION = 1,
    EN_PASSANT = 2,
    CASTLING = 3
};

static constexpr uint16_t SHIFT_MOVE_TYPE = 14U;
static constexpr uint16_t SHIFT_PROMOTION_PIECE = 12U;
static constexpr uint16_t SHIFT_ORIGIN_SQUARE = 6U;
static constexpr uint16_t SHIFT_END_SQUARE = 0U;

static constexpr uint16_t MASK_MOVE_TYPE = (0b11U << SHIFT_MOVE_TYPE);
static constexpr uint16_t MASK_PROMOTION_PIECE = (0b11U << SHIFT_PROMOTION_PIECE);
static constexpr uint16_t MASK_ORIGIN_SQUARE = (0b111111U << SHIFT_ORIGIN_SQUARE);
static constexpr uint16_t MASK_END_SQUARE = (0b111111U << SHIFT_END_SQUARE);


/**
 * @brief Move
 * 
 * Represents a chess move.
 * Move::null() = 0.
 * 
 * Move is valid if origin_square != destination_square.
 * 
 * @note move is stored as a 16-bit number :
 * 
 * 14-15: special move flag: NORMAL(0), PROMOTION(1), EN_PASSANT(2), CASTLING(3).
 * 12-13: promotion piece : KNIGHT(0), BISHOP(1), ROOK(2), QUEEN(3).
 * 6-11: square from (origin) : (0 to 63).
 * 0- 5: square to (destination): (0 to 63).
 * 
 */
class Move
{
public:
    /**
     * @brief Move
     * 
     * Empty Constructor, initializes Move to 0 (null move).
     * 
     */
    constexpr Move() : data(0U) { }

    /**
     * @brief Move(const Move& move)
     * 
     * copy constructor.
     * 
     * @param[in] move 
     * 
     */
    constexpr Move(const Move& move) : data(move.data) { }

    /**
     * @brief Move(std::uint16_t move_data)
     * 
     * Constructor with move_data raw value.
     * 
     * @param[in] move_data raw data value.
     * 
     */
    constexpr explicit Move(std::uint16_t move_data) : data(move_data) { }

    /**
     * @brief Move(Square from, Square to, MoveType type, PieceType promotionPiece)
     * 
     * Constructor with all information.
     * 
     * @param[in] square_from origin square.
     * @param[in] square_to   end square.
     * @param[in] move_type move type.
     * @param[in] promotion_piece promotion piece.
     * 
     */
    constexpr Move(Square square_from, Square square_to, MoveType move_type = MoveType::NORMAL,
                   PieceType promotion_piece = PieceType::KNIGHT)
        : data((square_from.value() << SHIFT_ORIGIN_SQUARE) |
               square_to.value() << SHIFT_END_SQUARE |
               ((static_cast<uint16_t>(promotion_piece) - 1) << SHIFT_PROMOTION_PIECE) |
               (static_cast<uint16_t>(move_type) << SHIFT_MOVE_TYPE))
    {

        // we substract -1 because PieceType enum values have an offset of 1.
        // E.g Knight = 0 and PieceType::KNIGHT = 1
    }

    /**
     * @brief square_from
     * 
     * Return the origin square.
     * 
     * @return origin square.
     * 
     */
    constexpr inline Square square_from() const
    {
        return Square((data & MASK_ORIGIN_SQUARE) >> SHIFT_ORIGIN_SQUARE);
    }

    /**
     * @brief square_to
     * 
     * Return the destination square.
     * 
     * @return destination square.
     * 
     */
    constexpr inline Square square_to() const
    {
        return Square((data & MASK_END_SQUARE) << SHIFT_END_SQUARE);
    }

    /**
     * @brief type
     * 
     * Return move type.
     * 
     * @return MoveType
     *  - NORMAL(00)
     *  - PROMOTION(01)
     *  - EN_PASSANT(10)
     *  - CASTLING(11)
     * 
     */
    constexpr inline MoveType type() const
    {
        return static_cast<MoveType>((data & MASK_MOVE_TYPE) >> SHIFT_MOVE_TYPE);
    }

    /**
     * @brief promotion_piece
     * 
     * Return promotion piece.
     * 
     * @return PieceType
     *  - KNIGHT(00)
     *  - BISHOP(01)
     *  - ROOK(10)
     *  - QUEEN(11)
     * 
     */
    constexpr inline PieceType promotion_piece() const
    {
        // we substract -1 because PieceType enum values have an offset of 1.
        // E.g Knight = 0 and PieceType::KNIGHT = 1
        return static_cast<PieceType>(((data & MASK_PROMOTION_PIECE) >> SHIFT_PROMOTION_PIECE) + 1);
    }

    /**
     * @brief is_valid
     * 
     * Calculates if move is valid.
     * Null move is also not valid.
     * 
     * @note This just checks if the move if pseudo-valid.
     * 
     * @return 
     *  - TRUE if origin != destination.
     *  - FALSE if origin == destination.
     * 
     */
    constexpr bool is_valid() const { return square_from() != square_to(); }

    /**
     * @brief null
     * 
     * null move is 0.
     * 
     * @return Move(0)
     * 
     */
    static constexpr Move null() { return Move(0U); }

    /**
     * @brief castle_white_king
     * 
     * white king side castle move.
     * 
     * @return Move(Square::SQ_E1, Square::SQ_G1, MoveType::CASTLING).
     * 
     */
    static constexpr Move castle_white_king()
    {
        return Move(Square::SQ_E1, Square::SQ_G1, MoveType::CASTLING);
    }

    /**
     * @brief castle_white_queen
     * 
     * white queen side castle move.
     * 
     * @return Move(Square::SQ_E1, Square::SQ_C1, MoveType::CASTLING).
     * 
     */
    static constexpr Move castle_white_queen()
    {
        return Move(Square::SQ_E1, Square::SQ_C1, MoveType::CASTLING);
    }

    /**
     * @brief castle_black_king
     * 
     * black king side castle move.
     * 
     * @return Move(Square::SQ_E8, Square::SQ_G8, MoveType::CASTLING).
     * 
     */
    static constexpr Move castle_black_king()
    {
        return Move(Square::SQ_E8, Square::SQ_G8, MoveType::CASTLING);
    }

    /**
     * @brief castle_black_queen
     * 
     * black queen side castle move.
     * 
     * @return Move(Square::SQ_E8, Square::SQ_C8, MoveType::CASTLING).
     * 
     */
    static constexpr Move castle_black_queen()
    {
        return Move(Square::SQ_E8, Square::SQ_C8, MoveType::CASTLING);
    }

    /**
     * @brief operator==(const Move& sq)
     * equality operator overload
     * 
     * @return
     *  - TRUE if data == other.data.
     *  - FALSE data != other.data.
     */
    constexpr bool operator==(const Move& other) const { return data == other.data; }

    /**
     * @brief operator==(const Move& sq)
     * equality operator overload
     * 
     * @return
     *  - TRUE if data == other.data.
     *  - FALSE data != other.data.
     */
    constexpr bool operator!=(const Move& other) const { return data != other.data; }

    /**
     * @brief operator=
     * 
     * Assignment operator overload.
     * 
     * @param[in] other move where to copy the data.
     * 
     * @return *this
     * 
     */
    constexpr Move& operator=(const Move& other)
    {
        if (this != &other)   // not a self-assignment
        {
            this->data = other.data;
        }
        return *this;
    }

    /**
     * @brief to_string
     * 
     * calculate string representation. E.g : e2e4, e7e8q.
     * 
     * @return 
     *  - std::string representation if move is valid.
     *  - "invalid" if move is invalid
     */
    inline std::string to_string() const;

private:
    std::uint16_t data;
};

/**
 * @brief to_string
 * 
 * calculate string representation. E.g : e2e4, e7e8q.
 * 
 * @return 
 *  - std::string representation if move is valid.
 *  - "invalid" if move is invalid
 */
inline std::string Move::to_string() const
{
    if (!is_valid()) {
        return "invalid";
    }
    else if (type() == MoveType::PROMOTION) {
        return square_from().to_string() + square_to().to_string() +
            pieceType_to_char(promotion_piece());
    }
    else {
        return square_from().to_string() + square_to().to_string();
    }
}