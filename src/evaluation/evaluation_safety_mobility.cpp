/**
 * @file evaluation_dynamic.cpp
 * @brief evaluation services implementation.
 *
 * chess position dynamic evaluation implementation.
 * 
 * https://www.chessprogramming.org/Evaluation
 * https://www.chessprogramming.org/Simplified_Evaluation_Function
 * https://www.chessprogramming.org/Tapered_Eval
 * 
 */
#include "evaluation.hpp"
#include "move_list.hpp"
#include "move_generator.hpp"
#include "precomputed_eval_data.hpp"
#include "precomputed_move_data.hpp"
#include "bit_utilities.hpp"

static constexpr inline int calculate_middlegame_percentage(const Board& board);
static inline int mobility_piece_score(Square square, Piece piece, const Board& board);
template<ChessColor color>
static int king_shield(Square king_sq, const Board& board);
template<ChessColor color>
static int king_safety_penalization(Square king_sq, const Board& board);

/** 
 * @brief evaluate_position
 *
 * Evaluate chess position.
 *  
 * @note None.
 * 
 * @param[in] board board to evaluate.
 *
 * @returns
 *  - (0) if position is evaluated as equal.
 *  - (+) if position is evaluated as white is better.
 *  - (-) if position is evaluated as black is better.
 */
int evaluate_position(Board& board)
{
    board.update_attacks_bb();

    int middlegame_eval = 0;
    int endgame_eval = 0;

    const int middlegame_percentage = calculate_middlegame_percentage(board);
    const int endgame_percentage = 24 - calculate_middlegame_percentage(board);

    const Square white_king_sq = lsb(board.get_bitboard_piece(Piece::W_KING));
    const Square black_king_sq = lsb(board.get_bitboard_piece(Piece::B_KING));

    uint64_t pieces = board.get_bitboard_all();

    while (pieces) {

        const Square square(pop_lsb(pieces));
        const Piece piece = board.get_piece(square);
        const ChessColor color = get_color(piece);
        const int piece_raw_value = raw_value(piece);
        const int bonus_middlegame = PrecomputedEvalData::get_piece_square_table<PST_TYPE_MIDDLEGAME>(piece, square);
        const int bonus_endgame = PrecomputedEvalData::get_piece_square_table<PST_TYPE_ENDGAME>(piece, square);
        const int mobility = mobility_piece_score(square, piece, board);

        const int piece_value_middlegame = piece_raw_value + bonus_middlegame + 2 * mobility;
        const int piece_value_endgame = piece_raw_value + bonus_endgame + 2 * mobility;

        middlegame_eval += is_white(color) ? piece_value_middlegame : -piece_value_middlegame;
        endgame_eval += is_white(color) ? piece_value_endgame : -piece_value_endgame;
    }

    const int king_safety_penality_white = king_safety_penalization<ChessColor::WHITE>(white_king_sq, board);
    const int king_safety_penality_black = king_safety_penalization<ChessColor::BLACK>(black_king_sq, board);
    const int king_safety_penality = king_safety_penality_black - king_safety_penality_white;

    const int king_shield_bonus_white = king_shield<ChessColor::WHITE>(white_king_sq, board);
    const int king_shield_bonus_black = king_shield<ChessColor::BLACK>(black_king_sq, board);
    const int king_shield_bonus = king_shield_bonus_white - king_shield_bonus_black;

    middlegame_eval += king_shield_bonus + king_safety_penality;
    endgame_eval += king_safety_penality / 4;


    const int blended_eval = (middlegame_eval * middlegame_percentage + endgame_eval * endgame_percentage) / 24;

    return blended_eval;
}

static inline int mobility_piece_score(Square square, Piece piece, const Board& board)
{
    const PieceType piece_type = piece_to_pieceType(piece);

    if (piece_type == PieceType::KING || piece_type == PieceType::PAWN) {
        return 0;   // we dont care about king or pawn mobility
    }

    const ChessColor color = get_color(piece);
    const uint64_t blockers = board.get_bitboard_all();
    const uint64_t friendly_pieces = board.get_bitboard_color(color);
    const uint64_t enemy_pawn_attacks = board.get_attacks_bb(create_piece(PieceType::PAWN, opposite_color(color)));
    const uint64_t moves = PrecomputedMoveData::pieceMoves(square, piece, blockers);

    return number_of_1_bits(moves & ~(friendly_pieces | enemy_pawn_attacks));
}

/** 
 * @brief calculates the king shield bonus
 *
 * @note bonus between (0-100)
 * 
 * the shield pawns are the pawns in the three squares in front of the king
 * 
 * @tparam color side to evaluate
 * @param[in] king_sq king square in the position
 * @param[in] board chess position
 * 
 * @returns (int)
 *  - (0) if no shield pawns
 *  - (33) if 1 shield pawn
 *  - (66) if 2 shield pawns
 *  - (100) if 3 shield pawns
 */
template<ChessColor color>
static int king_shield(Square king_sq, const Board& board)
{
    assert(king_sq == Square(lsb(board.get_bitboard_piece(is_white(color) ? Piece::W_KING : Piece::B_KING))));

    static constexpr int shield_bonus[4] {0, 33, 66, 100};

    const uint64_t friendly_pawns = board.get_bitboard_piece(create_piece(PieceType::PAWN, color));

    const Row next_row = Row(is_white(color) ? king_sq.row() + 1 : king_sq.row() - 1);
    const uint64_t next_row_mask = is_valid_row(next_row) ? get_row_mask(next_row) : 0ULL;
    const uint64_t king_shield_zone = PrecomputedMoveData::kingAttacks(king_sq) & next_row_mask;

    const int number_of_shield_pawns = number_of_1_bits(king_shield_zone & friendly_pawns);
    assert(number_of_shield_pawns >= 0 && number_of_shield_pawns <= 3);

    return shield_bonus[number_of_shield_pawns];
}

template<ChessColor color>
static int king_safety_penalization(Square king_sq, const Board& board)
{
    // ATTACK_VALUE : PAWN(1), KNIGHT(2), BISHOP(2), ROOK(3), QUEEN(4), KING(1)
    assert(king_sq == Square(lsb(board.get_bitboard_piece(is_white(color) ? Piece::W_KING : Piece::B_KING))));

    constexpr ChessColor attack_color = opposite_color(color);
    constexpr Piece attack_knight = create_piece(PieceType::KNIGHT, attack_color);
    constexpr Piece attack_bishop = create_piece(PieceType::BISHOP, attack_color);
    constexpr Piece attack_rook = create_piece(PieceType::ROOK, attack_color);
    constexpr Piece attack_queen = create_piece(PieceType::QUEEN, attack_color);

    const uint64_t zone = PrecomputedEvalData::get_king_danger_zone(king_sq);   // 3x3 zone around king

    // 1 point penalty for each square attacked in the king zone
    int penalty = number_of_1_bits(zone & board.get_attacks_bb(attack_color));

    // extra 1 point penalty for each square attacked by minor piece
    penalty += number_of_1_bits(zone & ((board.get_attacks_bb(attack_bishop) | board.get_attacks_bb(attack_knight))));

    // extra 2 point penalty for each square attacked by rook
    penalty += 2 * number_of_1_bits(zone & (board.get_attacks_bb(attack_rook)));

    // extra 3 point penalty for each square attacked by queen
    penalty += 3 * number_of_1_bits(zone & (board.get_attacks_bb(attack_queen)));

    return PrecomputedEvalData::get_safety_table(penalty);
}

/**
 * @brief Calculate the middlegame percentage (0 = endgame, 24 = middlegame)
 * 
 * minor piece weight : 1
 * rook weight : 2
 * queen weight : 4
 * 
 * @param[in] board Chess position
 * @returns int
 *  - (24) for full middlegame (8 minor pieces, 4 rooks and 2 queens)
 *  - (0) for full endgame
 */
static constexpr inline int calculate_middlegame_percentage(const Board& board)
{
    const int queens = board.get_piece_counter(Piece::W_QUEEN) + board.get_piece_counter(Piece::B_QUEEN);

    const int rooks = board.get_piece_counter(Piece::W_ROOK) + board.get_piece_counter(Piece::B_ROOK);

    const int minor_pieces = board.get_piece_counter(Piece::W_KNIGHT) + board.get_piece_counter(Piece::B_KNIGHT) +
        board.get_piece_counter(Piece::W_BISHOP) + board.get_piece_counter(Piece::B_BISHOP);

    const int game_phase = minor_pieces + 2 * rooks + 4 * queens;

    return std::min(game_phase, 24);   // max gamephase is 24
}