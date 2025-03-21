/**
 * @file evaluation_basic.cpp
 * @brief evaluation services implementation.
 *
 * chess position basic evaluation implementation.
 * https://www.chessprogramming.org/Evaluation
 * https://www.chessprogramming.org/Simplified_Evaluation_Function
 * 
 */
#include "evaluation.hpp"
#include "move_list.hpp"
#include "move_generator.hpp"
#include "precomputed_data.hpp"

#define MATE_VALUE -10000

// Pieces values
// 1. Avoid exchanging one minor piece for three pawns
// B > 3P
// N > 3P
// 2. Encourage the engine to have the bishop pair
// B > N > 3P
// 3. Avoid exchanging of two minor pieces for a rook and a pawn
// B + N > R + P
// B + N = R + 1.5P
// Q + P = 2R

#define DOUBLED_PAWNS_PENALTY_VALUE -50
#define BLOCKED_PAWNS_PENALTY_VALUE -50
#define ISOLATED_PAWNS_PENALTY_VALUE -50

#define PAWN_SHIELD_FOR_KING_SAFETY_PENALTY_VALUE -10
#define PAWN_STORM_FOR_KING_SAFETY_PENALTY_VALUE -15

#define ROOK_CONSTANT_ATTACKING_KING_ZONE_PENALTY_VALUE -40
#define KNIGHT_CONSTANT_ATTACKING_KING_ZONE_PENALTY_VALUE -20
#define BISHOP_CONSTANT_ATTACKING_KING_ZONE_PENALTY_VALUE -20
#define QUEEN_CONSTANT_ATTACKING_KING_ZONE_PENALTY_VALUE -80

#define ROOK_CONSTANT_ATTACKING_UNIT_VALUE 3
#define KNIGHT_CONSTANT_ATTACKING_UNIT_VALUE 2
#define BISHOP_CONSTANT_ATTACKING_UNIT_VALUE 2
#define QUEEN_CONSTANT_ATTACKING_UNIT_VALUE 5

#define MOBILITY_VALUE 10

#define OPENING_MATERIAL 40
#define MIDDLEGAME_MATERIAL 20

int evaluate_legal_moves(const Board& board);
GamePhase determine_game_phase(const Board& board);
int evaluate_piece(const Board& board, const GamePhase& game_phase, Piece piece, int row, int col);
int evaluate_pawn(const Board& board, Piece piece, int pawn_row, int pawn_col, ChessColor color);
int evaluate_rook(Piece piece, int rook_row, int rook_col, ChessColor color);
int evaluate_knight(Piece piece, int knight_row, int knight_col, ChessColor color);
int evaluate_bishop(Piece piece, int bishop_row, int bishop_col, ChessColor color);
int evaluate_queen(Piece piece, int queen_row, int queen_col, ChessColor color);
int evaluate_king(const GamePhase& game_phase, const Board& board, int row, int col, ChessColor color);
uint64_t get_king_zone_mask(int king_row, int king_col, ChessColor color);
int evaluate_king_safety_middle_game(const Board& board, int king_row, int king_col, ChessColor color);
int evaluate_king_activity_end_game(int king_row, int king_col, ChessColor color);

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

    // Evaluate legal moves for each side
    ChessColor side_to_move = board.state().side_to_move();
    ChessColor opponent = (side_to_move == ChessColor::WHITE) ? ChessColor::BLACK : ChessColor::WHITE;
    int sign = (side_to_move == ChessColor::WHITE) ? 1 : -1;
    int sign_opponent = -sign;
    //Board board_copy = board;
    //board_copy.set_side_to_move(opponent);

    //evaluation += evaluate_legal_moves(board) * sign;
    //evaluation += evaluate_legal_moves(board_copy) * sign_opponent;

    // Evaluate game phase
    GamePhase game_phase = determine_game_phase(board);

    // Evaluate pieces
    for (int i = 0; i < 64; i++) {
        int row = i / 8;
        int col = i % 8;
        Piece piece = board.get_piece(i);
        if (piece != Piece::EMPTY) {
            evaluation += evaluate_piece(board, game_phase, piece, row, col);
        }
    }

    // TODO: Draw Evaluation (Insufficient material)
    // https://www.chessprogramming.org/Draw_Evaluation
    // https://www.chessprogramming.org/Interior_Node_Recognizer
    // Related to transposition table

    return evaluation;
}

/** 
 * @brief evaluate_legal_moves
 *
 * Evaluate list of legal moves taking into consideration the side to move.
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
int evaluate_legal_moves(const Board& board)
{
    // TODO , right know is not worth it calling generate_legal_moves again to calculate mobility

    /*MoveList legal_moves;
    bool incheck, isMate, isStaleMate;
    generate_legal_moves<ALL_MOVES>(legal_moves, board, &isMate, &isStaleMate);
    if (isMate || isStaleMate) {
        return (isMate) ? MATE_VALUE : 0;
    }

    int evaluation = 0;

    for (int i = 0; i < legal_moves.size(); i++) {
        if (legal_moves[i].is_valid()) {
            evaluation += MOBILITY_VALUE;
        }
    }

    return evaluation;*/
    return 0;
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
GamePhase determine_game_phase(const Board& board)
{
    int total_material = 0;
    int queens = 0;

    for (int i = 0; i < 64; i++) {
        Piece piece = board.get_piece(i);
        total_material += raw_value(piece);
    }

    if (total_material > OPENING_MATERIAL) {
        return GamePhase::OPENING;
    }
    else if (total_material > MIDDLEGAME_MATERIAL || queens > 0) {
        return GamePhase::MIDDLEGAME;
    }
    else {
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
int evaluate_piece(const Board& board, const GamePhase& game_phase, Piece piece, int row, int col)
{
    int evaluation = 0;
    PieceType type = piece_to_pieceType(piece);
    ChessColor color = get_color(piece);
    int sign = (color == ChessColor::WHITE) ? 1 : -1;

    // Further evaluation based on piece type
    // https://www.chessprogramming.org/Evaluation_of_Pieces
    switch (type) {
    case PieceType::PAWN: evaluation += evaluate_pawn(board, piece, row, col, color); break;
    case PieceType::ROOK: evaluation += evaluate_rook(piece, row, col, color); break;
    case PieceType::KNIGHT: evaluation += evaluate_knight(piece, row, col, color); break;
    case PieceType::BISHOP: evaluation += evaluate_bishop(piece, row, col, color); break;
    case PieceType::QUEEN: evaluation += evaluate_queen(piece, row, col, color); break;
    case PieceType::KING: evaluation += evaluate_king(game_phase, board, row, col, color); break;
    default: break;
    }

    return evaluation * sign;
}

/** 
 * @brief evaluate_pawn
 *
 * Get the pawn evaluation.
 * 
 * @note None.
 * 
 * @param[in] board board to evaluate.
 * @param[in] piece piece to evaluate.
 * @param[in] pawn_row row of the pawn.
 * @param[in] pawn_col column of the pawn.
 * @param[in] color color of the pawn.
 *
 * @returns evaluation of the pawn.
 */
int evaluate_pawn(const Board& board, Piece piece, int pawn_row, int pawn_col, ChessColor color)
{
    // Pawn base material value
    int evaluation = raw_value(piece);
    int sign = (color == ChessColor::WHITE) ? 1 : -1;

    // Pawn structure
    // TODO: Pawn Hash Table for better performance
    // https://www.chessprogramming.org/Pawn_Hash_Table
    // related to magic bitboards
    // TODO: Others
    // https://www.chessprogramming.org/Pawn_Structure
    if (pawn_row > 0 && pawn_row < 7) {
        // Doubled pawns = -50 penalty for each side
        // Blocked pawns = -50 penalty for each side
        // https://en.wikipedia.org/wiki/Doubled_pawns
        if (Square(Row(pawn_row + sign), Col(pawn_col)).is_valid()) {
            Piece front_piece = board.get_piece(Square(Row(pawn_row + sign), Col(pawn_col)));
            if (front_piece != Piece::EMPTY) {
                if (front_piece == piece) {
                    evaluation += DOUBLED_PAWNS_PENALTY_VALUE;
                }
                else if (get_color(front_piece) != color) {
                    evaluation += BLOCKED_PAWNS_PENALTY_VALUE;
                }
            }
        }

        // Isolated pawns = -50 penalty for each side
        // https://en.wikipedia.org/wiki/Isolated_pawn
        // Check if there are pawns in the adjacent files/columns
        bool isolated = true;
        int i = 1;
        int left_col = pawn_col - 1;
        int right_col = pawn_col + 1;
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

        // TODO: Passed pawns
        // https://en.wikipedia.org/wiki/Passed_pawn
        // https://www.chessprogramming.org/Blockade_of_Stop
    }

    evaluation += PrecomputedData::get_pawn_piece_square_table(Square(Row(pawn_row), Col(pawn_col)), color);

    return evaluation;
}

/** 
 * @brief evaluate_rook
 *
 * Get the rook evaluation.
 * 
 * @note None.
 * 
 * @param[in] rook_row row of the rook.
 * @param[in] rook_col column of the rook.
 *
 * @returns evaluation of the rook.
 */
int evaluate_rook(Piece piece, int rook_row, int rook_col, ChessColor color)
{
    // Rook base material value
    int evaluation = raw_value(piece);

    evaluation += PrecomputedData::get_rook_piece_square_table(Square(Row(rook_row), Col(rook_col)), color);

    return evaluation;
}

/** 
 * @brief evaluate_knight
 *
 * Get the knight evaluation.
 * 
 * @note None.
 * 
 * @param[in] knight_row row of the knight.
 * @param[in] knight_col column of the knight.
 *
 * @returns evaluation of the knight.
 */
int evaluate_knight(Piece piece, int knight_row, int knight_col, ChessColor color)
{
    // Knight base material value
    int evaluation = raw_value(piece);

    evaluation += PrecomputedData::get_knight_piece_square_table(Square(Row(knight_row), Col(knight_col)), color);

    return evaluation;
}

/** 
 * @brief evaluate_bishop
 *
 * Get the bishop evaluation.
 * 
 * @note None.
 * 
 * @param[in] bishop_row row of the bishop.
 * @param[in] bishop_col column of the bishop.
 *
 * @returns evaluation of the bishop.
 */
int evaluate_bishop(Piece piece, int bishop_row, int bishop_col, ChessColor color)
{
    // Bishop base material value
    int evaluation = raw_value(piece);

    evaluation += PrecomputedData::get_bishop_piece_square_table(Square(Row(bishop_row), Col(bishop_col)), color);

    return evaluation;
}

/** 
 * @brief evaluate_queen
 *
 * Get the queen evaluation.
 * 
 * @note None.
 * 
 * @param[in] queen_row row of the queen.
 * @param[in] queen_col column of the queen.
 *
 * @returns evaluation of the queen.
 */
int evaluate_queen(Piece piece, int queen_row, int queen_col, ChessColor color)
{
    // Queen base material value
    int evaluation = raw_value(piece);

    evaluation += PrecomputedData::get_queen_piece_square_table(Square(Row(queen_row), Col(queen_col)), color);

    return evaluation;
}

/** 
 * @brief evaluate_king
 *
 * Get the king evaluation.
 * 
 * @note None.
 * 
 * @param[in] game_phase game phase to evaluate.
 * @param[in] board board to evaluate.
 * @param[in] king_row row of the king.
 * @param[in] king_col column of the king.
 * @param[in] color color of the king.
 *
 * @returns set of squares that are in the king zone.
 */
int evaluate_king(const GamePhase& game_phase, const Board& board, int row, int col, ChessColor color)
{
    int evaluation = 0;

    switch (game_phase) {
    case GamePhase::OPENING:
    case GamePhase::MIDDLEGAME: evaluation += evaluate_king_safety_middle_game(board, row, col, color); break;
    case GamePhase::ENDGAME: evaluation += evaluate_king_activity_end_game(row, col, color); break;
    }

    return evaluation;
}

/** 
 * @brief get_king_zone
 *
 * Get the king zone of a given king square.
 * 
 * @note None.
 * 
 * @param[in] king_row row of the king.
 * @param[in] king_col column of the king.
 * @param[in] color color of the king.
 *
 * @returns set of squares that are in the king zone.
 */
uint64_t get_king_zone_mask(int king_row, int king_col, ChessColor color)
{
    uint64_t king_zone_mask = 0;
    int sign = (color == ChessColor::WHITE) ? 1 : -1;

    // Adjascent squares + three more forward squares facing enemy position
    // (adjascent facing enemy position + two more forward = three move forward)
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i != 0 || j != 0) {
                Square adj_square = Square(Row(king_row + i), Col(king_col + j));
                if (adj_square.is_valid()) {
                    king_zone_mask |= adj_square.mask();
                }
            }
        }
    }
    for (int i = 2; i <= 3; i++) {
        for (int j = -1; j <= 1; j++) {
            Square forward_square = Square(Row(king_row + i * sign), Col(king_col + j));
            if (forward_square.is_valid()) {
                king_zone_mask |= forward_square.mask();
            }
        }
    }

    return king_zone_mask;
}

/** 
 * @brief evaluate_king_safety_middle_game
 *
 * Evaluate the king safety of a given king square and color during the middle game.
 * 
 * @note None.
 * 
 * @param[in] board board to evaluate.
 * @param[in] king_row row of the king.
 * @param[in] king_col column of the king.
 * @param[in] color color of the king.
 *
 * @returns evaluation of the king safety.
 */
int evaluate_king_safety_middle_game(const Board& board, int king_row, int king_col, ChessColor color)
{
    int evaluation = 0;
    int sign = (color == ChessColor::WHITE) ? 1 : -1;

    // King safety
    // https://www.chessprogramming.org/King_Safety

    // If king has castled
    if ((color == ChessColor::WHITE && board.state().has_castled_white()) ||
        (color == ChessColor::BLACK && board.state().has_castled_black())) {
        for (int i = 0; i < 3; i++) {
            if (Square(Row(king_row + sign), Col(king_col - 1 + i)).is_valid()) {
                Piece possiblePawn = board.get_piece(Square(Row(king_row + sign), Col(king_col - 1 + i)));
                // Pawn shield
                // Get only the 3 same color pawns in the front row of the king after castled
                // If there are not pawns, then it is a penalty
                // (NOT taking into consideration of long castle)
                // If there is no piece or the piece is not the same color
                // or the piece is not a pawn, give penalty
                if (possiblePawn == Piece::EMPTY || get_color(possiblePawn) != color ||
                    piece_to_pieceType(possiblePawn) != PieceType::PAWN) {
                    evaluation += PAWN_SHIELD_FOR_KING_SAFETY_PENALTY_VALUE;
                }

                // Pawn storm
                // If the enemy pawns are near to the king, there might be a threat of
                // opening a file, even if the pawn shield is intact
                // Iterate columns
                for (int j = 1; j < 7; j++) {
                    if (Square(Row(king_row + j * sign), Col(king_col - 1 + i)).is_valid()) {
                        Piece possiblePawn = board.get_piece(Square(Row(king_row + j * sign), Col(king_col - 1 + i)));
                        if (possiblePawn != Piece::EMPTY && get_color(possiblePawn) != color &&
                            piece_to_pieceType(possiblePawn) == PieceType::PAWN) {
                            // Smaller the distance, bigger the penalty
                            float distance = PrecomputedData::get_distance_chebyshev(
                                Square(Row(king_row), Col(king_col)),
                                Square(Row(king_row + j * sign), Col(king_col - 1 + i)));
                            evaluation += PAWN_STORM_FOR_KING_SAFETY_PENALTY_VALUE * 1 / distance;
                        }
                    }
                }
            }
        }
    }

    // TODO: King Tropism
    // TODO: Attacking King Zone
    // TODO: Attack Units
    // uint64_t king_zone_mask = get_king_zone_mask(king_row, king_col, color);
    // const int attack_weights[] = { 0, 0, 50, 75, 88, 94, 97, 99 };
    // int attackingPiecesCount = 0;
    // int attackUnits = 0;

    // For each piece on the board, check if it attacks the king zone
    // considering if the piece is an enemy piece and counting the
    // number of squares that are attacked in the king zone. Then,
    // multiply it by X_CONSTANT_ATTACKING_KING_ZONE_PENALTY_VALUE.
    // Also, check the distance of the piece to the king. In case of
    // a rook or queen in a open row or open column, give a penalty

    // valueOfAttack += numberOfSquaresAttacked * X_CONSTANT_ATTACKING_KING_ZONE_PENALTY_VALUE
    // attackingPiecesCount++
    // attackUnits += X_CONSTANT_ATTACKING_UNIT_VALUE

    // Finally, evaluate the king safety as:
    // evaluation += valueOfAttack * attack_weights[attackingPiecesCount];
    // evaluation += PrecomputedData::get_safety_table(attackUnits);

    // TODO: Square Control
    // ...

    // Scaling? Tend to exchange pieces if king is in danger to reduce the attack
    // ...

    evaluation += PrecomputedData::get_king_middle_game_piece_square_table(Square(Row(king_row), Col(king_col)), color);

    return evaluation;
}

/** 
 * @brief evaluate_king_activity_end_game
 *
 * Evaluate the king safety of a given king square and color during the end game.
 * 
 * @note None.
 * 
 * @param[in] king_row row of the king.
 * @param[in] king_col column of the king.
 * @param[in] color color of the king.
 *
 * @returns set of squares that are in the king zone.
 */
int evaluate_king_activity_end_game(int king_row, int king_col, ChessColor color)
{
    int evaluation = 0;

    evaluation += PrecomputedData::get_king_end_game_piece_square_table(Square(Row(king_row), Col(king_col)), color);

    return evaluation;
}
