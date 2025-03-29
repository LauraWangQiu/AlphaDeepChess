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

static void update_king_danger(Square king_sq, MoveGeneratorInfo& moveGeneratorInfo);
static void update_pawn_danger(Square pawn_sq, MoveGeneratorInfo& moveGeneratorInfo);
static void update_knight_danger(Square knight_sq, MoveGeneratorInfo& moveGeneratorInfo);
static void update_rook_danger(Square rook_sq, MoveGeneratorInfo& moveGeneratorInfo);
static void update_bishop_danger(Square bishop_sq, MoveGeneratorInfo& moveGeneratorInfo);
static void update_queen_danger(Square queen_sq, MoveGeneratorInfo& moveGeneratorInfo);
static void update_pin_in_dir(Square king_sq, Direction d, MoveGeneratorInfo& moveGeneratorInfo);
static void update_pins_and_checks(Square king_sq, MoveGeneratorInfo& moveGeneratorInfo);

static void calculate_castling_moves(Square king_sq, MoveGeneratorInfo& moveGeneratorInfo);
template<MoveGeneratorType genType>
static void calculate_king_moves(Square king_sq, MoveGeneratorInfo& moveGeneratorInfo);
template<MoveGeneratorType genType>
static void calculate_pawn_moves(Square pawn_sq, MoveGeneratorInfo& moveGeneratorInfo);
template<MoveGeneratorType genType>
static void calculate_knight_moves(Square knight_sq, MoveGeneratorInfo& moveGeneratorInfo);
template<MoveGeneratorType genType>
static void calculate_rook_moves(Square rook_sq, MoveGeneratorInfo& moveGeneratorInfo);
template<MoveGeneratorType genType>
static void calculate_bishop_moves(Square bishop_sq, MoveGeneratorInfo& moveGeneratorInfo);
template<MoveGeneratorType genType>
static void calculate_queen_moves(Square queen_sq, MoveGeneratorInfo& moveGeneratorInfo);

static bool en_passant_move_doesnt_allow_king_capture(Move enPassant_move, MoveGeneratorInfo& moveGeneratorInfo);

/**
 * @brief generate_legal_moves
 * 
 * Calculate all the legal moves in the chess position.
 * 
 * @param[out] moves move list.
 * @param[in] board chess position.
 * @param[out] inCheck (optional) return true if the king is in check.

 * 
 */
template<MoveGeneratorType genType>
void generate_legal_moves(MoveList& moves, const Board& board, bool* inCheck)
{
    MoveGeneratorInfo moveGeneratorInfo(board, moves);

    update_move_generator_info(moveGeneratorInfo);

    const ChessColor side_to_move = board.state().side_to_move();
    const uint8_t num_checkers = moveGeneratorInfo.number_of_checkers;

    if (num_checkers >= 2) {
        // when double check only king moves allowed
        calculate_king_moves<genType>(moveGeneratorInfo.side_to_move_king_square, moveGeneratorInfo);
        if (inCheck) *inCheck = true;
        return;
    }
    uint64_t side_to_move_pieces = moveGeneratorInfo.side_to_move_pieces_mask;

    while (side_to_move_pieces) {
        const Square square(pop_lsb(side_to_move_pieces));
        const Piece piece = board.get_piece(square);
        const PieceType piece_type = piece_to_pieceType(piece);

        assert(!board.is_empty(square));
        assert(get_color(piece) == side_to_move);

        switch (piece_type) {
        case PieceType::PAWN: calculate_pawn_moves<genType>(square, moveGeneratorInfo); break;
        case PieceType::KNIGHT: calculate_knight_moves<genType>(square, moveGeneratorInfo); break;
        case PieceType::KING: calculate_king_moves<genType>(square, moveGeneratorInfo); break;
        case PieceType::QUEEN: calculate_queen_moves<genType>(square, moveGeneratorInfo); break;
        case PieceType::ROOK: calculate_rook_moves<genType>(square, moveGeneratorInfo); break;
        case PieceType::BISHOP: calculate_bishop_moves<genType>(square, moveGeneratorInfo); break;
        default: break;
        }
    }
    if (inCheck) *inCheck = (num_checkers > 0);
}

// Explicit template instantiations
template void generate_legal_moves<ALL_MOVES>(MoveList& moves, const Board& board, bool* inCheck);
template void generate_legal_moves<ONLY_CAPTURES>(MoveList& moves, const Board& board, bool* inCheck);

static void update_move_generator_info(MoveGeneratorInfo& moveGeneratorInfo)
{
    const Board& board = moveGeneratorInfo.board;
    const ChessColor side_waiting = moveGeneratorInfo.side_waiting;

    update_pins_and_checks(moveGeneratorInfo.side_to_move_king_square, moveGeneratorInfo);

    uint64_t side_waiting_pieces = moveGeneratorInfo.side_waiting_pieces_mask;

    while (side_waiting_pieces) {

        const Square square(pop_lsb(side_waiting_pieces));
        const Piece piece = board.get_piece(square);
        const PieceType piece_type = piece_to_pieceType(piece);

        assert(!board.is_empty(square));
        assert(get_color(piece) == side_waiting);

        switch (piece_type) {
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

static void update_pins_and_checks(Square king_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(king_sq.is_valid());
    const Direction directions[8] {Direction::NORTH, Direction::NORTH_EAST, Direction::EAST, Direction::SOUTH_EAST,
                                   Direction::SOUTH, Direction::SOUTH_WEST, Direction::WEST, Direction::NORTH_WEST};

    for (const Direction dir : directions) {
        update_pin_in_dir(king_sq, dir, moveGeneratorInfo);
    }
}

static void update_pin_in_dir(Square king_sq, Direction d, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(king_sq.is_valid());

    const Board& board = moveGeneratorInfo.board;
    const uint64_t side_to_move_pieces = moveGeneratorInfo.side_to_move_pieces_mask;
    const ChessColor side_waiting_color = moveGeneratorInfo.side_waiting;
    const uint64_t blockers = board.get_bitboard_all();
    const Piece enemy_rook_piece = create_piece(PieceType::ROOK, side_waiting_color);
    const Piece enemy_bishop_piece = create_piece(PieceType::BISHOP, side_waiting_color);
    const Piece enemy_queen_piece = create_piece(PieceType::QUEEN, side_waiting_color);

    const uint64_t possible_checkers_pieces = is_diagonal_direction(d)
        ? board.get_bitboard_piece(enemy_bishop_piece) | board.get_bitboard_piece(enemy_queen_piece)
        : board.get_bitboard_piece(enemy_rook_piece) | board.get_bitboard_piece(enemy_queen_piece);

    uint64_t push_mask = 0ULL;

    Square possible_pinned_piece_sq = Square::INVALID;
    Square sq = king_sq;
    sq.to_direction(d);

    while (sq.is_valid()) {

        if (sq.mask() & possible_checkers_pieces) {
            moveGeneratorInfo.new_checker_found(sq, push_mask);   // checker found
            // the king cannot move to the squares behind him in the same dir of the check
            // we put all the direction minus the checker sq as danger as a sufficient approximation
            moveGeneratorInfo.king_danger_squares_mask |= get_direction_mask(sq, king_sq) & ~sq.mask();
            break;
        }
        else if (sq.mask() & side_to_move_pieces) {
            possible_pinned_piece_sq = sq;   // possible pinned piece found
            break;
        }
        else if (sq.mask() & blockers) {
            break;   // piece that is not a checker or po pinned piece blocks path
        }
        else {
            push_mask |= sq.mask();
            sq.to_direction(d);
        }
    }

    if (!possible_pinned_piece_sq.is_valid()) {
        return;
    }

    sq.to_direction(d);
    while (sq.is_valid()) {
        if (sq.mask() & possible_checkers_pieces) {
            // pinned piece found
            moveGeneratorInfo.pinned_squares_mask |= possible_pinned_piece_sq.mask();
            break;
        }
        else if (sq.mask() & blockers) {
            break;   // piece that is not a checker blocks path
        }
        else {
            sq.to_direction(d);
        }
    }
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

    const uint64_t pawn_attacks_mask = PrecomputedMoveData::pawnAttacks(pawn_sq, side_waiting);

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

    const uint64_t blockers = moveGeneratorInfo.board.get_bitboard_all();

    const uint64_t moves_mask = PrecomputedMoveData::rookMoves(rook_sq, blockers);

    moveGeneratorInfo.king_danger_squares_mask |= moves_mask;
}

static void update_bishop_danger(Square bishop_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(bishop_sq.is_valid());

    const uint64_t blockers = moveGeneratorInfo.board.get_bitboard_all();

    const uint64_t moves_mask = PrecomputedMoveData::bishopMoves(bishop_sq, blockers);

    moveGeneratorInfo.king_danger_squares_mask |= moves_mask;
}

static void update_queen_danger(Square queen_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(queen_sq.is_valid());

    const uint64_t blockers = moveGeneratorInfo.board.get_bitboard_all();

    const uint64_t moves_mask = PrecomputedMoveData::queenMoves(queen_sq, blockers);

    moveGeneratorInfo.king_danger_squares_mask |= moves_mask;
}

static void calculate_castling_moves(Square king_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(king_sq.is_valid());

    const Board& board = moveGeneratorInfo.board;
    MoveList& moves = moveGeneratorInfo.moves;
    const ChessColor side_to_move = moveGeneratorInfo.side_to_move;

    const Square origin_castle_king_square = is_white(side_to_move) ? Square ::E1 : Square::E8;

    const Piece king_piece = is_white(side_to_move) ? Piece::W_KING : Piece::B_KING;
    const Piece rook_piece = is_white(side_to_move) ? Piece::W_ROOK : Piece::B_ROOK;

    if (king_sq != origin_castle_king_square || moveGeneratorInfo.number_of_checkers > 0 ||
        board.get_piece(origin_castle_king_square) != king_piece) {
        // castling not available if in check
        return;
    }

    const Square king_side_rook_sq(origin_castle_king_square.row(), COL_H);
    const Square queen_side_rook_sq(origin_castle_king_square.row(), COL_A);

    const uint64_t king_danger_mask = moveGeneratorInfo.king_danger_squares_mask;
    const uint64_t empty_mask = ~board.get_bitboard_all();

    const bool king_side_castle_available =
        is_white(side_to_move) ? board.state().castle_king_white() : board.state().castle_king_black();

    const bool queen_side_castle_available =
        is_white(side_to_move) ? board.state().castle_queen_white() : board.state().castle_queen_black();

    if (king_side_castle_available && board.get_piece(king_side_rook_sq) == rook_piece) {

        const uint64_t in_between_squares_mask =
            Square(king_sq.row(), COL_F).mask() | Square(king_sq.row(), COL_G).mask();

        const bool available = (in_between_squares_mask & empty_mask & ~king_danger_mask) == in_between_squares_mask;

        if (available) {
            moves.add(is_white(side_to_move) ? Move::castle_white_king() : Move::castle_black_king());
        }
    }


    if (queen_side_castle_available && board.get_piece(queen_side_rook_sq) == rook_piece) {
        const uint64_t in_between_squares_mask =
            Square(king_sq.row(), COL_D).mask() | Square(king_sq.row(), COL_C).mask();

        const bool available =
            ((in_between_squares_mask & empty_mask & ~king_danger_mask) == in_between_squares_mask) &&
            board.is_empty(Square(king_sq.row(), COL_B));

        if (available) {
            moves.add(is_white(side_to_move) ? Move::castle_white_queen() : Move::castle_black_queen());
        }
    }
}

template<MoveGeneratorType genType>
static void calculate_king_moves(Square king_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(king_sq.is_valid());

    MoveList& moves = moveGeneratorInfo.moves;

    const uint64_t king_attacks = PrecomputedMoveData::kingAttacks(king_sq);
    const uint64_t king_danger_mask = moveGeneratorInfo.king_danger_squares_mask;
    const uint64_t blockers_mask = moveGeneratorInfo.side_to_move_pieces_mask;

    uint64_t king_moves_mask = king_attacks & ~king_danger_mask & ~blockers_mask;

    if constexpr (genType == ONLY_CAPTURES) {
        const uint64_t enemy_mask = moveGeneratorInfo.side_waiting_pieces_mask;
        king_moves_mask &= enemy_mask;
    }

    while (king_moves_mask) {
        // pop least significant bit until is zero
        const Square available_square = static_cast<Square>(pop_lsb(king_moves_mask));

        // each lsb is an square where the piece could move
        moves.add(Move(king_sq, available_square));
    }

    if constexpr (genType == ALL_MOVES) {
        calculate_castling_moves(king_sq, moveGeneratorInfo);
    }
}

template<MoveGeneratorType genType>
static void calculate_pawn_moves(Square pawn_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(pawn_sq.is_valid());

    const Board& board = moveGeneratorInfo.board;
    MoveList& moves = moveGeneratorInfo.moves;

    const ChessColor side_to_move = moveGeneratorInfo.side_to_move;

    const Square en_passant_square = board.state().en_passant_square();

    const uint64_t pawn_attacks = PrecomputedMoveData::pawnAttacks(pawn_sq, side_to_move);

    const uint64_t enemy_mask = moveGeneratorInfo.side_waiting_pieces_mask;
    const uint64_t capture_mask = moveGeneratorInfo.capture_squares_mask;
    const uint64_t push_mask = moveGeneratorInfo.push_squares_mask;

    // pawn captures
    uint64_t pawn_moves_mask = pawn_attacks & enemy_mask;

    // en passant capture, en passant mask is 0 if not available
    pawn_moves_mask |= pawn_attacks & en_passant_square.mask();

    // pawn push
    if constexpr (genType == ALL_MOVES) {

        const Square pawn_push_sq = is_white(side_to_move) ? pawn_sq.north() : pawn_sq.south();

        const Square pawn_double_push_sq = is_white(side_to_move) ? pawn_push_sq.north() : pawn_push_sq.south();

        const bool double_push_available = pawn_sq.row() == moveGeneratorInfo.row_where_double_push_is_available;

        if (board.is_empty(pawn_push_sq)) {
            pawn_moves_mask |= pawn_push_sq.mask();

            if (double_push_available && board.is_empty(pawn_double_push_sq)) {
                pawn_moves_mask |= pawn_double_push_sq.mask();
            }
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

template<MoveGeneratorType genType>
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

    if constexpr (genType == ONLY_CAPTURES) {
        const uint64_t enemy_mask = moveGeneratorInfo.side_waiting_pieces_mask;
        knight_moves_mask &= enemy_mask;
    }

    while (knight_moves_mask) {
        // pop least significant bit until is zero
        const Square available_square = static_cast<Square>(pop_lsb(knight_moves_mask));
        moves.add(Move(knight_sq, available_square));
    }
}

template<MoveGeneratorType genType>
static void calculate_rook_moves(Square rook_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(rook_sq.is_valid());

    const uint64_t capture_mask = moveGeneratorInfo.capture_squares_mask;
    const uint64_t push_mask = moveGeneratorInfo.push_squares_mask;
    const uint64_t blockers = moveGeneratorInfo.board.get_bitboard_all();
    const uint64_t friendly_mask = moveGeneratorInfo.side_to_move_pieces_mask;

    uint64_t moves_mask = PrecomputedMoveData::rookMoves(rook_sq, blockers);

    moves_mask &= ~friendly_mask & (capture_mask | push_mask);

    if constexpr (genType == ONLY_CAPTURES) {
        const uint64_t enemy_mask = moveGeneratorInfo.side_waiting_pieces_mask;
        moves_mask &= enemy_mask;
    }

    // if piece is pinned, it can only moved in the direction of the pin
    if (moveGeneratorInfo.pinned_squares_mask & rook_sq.mask()) {
        const Square side_to_move_king_square = moveGeneratorInfo.side_to_move_king_square;
        const uint64_t pin_direction_mask = get_direction_mask(rook_sq, side_to_move_king_square);

        moves_mask &= pin_direction_mask;
    }

    while (moves_mask) {
        // pop least significant bit until is zero
        const Square available_square = static_cast<Square>(pop_lsb(moves_mask));
        moveGeneratorInfo.moves.add(Move(rook_sq, available_square));
    }
}

template<MoveGeneratorType genType>
static void calculate_bishop_moves(Square bishop_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(bishop_sq.is_valid());

    const uint64_t capture_mask = moveGeneratorInfo.capture_squares_mask;
    const uint64_t push_mask = moveGeneratorInfo.push_squares_mask;
    const uint64_t blockers = moveGeneratorInfo.board.get_bitboard_all();
    const uint64_t friendly_mask = moveGeneratorInfo.side_to_move_pieces_mask;

    uint64_t moves_mask = PrecomputedMoveData::bishopMoves(bishop_sq, blockers);

    moves_mask &= ~friendly_mask & (capture_mask | push_mask);

    if constexpr (genType == ONLY_CAPTURES) {
        const uint64_t enemy_mask = moveGeneratorInfo.side_waiting_pieces_mask;
        moves_mask &= enemy_mask;
    }

    // if piece is pinned, it can only moved in the direction of the pin
    if (moveGeneratorInfo.pinned_squares_mask & bishop_sq.mask()) {
        const Square side_to_move_king_square = moveGeneratorInfo.side_to_move_king_square;
        const uint64_t pin_direction_mask = get_direction_mask(bishop_sq, side_to_move_king_square);

        moves_mask &= pin_direction_mask;
    }

    while (moves_mask) {
        // pop least significant bit until is zero
        const Square available_square = static_cast<Square>(pop_lsb(moves_mask));
        moveGeneratorInfo.moves.add(Move(bishop_sq, available_square));
    }
}

template<MoveGeneratorType genType>
static void calculate_queen_moves(Square queen_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(queen_sq.is_valid());

    const uint64_t capture_mask = moveGeneratorInfo.capture_squares_mask;
    const uint64_t push_mask = moveGeneratorInfo.push_squares_mask;
    const uint64_t blockers = moveGeneratorInfo.board.get_bitboard_all();
    const uint64_t friendly_mask = moveGeneratorInfo.side_to_move_pieces_mask;

    uint64_t moves_mask = PrecomputedMoveData::queenMoves(queen_sq, blockers);

    moves_mask &= ~friendly_mask & (capture_mask | push_mask);

    if constexpr (genType == ONLY_CAPTURES) {
        const uint64_t enemy_mask = moveGeneratorInfo.side_waiting_pieces_mask;
        moves_mask &= enemy_mask;
    }

    // if piece is pinned, it can only moved in the direction of the pin
    if (moveGeneratorInfo.pinned_squares_mask & queen_sq.mask()) {
        const Square side_to_move_king_square = moveGeneratorInfo.side_to_move_king_square;
        const uint64_t pin_direction_mask = get_direction_mask(queen_sq, side_to_move_king_square);

        moves_mask &= pin_direction_mask;
    }

    while (moves_mask) {
        // pop least significant bit until is zero
        const Square available_square = static_cast<Square>(pop_lsb(moves_mask));
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