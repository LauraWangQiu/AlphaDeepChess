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
#include "coordinates.hpp"

/**
 * @brief Array of bitboards for each square.
 *
 * This typedef represents an array of 64-bit integers (`uint64_t`), where each
 * element corresponds to a bitboard for a specific square on the chessboard.
 */
using ArrayBB = std::array<uint64_t, 64>;

/**
 * @brief Constant array of bitboards for each square.
 *
 * This typedef represents a constant array of 64-bit integers (`uint64_t`),
 * where each element corresponds to a bitboard for a specific square on the chessboard.
 */
using ArrayBBConst = const std::array<uint64_t, 64>;

static constexpr ArrayBBConst init_white_pawn_attacks();
static constexpr ArrayBBConst init_black_pawn_attacks();
static constexpr ArrayBBConst init_king_attacks();
static constexpr ArrayBBConst init_knight_attacks();
static constexpr ArrayBBConst init_rook_attacks();
static constexpr ArrayBBConst init_bishop_attacks();
static constexpr ArrayBBConst init_queen_attacks(ArrayBBConst& rookAttacks, ArrayBBConst& bishopAttacks);

static constexpr uint64_t calculate_rook_moves(Square square, uint64_t blockerBB);
static constexpr uint64_t calculate_bishop_moves(Square square, uint64_t blockerBB);

static const TableRookMoves init_rook_legal_moves(ArrayBBConst& ROOK_ATTACKS);
static const TableBishopMoves init_bishop_legal_moves(ArrayBBConst& BISHOP_ATTACKS);


ArrayBBConst PrecomputedMoveData::WHITE_PAWN_ATTACKS = init_white_pawn_attacks();
ArrayBBConst PrecomputedMoveData::BLACK_PAWN_ATTACKS = init_black_pawn_attacks();
ArrayBBConst PrecomputedMoveData::KING_ATTACKS = init_king_attacks();
ArrayBBConst PrecomputedMoveData::KNIGHT_ATTACKS = init_knight_attacks();
ArrayBBConst PrecomputedMoveData::BISHOP_ATTACKS = init_bishop_attacks();
ArrayBBConst PrecomputedMoveData::ROOK_ATTACKS = init_rook_attacks();
ArrayBBConst PrecomputedMoveData::QUEEN_ATTACKS = init_queen_attacks(ROOK_ATTACKS, BISHOP_ATTACKS);

const TableRookMoves PrecomputedMoveData::ROOK_MOVES = init_rook_legal_moves(ROOK_ATTACKS);
const TableBishopMoves PrecomputedMoveData::BISHOP_MOVES = init_bishop_legal_moves(BISHOP_ATTACKS);

const std::array<std::array<uint64_t, 64>, 64> PrecomputedMoveData::BETWEEN_BITBOARDS = init_between_bitboards();

static const TableRookMoves init_rook_legal_moves(ArrayBBConst& ROOK_ATTACKS)
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

static const TableBishopMoves init_bishop_legal_moves(ArrayBBConst& BISHOP_ATTACKS)
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


constexpr ArrayBBConst init_king_attacks()
{
    ArrayBB KING_ATTACKS {};

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


constexpr ArrayBBConst init_knight_attacks()
{
    ArrayBB KNIGHT_ATTACKS {};

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

constexpr ArrayBBConst init_white_pawn_attacks()
{
    ArrayBB WHITE_PAWN_ATTACKS {};

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

constexpr ArrayBBConst init_black_pawn_attacks()
{
    ArrayBB BLACK_PAWN_ATTACKS {};

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

constexpr ArrayBBConst init_rook_attacks()
{
    ArrayBB ROOK_ATTACKS {};

    for (Row row = ROW_1; is_valid_row(row); row++) {
        for (Col col = COL_A; is_valid_col(col); col++) {
            const Square rook_square = Square(row, col);
            ROOK_ATTACKS[rook_square] = get_row_mask(row) | get_col_mask(col);
            ROOK_ATTACKS[rook_square] &= ~rook_square.mask();   // Remove the piece itself
        }
    }
    return ROOK_ATTACKS;
}
constexpr ArrayBBConst init_bishop_attacks()
{
    ArrayBB BISHOP_ATTACKS {};

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

constexpr ArrayBBConst init_queen_attacks(ArrayBBConst& rookAttacks, ArrayBBConst& bishopAttacks)
{
    ArrayBB QUEEN_ATTACKS {};

    for (Square queen_sq = Square::A1; queen_sq.is_valid(); queen_sq++) {

        QUEEN_ATTACKS[queen_sq] = rookAttacks[queen_sq] | bishopAttacks[queen_sq];
    }
    return QUEEN_ATTACKS;
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


constexpr const std::array<std::array<uint64_t, 64>, 64> PrecomputedMoveData::init_between_bitboards()
{
    std::array<std::array<uint64_t, 64>, 64> between_bb = {};

    for (Square sq1 = Square::A1; sq1.is_valid(); sq1++) {

        const uint64_t sq1_blocker = sq1.mask();
        for (Square sq2 = Square::A1; sq2.is_valid(); sq2++) {
            const uint64_t sq2_blocker = sq2.mask();

            const uint64_t direction_mask = get_direction_mask(sq1, sq2);

            // the in between moves are the intersection between the rays of moves casted from sq1 and sq2.
            between_bb[sq1][sq2] = (rookMoves(sq1, sq2_blocker) | bishopMoves(sq1, sq2_blocker)) &
                (rookMoves(sq2, sq1_blocker) | bishopMoves(sq2, sq1_blocker));

            between_bb[sq1][sq2] &= direction_mask;   // filter squares outside the direction
        }
    }

    return between_bb;
}