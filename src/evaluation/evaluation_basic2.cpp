/**
 * @file evaluation_basic2.cpp
 * @brief evaluation services implementation.
 *
 * chess position basic evaluation implementation. 
 * 
 */

#include <iostream>
#include "evaluation.hpp"
#include "move_list.hpp"
#include "move_generator.hpp"

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
int evaluate_piece(Piece piece, int row, int col, const Board& board) {
    int evaluation = 0;
    int sign = (piece == Piece::W_PAWN || piece == Piece::W_ROOK || piece == Piece::W_KNIGHT || piece == Piece::W_BISHOP || piece == Piece::W_QUEEN) ? 1 : -1;

    switch (piece) {
        case Piece::W_PAWN:
        case Piece::B_PAWN:
            evaluation += 1 * sign;
            // Pawn structure
            if (row > 0 && row < 7) {
                // Pawns in the second and seventh rank are better
                // depending on the color of the pawn
                if ((piece == Piece::W_PAWN && row == 6) || (piece == Piece::B_PAWN && row == 1)) {
                    evaluation += 1 * sign;
                }

                // Doubled pawns = -1 penalty for each side
                // https://en.wikipedia.org/wiki/Doubled_pawns
                if (Square(Row(row + sign), Col(col)).is_valid()) {
                    Piece front_piece = board.get_piece(Square(Row(row + sign), Col(col)));
                    if (front_piece == piece) {
                        evaluation += -1 * sign;
                    }
                }

                // Isolated pawns = -1 penalty for each side
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
                    evaluation += -1 * sign;
                }

                // TODO: Passed pawns
            }
            break;
        case Piece::W_ROOK:
        case Piece::B_ROOK:
            evaluation += 5 * sign;
            break;
        case Piece::W_KNIGHT:
        case Piece::B_KNIGHT:
            evaluation += 3 * sign;
            // Knights in the center of the board are better
            if (row >= 2 && row <= 5 && col >= 2 && col <= 5) {
                evaluation += 1 * sign;
            }
            break;
        case Piece::W_BISHOP:
        case Piece::B_BISHOP:
            evaluation += 3 * sign;
            // Bishops are better in open positions
            // Open position = no pawns in the center
            if (board.get_piece(Square::SQ_E4) == Piece::EMPTY ||
                board.get_piece(Square::SQ_E5) == Piece::EMPTY ||
                board.get_piece(Square::SQ_D4) == Piece::EMPTY ||
                board.get_piece(Square::SQ_D5) == Piece::EMPTY) {
                evaluation += 1 * sign;
            }
            break;
        case Piece::W_QUEEN:
        case Piece::B_QUEEN:
            evaluation += 9 * sign;
            break;
        case Piece::W_KING:
        case Piece::B_KING:
            // TODO:
            // - if the king is in a safe position 
            // during opening and middle game, 
            // - if the king has castled
            // - if the king is in the center of the board
            // during the endgame (king activity)
            break;
        default:
            break;
    }

    return evaluation;
}

/** 
 * @brief evaluate_legal_moves
 *
 * Evaluate list of legal moves taking into consideration the side to move.
 *  
 * @note None.
 * 
 * @param[in] legal_moves list of legal moves.
 * @param[in] side_to_move side to move.
 *
 * @returns
 *  - (0) if position is evaluated as equal.
 *  - (+) if position is evaluated as white is better.
 *  - (-) if position is evaluated as black is better.
 */
int evaluate_legal_moves(const MoveList& legal_moves, int side_to_move) {
    int evaluation = 0;

    for (int i = 0; i < legal_moves.size(); i++) {
        if (legal_moves[i].is_valid()) {
            switch (legal_moves[i].type()) {
                case MoveType::NORMAL:
                case MoveType::EN_PASSANT:
                    evaluation += 1 * side_to_move;
                    break;
                case MoveType::CASTLING:
                    evaluation += 5 * side_to_move;
                    break;
                case MoveType::PROMOTION:
                    evaluation += 9 * side_to_move;
                    break;
                default:
                    break;
            }
        }
    }

    return evaluation;
}

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
int evaluate_position(const Board& board)
{
    int evaluation = 0;

    // Evaluate pieces
    for (int i = 0; i < 64; i++) {
        int row = i / 8;
        int col = i % 8;
        Piece piece = board.get_piece(i);
        evaluation += evaluate_piece(piece, row, col, board);
    }

    // Evaluate legal moves for each side
    MoveList legal_moves;
    generate_legal_moves(legal_moves, board);
    int side_to_move = (board.state().side_to_move() == ChessColor::WHITE) ? 1 : -1;
    evaluation += evaluate_legal_moves(legal_moves, side_to_move);

    Board board_copy = board;
    side_to_move = -side_to_move;
    board_copy.state().set_side_to_move((side_to_move == 1) ? ChessColor::WHITE : ChessColor::BLACK);
    generate_legal_moves(legal_moves, board_copy);
    evaluation += evaluate_legal_moves(legal_moves, side_to_move);

    // Other evaluations to implement:
    // - Central control: if one color has more control
    // of the center.
    // - Development: if one color has more pieces
    // developed.
    
    return evaluation;
}
