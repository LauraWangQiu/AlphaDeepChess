#pragma once

/**
 * @file precomputed_data.hpp
 * @brief precomputed data.
 *
 * precomputed data
 * 
 */
#include "square.hpp"
#include <array>
#include <cassert>

/**
 * @brief PrecomputedData
 *
 * class with the precomputed data between chess pieces
 * 
 */
class PrecomputedData
{
public:
    PrecomputedData() = delete;

    ~PrecomputedData() = delete;

    /**
     * @brief get_distance_chebyshev
     * 
     * Get the distance between two squares using Chebyshev's Distance.
     * https://en.wikipedia.org/wiki/Chebyshev_distance
     * 
     * @note None.
     * 
     * @param[in] square1 First square to calculate distance
     * @param[in] square2 Second square to calculate distance
     * 
     * @return chebyshevDistance(square1, square2)
     */
    static inline float get_distance_chebyshev(Square square1, Square square2)
    {
        assert(square1.is_valid() && square2.is_valid());
        return CHEBYSHEV_DISTANCE[square1][square2];
    }

    /**
     * @brief get_distance_manhattan 
     * 
     * Get the distance between two squares using Manhattan's Distance.
     * https://simple.wikipedia.org/wiki/Manhattan_distance
     * 
     * @note None.
     * 
     * @param[in] square1 First square to calculate distance
     * @param[in] square2 Second square to calculate distance
     * 
     * @return manhattanDistance(square1, square2)
     */
    static inline float get_distance_manhattan(Square square1, Square square2)
    {
        assert(square1.is_valid() && square2.is_valid());
        return MANHATTAN_DISTANCE[square1][square2];
    }

private:
    static constexpr void initialize_distance_matrices();

    static float CHEBYSHEV_DISTANCE[64][64];
    static float MANHATTAN_DISTANCE[64][64];

    static const bool initialized;
};

float PrecomputedData::CHEBYSHEV_DISTANCE[64][64];
float PrecomputedData::MANHATTAN_DISTANCE[64][64];

const bool PrecomputedData::initialized = []() {
    PrecomputedData::initialize_distance_matrices();
    return true;
}();

constexpr void PrecomputedData::initialize_distance_matrices()
{
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            const Square sq1 = Square(i);
            const Square sq2 = Square(j);
            CHEBYSHEV_DISTANCE[i][j] = std::abs(int(sq1.row()) - int(sq2.row())) + std::abs(int(sq1.col()) - int(sq2.col()));
            MANHATTAN_DISTANCE[i][j] = std::max(std::abs(int(sq1.row()) - int(sq2.row())), std::abs(int(sq1.col()) - int(sq2.col())));
        }
    }
}
