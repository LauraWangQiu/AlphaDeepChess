/**
 * @file evaluation_basic2.cpp
 * @brief evaluation services implementation.
 *
 * chess position basic evaluation implementation.
 * https://www.chessprogramming.org/Evaluation
 * 
 */
#include <iostream>
#include "evaluation.hpp"
#include "move_list.hpp"
#include "move_generator.hpp"

// https://www.chessprogramming.org/Evaluation

// Based in Larry Kaufman's basic values
// https://www.chessprogramming.org/Point_Value
#define PAWN_VALUE 1.f
#define ROOK_VALUE 5.25f
#define KNIGHT_VALUE 3.5f
#define BISHOP_VALUE 3.5f
#define QUEEN_VALUE 10.f

#define DOUBLED_PAWNS_PENALTY_VALUE -0.5
#define BLOCKED_PAWNS_PENALTY_VALUE -0.5
#define ISOLATED_PAWNS_PENALTY_VALUE -0.5

#define PAWN_SHIELD_FOR_KING_SAFETY_PENALTY_VALUE 0.1

#define MOBILITY_VALUE 0.1

#define OPENING_MATERIAL 40
#define MIDDLEGAME_MATERIAL 20

GamePhase determine_game_phase(const Board& board);
float evaluate_piece(Piece piece, int row, int col, const Board& board, const GamePhase& game_phase);
float evaluate_legal_moves(const MoveList& legal_moves);

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
float evaluate_position(const Board& board)
{
    float evaluation = 0;

    // Evaluate game phase
    GamePhase game_phase = determine_game_phase(board);

    // Evaluate pieces
    for (int i = 0; i < 64; i++) {
        int row = i / 8;
        int col = i % 8;
        Piece piece = board.get_piece(i);
        if (piece == Piece::EMPTY) {
            continue;
        }
        evaluation += evaluate_piece(piece, row, col, board, game_phase);
    }

    // Evaluate legal moves for each side
    ChessColor side_to_move = board.state().side_to_move();
    ChessColor opponent = (side_to_move == ChessColor::WHITE) ? ChessColor::BLACK : ChessColor::WHITE;
    int sign = (side_to_move == ChessColor::WHITE) ? 1 : -1;
    int sign_opponent = -sign;
    Board board_copy = board;
    board_copy.set_side_to_move(opponent);
    MoveList legal_moves;

    generate_legal_moves(legal_moves, board);
    evaluation += evaluate_legal_moves(legal_moves) * sign;

    generate_legal_moves(legal_moves, board_copy);
    evaluation += evaluate_legal_moves(legal_moves) * sign_opponent;

    return evaluation;
}

/** 
 * @brief determine_game_phase
 *
 * Evaluate the game phase based in the current position on board.
 *  
 * @note None.
 * 
 * @param[in] board board to evaluate.
 *
 * @returns
 *  - (0) if position is evaluated as OPENING.
 *  - (1) if position is evaluated as MIDDLEGAME.
 *  - (2) if position is evaluated as ENDGAME.
 */
GamePhase determine_game_phase(const Board& board) {
    int total_material = 0;
    int queens = 0;

    for (int i = 0; i < 64; i++) {
        Piece piece = board.get_piece(i);
        PieceType type = piece_to_pieceType(piece);
        switch (type) {
            case PieceType::PAWN:
                total_material += PAWN_VALUE;
                break;
            case PieceType::ROOK:
                total_material += ROOK_VALUE;
                break;
            case PieceType::KNIGHT:
                total_material += KNIGHT_VALUE;
                break;
            case PieceType::BISHOP:
                total_material += BISHOP_VALUE;
                break;
            case PieceType::QUEEN:
                total_material += QUEEN_VALUE;
                ++queens;
                break;
            default:
                break;
        }
    }

    if (total_material > OPENING_MATERIAL) {
        return GamePhase::OPENING;
    } else if (total_material > MIDDLEGAME_MATERIAL || queens > 0) {
        return GamePhase::MIDDLEGAME;
    } else {
        return GamePhase::ENDGAME;
    }
}

/** 
 * @brief evaluate_legal_moves
 *
 * Evaluate list of legal moves taking into consideration the side to move.
 * 
 * @note None.
 * 
 * @param[in] piece piece to evaluate.
 * @param[in] row row of the piece.
 * @param[in] col column of the piece.
 * @param[in] board board to evaluate.
 *
 * @returns
 *  - (0) if position is evaluated as equal.
 *  - (+) if position is evaluated as white is better.
 *  - (-) if position is evaluated as black is better.
 */
float evaluate_piece(Piece piece, int row, int col, const Board& board, const GamePhase& game_phase) {
    float evaluation = 0;
    PieceType type = piece_to_pieceType(piece);
    ChessColor color = get_color(piece);
    int sign = (color == ChessColor::WHITE) ? 1 : -1;

    switch (type) {
        case PieceType::PAWN:
            // Pawn base material value
            evaluation += PAWN_VALUE;
            // Pawn structure
            if (row > 0 && row < 7) {
                // Doubled pawns = -0.5 penalty for each side
                // Blocked pawns = -0.5 penalty for each side
                // https://en.wikipedia.org/wiki/Doubled_pawns
                if (Square(Row(row + sign), Col(col)).is_valid()) {
                    Piece front_piece = board.get_piece(Square(Row(row + sign), Col(col)));
                    if (front_piece != Piece::EMPTY) {
                        if (front_piece == piece) {
                            evaluation += DOUBLED_PAWNS_PENALTY_VALUE;
                        } else if (get_color(front_piece) != color) {
                            evaluation += BLOCKED_PAWNS_PENALTY_VALUE;
                        }
                    }
                }

                // Isolated pawns = -0.5 penalty for each side
                // https://en.wikipedia.org/wiki/Isolated_pawn
                // Check if there are pawns in the adjacent files/columns
                bool isolated = true;
                int i = 1;
                int left_col = col - 1;
                int right_col = col + 1;
                while (i < 7) {
                    if (Square(Row(i), Col(left_col)).is_valid()) {
                        Piece left_piece = board.get_piece(Square(Row(i), Col(left_col)));
                        if (left_piece == piece) {
                            isolated = false;
                            break;
                        }
                    }
                    if (Square(Row(i), Col(right_col)).is_valid()) {
                        Piece right_piece = board.get_piece(Square(Row(i), Col(right_col)));
                        if (right_piece == piece) {
                            isolated = false;
                            break;
                        }
                    }
                    ++i;
                }
                if (isolated) {
                    evaluation += ISOLATED_PAWNS_PENALTY_VALUE;
                }

                // TODO:
                // Passed pawns = 1 point for each side
                // https://en.wikipedia.org/wiki/Passed_pawn
            }
            break;
        case PieceType::ROOK:
            // Rook base material value
            evaluation += ROOK_VALUE;
            break;
        case PieceType::KNIGHT:
            // Knight base material value
            evaluation += KNIGHT_VALUE;
            break;
        case PieceType::BISHOP:
            // Bishop base material value
            evaluation += BISHOP_VALUE;
            break;
        case PieceType::QUEEN:
            // Queen base material value
            evaluation += QUEEN_VALUE;
            break;
        case PieceType::KING:
            switch (game_phase) {
                case GamePhase::OPENING:
                case GamePhase::MIDDLEGAME:
                    // King safety
                    // https://www.chessprogramming.org/King_Safety
                    // If king has castled
                    if ((color == ChessColor::WHITE && board.state().has_castled_white()) || (color == ChessColor::BLACK && board.state().has_castled_black())) {
                        // Pawn shield
                        // Get only the 3 same color pawns in the front row of the king after castled
                        // If there are not pawns, then it is a penalty
                        // (NOT taking into consideration of long castle)
                        for (int i = 0; i < 3; i++) {
                            if (Square(Row(row + sign), Col(col - 1 + i)).is_valid()) {
                                Piece possiblePawn = board.get_piece(Square(Row(row + sign), Col(col - 1 + i)));
                                // If there is no piece or the piece is not the same color
                                // or the piece is not a pawn, give penalty
                                if (possiblePawn == Piece::EMPTY || get_color(possiblePawn) != color || piece_to_pieceType(possiblePawn) != PieceType::PAWN) {
                                    evaluation += PAWN_SHIELD_FOR_KING_SAFETY_PENALTY_VALUE;
                                }
                            }
                        }

                        // TODO: Pawn storm
                        // TODO: King Tropism
                        // TODO: Attacking King Zone
                        // TODO: Square Control
                        // TODO: Attack Units
                    }
                    break;
                case GamePhase::ENDGAME:
                    // TODO: King activity
                    break;
            }
            break;
        default:
            break;
    }

    return evaluation * sign;
}

/** 
 * @brief evaluate_legal_moves
 *
 * Evaluate list of legal moves taking into consideration the side to move.
 *  
 * @note None.
 * 
 * @param[in] legal_moves list of legal moves.
 *
 * @returns
 *  - (0) if position is evaluated as equal.
 *  - (+) if position is evaluated as white is better.
 *  - (-) if position is evaluated as black is better.
 */
float evaluate_legal_moves(const MoveList& legal_moves) {
    float evaluation = 0;
    for (int i = 0; i < legal_moves.size(); i++) {
        if (legal_moves[i].is_valid()) {
            evaluation += MOBILITY_VALUE;
        }
    }
    return evaluation;
}
