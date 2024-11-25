/**
 * @file evaluation_basic.cpp
 * @brief evaluation services implementation.
 *
 * chess position basic evaluation implementation. 
 * 
 */

#include "evaluation.hpp"

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

    // Take into consideration that there should be 
    // minimum the two kings on the board and in a 
    // legal position.

    // Evaluate the position by counting the pieces
    // and giving a score to each piece:
    // - PAWN = ±1
    // - ROOK = ±5
    // - KNIGHT/BISHOP = ±3
    // - QUEEN = ±9

    for (int i = 0; i < 64; i++)
    {
        Piece piece = board.get_piece(i);
        switch (piece) {
            case Piece::W_PAWN:
                evaluation += 1;
                break;
            case Piece::B_PAWN:
                evaluation -= 1;
                break;
            case Piece::W_ROOK:
                evaluation += 5;
                break;
            case Piece::B_ROOK:
                evaluation -= 5;
                break;
            case Piece::W_KNIGHT:
            case Piece::W_BISHOP:
                evaluation += 3;
                break;
            case Piece::B_KNIGHT:
            case Piece::B_BISHOP:
                evaluation -= 3;
                break;
            case Piece::W_QUEEN:
                evaluation += 9;
                break;
            case Piece::B_QUEEN:
                evaluation -= 9;
                break;
            default:
            case Piece::EMPTY: 
            case Piece::W_KING: 
            case Piece::B_KING: 
            case Piece::NUM_PIECES:
                break;
        }
    }

    // Other evaluations to implement:
    // - Situational position: if it is an open
    // position, bishops are better than knights.
    // Otherwise, knights are better than bishops.
    // - Two bishops: if there are two bishops.
    // - Knights: if there are knights in the center.
    // - Pawn structure: doubled pawns, isolated pawns,
    // passed pawns, etc. During the endgame, passed
    // pawns are very important.
    // - Mobility: if one color has more mobility.
    // Rooks in open files, bishops in diagonals, etc.
    // - King: during opening and middle game,
    // if the king is in a safe position. If the king
    // has castled. During the endgame, if the king
    // is active.
    // - Central control: if one color has more control
    // of the center.
    // - Development: if one color has more pieces
    // developed.
    
    return evaluation;
}
