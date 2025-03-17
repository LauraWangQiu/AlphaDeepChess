/**
 * @file precomputed_move_data.cpp
 * @brief precomputed move data services.
 *
 * precomputed move data and magic bitboards
 * 
 * https://www.chessprogramming.org/Magic_Bitboards
 * 
 */

#include "precomputed_move_data.hpp"


std::array<uint64_t, 64> PrecomputedMoveData::WHITE_PAWN_ATTACKS = initializeWhitePawnAttacks();
std::array<uint64_t, 64> PrecomputedMoveData::BLACK_PAWN_ATTACKS = initializeBlackPawnAttacks();
std::array<uint64_t, 64> PrecomputedMoveData::KING_ATTACKS = initializeKingAttacks();
std::array<uint64_t, 64> PrecomputedMoveData::KNIGHT_ATTACKS = initializeKnightAttacks();
std::array<uint64_t, 64> PrecomputedMoveData::BISHOP_ATTACKS = initializeBishopAttacks();
std::array<uint64_t, 64> PrecomputedMoveData::ROOK_ATTACKS = initializeRookAttacks();
std::array<uint64_t, 64> PrecomputedMoveData::QUEEN_ATTACKS = initializeQueenAttacks();

constexpr std::array<uint64_t, 64> PrecomputedMoveData::initializeKingAttacks()
{
    std::array<uint64_t, 64> KING_ATTACKS {};

    const int dirs[8][2] = {{1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}};


    for (Row row = ROW_1; is_valid_row(row); row++) {
        for (Col col = COL_A; is_valid_col(col); col++) {

            const Square king_square = Square(row, col);

            for (auto& dir : dirs) {

                const Row attack_row = row + static_cast<Row>(dir[0]);
                const Col attack_col = col + static_cast<Col>(dir[1]);

                const Square attack_square = Square(attack_row, attack_col);

                if (attack_square.is_valid()) {

                    KING_ATTACKS[king_square] |= attack_square.mask();
                }
            }
        }
    }
    return KING_ATTACKS;
}


constexpr std::array<uint64_t, 64> PrecomputedMoveData::initializeKnightAttacks()
{
    std::array<uint64_t, 64> KNIGHT_ATTACKS {};

    const int dirs[8][2] = {{-2, -1}, {-2, 1}, {2, -1}, {2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}};

    for (Row row = ROW_1; is_valid_row(row); row++) {
        for (Col col = COL_A; is_valid_col(col); col++) {

            const Square knight_square = Square(row, col);

            for (auto& dir : dirs) {

                const Row attack_row = row + static_cast<Row>(dir[0]);
                const Col attack_col = col + static_cast<Col>(dir[1]);

                const Square attack_square = Square(attack_row, attack_col);

                if (attack_square.is_valid()) {
                    KNIGHT_ATTACKS[knight_square] |= attack_square.mask();
                }
            }
        }
    }
    return KNIGHT_ATTACKS;
}

constexpr std::array<uint64_t, 64> PrecomputedMoveData::initializeWhitePawnAttacks()
{
    std::array<uint64_t, 64> WHITE_PAWN_ATTACKS {};

    const int dirs[2][2] = {{1, 1}, {1, -1}};

    for (Row row = ROW_1; is_valid_row(row); row++) {
        for (Col col = COL_A; is_valid_col(col); col++) {

            const Square pawn_square = Square(row, col);

            for (auto& dir : dirs) {

                const Row attack_row = row + static_cast<Row>(dir[0]);
                const Col attack_col = col + static_cast<Col>(dir[1]);

                const Square attack_square = Square(attack_row, attack_col);

                if (attack_square.is_valid()) {

                    WHITE_PAWN_ATTACKS[pawn_square] |= attack_square.mask();
                }
            }
        }
    }
    return WHITE_PAWN_ATTACKS;
}

constexpr std::array<uint64_t, 64> PrecomputedMoveData::initializeBlackPawnAttacks()
{
    std::array<uint64_t, 64> BLACK_PAWN_ATTACKS {};

    const int dirs[2][2] = {{-1, -1}, {-1, +1}};

    for (Row row = ROW_1; is_valid_row(row); row++) {
        for (Col col = COL_A; is_valid_col(col); col++) {

            const Square pawn_square = Square(row, col);

            for (auto& dir : dirs) {

                const Row attack_row = row + static_cast<Row>(dir[0]);
                const Col attack_col = col + static_cast<Col>(dir[1]);

                const Square attack_square = Square(attack_row, attack_col);

                if (attack_square.is_valid()) {

                    BLACK_PAWN_ATTACKS[pawn_square] |= attack_square.mask();
                }
            }
        }
    }
    return BLACK_PAWN_ATTACKS;
}

constexpr std::array<uint64_t, 64> PrecomputedMoveData::initializeRookAttacks()
{
    std::array<uint64_t, 64> ROOK_ATTACKS {};

    for (Row row = ROW_1; is_valid_row(row); row++) {
        for (Col col = COL_A; is_valid_col(col); col++) {
            const Square rook_square = Square(row, col);
            ROOK_ATTACKS[rook_square] = get_row_mask(row) | get_col_mask(col);
            ROOK_ATTACKS[rook_square] &= ~rook_square.mask();   // Remove the piece itself
        }
    }
    return ROOK_ATTACKS;
}
constexpr std::array<uint64_t, 64> PrecomputedMoveData::initializeBishopAttacks()
{
    std::array<uint64_t, 64> BISHOP_ATTACKS {};

    for (Row row = ROW_1; is_valid_row(row); row++) {
        for (Col col = COL_A; is_valid_col(col); col++) {

            const Square bishop_square = Square(row, col);

            const Diagonal bishop_diagonal = bishop_square.diagonal();
            const AntiDiagonal bishop_antidiagonal = bishop_square.antidiagonal();

            BISHOP_ATTACKS[bishop_square] = get_diagonal_mask(bishop_diagonal);
            BISHOP_ATTACKS[bishop_square] |= get_antidiagonal_mask(bishop_antidiagonal);

            BISHOP_ATTACKS[bishop_square] &= ~bishop_square.mask();   // Remove the piece itself
        }
    }
    return BISHOP_ATTACKS;
}
constexpr std::array<uint64_t, 64> PrecomputedMoveData::initializeQueenAttacks()
{
    std::array<uint64_t, 64> QUEEN_ATTACKS {};

    for (Square sq = Square::SQ_A1; sq.is_valid(); sq++) {
        QUEEN_ATTACKS[sq] = ROOK_ATTACKS[sq] | BISHOP_ATTACKS[sq];
    }
    return QUEEN_ATTACKS;
}

uint64_t PrecomputedMoveData::calculateLegalRookMoves(Square square, uint64_t blockerBB)
{
    uint64_t movesBitboard = 0;

    int row = square.row();
    int col = square.col();

    int rookDirections[4][2] = {
        {1, 0},    // Up
        {-1, 0},   // Down
        {0, 1},    // Right
        {0, -1}    // Left
    };

    for (int i = 0; i < 4; ++i) {
        int dirRow = rookDirections[i][0];
        int dirCol = rookDirections[i][1];

        int auxRow = row + dirRow;
        int auxCol = col + dirCol;

        Square auxSquare((Row)(auxRow), (Col)(auxCol));

        while (auxSquare.is_valid()) {

            // set the square to 1 in the bitboard
            movesBitboard |= auxSquare.mask();

            // if there is a blocker in the square we stop in this direction
            if (blockerBB & auxSquare.mask()) {
                break;
            }

            auxRow += dirRow;
            auxCol += dirCol;

            auxSquare = Square((Row)(auxRow), (Col)(auxCol));
        }
    }

    return movesBitboard;
}

uint64_t PrecomputedMoveData::calculateLegalBishopMoves(Square square, uint64_t blockerBB)
{
    uint64_t movesBitboard = 0;

    int row = square.row();
    int col = square.col();

    int bishopDirections[4][2] = {
        {1, 1},    // Up-right
        {1, -1},   // Up-left
        {-1, 1},   // Down-right
        {-1, -1}   // Down-left
    };

    for (int i = 0; i < 4; ++i) {
        int dirRow = bishopDirections[i][0];
        int dirCol = bishopDirections[i][1];

        int auxRow = row + dirRow;
        int auxCol = col + dirCol;

        Square auxSquare((Row)(auxRow), (Col)(auxCol));

        while (auxSquare.is_valid()) {

            // set the square to 1 in the bitboard
            movesBitboard |= auxSquare.mask();

            // if there is a blocker in the square we stop in this direction
            if (blockerBB & auxSquare.mask()) {
                break;
            }

            auxRow += dirRow;
            auxCol += dirCol;

            auxSquare = Square((Row)(auxRow), (Col)(auxCol));
        }
    }

    return movesBitboard;
}