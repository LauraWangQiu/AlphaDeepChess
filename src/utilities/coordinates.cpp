/**
 * @file coordinates.cpp
 * @brief coordinates services.
 *
 */

#include "coordinates.hpp"
#include <array>

/**
 * @brief get_direction 
 * 
 * Calculates the direction to go from origin square to end square.
 * 
 * @note if the two coordinates are not aligned vertically or horizontally 
 * the direction to go will be an approximation.
 * 
 * @param[in] origin_row The origin row coordinate
 * @param[in] origin_col The origin column coordinate
 * @param[in] end_row The end row coordinate
 * @param[in] end_col The end column coordinate
 * 
 * @return
 * - Direction::EAST,
 * - Direction::SOUTH_EAST,
 * - Direction::SOUTH,
 * - Direction::SOUTH_WEST,
 * - Direction::WEST,
 * - Direction::NORTH_WEST,
 * - Direction::NORTH,
 * - Direction::NORTH_EAST
 * 
 */
Direction get_direction(Row origin_row, Col origin_col, Row end_row, Col end_col)
{
    assert(is_valid_row(origin_row) && is_valid_col(origin_col));
    assert(is_valid_row(end_row) && is_valid_col(end_col));

    // Calculate row and column differences
    const int row_diff = static_cast<int>(end_row) - static_cast<int>(origin_row);
    const int col_diff = static_cast<int>(end_col) - static_cast<int>(origin_col);

    // Normalize row and column differences to {-1, 0, 1}
    const int normalized_row = (row_diff > 0) - (row_diff < 0);
    const int normalized_col = (col_diff > 0) - (col_diff < 0);

    // Compute the direction based taking advantage of the int value of each direction
    return static_cast<Direction>(8 * normalized_row + normalized_col);
}

static constexpr const std::array<std::array<uint64_t, 64>, 64> init_direction_mask()
{
    std::array<std::array<uint64_t, 64>, 64> PRECOMPUTED_DIR_MASK = {};

    for (Square sq1 = Square::A1; sq1.is_valid(); sq1++) {
        for (Square sq2 = Square::A1; sq2.is_valid(); sq2++) {

            if (sq1.row() == sq2.row()) {
                PRECOMPUTED_DIR_MASK[sq1][sq2] = get_row_mask(sq1.row());
            }
            else if (sq1.col() == sq2.col()) {
                PRECOMPUTED_DIR_MASK[sq1][sq2] = get_col_mask(sq1.col());
            }
            else if (squares_in_same_diagonal(sq1, sq2)) {
                PRECOMPUTED_DIR_MASK[sq1][sq2] = get_diagonal_mask(sq1.diagonal());
            }
            else if (squares_in_same_antidiagonal(sq1, sq2)) {
                PRECOMPUTED_DIR_MASK[sq1][sq2] = get_antidiagonal_mask(sq1.antidiagonal());
            }
            else {
                PRECOMPUTED_DIR_MASK[sq1][sq2] = 0ULL;
            }
        }
    }

    return PRECOMPUTED_DIR_MASK;
}

static constexpr const std::array<std::array<uint64_t, 64>, 64> PRECOMPUTED_DIR_MASK = init_direction_mask();

/**
 * @brief get_direction_mask 
 * 
 * Returns the mask of the common direction of two squares
 * 
 * @param[in] sq1 first square
 * @param[in] sq2 second square
 * 
 * @return
 * -
 * - row mask of sq1,sq2 if sq1.row() == sq2.row()
 * - col mask of sq1,sq2 if sq1.col() == sq2.col()
 * - diagonal_mask of sq1,sq2 if squares_in_same_diagonal(sq1, sq2).
 * - antidiagonal_mask of sq1,sq2 if squares_in_same_antidiagonal(sq1, sq2).
 * - 0ULL in other case
 */
uint64_t get_direction_mask(Square sq1, Square sq2)
{
    assert(sq1.is_valid() && sq2.is_valid());

    return PRECOMPUTED_DIR_MASK[sq1][sq2];
}