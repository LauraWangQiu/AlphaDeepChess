/**
 * @file move_ordering_MVV_LVA.cpp
 * @brief move ordering services.
 *
 * chess move ordering implementation. 
 * MVV-LVA (Most Valuable Victim - Least Valuable Aggressor)
 * 
 * https://www.chessprogramming.org/MVV-LVA
 */

#include "move_ordering.hpp"
#include "killer_moves.hpp"
#include <cassert>
#include <algorithm>

thread_local uint8_t MOVE_SCORE_TABLE[Move::MAX_ID() + 1U];

/**
 * @brief move_value
 * 
 * Orders moves using MVV-LVA and promotion heuristics.
 * 
 * @param[in] move move.
 * @param[in] board chess position.
 * @param[in] ply actual search depth ply.
 * 
 * @return move value
 * 
 */
static uint8_t move_value(const Move& move, const Board& board, uint32_t ply);

/**
 * @brief order the move list by priority from best to worst
 *  
 * @note the first move in the list is the move with the best prospects of being the best move in the position.
 * 
 * @param[in,out] moves move list.
 * @param[in] board chess position.
 * @param[in] ply actual search depth ply.
 */
void order_moves(MoveList& moves, const Board& board, uint32_t ply)
{

    for (const Move& move : moves) {
        MOVE_SCORE_TABLE[move.id()] = move_value(move, board, ply);
    }

    std::sort(moves.begin(), moves.end(), [](const Move& move_1, const Move& move_2) {
        return MOVE_SCORE_TABLE[move_1.id()] > MOVE_SCORE_TABLE[move_2.id()];
    });
}

/**
 * @brief move_value
 * 
 * Orders moves using MVV-LVA and promotion heuristics.
 * 
 * @param[in] move move.
 * @param[in] board chess position.
 * @param[in] ply actual search depth ply.
 * 
 * @return move value (0-255)
 * 
 */
static uint8_t move_value(const Move& move, const Board& board, uint32_t ply)
{
    assert(move.is_valid());

    // precomputed move value MVV_LVA_table[victim_piece][attacker_piece]
    static constexpr uint8_t MVV_LVA_table[NUM_CHESS_PIECE_TYPES][NUM_CHESS_PIECE_TYPES] = {
        {15, 14, 13, 12, 11, 10, 0},   // victim P, attacker P, N, B, R, Q, K, EMPTY
        {25, 24, 23, 22, 21, 20, 0},   // victim N, attacker P, N, B, R, Q, K, EMPTY
        {35, 34, 33, 32, 31, 30, 0},   // victim B, attacker P, N, B, R, Q, K, EMPTY
        {45, 44, 43, 42, 41, 40, 0},   // victim R, attacker P, N, B, R, Q, K, EMPTY
        {55, 54, 53, 52, 51, 50, 0},   // victim Q, attacker P, N, B, R, Q, K, EMPTY
        {0, 0, 0, 0, 0, 0, 0},         // victim K, attacker P, N, B, R, Q, K, EMPTY
        {0, 0, 0, 0, 0, 0, 0}          // victim EMPTY, attacker P, N, B, R, Q, K, EMPTY
    };

    // precomputed promotion bonuses [promo_piece_type] P, N, B, R, Q, K, EMPTY
    static constexpr uint8_t promo_value_table[NUM_CHESS_PIECE_TYPES] = {0, 62, 60, 61, 63, 0, 0};

    constexpr uint8_t KILLER_MOVE_BONUS = 70;

    const Piece origin_piece = board.get_piece(move.square_from());
    const Piece end_piece = board.get_piece(move.square_to());

    const PieceType attacker = piece_to_pieceType(origin_piece);
    const PieceType victim = move.type() != MoveType::EN_PASSANT ? piece_to_pieceType(end_piece) : PieceType::PAWN;

    const Move killer_move_1 = KillerMoves::get_killer_1(ply);
    const Move killer_move_2 = KillerMoves::get_killer_2(ply);
    const uint8_t promo_piece_value = promo_value_table[static_cast<int>(move.promotion_piece())];

    const uint8_t promotion_bonus = move.type() == MoveType::PROMOTION ? promo_piece_value : 0;
    const uint8_t killer_bonus = (move == killer_move_1 || move == killer_move_2) ? KILLER_MOVE_BONUS : 0;

    // detect overflow
    assert(uint32_t(MVV_LVA_table[int(victim)][int(attacker)]) + uint32_t(promotion_bonus + killer_bonus) <= 255);

    return MVV_LVA_table[static_cast<int>(victim)][static_cast<int>(attacker)] + promotion_bonus + killer_bonus;
}
