/**
 * @file move_generator_magic_bitboards.cpp
 * @brief move generator services.
 *
 * chess move generator magic bitboards implementation. 
 * 
 * https://www.chessprogramming.org/Magic_Bitboards
 * 
 */

#include "move_generator.hpp"
#include "precomputed_move_data.hpp"
#include "move_generator_info.hpp"
#include "coordinates.hpp"


#include "move_generator.hpp"
#include "precomputed_move_data.hpp"
#include "move_generator_info.hpp"
#include "coordinates.hpp"

static void update_move_generator_info(MoveGeneratorInfo& moveGeneratorInfo);

static void update_danger_in_direction(Square piece_sq, Direction d, MoveGeneratorInfo& moveGeneratorInfo);

static void update_king_danger(Square king_sq, MoveGeneratorInfo& moveGeneratorInfo);
static void update_pawn_danger(Square pawn_sq, MoveGeneratorInfo& moveGeneratorInfo);
static void update_knight_danger(Square knight_sq, MoveGeneratorInfo& moveGeneratorInfo);
static void update_rook_danger(Square rook_sq, MoveGeneratorInfo& moveGeneratorInfo);
static void update_bishop_danger(Square bishop_sq, MoveGeneratorInfo& moveGeneratorInfo);
static void update_queen_danger(Square queen_sq, MoveGeneratorInfo& moveGeneratorInfo);

static void calculate_moves_in_direction(Square piece_sq, Direction d, MoveGeneratorInfo& moveGeneratorInfo);

static void calculate_castling_moves(Square king_sq, MoveGeneratorInfo& moveGeneratorInfo);
static void calculate_king_moves(Square king_sq, MoveGeneratorInfo& moveGeneratorInfo);
static void calculate_pawn_moves(Square pawn_sq, MoveGeneratorInfo& moveGeneratorInfo);
static void calculate_knight_moves(Square knight_sq, MoveGeneratorInfo& moveGeneratorInfo);
static void calculate_rook_moves(Square rook_sq, MoveGeneratorInfo& moveGeneratorInfo);
static void calculate_bishop_moves(Square bishop_sq, MoveGeneratorInfo& moveGeneratorInfo);
static void calculate_queen_moves(Square queen_sq, MoveGeneratorInfo& moveGeneratorInfo);

static bool en_passant_move_doesnt_allow_king_capture(Move enPassant_move, MoveGeneratorInfo& moveGeneratorInfo);

/**
 * @brief generate_legal_moves
 * 
 * Calculate all the legal moves in the chess position.
 * 
 * @param[out] moves move list.
 * @param[in] board chess position.
 * @param[out] isMate (optional) return true if the position is check mate.
 * @param[out] isStaleMate (optional) return true if the position is stalemate.
 * 
 */
void generate_legal_moves(MoveList& moves, const Board& board, bool* isMate, bool* isStaleMate)
{
    MoveGeneratorInfo moveGeneratorInfo(board, moves);

    update_move_generator_info(moveGeneratorInfo);

    const ChessColor side_to_move = moveGeneratorInfo.side_to_move;
    const uint8_t num_checkers = moveGeneratorInfo.number_of_checkers;

    if (num_checkers >= 2) {
        // when double check only king moves allowed
        calculate_king_moves(moveGeneratorInfo.side_to_move_king_square, moveGeneratorInfo);
        if (isMate) *isMate = (moves.size() == 0);
        if (isStaleMate) *isStaleMate = false;
        return;
    }

    for (Square square = Square::SQ_A1; square <= Square::SQ_H8; square++) {

        const Piece piece = board.get_piece(square);
        const ChessColor piece_color = get_color(piece);

        if (board.is_empty(square) || piece_color != side_to_move) continue;

        switch (piece_to_pieceType(piece)) {
        case PieceType::PAWN: calculate_pawn_moves(square, moveGeneratorInfo); break;
        case PieceType::KNIGHT: calculate_knight_moves(square, moveGeneratorInfo); break;
        case PieceType::KING: calculate_king_moves(square, moveGeneratorInfo); break;
        case PieceType::QUEEN: calculate_queen_moves(square, moveGeneratorInfo); break;
        case PieceType::ROOK: calculate_rook_moves(square, moveGeneratorInfo); break;
        case PieceType::BISHOP: calculate_bishop_moves(square, moveGeneratorInfo); break;
        default: break;
        }
    }

    if (isMate) *isMate = (moves.size() == 0) && (num_checkers > 0);
    if (isStaleMate) *isStaleMate = (moves.size() == 0) && (num_checkers == 0);
}

static void update_move_generator_info(MoveGeneratorInfo& moveGeneratorInfo)
{
    const Board& board = moveGeneratorInfo.board;
    const ChessColor side_to_move = moveGeneratorInfo.side_to_move;

    for (Square square = Square::SQ_A1; square <= Square::SQ_H8; square++) {

        const Piece piece = board.get_piece(square);
        const ChessColor piece_color = get_color(piece);

        if (board.is_empty(square) || piece_color == side_to_move) continue;

        switch (piece_to_pieceType(piece)) {
        case PieceType::PAWN: update_pawn_danger(square, moveGeneratorInfo); break;
        case PieceType::KNIGHT: update_knight_danger(square, moveGeneratorInfo); break;
        case PieceType::KING: update_king_danger(square, moveGeneratorInfo); break;
        case PieceType::QUEEN: update_queen_danger(square, moveGeneratorInfo); break;
        case PieceType::ROOK: update_rook_danger(square, moveGeneratorInfo); break;
        case PieceType::BISHOP: update_bishop_danger(square, moveGeneratorInfo); break;
        default: break;
        }
    }
}

static void update_danger_in_direction(Square piece_sq, Direction d, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(piece_sq.is_valid());

    const Board& board = moveGeneratorInfo.board;

    const uint64_t side_to_move_pieces_mask = moveGeneratorInfo.side_to_move_pieces_mask;
    const Square side_to_move_king_sq = moveGeneratorInfo.side_to_move_king_square;

    uint64_t piece_atacks = 0ULL;

    Square attack_sq = piece_sq;
    attack_sq.to_direction(d);

    // first calculate attack squares
    while (attack_sq.is_valid()) {

        piece_atacks |= attack_sq.mask();

        if (board.is_empty(attack_sq)) {
            attack_sq.to_direction(d);
        }
        else {
            break;   // collision detected
        }
    }

    //second king is in check procedure
    if (attack_sq.is_valid() && (attack_sq == side_to_move_king_sq)) {

        moveGeneratorInfo.new_checker_found(piece_sq, piece_atacks);

        // now continue calculating the king dangers
        attack_sq.to_direction(d);

        while (attack_sq.is_valid()) {

            piece_atacks |= attack_sq.mask();

            if (board.is_empty(attack_sq)) {
                attack_sq.to_direction(d);
            }
            else {
                break;   // collision detected
            }
        }
    }
    // third, calculate pinned pieces
    else if (attack_sq.is_valid() && (attack_sq.mask() & side_to_move_pieces_mask)) {
        const Square pseudo_pinned_piece_sq = attack_sq;

        attack_sq.to_direction(d);
        while (attack_sq.is_valid()) {
            // if we found the enemy king, then the pin is real
            if (attack_sq == side_to_move_king_sq) {
                moveGeneratorInfo.pinned_squares_mask |= pseudo_pinned_piece_sq.mask();
            }

            if (board.is_empty(attack_sq)) {
                attack_sq.to_direction(d);
            }
            else {
                break;   // collision detected
            }
        }
    }

    moveGeneratorInfo.king_danger_squares_mask |= piece_atacks;
}

static void update_king_danger(Square king_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(king_sq.is_valid());

    const uint64_t king_attacks_mask = PrecomputedMoveData::kingAttacks(king_sq);

    moveGeneratorInfo.king_danger_squares_mask |= king_attacks_mask;
}

static void update_pawn_danger(Square pawn_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(pawn_sq.is_valid());

    const ChessColor side_waiting = moveGeneratorInfo.side_waiting;

    const uint64_t pawn_attacks_mask = side_waiting == ChessColor::WHITE
        ? PrecomputedMoveData::whitePawnAttacks(pawn_sq)
        : PrecomputedMoveData::blackPawnAttacks(pawn_sq);

    moveGeneratorInfo.king_danger_squares_mask |= pawn_attacks_mask;

    // there is a check if the enemy king mask is inside the pawn attack mask
    const Square side_to_move_king_square = moveGeneratorInfo.side_to_move_king_square;

    if (pawn_attacks_mask & side_to_move_king_square.mask()) {
        moveGeneratorInfo.new_checker_found(pawn_sq, 0ULL);
    }
}

static void update_knight_danger(Square knight_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(knight_sq.is_valid());

    const uint64_t knight_attacks_mask = PrecomputedMoveData::knightAttacks(knight_sq);

    moveGeneratorInfo.king_danger_squares_mask |= knight_attacks_mask;

    // there is a check if the enemy king mask is inside the knight attack mask
    const Square side_to_move_king_square = moveGeneratorInfo.side_to_move_king_square;

    if (knight_attacks_mask & side_to_move_king_square.mask()) {
        moveGeneratorInfo.new_checker_found(knight_sq, 0ULL);
    }
}

static void update_rook_danger(Square rook_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(rook_sq.is_valid());

    const Direction dirs[4] = {NORTH, SOUTH, EAST, WEST};

    for (Direction dir : dirs) {
        update_danger_in_direction(rook_sq, dir, moveGeneratorInfo);
    }
}

static void update_bishop_danger(Square bishop_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(bishop_sq.is_valid());

    const Direction dirs[4] = {NORTH_EAST, NORTH_WEST, SOUTH_EAST, SOUTH_WEST};

    for (Direction dir : dirs) {
        update_danger_in_direction(bishop_sq, dir, moveGeneratorInfo);
    }
}

static void update_queen_danger(Square queen_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(queen_sq.is_valid());

    const Direction dirs[8] = {NORTH, SOUTH, EAST, WEST, NORTH_EAST, NORTH_WEST, SOUTH_EAST, SOUTH_WEST};

    for (Direction dir : dirs) {
        update_danger_in_direction(queen_sq, dir, moveGeneratorInfo);
    }
}

static void calculate_moves_in_direction(Square piece_sq, Direction d, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(piece_sq.is_valid());

    const Board& board = moveGeneratorInfo.board;
    MoveList& moves = moveGeneratorInfo.moves;

    const uint64_t side_to_move_pieces_mask = moveGeneratorInfo.side_to_move_pieces_mask;
    const uint64_t capture_mask = moveGeneratorInfo.capture_squares_mask;
    const uint64_t push_mask = moveGeneratorInfo.push_squares_mask;
    uint64_t attacks = 0ULL;

    Square attack_sq = piece_sq;
    attack_sq.to_direction(d);

    while (attack_sq.is_valid()) {
        // move is valid if square is not friendly piece
        attacks |= (side_to_move_pieces_mask & attack_sq.mask()) ? 0ULL : attack_sq.mask();

        if (board.is_empty(attack_sq)) {
            attack_sq.to_direction(d);
        }
        else {
            break;   // collision detected
        }
    }

    uint64_t legal_moves_mask = attacks & (capture_mask | push_mask);

    // if piece is pinned pawn can only moved in the direction of the pin
    if (moveGeneratorInfo.pinned_squares_mask & piece_sq.mask()) {
        const Square side_to_move_king_square = moveGeneratorInfo.side_to_move_king_square;
        const uint64_t pin_direction_mask = get_direction_mask(piece_sq, side_to_move_king_square);

        legal_moves_mask &= pin_direction_mask;
    }

    while (legal_moves_mask) {
        // pop least significant bit until is zero
        Square available_square = static_cast<Square>(pop_lsb(legal_moves_mask));
        moves.add(Move(piece_sq, available_square));
    }
}

static void calculate_castling_moves(Square king_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(king_sq.is_valid());

    const Board& board = moveGeneratorInfo.board;
    MoveList& moves = moveGeneratorInfo.moves;
    const ChessColor side_to_move = moveGeneratorInfo.side_to_move;

    const Square origin_castle_king_square = side_to_move == ChessColor::WHITE ? Square ::SQ_E1 : Square::SQ_E8;

    const Piece king_piece = side_to_move == ChessColor::WHITE ? Piece::W_KING : Piece::B_KING;
    const Piece rook_piece = side_to_move == ChessColor::WHITE ? Piece::W_ROOK : Piece::B_ROOK;

    if (king_sq != origin_castle_king_square || moveGeneratorInfo.number_of_checkers > 0 ||
        board.get_piece(origin_castle_king_square) != king_piece) {
        // castling not avaliable if in check
        return;
    }

    const Square king_side_rook_sq(origin_castle_king_square.row(), COL_H);
    const Square queen_side_rook_sq(origin_castle_king_square.row(), COL_A);

    const uint64_t king_danger_mask = moveGeneratorInfo.king_danger_squares_mask;
    const uint64_t empty_mask = ~board.get_bitboard_all();

    const bool king_side_castle_available =
        side_to_move == ChessColor::WHITE ? board.state().castle_king_white() : board.state().castle_king_black();

    const bool queen_side_castle_available =
        side_to_move == ChessColor::WHITE ? board.state().castle_queen_white() : board.state().castle_queen_black();

    if (king_side_castle_available && board.get_piece(king_side_rook_sq) == rook_piece) {

        const uint64_t in_between_squares_mask =
            Square(king_sq.row(), COL_F).mask() | Square(king_sq.row(), COL_G).mask();

        const bool available = (in_between_squares_mask & empty_mask & ~king_danger_mask) == in_between_squares_mask;

        if (available) {
            moves.add(side_to_move == ChessColor::WHITE ? Move::castle_white_king() : Move::castle_black_king());
        }
    }


    if (queen_side_castle_available && board.get_piece(queen_side_rook_sq) == rook_piece) {
        const uint64_t in_between_squares_mask =
            Square(king_sq.row(), COL_D).mask() | Square(king_sq.row(), COL_C).mask();

        const bool avaliable =
            ((in_between_squares_mask & empty_mask & ~king_danger_mask) == in_between_squares_mask) &&
            board.is_empty(Square(king_sq.row(), COL_B));

        if (avaliable) {
            moves.add(side_to_move == ChessColor::WHITE ? Move::castle_white_queen() : Move::castle_black_queen());
        }
    }
}

static void calculate_king_moves(Square king_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(king_sq.is_valid());

    MoveList& moves = moveGeneratorInfo.moves;

    const uint64_t king_attacks = PrecomputedMoveData::kingAttacks(king_sq);
    const uint64_t king_danger_mask = moveGeneratorInfo.king_danger_squares_mask;
    const uint64_t blockers_mask = moveGeneratorInfo.side_to_move_pieces_mask;

    uint64_t king_moves_mask = king_attacks & ~king_danger_mask & ~blockers_mask;

    while (king_moves_mask) {
        // pop least significant bit until is zero
        Square available_square = static_cast<Square>(pop_lsb(king_moves_mask));

        // each lsb is an square where the piece could move
        moves.add(Move(king_sq, available_square));
    }

    calculate_castling_moves(king_sq, moveGeneratorInfo);
}

static void calculate_pawn_moves(Square pawn_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(pawn_sq.is_valid());

    const Board& board = moveGeneratorInfo.board;
    MoveList& moves = moveGeneratorInfo.moves;

    const ChessColor side_to_move = moveGeneratorInfo.side_to_move;

    const Square en_passant_square = board.state().en_passant_square();

    const uint64_t pawn_attacks = side_to_move == ChessColor::WHITE ? PrecomputedMoveData::whitePawnAttacks(pawn_sq)
                                                                    : PrecomputedMoveData::blackPawnAttacks(pawn_sq);

    const uint64_t enemy_mask = moveGeneratorInfo.side_waiting_pieces_mask;
    const uint64_t capture_mask = moveGeneratorInfo.capture_squares_mask;
    const uint64_t push_mask = moveGeneratorInfo.push_squares_mask;

    // pawn captures
    uint64_t pawn_moves_mask = pawn_attacks & enemy_mask;

    // en passant capture, en passant mask is 0 if not available
    pawn_moves_mask |= pawn_attacks & en_passant_square.mask();

    // pawn push
    const Square pawn_push_sq = side_to_move == ChessColor::WHITE ? pawn_sq.north() : pawn_sq.south();

    const Square pawn_double_push_sq = side_to_move == ChessColor::WHITE ? pawn_push_sq.north() : pawn_push_sq.south();

    const bool double_push_available = pawn_sq.row() == moveGeneratorInfo.row_where_double_push_is_available;

    if (board.is_empty(pawn_push_sq)) {
        pawn_moves_mask |= pawn_push_sq.mask();

        if (double_push_available && board.is_empty(pawn_double_push_sq)) {
            pawn_moves_mask |= pawn_double_push_sq.mask();
        }
    }

    uint64_t en_passant_square_capturable_mask = 0ULL;
    if (pawn_attacks & en_passant_square.mask()) {
        // edge case pawn that gives check (is in capture mask) can be capture via enPassant
        const Square capturable_pawn(pawn_sq.row(), en_passant_square.col());
        if (capture_mask & capturable_pawn.mask()) {
            en_passant_square_capturable_mask |= en_passant_square.mask();
        }
    }

    // restrict moves if king is in check to capture and push mask
    pawn_moves_mask &= (push_mask | capture_mask | en_passant_square_capturable_mask);

    // if pawn is pinned pawn can only moved in the direction of the pin
    if (moveGeneratorInfo.pinned_squares_mask & pawn_sq.mask()) {
        const Square side_to_move_king_square = moveGeneratorInfo.side_to_move_king_square;
        const uint64_t pin_direction_mask = get_direction_mask(pawn_sq, side_to_move_king_square);

        pawn_moves_mask &= pin_direction_mask;
    }

    // check for enPassant
    if (pawn_sq.row() == moveGeneratorInfo.row_where_en_passant_is_available) {
        // en passant mask will be 0 if en passant not valid
        if (pawn_moves_mask & en_passant_square.mask()) {
            const Move enPassant_move(pawn_sq, en_passant_square, MoveType::EN_PASSANT);

            // check edge case where en passant is illegal because king and enemy slider are behind the pawns
            if (en_passant_move_doesnt_allow_king_capture(enPassant_move, moveGeneratorInfo)) {
                moves.add(enPassant_move);
            }
            // delete the en passant move from pawn_moves_mask
            pawn_moves_mask &= ~en_passant_square.mask();
        }
    }

    while (pawn_moves_mask) {
        // pop least significant bit until is zero
        Square available_square = static_cast<Square>(pop_lsb(pawn_moves_mask));
        if (pawn_sq.row() == moveGeneratorInfo.row_where_promotion_is_available) {

            moves.add(Move(pawn_sq, available_square, MoveType::PROMOTION, PieceType::KNIGHT));
            moves.add(Move(pawn_sq, available_square, MoveType::PROMOTION, PieceType::BISHOP));
            moves.add(Move(pawn_sq, available_square, MoveType::PROMOTION, PieceType::ROOK));
            moves.add(Move(pawn_sq, available_square, MoveType::PROMOTION, PieceType::QUEEN));
        }
        else {
            moves.add(Move(pawn_sq, available_square));
        }
    }
}

static void calculate_knight_moves(Square knight_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(knight_sq.is_valid());

    // knight cant move if it is pinned
    if (moveGeneratorInfo.pinned_squares_mask & knight_sq.mask()) {
        return;
    }

    MoveList& moves = moveGeneratorInfo.moves;

    const uint64_t knight_attacks = PrecomputedMoveData::knightAttacks(knight_sq);
    const uint64_t blockers_mask = moveGeneratorInfo.side_to_move_pieces_mask;
    const uint64_t capture_mask = moveGeneratorInfo.capture_squares_mask;
    const uint64_t push_mask = moveGeneratorInfo.push_squares_mask;

    uint64_t knight_moves_mask = knight_attacks & ~blockers_mask & (capture_mask | push_mask);

    while (knight_moves_mask) {
        // pop least significant bit until is zero
        Square available_square = static_cast<Square>(pop_lsb(knight_moves_mask));
        moves.add(Move(knight_sq, available_square));
    }
}


static void calculate_rook_moves(Square rook_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(rook_sq.is_valid());

    const uint64_t edges = ((ROW_1_MASK | ROW_8_MASK) & ~get_row_mask(rook_sq.row())) |
        ((COL_A_MASK | COL_H_MASK) & ~get_col_mask(rook_sq.col()));

    const uint64_t capture_mask = moveGeneratorInfo.capture_squares_mask;
    const uint64_t push_mask = moveGeneratorInfo.push_squares_mask;
    const uint64_t all_pieces_mask = moveGeneratorInfo.board.get_bitboard_all();
    const uint64_t blockers = all_pieces_mask & PrecomputedMoveData::rookAttacks(rook_sq) & ~edges;
    const uint64_t friendly_mask = moveGeneratorInfo.side_to_move_pieces_mask;

    uint64_t moves_mask = PrecomputedMoveData::rookMoves(rook_sq, blockers);

    moves_mask &= ~friendly_mask & (capture_mask | push_mask);

    // if piece is pinned, it can only moved in the direction of the pin
    if (moveGeneratorInfo.pinned_squares_mask & rook_sq.mask()) {
        const Square side_to_move_king_square = moveGeneratorInfo.side_to_move_king_square;
        const uint64_t pin_direction_mask = get_direction_mask(rook_sq, side_to_move_king_square);

        moves_mask &= pin_direction_mask;
    }

    while (moves_mask) {
        // pop least significant bit until is zero
        Square available_square = static_cast<Square>(pop_lsb(moves_mask));
        moveGeneratorInfo.moves.add(Move(rook_sq, available_square));
    }
}

static void calculate_bishop_moves(Square bishop_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(bishop_sq.is_valid());

    const uint64_t edges = ((ROW_1_MASK | ROW_8_MASK) & ~get_row_mask(bishop_sq.row())) |
        ((COL_A_MASK | COL_H_MASK) & ~get_col_mask(bishop_sq.col()));

    const uint64_t capture_mask = moveGeneratorInfo.capture_squares_mask;
    const uint64_t push_mask = moveGeneratorInfo.push_squares_mask;
    const uint64_t all_pieces_mask = moveGeneratorInfo.board.get_bitboard_all();
    const uint64_t blockers = all_pieces_mask & PrecomputedMoveData::bishopAttacks(bishop_sq) & ~edges;
    const uint64_t friendly_mask = moveGeneratorInfo.side_to_move_pieces_mask;

    uint64_t moves_mask = PrecomputedMoveData::bishopMoves(bishop_sq, blockers);

    moves_mask &= ~friendly_mask & (capture_mask | push_mask);

    // if piece is pinned, it can only moved in the direction of the pin
    if (moveGeneratorInfo.pinned_squares_mask & bishop_sq.mask()) {
        const Square side_to_move_king_square = moveGeneratorInfo.side_to_move_king_square;
        const uint64_t pin_direction_mask = get_direction_mask(bishop_sq, side_to_move_king_square);

        moves_mask &= pin_direction_mask;
    }

    while (moves_mask) {
        // pop least significant bit until is zero
        Square available_square = static_cast<Square>(pop_lsb(moves_mask));
        moveGeneratorInfo.moves.add(Move(bishop_sq, available_square));
    }
}

static void calculate_queen_moves(Square queen_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(queen_sq.is_valid());

    const uint64_t edges = ((ROW_1_MASK | ROW_8_MASK) & ~get_row_mask(queen_sq.row())) |
        ((COL_A_MASK | COL_H_MASK) & ~get_col_mask(queen_sq.col()));

    const uint64_t capture_mask = moveGeneratorInfo.capture_squares_mask;
    const uint64_t push_mask = moveGeneratorInfo.push_squares_mask;
    const uint64_t all_pieces_mask = moveGeneratorInfo.board.get_bitboard_all();
    const uint64_t orthogonal_blockers = all_pieces_mask & PrecomputedMoveData::rookAttacks(queen_sq) & ~edges;
    const uint64_t diagonal_blockers = all_pieces_mask & PrecomputedMoveData::bishopAttacks(queen_sq) & ~edges;
    const uint64_t blockers = orthogonal_blockers | diagonal_blockers;
    const uint64_t friendly_mask = moveGeneratorInfo.side_to_move_pieces_mask;

    uint64_t moves_mask = PrecomputedMoveData::queenMoves(queen_sq, orthogonal_blockers, diagonal_blockers);

    moves_mask &= ~friendly_mask & (capture_mask | push_mask);

    // if piece is pinned, it can only moved in the direction of the pin
    if (moveGeneratorInfo.pinned_squares_mask & queen_sq.mask()) {
        const Square side_to_move_king_square = moveGeneratorInfo.side_to_move_king_square;
        const uint64_t pin_direction_mask = get_direction_mask(queen_sq, side_to_move_king_square);

        moves_mask &= pin_direction_mask;
    }

    while (moves_mask) {
        // pop least significant bit until is zero
        Square available_square = static_cast<Square>(pop_lsb(moves_mask));
        moveGeneratorInfo.moves.add(Move(queen_sq, available_square));
    }
}

/**
 * @brief en_passant_move_doesnt_allow_king_capture
 * 
 * Calculate if en passant move is legal, it covers the edge case where 
 * the king and an enemy slider are on the same row as the enPassant pawns.
 * If en passant is produced the king could be capture by the slider if no blockers are in the middle.
 * 
 * the move is illegal if between the enemy slider and the king there are only the 
 * two pawns taking part in the en passant
 * 
 * @param[out] enPassant_move en passant move to check
 * @param[in] moveGeneratorInfo info about the position.
 * 
 * @return TRUE if move is legal
 *         FALSE if move is not legal
 */
static bool en_passant_move_doesnt_allow_king_capture(Move enPassant_move, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(enPassant_move.is_valid());
    assert(enPassant_move.type() == MoveType::EN_PASSANT);

    const Board& board = moveGeneratorInfo.board;
    const ChessColor enemy_color = moveGeneratorInfo.side_waiting;
    const Square friendly_king_square = moveGeneratorInfo.side_to_move_king_square;
    const Square origin_square = enPassant_move.square_from();
    const Row row = origin_square.row();

    if (friendly_king_square.row() != row) {
        return true;   // legal move, king is not in the same row as the en passant pawns
    }

    uint8_t pieces_between = 0;

    const Direction dir = get_direction(row, friendly_king_square.col(), row, origin_square.col());

    assert(dir == Direction::EAST || dir == Direction::WEST);

    Square aux_sq = friendly_king_square;

    aux_sq.to_direction(dir);

    while (aux_sq.is_valid()) {

        const Piece piece_found = board.get_piece(aux_sq);
        const PieceType pieceType_found = piece_to_pieceType(piece_found);

        if (piece_found != Piece::EMPTY) {

            if (is_slider(pieceType_found) && get_color(piece_found) == enemy_color) {
                return pieces_between != 2;   // illegal move if only two pieces between
            }
            else {
                if (++pieces_between > 2) {
                    return true;   // legal move if more that two pieces between
                }
            }
        }
        aux_sq.to_direction(dir);
    }

    return true;   // move legal
}