/**
 * @file precomputed_move_data.cpp
 * @brief precomputed move data services.
 *
 * precomputed move data with fancy magic bitboards
 * 
 * https://www.chessprogramming.org/Magic_Bitboards
 * 
 */

#include "precomputed_move_data.hpp"

using ArrayBitboards = std::array<uint64_t, 64>;
using ArrayBitboardsConst = const std::array<uint64_t, 64>;

static constexpr ArrayBitboardsConst initialize_white_pawn_attacks();
static constexpr ArrayBitboardsConst initialize_black_pawn_attacks();
static constexpr ArrayBitboardsConst initialize_king_attacks();
static constexpr ArrayBitboardsConst initialize_knight_attacks();
static constexpr ArrayBitboardsConst initialize_rook_attacks();
static constexpr ArrayBitboardsConst initialize_bishop_attacks();

static constexpr uint64_t calculate_rook_moves(Square square, uint64_t blockerBB);
static constexpr uint64_t calculate_bishop_moves(Square square, uint64_t blockerBB);

static const TableRookMoves initialize_rook_legal_moves(ArrayBitboardsConst& ROOK_ATTACKS);
static const TableBishopMoves initialize_bishop_legal_moves(ArrayBitboardsConst& BISHOP_ATTACKS);


ArrayBitboardsConst PrecomputedMoveData::WHITE_PAWN_ATTACKS = initialize_white_pawn_attacks();
ArrayBitboardsConst PrecomputedMoveData::BLACK_PAWN_ATTACKS = initialize_black_pawn_attacks();
ArrayBitboardsConst PrecomputedMoveData::KING_ATTACKS = initialize_king_attacks();
ArrayBitboardsConst PrecomputedMoveData::KNIGHT_ATTACKS = initialize_knight_attacks();
ArrayBitboardsConst PrecomputedMoveData::BISHOP_ATTACKS = initialize_bishop_attacks();
ArrayBitboardsConst PrecomputedMoveData::ROOK_ATTACKS = initialize_rook_attacks();

const TableRookMoves PrecomputedMoveData::ROOK_MOVES = initialize_rook_legal_moves(ROOK_ATTACKS);
const TableBishopMoves PrecomputedMoveData::BISHOP_MOVES = initialize_bishop_legal_moves(BISHOP_ATTACKS);


static const TableRookMoves initialize_rook_legal_moves(ArrayBitboardsConst& ROOK_ATTACKS)
{
    TableRookMoves ROOK_MOVES;

    for (Square square = Square::A1; square.is_valid(); square++) {
        const uint64_t attacks = ROOK_ATTACKS[square];

        int indicesInMoveMask[64] = {0};
        int numIndices = 0;

        // Create a list of the indices of the bits that are set to 1 in the movement mask
        for (int i = 0; i < 64; i++) {
            if (attacks & (1ULL << i)) {
                indicesInMoveMask[numIndices++] = i;
            }
        }

        // Calculate total number of different bitboards (2^n)
        int numPatterns = 1 << numIndices;

        // Create all bitboards
        for (int patternIndex = 0; patternIndex < numPatterns; patternIndex++) {
            // calculate blocker bitboard
            uint64_t blockers = 0;
            for (int bitIndex = 0; bitIndex < numIndices; bitIndex++) {
                uint64_t bit = (patternIndex >> bitIndex) & 1;
                blockers |= bit << indicesInMoveMask[bitIndex];
            }

            const uint64_t index = magic_index_rook(blockers, square, attacks);
            assert(index < 4096);

            // create entry in rook lookupTable
            ROOK_MOVES[square][index] = calculate_rook_moves(square, blockers);
        }
    }

    return ROOK_MOVES;
}

static const TableBishopMoves initialize_bishop_legal_moves(ArrayBitboardsConst& BISHOP_ATTACKS)
{
    TableBishopMoves BISHOP_MOVES;

    for (Square square = Square::A1; square.is_valid(); square++) {

        const uint64_t attacks = BISHOP_ATTACKS[square];
        const uint64_t edges = ((ROW_1_MASK | ROW_8_MASK) & ~get_row_mask(square.row())) |
            ((COL_A_MASK | COL_H_MASK) & ~get_col_mask(square.col()));

        int indicesInMoveMask[64] = {0};
        int numIndices = 0;

        // Create a list of the indices of the bits that are set to 1 in the movement mask
        for (int i = 0; i < 64; i++) {
            if (attacks & (1ULL << i)) {
                indicesInMoveMask[numIndices++] = i;
            }
        }

        // Calculate total number of different bitboards (2^n)
        int numPatterns = 1 << numIndices;

        // Create all bitboards
        for (int patternIndex = 0; patternIndex < numPatterns; patternIndex++) {
            // calculate blocker bitboard
            uint64_t blockers = 0;
            for (int bitIndex = 0; bitIndex < numIndices; bitIndex++) {
                uint64_t bit = (patternIndex >> bitIndex) & 1;
                blockers |= bit << indicesInMoveMask[bitIndex];
            }
            blockers &= ~edges;   // blockers minus the board edges, because they dont matter in the calculation

            const uint64_t index = magic_index_bishop(blockers, square, attacks);
            assert(index < 512);

            // create entry in rook lookupTable
            BISHOP_MOVES[square][index] = calculate_bishop_moves(square, blockers);
        }
    }

    return BISHOP_MOVES;
}


constexpr ArrayBitboardsConst initialize_king_attacks()
{
    ArrayBitboards KING_ATTACKS {};

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


constexpr ArrayBitboardsConst initialize_knight_attacks()
{
    ArrayBitboards KNIGHT_ATTACKS {};

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

constexpr ArrayBitboardsConst initialize_white_pawn_attacks()
{
    ArrayBitboards WHITE_PAWN_ATTACKS {};

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

constexpr ArrayBitboardsConst initialize_black_pawn_attacks()
{
    ArrayBitboards BLACK_PAWN_ATTACKS {};

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

constexpr ArrayBitboardsConst initialize_rook_attacks()
{
    ArrayBitboards ROOK_ATTACKS {};

    for (Row row = ROW_1; is_valid_row(row); row++) {
        for (Col col = COL_A; is_valid_col(col); col++) {
            const Square rook_square = Square(row, col);
            ROOK_ATTACKS[rook_square] = get_row_mask(row) | get_col_mask(col);
            ROOK_ATTACKS[rook_square] &= ~rook_square.mask();   // Remove the piece itself
        }
    }
    return ROOK_ATTACKS;
}
constexpr ArrayBitboardsConst initialize_bishop_attacks()
{
    ArrayBitboards BISHOP_ATTACKS {};

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

constexpr uint64_t calculate_rook_moves(Square square, uint64_t blockers)
{
    assert(square.is_valid());

    uint64_t moves_mask = 0ULL;

    const Direction dirs[4] = {NORTH, SOUTH, EAST, WEST};

    for (const Direction dir : dirs) {

        Square aux_sq = square;
        aux_sq.to_direction(dir);

        while (aux_sq.is_valid()) {

            moves_mask |= aux_sq.mask();

            if (blockers & aux_sq.mask()) {
                break;   // if there is a blocker in the square we stop in this direction
            }
            else {
                aux_sq.to_direction(dir);
            }
        }
    }

    return moves_mask;
}

constexpr uint64_t calculate_bishop_moves(Square square, uint64_t blockers)
{
    assert(square.is_valid());

    uint64_t moves_mask = 0ULL;

    const Direction dirs[4] = {NORTH_EAST, NORTH_WEST, SOUTH_EAST, SOUTH_WEST};

    for (const Direction dir : dirs) {

        Square aux_sq = square;
        aux_sq.to_direction(dir);

        while (aux_sq.is_valid()) {

            moves_mask |= aux_sq.mask();

            if (blockers & aux_sq.mask()) {
                break;   // if there is a blocker in the square we stop in this direction
            }
            else {
                aux_sq.to_direction(dir);
            }
        }
    }

    return moves_mask;
}