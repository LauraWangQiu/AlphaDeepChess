/**
 * @file move_generator_basic.cpp
 * @brief move generator services.
 *
 * chess move generator basic implementation. 
 * 
 * https://peterellisjones.com/posts/generating-legal-chess-moves-efficiently/
 * 
 */

#include "move_generator.hpp"
#include "precomputed_move_data.hpp"
#include "move_generator_info.hpp"
#include "bit_utilities.hpp"


static void calculate_dangers(const Board& board, MoveGeneratorInfo& MoveGeneratorInfo);
static void calculate_pawn_dangers(const Board& board, Square sq,
                                   MoveGeneratorInfo& MoveGeneratorInfo);
static void calculate_knight_dangers(const Board& board, Square sq,
                                     MoveGeneratorInfo& MoveGeneratorInfo);
static void calculate_king_dangers(const Board& board, Square sq,
                                   MoveGeneratorInfo& MoveGeneratorInfo);
static void calculate_queen_dangers(const Board& board, Square sq,
                                    MoveGeneratorInfo& MoveGeneratorInfo);
static void calculate_rook_dangers(const Board& board, Square sq,
                                   MoveGeneratorInfo& MoveGeneratorInfo);
static void calculate_bishop_dangers(const Board& board, Square sq,
                                     MoveGeneratorInfo& MoveGeneratorInfo);

static inline bool is_enemy_king(const Board& board, Square king_square, ChessColor friend_color);
static inline bool is_enemy_piece(const Board& board, Square piece_square, ChessColor friend_color);

static void calculate_danger_to_square(const Board& board, Square attacker_sq, Square defender_sq,
                                       ChessColor friend_color,
                                       MoveGeneratorInfo& MoveGeneratorInfo);

static void calculate_danger_to_direction(const Board& board, Direction dir, Square attacker_sq,
                                          ChessColor friend_color,
                                          MoveGeneratorInfo& MoveGeneratorInfo);

static void calculate_pawn_moves(const Board& board, Square sq,
                                 MoveGeneratorInfo& MoveGeneratorInfo, MoveList& moves);
static void calculate_knight_moves(const Board& board, Square sq,
                                   MoveGeneratorInfo& MoveGeneratorInfo, MoveList& moves);
static void calculate_king_moves(const Board& board, Square sq,
                                 MoveGeneratorInfo& MoveGeneratorInfo, MoveList& moves);
static void calculate_queen_moves(const Board& board, Square sq,
                                  MoveGeneratorInfo& MoveGeneratorInfo, MoveList& moves);
static void calculate_rook_moves(const Board& board, Square sq,
                                 MoveGeneratorInfo& MoveGeneratorInfo, MoveList& moves);
static void calculate_bishop_moves(const Board& board, Square sq,
                                   MoveGeneratorInfo& MoveGeneratorInfo, MoveList& moves);
static void calculate_castle_moves(const Board& board, Square king_sq, ChessColor friendly_color,
                                   MoveGeneratorInfo& MoveGeneratorInfo, MoveList& moves);

static void calculate_moves_to_direction(const Board& board, Direction dir, Square attacker_sq,
                                         ChessColor friend_color,
                                         MoveGeneratorInfo& MoveGeneratorInfo, MoveList& moves);

static bool is_valid_move_pinned(const Board& board, Square piece_sq, Square end_sq,
                                 MoveGeneratorInfo& MoveGeneratorInfo);

static bool is_valid_move_en_passant(const Board& board, Square piece_sq, Square end_sq,
                                     MoveGeneratorInfo& MoveGeneratorInfo);


static void update_move_generator_info(MoveGeneratorInfo& moveGeneratorInfo);

static void update_king_danger(Square king_sq, MoveGeneratorInfo& moveGeneratorInfo);
static void update_pawn_danger(Square pawn_sq, MoveGeneratorInfo& moveGeneratorInfo);
static void update_knight_danger(Square knight_sq, MoveGeneratorInfo& moveGeneratorInfo);
static void update_rook_danger(Square rook_sq, MoveGeneratorInfo& moveGeneratorInfo);
static void update_bishop_danger(Square bishop_sq, MoveGeneratorInfo& moveGeneratorInfo);
static void update_queen_danger(Square queen_sq, MoveGeneratorInfo& moveGeneratorInfo);

static void calculate_king_moves(Square king_sq, MoveGeneratorInfo& moveGeneratorInfo);
static void calculate_pawn_moves(Square pawn_sq, MoveGeneratorInfo& moveGeneratorInfo);
static void calculate_knight_moves(Square knight_sq, MoveGeneratorInfo& moveGeneratorInfo);
static void calculate_rook_moves(Square rook_sq, MoveGeneratorInfo& moveGeneratorInfo);
static void calculate_bishop_moves(Square bishop_sq, MoveGeneratorInfo& moveGeneratorInfo);
static void calculate_queen_moves(Square queen_sq, MoveGeneratorInfo& moveGeneratorInfo);

/**
 * @brief generate_legal_moves
 * 
 * Calculate all the legal moves in the chess position.
 * 
 * @param[out] moves move list.
 * @param[in] board chess position.
 * 
 */
void generate_legal_moves(MoveList& moves, const Board& board)
{
    MoveGeneratorInfo moveGeneratorInfo(board, moves);

    update_move_generator_info(moveGeneratorInfo);


    const ChessColor side_to_move = moveGeneratorInfo.side_to_move;

    if (moveGeneratorInfo.number_of_checkers >= 2) {
        // when double check only king moves allowed
        calculate_king_moves(moveGeneratorInfo.side_to_move_king_square, moveGeneratorInfo);
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
}

void update_move_generator_info(MoveGeneratorInfo& moveGeneratorInfo)
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

void update_king_danger(Square king_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(king_sq.is_valid());

    const Board& board = moveGeneratorInfo.board;
    const ChessColor side_to_move = moveGeneratorInfo.side_to_move;

    const uint64_t king_attacks_mask = PrecomputedMoveData::kingAttacks(king_sq);

    moveGeneratorInfo.king_danger_squares_mask |= king_attacks_mask;

    // the real attacks are the squares where piece attacks minus the blockers friendly pieces
    const uint64_t blockers_mask = moveGeneratorInfo.side_to_move_pieces_mask;
    const uint64_t king_real_attacks = king_attacks_mask & ~(blockers_mask);

    moveGeneratorInfo.attacked_squares_mask |= king_real_attacks;
}

void update_pawn_danger(Square pawn_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(pawn_sq.is_valid());

    const Board& board = moveGeneratorInfo.board;
    const ChessColor side_waiting = moveGeneratorInfo.side_waiting;

    const uint64_t pawn_attacks_mask = side_waiting == ChessColor::WHITE
        ? PrecomputedMoveData::whitePawnAttacks(pawn_sq)
        : PrecomputedMoveData::blackPawnAttacks(pawn_sq);

    moveGeneratorInfo.king_danger_squares_mask |= pawn_attacks_mask;

    // the real attacks are the squares where piece attacks minus the blockers friendly pieces
    const uint64_t blockers_mask = moveGeneratorInfo.side_to_move_pieces_mask;
    const uint64_t pawn_real_attacks = pawn_attacks_mask & ~(blockers_mask);

    moveGeneratorInfo.attacked_squares_mask |= pawn_real_attacks;

    // there is a check if the enemy king mask is inside the pawn attack mask
    const Square friendly_king_square = moveGeneratorInfo.side_to_move_king_square;

    if (pawn_attacks_mask & friendly_king_square.mask()) {
        moveGeneratorInfo.new_checker_found(pawn_sq);
    }
}

void update_knight_danger(Square knight_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(knight_sq.is_valid());

    const uint64_t knight_attacks_mask = PrecomputedMoveData::knightAttacks(knight_sq);

    moveGeneratorInfo.king_danger_squares_mask |= knight_attacks_mask;

    // the real attacks are the squares where piece attacks minus the blockers friendly pieces
    const uint64_t blockers_mask = moveGeneratorInfo.side_to_move_pieces_mask;
    const uint64_t knight_real_attacks = knight_attacks_mask & ~(blockers_mask);

    moveGeneratorInfo.attacked_squares_mask |= knight_real_attacks;

    // there is a check if the enemy king mask is inside the knight attack mask
    const Square friendly_king_square = moveGeneratorInfo.side_to_move_king_square;

    if (knight_attacks_mask & friendly_king_square.mask()) {
        moveGeneratorInfo.new_checker_found(knight_sq);
    }
}

void update_rook_danger(Square rook_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(rook_sq.is_valid());
}

void update_bishop_danger(Square bishop_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(bishop_sq.is_valid());
}

void update_queen_danger(Square queen_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(queen_sq.is_valid());

    update_rook_danger(queen_sq, moveGeneratorInfo);
    update_bishop_danger(queen_sq, moveGeneratorInfo);
}

static void calculate_king_moves(Square king_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(king_sq.is_valid());

    MoveList& moves = moveGeneratorInfo.moves;

    const uint64_t king_attacks = PrecomputedMoveData::kingAttacks(king_sq);
    const uint64_t attack_mask = moveGeneratorInfo.attacked_squares_mask;
    const uint64_t king_danger_mask = moveGeneratorInfo.king_danger_squares_mask;
    const uint64_t blockers_mask = moveGeneratorInfo.side_to_move_pieces_mask;
    const uint64_t capture_mask = moveGeneratorInfo.capture_squares_mask;

    uint64_t king_moves_mask = king_attacks & ~king_danger_mask & ~blockers_mask & capture_mask;

    while (king_moves_mask) {
        // pop least significant bit of king_moves_bitboard until is zero
        Square available_square = static_cast<Square>(pop_lsb(king_moves_mask));

        // each lsb is an square where the piece could move
        moves.add(Move(king_sq, available_square));
    }
}

static void calculate_pawn_moves(Square pawn_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(pawn_sq.is_valid());

    const Board& board = moveGeneratorInfo.board;
    MoveList& moves = moveGeneratorInfo.moves;

    const ChessColor side_to_move = moveGeneratorInfo.side_to_move;

    const uint64_t pawn_attacks = side_to_move == ChessColor::WHITE
        ? PrecomputedMoveData::whitePawnAttacks(pawn_sq)
        : PrecomputedMoveData::blackPawnAttacks(pawn_sq);

    const uint64_t enemy_mask = moveGeneratorInfo.side_waiting_pieces_mask;
    const uint64_t capture_mask = moveGeneratorInfo.capture_squares_mask;
    const uint64_t push_mask = moveGeneratorInfo.push_squares_mask;

    // pawn captures
    uint64_t pawn_captures_mask = pawn_attacks & enemy_mask & capture_mask & push_mask;

    while (pawn_captures_mask) {
        Square available_square = static_cast<Square>(pop_lsb(pawn_captures_mask));
        moves.add(Move(pawn_sq, available_square));
    }


    // pawn push
    const Square pawn_push_sq =
        side_to_move == ChessColor::WHITE ? pawn_sq.north() : pawn_sq.south();

    const Row pawn_row = pawn_sq.row();

    if (pawn_row == moveGeneratorInfo.row_where_double_push_is_avaliable) {

        const Square pawn_double_push_sq =
            side_to_move == ChessColor::WHITE ? pawn_push_sq.north() : pawn_push_sq.south();

        if (board.is_empty(pawn_push_sq)) {
            moves.add(Move(pawn_sq, pawn_push_sq));

            if (board.is_empty(pawn_double_push_sq)) {
                moves.add(Move(pawn_sq, pawn_double_push_sq));
            }
        }
    }
    else if (pawn_row == moveGeneratorInfo.row_where_promotion_is_available) {
        if (board.is_empty(pawn_push_sq)) {
            moves.add(Move(pawn_sq, pawn_push_sq, MoveType::PROMOTION, PieceType::KNIGHT));
            moves.add(Move(pawn_sq, pawn_push_sq, MoveType::PROMOTION, PieceType::BISHOP));
            moves.add(Move(pawn_sq, pawn_push_sq, MoveType::PROMOTION, PieceType::ROOK));
            moves.add(Move(pawn_sq, pawn_push_sq, MoveType::PROMOTION, PieceType::QUEEN));
        }
    }
    else {
        if (board.is_empty(pawn_push_sq)) {
            moves.add(Move(pawn_sq, pawn_push_sq));
        }
    }
}

static void calculate_knight_moves(Square knight_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(knight_sq.is_valid());

    MoveList& moves = moveGeneratorInfo.moves;

    const uint64_t knight_attacks = PrecomputedMoveData::knightAttacks(knight_sq);
    const uint64_t blockers_mask = moveGeneratorInfo.side_to_move_pieces_mask;
    const uint64_t capture_mask = moveGeneratorInfo.capture_squares_mask;
    const uint64_t push_mask = moveGeneratorInfo.push_squares_mask;

    uint64_t knight_moves_mask = knight_attacks & ~blockers_mask & capture_mask & push_mask;

    while (knight_moves_mask) {
        Square available_square = static_cast<Square>(pop_lsb(knight_moves_mask));
        moves.add(Move(knight_sq, available_square));
    }
}

static void calculate_rook_moves(Square rook_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(rook_sq.is_valid());
}

static void calculate_bishop_moves(Square bishop_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(bishop_sq.is_valid());
}

static void calculate_queen_moves(Square queen_sq, MoveGeneratorInfo& moveGeneratorInfo)
{
    assert(queen_sq.is_valid());
    calculate_rook_moves(queen_sq, moveGeneratorInfo);
    calculate_bishop_moves(queen_sq, moveGeneratorInfo);
}

/*
static void calculate_dangers(const Board& board, MoveGeneratorInfo& MoveGeneratorInfo)
{

    for (Square square = Square::SQ_A1; square <= Square::SQ_H8; square++) {

        const Piece piece = board.get_piece(square);
        const ChessColor color = get_color(piece);

        if (board.is_empty(square) || color == board.state().side_to_move()) continue;

        switch (piece_to_pieceType(piece)) {
        case PieceType::PAWN: calculate_pawn_dangers(board, square, MoveGeneratorInfo); break;
        case PieceType::KNIGHT: calculate_knight_dangers(board, square, MoveGeneratorInfo); break;
        case PieceType::KING: calculate_king_dangers(board, square, MoveGeneratorInfo); break;
        case PieceType::QUEEN: calculate_queen_dangers(board, square, MoveGeneratorInfo); break;
        case PieceType::ROOK: calculate_rook_dangers(board, square, MoveGeneratorInfo); break;
        case PieceType::BISHOP: calculate_bishop_dangers(board, square, MoveGeneratorInfo); break;
        default: break;
        }
    }
};

static void calculate_pawn_dangers(const Board& board, Square square, MoveGeneratorInfo& MoveGeneratorInfo)
{
    const ChessColor side_to_move = board.state().side_to_move();

    const Square defender_square_left =
        side_to_move == ChessColor::WHITE ? square.northEast() : square.southEast();

    const Square defender_square_right =
        side_to_move == ChessColor::WHITE ? square.northWest() : square.southWest();

    calculate_danger_to_square(board, square, defender_square_left, side_to_move, MoveGeneratorInfo);
    calculate_danger_to_square(board, square, defender_square_right, side_to_move, MoveGeneratorInfo);
}

static void calculate_knight_dangers(const Board& board, Square sq, MoveGeneratorInfo& MoveGeneratorInfo)
{
    const ChessColor side_to_move = board.state().side_to_move();

    calculate_danger_to_square(board, sq, sq.north().northWest(), side_to_move, MoveGeneratorInfo);
    calculate_danger_to_square(board, sq, sq.north().northEast(), side_to_move, MoveGeneratorInfo);
    calculate_danger_to_square(board, sq, sq.east().northEast(), side_to_move, MoveGeneratorInfo);
    calculate_danger_to_square(board, sq, sq.east().southEast(), side_to_move, MoveGeneratorInfo);
    calculate_danger_to_square(board, sq, sq.south().southEast(), side_to_move, MoveGeneratorInfo);
    calculate_danger_to_square(board, sq, sq.south().southWest(), side_to_move, MoveGeneratorInfo);
    calculate_danger_to_square(board, sq, sq.west().northWest(), side_to_move, MoveGeneratorInfo);
    calculate_danger_to_square(board, sq, sq.west().southWest(), side_to_move, MoveGeneratorInfo);
}

static void calculate_king_dangers(const Board& board, Square sq, MoveGeneratorInfo& MoveGeneratorInfo)
{
    const ChessColor side_to_move = board.state().side_to_move();

    MoveGeneratorInfo.set_king_square(sq, side_to_move);

    calculate_danger_to_square(board, sq, sq.north(), side_to_move, MoveGeneratorInfo);
    calculate_danger_to_square(board, sq, sq.northEast(), side_to_move, MoveGeneratorInfo);
    calculate_danger_to_square(board, sq, sq.east(), side_to_move, MoveGeneratorInfo);
    calculate_danger_to_square(board, sq, sq.southEast(), side_to_move, MoveGeneratorInfo);
    calculate_danger_to_square(board, sq, sq.south(), side_to_move, MoveGeneratorInfo);
    calculate_danger_to_square(board, sq, sq.southWest(), side_to_move, MoveGeneratorInfo);
    calculate_danger_to_square(board, sq, sq.west(), side_to_move, MoveGeneratorInfo);
    calculate_danger_to_square(board, sq, sq.northWest(), side_to_move, MoveGeneratorInfo);
}

static void calculate_queen_dangers(const Board& board, Square sq, MoveGeneratorInfo& MoveGeneratorInfo)
{
    calculate_rook_dangers(board, sq, MoveGeneratorInfo);
    calculate_bishop_dangers(board, sq, MoveGeneratorInfo);
}

static void calculate_rook_dangers(const Board& board, Square sq, MoveGeneratorInfo& MoveGeneratorInfo)
{
    const ChessColor side_to_move = board.state().side_to_move();

    calculate_danger_to_direction(board, Direction::NORTH, sq, side_to_move, MoveGeneratorInfo);
    calculate_danger_to_direction(board, Direction::SOUTH, sq, side_to_move, MoveGeneratorInfo);
    calculate_danger_to_direction(board, Direction::EAST, sq, side_to_move, MoveGeneratorInfo);
    calculate_danger_to_direction(board, Direction::WEST, sq, side_to_move, MoveGeneratorInfo);
}

static void calculate_bishop_dangers(const Board& board, Square sq, MoveGeneratorInfo& MoveGeneratorInfo)
{
    const ChessColor side_to_move = board.state().side_to_move();

    calculate_danger_to_direction(board, Direction::NORTH_EAST, sq, side_to_move, MoveGeneratorInfo);
    calculate_danger_to_direction(board, Direction::NORTH_WEST, sq, side_to_move, MoveGeneratorInfo);
    calculate_danger_to_direction(board, Direction::SOUTH_EAST, sq, side_to_move, MoveGeneratorInfo);
    calculate_danger_to_direction(board, Direction::SOUTH_WEST, sq, side_to_move, MoveGeneratorInfo);
}

static inline bool is_enemy_king(const Board& board, Square king_square, ChessColor friend_color)
{
    const Piece piece = board.get_piece(king_square);
    return piece_to_pieceType(piece) == PieceType::KING && get_color(piece) != friend_color;
}

static inline bool is_enemy_piece(const Board& board, Square piece_square, ChessColor friend_color)
{
    const Piece piece = board.get_piece(piece_square);
    return !board.is_empty(piece_square) && get_color(piece) != friend_color;
}

static void calculate_danger_to_square(const Board& board, Square attacker_sq, Square defender_sq,
                                       ChessColor friend_color, MoveGeneratorInfo& MoveGeneratorInfo)
{
    if (defender_sq.is_valid()) {
        MoveGeneratorInfo.set_attack_square(defender_sq, true);
        MoveGeneratorInfo.set_kingDanger_square(defender_sq, true);

        if (is_enemy_king(board, defender_sq, friend_color)) {
            MoveGeneratorInfo.add_checker(attacker_sq);
        }
    }
}

static void calculate_danger_to_direction(const Board& board, Direction dir, Square attacker_sq,
                                          ChessColor friend_color, MoveGeneratorInfo& MoveGeneratorInfo)
{
    bool attack = true;
    bool danger = true;
    bool pin = false;
    Square pinned_piece_square;

    Square defender_sq;
    Square ini_sq = attacker_sq;
    ini_sq.to_direction(dir);

    for (defender_sq = ini_sq; defender_sq.is_valid(); defender_sq.to_direction(dir)) {

        if (attack) MoveGeneratorInfo.set_attack_square(defender_sq, true);

        if (danger) MoveGeneratorInfo.set_kingDanger_square(defender_sq, true);

        if (!board.is_empty(defender_sq)) {
            if (is_enemy_king(board, defender_sq, friend_color)) {
                if (attack) {
                    MoveGeneratorInfo.add_checker(attacker_sq);
                }
            }
            else {
                danger = false;
            }

            if (pin && is_enemy_king(board, defender_sq, friend_color)) {
                pin = false;
                MoveGeneratorInfo.set_pinned_square(pinned_piece_square, true);
            }
            else if (pin && !board.is_empty(defender_sq)) {
                pin = false;
            }

            if (!pin && attack && is_enemy_piece(board, defender_sq, friend_color)) {
                pin = true;
                pinned_piece_square = defender_sq;
            }

            attack = false;
        }

        if (!attack && !danger && !pin) break;
    }
}

static void calculate_pawn_moves(const Board& board, Square sq, MoveGeneratorInfo& MoveGeneratorInfo,
                                 MoveList& moves)
{
    const ChessColor friend_color = board.state().side_to_move();

    const Direction dir = friend_color == ChessColor::WHITE ? Direction::NORTH : Direction::SOUTH;
    const Row start_row = friend_color == ChessColor::WHITE ? ROW_2 : ROW_7;
    const Row pre_promotion_row = friend_color == ChessColor::WHITE ? ROW_7 : ROW_2;
    const Row en_passant_row = friend_color == ChessColor::WHITE ? ROW_5 : ROW_4;

    Square push_sq = sq;
    push_sq.to_direction(dir);

    // pawn push
    if (push_sq.is_valid() && board.is_empty(push_sq) && push_sq.row() != pre_promotion_row) {
        if (MoveGeneratorInfo.is_square_in_push(push_sq) &&
            is_valid_move_pinned(board, sq, push_sq, MoveGeneratorInfo)) {

            if (sq.row() == pre_promotion_row) {
                moves.add(Move(sq, push_sq, MoveType::PROMOTION, PieceType::KNIGHT));
                moves.add(Move(sq, push_sq, MoveType::PROMOTION, PieceType::ROOK));
                moves.add(Move(sq, push_sq, MoveType::PROMOTION, PieceType::BISHOP));
                moves.add(Move(sq, push_sq, MoveType::PROMOTION, PieceType::QUEEN));
            }
            else {
                moves.add(Move(sq, push_sq));
            }
        }
    }

    // diagonal captures
    const Square capture_sq1 = push_sq.east();
    if (capture_sq1.is_valid() && is_enemy_piece(board, capture_sq1, friend_color) &&
        capture_sq1.row() != pre_promotion_row) {
        if (MoveGeneratorInfo.is_square_in_capture(capture_sq1) &&
            is_valid_move_pinned(board, sq, capture_sq1, MoveGeneratorInfo)) {

            if (sq.row() == pre_promotion_row) {
                moves.add(Move(sq, capture_sq1, MoveType::PROMOTION, PieceType::KNIGHT));
                moves.add(Move(sq, capture_sq1, MoveType::PROMOTION, PieceType::ROOK));
                moves.add(Move(sq, capture_sq1, MoveType::PROMOTION, PieceType::BISHOP));
                moves.add(Move(sq, capture_sq1, MoveType::PROMOTION, PieceType::QUEEN));
            }
            else {
                moves.add(Move(sq, capture_sq1));
            }
        }
    }

    const Square capture_sq2 = push_sq.west();
    if (capture_sq2.is_valid() && is_enemy_piece(board, capture_sq2, friend_color) &&
        capture_sq2.row() != pre_promotion_row) {
        if (MoveGeneratorInfo.is_square_in_capture(capture_sq2) &&
            is_valid_move_pinned(board, sq, capture_sq2, MoveGeneratorInfo))
            moves.add(Move(sq, capture_sq2));
    }


    if (sq.row() == start_row) {

        // double push
        Square double_push_sq = push_sq;
        double_push_sq.to_direction(dir);

        if (board.is_empty(push_sq) && board.is_empty(double_push_sq)) {
            if (MoveGeneratorInfo.is_square_in_push(double_push_sq) &&
                is_valid_move_pinned(board, sq, double_push_sq, MoveGeneratorInfo))
                moves.add(Move(sq, double_push_sq));
        }
    }

    if (sq.row() == en_passant_row) {
        if (board.state().en_passant_square() == capture_sq1) {
            if (MoveGeneratorInfo.is_square_in_capture(capture_sq1) &&
                is_valid_move_en_passant(board, sq, capture_sq1, MoveGeneratorInfo))
                moves.add(Move(sq, capture_sq1, MoveType::EN_PASSANT));
        }

        if (board.state().en_passant_square() == capture_sq2) {
            if (MoveGeneratorInfo.is_square_in_capture(capture_sq2) &&
                is_valid_move_en_passant(board, sq, capture_sq2, MoveGeneratorInfo))
                moves.add(Move(sq, capture_sq2, MoveType::EN_PASSANT));
        }
    }
}

static void calculate_knight_moves(const Board& board, Square sq, MoveGeneratorInfo& MoveGeneratorInfo,
                                   MoveList& moves)
{
    auto calculate_knight_move_to_square = [&](Square end_sq, ChessColor friend_color) {
        if (end_sq.is_valid() && is_valid_move_pinned(board, sq, end_sq, MoveGeneratorInfo)) {
            if (board.is_empty(end_sq) && MoveGeneratorInfo.is_square_in_push(end_sq)) {
                moves.add(Move(sq, end_sq));
            }
            else if (is_enemy_piece(board, end_sq, friend_color) &&
                     MoveGeneratorInfo.is_square_in_capture(end_sq)) {
                moves.add(Move(sq, end_sq));
            }
        }
    };

    const ChessColor friend_color = board.state().side_to_move();

    calculate_knight_move_to_square(sq.north(), friend_color);
    calculate_knight_move_to_square(sq.northEast(), friend_color);
    calculate_knight_move_to_square(sq.east(), friend_color);
    calculate_knight_move_to_square(sq.southEast(), friend_color);
    calculate_knight_move_to_square(sq.south(), friend_color);
    calculate_knight_move_to_square(sq.southWest(), friend_color);
    calculate_knight_move_to_square(sq.west(), friend_color);
    calculate_knight_move_to_square(sq.northWest(), friend_color);
}
static void calculate_king_moves(const Board& board, Square sq, MoveGeneratorInfo& MoveGeneratorInfo,
                                 MoveList& moves)
{
    auto calculate_king_move_to_square = [&](Square end_sq, ChessColor friend_color) {
        if (end_sq.is_valid() && !MoveGeneratorInfo.is_square_in_king_danger(end_sq)) {
            moves.add(Move(sq, end_sq));
        }
        else if (is_enemy_piece(board, end_sq, friend_color)) {
            moves.add(Move(sq, end_sq));
        }
    };

    const ChessColor friend_color = board.state().side_to_move();

    calculate_king_move_to_square(sq.north(), friend_color);
    calculate_king_move_to_square(sq.northEast(), friend_color);
    calculate_king_move_to_square(sq.east(), friend_color);
    calculate_king_move_to_square(sq.southEast(), friend_color);
    calculate_king_move_to_square(sq.south(), friend_color);
    calculate_king_move_to_square(sq.southWest(), friend_color);
    calculate_king_move_to_square(sq.west(), friend_color);
    calculate_king_move_to_square(sq.northWest(), friend_color);

    calculate_castle_moves(board, sq, friend_color, MoveGeneratorInfo, moves);
}
static void calculate_queen_moves(const Board& board, Square sq, MoveGeneratorInfo& MoveGeneratorInfo,
                                  MoveList& moves)
{
    calculate_rook_moves(board, sq, MoveGeneratorInfo, moves);
    calculate_bishop_moves(board, sq, MoveGeneratorInfo, moves);
}
static void calculate_rook_moves(const Board& board, Square sq, MoveGeneratorInfo& MoveGeneratorInfo,
                                 MoveList& moves)
{
    const ChessColor side_to_move = board.state().side_to_move();

    calculate_moves_to_direction(board, Direction::NORTH, sq, side_to_move, MoveGeneratorInfo, moves);
    calculate_moves_to_direction(board, Direction::SOUTH, sq, side_to_move, MoveGeneratorInfo, moves);
    calculate_moves_to_direction(board, Direction::EAST, sq, side_to_move, MoveGeneratorInfo, moves);
    calculate_moves_to_direction(board, Direction::WEST, sq, side_to_move, MoveGeneratorInfo, moves);
}
static void calculate_bishop_moves(const Board& board, Square sq, MoveGeneratorInfo& MoveGeneratorInfo,
                                   MoveList& moves)
{
    const ChessColor side_to_move = board.state().side_to_move();


    calculate_moves_to_direction(board, Direction::NORTH_EAST, sq, side_to_move, MoveGeneratorInfo,
                                 moves);
    calculate_moves_to_direction(board, Direction::NORTH_WEST, sq, side_to_move, MoveGeneratorInfo,
                                 moves);
    calculate_moves_to_direction(board, Direction::SOUTH_EAST, sq, side_to_move, MoveGeneratorInfo,
                                 moves);
    calculate_moves_to_direction(board, Direction::SOUTH_WEST, sq, side_to_move, MoveGeneratorInfo,
                                 moves);
}

static void calculate_castle_moves(const Board& board, Square king_sq, ChessColor friendly_color,
                                   MoveGeneratorInfo& MoveGeneratorInfo, MoveList& moves)
{
    if (MoveGeneratorInfo.get_checkers_number() > 0) {
        // when king is in check castle is not avaliable
        return;
    }

    const bool castle_king_avaliable = friendly_color == ChessColor::WHITE
        ? board.state().castle_king_white()
        : board.state().castle_king_black();

    const bool castle_queen_avaliable = friendly_color == ChessColor::WHITE
        ? board.state().castle_queen_white()
        : board.state().castle_queen_black();

    if (castle_king_avaliable) {

        // king side castle avaliable if there is no pieces between the king and the rook
        // also the king and the in between squares cant be attacked by enemy pieces
        const Square between_sq1 = king_sq.east();
        const Square between_sq2 = between_sq1.east();

        if (board.is_empty(between_sq1) && !MoveGeneratorInfo.is_square_in_king_danger(between_sq1) &&
            board.is_empty(between_sq2) && !MoveGeneratorInfo.is_square_in_king_danger(between_sq2)) {

            moves.add(friendly_color == ChessColor::WHITE ? Move::castle_white_king()
                                                          : Move::castle_black_king());
        }
    }

    if (castle_queen_avaliable) {

        // queen side castle avaliable if there is no pieces between the king and the rook
        // also the king and the in between squares that the king has to travel
        // cant be attacked by enemy pieces

        const Square between_sq1 = king_sq.west();
        const Square between_sq2 = between_sq1.west();
        const Square between_sq3 = between_sq2.west();

        // between_sq3 (COL_B) can be attacked, because king dont need to travel through that square
        if (board.is_empty(between_sq1) && !MoveGeneratorInfo.is_square_in_king_danger(between_sq1) &&
            board.is_empty(between_sq2) && !MoveGeneratorInfo.is_square_in_king_danger(between_sq2) &&
            board.is_empty(between_sq3)) {

            moves.add(friendly_color == ChessColor::WHITE ? Move::castle_white_queen()
                                                          : Move::castle_black_queen());
        }
    }
}

static void calculate_moves_to_direction(const Board& board, Direction dir, Square attacker_sq,
                                         ChessColor friend_color, MoveGeneratorInfo& MoveGeneratorInfo,
                                         MoveList& moves)
{

    Square defender_sq;
    Square ini_sq = attacker_sq;
    ini_sq.to_direction(dir);

    for (defender_sq = ini_sq; defender_sq.is_valid(); defender_sq.to_direction(dir)) {

        if (!defender_sq.is_valid() ||
            !is_valid_move_pinned(board, attacker_sq, defender_sq, MoveGeneratorInfo)) {
            break;
        }

        if (board.is_empty(defender_sq)) {
            if (MoveGeneratorInfo.is_square_in_push(defender_sq)) {
                moves.add(Move(attacker_sq, defender_sq));
            }
        }
        else {
            if (is_enemy_piece(board, defender_sq, friend_color) &&
                MoveGeneratorInfo.is_square_in_capture(defender_sq)) {
                moves.add(Move(attacker_sq, defender_sq));
            }
            break;
        }
    }
}

static bool is_valid_move_pinned(const Board& board, Square piece_sq, Square end_sq,
                                 MoveGeneratorInfo& MoveGeneratorInfo)
{

    if (!MoveGeneratorInfo.is_square_pinned(piece_sq)) return true;   // piece is not pinned

    bool is_legal = false;

    const ChessColor color = board.state().side_to_move();
    Square king_sq = MoveGeneratorInfo.get_king_square(color);

    if (piece_sq.row() == king_sq.row())   // horizontal pin
    {
        is_legal = end_sq.row() == piece_sq.row();
    }
    else if (piece_sq.col() == king_sq.col())   // vertical pin
    {
        is_legal = end_sq.col() == piece_sq.col();
    }
    else   // diagonal pin
    {

        // Calculate the slope of the line
        int numerator = (int)(king_sq.col() - piece_sq.col());
        numerator *= (int)(end_sq.row() - piece_sq.row());
        int denominator = (king_sq.row() - piece_sq.row());

        // Check if the y-coordinate of the destPos on the line matches the y-coordinate of the destPos
        is_legal = (numerator % denominator == 0) &&
            (numerator == (end_sq.col() - piece_sq.col()) * denominator);
    }

    return is_legal;
}

static bool is_valid_move_en_passant(const Board& board, Square piece_sq, Square end_sq,
                                     MoveGeneratorInfo& MoveGeneratorInfo)
{
    if (!is_valid_move_pinned(board, piece_sq, end_sq, MoveGeneratorInfo)) {
        return false;
    }

    bool is_legal = true;

    const ChessColor color = board.state().side_to_move();
    Square king_sq = MoveGeneratorInfo.get_king_square(color);

    Square en_passant_sq = board.state().en_passant_square();

    if (king_sq.row() == en_passant_sq.row()) {
        Direction dir = king_sq.col() < piece_sq.col() ? Direction::WEST : Direction::EAST;

        Square aux_square = king_sq;
        aux_square.to_direction(dir);

        while (aux_square.is_valid()) {
            if (aux_square.col() != end_sq.col() && aux_square.col() != piece_sq.col()) {
                if (!board.is_empty(aux_square)) {
                    const PieceType enemy = piece_to_pieceType(board.get_piece(aux_square));
                    is_legal = !(is_enemy_piece(board, aux_square, color) &&
                                 (enemy == PieceType::QUEEN || enemy == PieceType::ROOK));

                    break;
                }
            }
            aux_square.to_direction(dir);
        }
    }

    return is_legal;
}

static void calculate_checkers(const Board& board, MoveGeneratorInfo& MoveGeneratorInfo)
{
    if (MoveGeneratorInfo.get_checkers_number() != 1) {
        // if there is no check or there is double check there is nothing to calculate
        return;
    }
    const ChessColor friend_color = board.state().side_to_move();

    const Square king_sq = MoveGeneratorInfo.get_king_square(friend_color);
    const Square checker_sq = MoveGeneratorInfo.get_checker_square();

    // insert checker in capture mask, when in check it is legal to capture the checker.
    MoveGeneratorInfo.set_capture_square(checker_sq, true);

    // insert in between squares in push mask, when in check it is legal to block check with a piece.
    if (is_slider(piece_to_pieceType(board.get_piece(checker_sq)))) {

        const Direction direction =
            get_direction(king_sq.row(), king_sq.col(), checker_sq.row(), checker_sq.col());

        Square aux_sq = king_sq;
        aux_sq.to_direction(direction);

        while (aux_sq != checker_sq) {
            MoveGeneratorInfo.set_push_square(aux_sq, true);
            aux_sq.to_direction(direction);
        }
    }
}

// return the mask of the squares that the knight could move
static uint64_t get_knight_moves_mask(Square knight_square)
{

    
} 
*/