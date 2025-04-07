#pragma once

/**
 * @file magic_bitboards.hpp
 * @brief magic bitboards services.
 *
 * https://www.chessprogramming.org/Magic_Bitboards
 * 
 * This technique is used to optimize the memory space of the lookup table of rook and bishop moves.
 * 
 * The lookup table is indexed by piece square and the blockers bitboard (pieces that block the path of the piece).
 * We cut off unnecesary information like the board borders and the squares outside its attack pattern.
 * Optimally we could use 11 bits for bishop moves (2048 > 1428) and 13 bits for rooks (8196 > 4900).
 * 
 * A magic number is a multiplier to the index with the following property: 
 *      - keeps intant the important info about the blockers (the nearest blockers to the piece are intact).
 *        For example: [Rook] -> -> -> [pawn1][pawn2].
 *        In this case pawn2 is an unnecesary piece inside the blockers bitboard, because the piece that actually
 *        block the path of the rook is pawn1.
 *      
 *      - Reduces the index number close to the optimal size (ideally to fit inside 11 or 13 bits)
 * 
 * The magic numbers are found by brute force and better and better magics are still being found.
 * We didn't reach yet the optimal hash size.
 */

#include "square.hpp"
#include "bit_utilities.hpp"
#include "piece.hpp"
#include <array>
#ifdef __BMI2__
#include <immintrin.h>   // for _pext_u64
#endif

constexpr int ROOK_TABLE_SIZE = 4096;
constexpr int BISHOP_TABLE_SIZE = 512;

/**
 * @brief magic_index_rook(uint64_t,Square,uint64_t)
 * 
 * calculate the magic index in the hash moves table
 * 
 * @note square must be valid
 * 
 * @param[in] blockers bitboard with all pieces on the board, they block the path of the piece
 * @param[in] rook_square rook square
 * @param[in] rook_attacks rook attack pattern in that square
 * 
 * @return blockers * magic_number >> (64 - number_occupancy_squares)
 */
inline uint64_t magic_index_rook(uint64_t blockers, Square rook_square, uint64_t rook_attacks);

/**
 * @brief magic_index_bishop(uint64_t,Square,uint64_t)
 * 
 * calculate the magic index in the hash moves table
 * 
 * @note square must be valid
 * 
 * @param[in] blockers bitboard with all pieces on the board, they block the path of the piece
 * @param[in] bishop_square bishop square
 * @param[in] bishop_attacks bishop attack pattern in that square
 * 
 * @return blockers * magic_number >> (64 - number_occupancy_squares)
 */
inline uint64_t magic_index_bishop(uint64_t blockers, Square bishop_square, uint64_t bishop_attacks);

#ifndef __BMI2__

/**
 * @brief rook_magic(Square)
 * 
 * returns the magic number for the square
 * 
 * @note square must be valid
 * 
 * @param[in] square rook square
 * 
 * @return ROOK_MAGICS[square]
 */
static inline constexpr uint64_t rook_magic(Square square);

/**
 * @brief bishop_magic(Square)
 * 
 * returns the magic number for the square
 * 
 * @note square must be valid
 * 
 * @param[in] square bishop square
 * 
 * @return BISHOP_MAGICS[square]
 */
static inline constexpr uint64_t bishop_magic(Square square);

/**
 * @brief ROOK_MAGICS
 * 
 * @note magics from this source: https://github.com/maksimKorzh/chess_programming/blob/master/src/magics/magics.txt
 * 
 */
static constexpr uint64_t ROOK_MAGICS[NUM_SQUARES] = {
    0x8a80104000800020ULL, 0x140002000100040ULL,  0x2801880a0017001ULL,  0x100081001000420ULL,  0x200020010080420ULL,
    0x3001c0002010008ULL,  0x8480008002000100ULL, 0x2080088004402900ULL, 0x800098204000ULL,     0x2024401000200040ULL,
    0x100802000801000ULL,  0x120800800801000ULL,  0x208808088000400ULL,  0x2802200800400ULL,    0x2200800100020080ULL,
    0x801000060821100ULL,  0x80044006422000ULL,   0x100808020004000ULL,  0x12108a0010204200ULL, 0x140848010000802ULL,
    0x481828014002800ULL,  0x8094004002004100ULL, 0x4010040010010802ULL, 0x20008806104ULL,      0x100400080208000ULL,
    0x2040002120081000ULL, 0x21200680100081ULL,   0x20100080080080ULL,   0x2000a00200410ULL,    0x20080800400ULL,
    0x80088400100102ULL,   0x80004600042881ULL,   0x4040008040800020ULL, 0x440003000200801ULL,  0x4200011004500ULL,
    0x188020010100100ULL,  0x14800401802800ULL,   0x2080040080800200ULL, 0x124080204001001ULL,  0x200046502000484ULL,
    0x480400080088020ULL,  0x1000422010034000ULL, 0x30200100110040ULL,   0x100021010009ULL,     0x2002080100110004ULL,
    0x202008004008002ULL,  0x20020004010100ULL,   0x2048440040820001ULL, 0x101002200408200ULL,  0x40802000401080ULL,
    0x4008142004410100ULL, 0x2060820c0120200ULL,  0x1001004080100ULL,    0x20c020080040080ULL,  0x2935610830022400ULL,
    0x44440041009200ULL,   0x280001040802101ULL,  0x2100190040002085ULL, 0x80c0084100102001ULL, 0x4024081001000421ULL,
    0x20030a0244872ULL,    0x12001008414402ULL,   0x2006104900a0804ULL,  0x1004081002402ULL,
};

/**
 * @brief BISHOP_MAGICS
 * 
 * @note magics from this source: https://github.com/maksimKorzh/chess_programming/blob/master/src/magics/magics.txt
 * 
 */
static constexpr uint64_t BISHOP_MAGICS[NUM_SQUARES] = {
    0x40040844404084ULL,   0x2004208a004208ULL,   0x10190041080202ULL,   0x108060845042010ULL,  0x581104180800210ULL,
    0x2112080446200010ULL, 0x1080820820060210ULL, 0x3c0808410220200ULL,  0x4050404440404ULL,    0x21001420088ULL,
    0x24d0080801082102ULL, 0x1020a0a020400ULL,    0x40308200402ULL,      0x4011002100800ULL,    0x401484104104005ULL,
    0x801010402020200ULL,  0x400210c3880100ULL,   0x404022024108200ULL,  0x810018200204102ULL,  0x4002801a02003ULL,
    0x85040820080400ULL,   0x810102c808880400ULL, 0xe900410884800ULL,    0x8002020480840102ULL, 0x220200865090201ULL,
    0x2010100a02021202ULL, 0x152048408022401ULL,  0x20080002081110ULL,   0x4001001021004000ULL, 0x800040400a011002ULL,
    0xe4004081011002ULL,   0x1c004001012080ULL,   0x8004200962a00220ULL, 0x8422100208500202ULL, 0x2000402200300c08ULL,
    0x8646020080080080ULL, 0x80020a0200100808ULL, 0x2010004880111000ULL, 0x623000a080011400ULL, 0x42008c0340209202ULL,
    0x209188240001000ULL,  0x400408a884001800ULL, 0x110400a6080400ULL,   0x1840060a44020800ULL, 0x90080104000041ULL,
    0x201011000808101ULL,  0x1a2208080504f080ULL, 0x8012020600211212ULL, 0x500861011240000ULL,  0x180806108200800ULL,
    0x4000020e01040044ULL, 0x300000261044000aULL, 0x802241102020002ULL,  0x20906061210001ULL,   0x5a84841004010310ULL,
    0x4010801011c04ULL,    0xa010109502200ULL,    0x4a02012000ULL,       0x500201010098b028ULL, 0x8040002811040900ULL,
    0x28000010020204ULL,   0x6000020202d0240ULL,  0x8918844842082200ULL, 0x4010011029020020ULL,
};

// clang-format off

/**
 * @brief ROOK_OCCUPANCY_NUMBER
 * 
 * number of squares where the rook could move from each square minus the board border squares
 */
static constexpr int ROOK_OCCUPANCY_NUMBER[NUM_SQUARES] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};

/**
 * @brief BISHOP_OCCUPANCY_NUMBER
 * 
 * number of squares where the bishop could move from each square minus the board border squares
 */
static constexpr int BISHOP_OCCUPANCY_NUMBER[NUM_SQUARES] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};

// clang-format on


/**
 * @brief rook_magic(Square)
 * 
 * returns the magic number for the square
 * 
 * @note square must be valid
 * 
 * @param[in] square rook square
 * 
 * @return ROOK_MAGICS[square]
 */
static inline constexpr uint64_t rook_magic(Square square)
{
    assert(square.is_valid());
    return ROOK_MAGICS[square];
}

/**
 * @brief bishop_magic(Square)
 * 
 * returns the magic number for the square
 * 
 * @note square must be valid
 * 
 * @param[in] square bishop square
 * 
 * @return BISHOP_MAGICS[square]
 */
static inline constexpr uint64_t bishop_magic(Square square)
{
    assert(square.is_valid());
    return BISHOP_MAGICS[square];
}
#endif
/**
 * @brief magic_index_rook(uint64_t,Square,uint64_t)
 * 
 * calculate the magic index in the hash moves table
 * 
 * @note square must be valid
 * 
 * @param[in] blockers bitboard with all pieces on the board, they block the path of the piece
 * @param[in] rook_square rook square
 * @param[in] rook_attacks rook attack pattern in that square
 * 
 * @return blockers * magic_number >> (64 - number_occupancy_squares)
 */
inline uint64_t magic_index_rook(uint64_t blockers, Square rook_square, uint64_t rook_attacks)
{
    // we cut unnecesary information to optimize memory space: board edges and squares outside the attack mask.
    const uint64_t edges = ((ROW_1_MASK | ROW_8_MASK) & ~get_row_mask(rook_square.row())) |
        ((COL_A_MASK | COL_H_MASK) & ~get_col_mask(rook_square.col()));
    blockers &= rook_attacks & ~edges;

#ifdef __BMI2__
    const uint64_t occupancy_mask = rook_attacks & ~edges;
    return _pext_u64(blockers, occupancy_mask);   // Optimized version using pext if available.
#else
    // index is calculated by multiplying blockers by the magic number, then squeeze all bits to the right
    return (blockers * rook_magic(rook_square)) >> (64 - ROOK_OCCUPANCY_NUMBER[rook_square]);
#endif
}

/**
 * @brief magic_index_bishop(uint64_t,Square,uint64_t)
 * 
 * calculate the magic index in the hash moves table
 * 
 * @note square must be valid
 * 
 * @param[in] blockers bitboard with all pieces on the board, they block the path of the piece
 * @param[in] bishop_square bishop square
 * @param[in] bishop_attacks bishop attack pattern in that square
 * 
 * @return blockers * magic_number >> (64 - number_occupancy_squares)
 */
inline uint64_t magic_index_bishop(uint64_t blockers, Square bishop_square, uint64_t bishop_attacks)
{
    // we cut unnecesary information to optimize memory space: board edges and squares outside the attack mask.
    const uint64_t edges = ((ROW_1_MASK | ROW_8_MASK) & ~get_row_mask(bishop_square.row())) |
        ((COL_A_MASK | COL_H_MASK) & ~get_col_mask(bishop_square.col()));

    blockers &= bishop_attacks & ~edges;

#ifdef __BMI2__
    const uint64_t occupancy_mask = bishop_attacks & ~edges;
    return _pext_u64(blockers, occupancy_mask);   // Optimized version using pext if available.
#else
    // index is calculated by multiplying blockers by the magic number, then squeeze all bits to the right
    return (blockers * bishop_magic(bishop_square)) >> (64 - BISHOP_OCCUPANCY_NUMBER[bishop_square]);
#endif
}