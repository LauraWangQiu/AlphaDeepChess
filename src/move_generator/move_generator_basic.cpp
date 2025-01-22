/**
 * @file move_generator_basic.hpp
 * @brief move generator services.
 *
 * chess move generator basic implementation. 
 * 
 */

#include "move_generator.hpp"

struct MoveGenInfo
{
public:
    inline bool is_square_attacked(Square square) { return attack_mask & square.mask(); }
    inline bool is_square_pinned(Square square) { return pin_mask & square.mask(); }
    inline bool is_square_in_king_danger(Square square) { return kingDanger_mask & square.mask(); }
    inline bool is_square_in_push(Square square) { return push_mask & square.mask(); }
    inline bool is_square_in_capture(Square square) { return push_mask & square.mask(); }
    inline Square get_checker_square() { return checker_square; }
    inline uint8_t get_checkers_number() { return checkers_number; }

    inline Square get_king_square(ChessColor color)
    {
        return color == ChessColor::WHITE ? king_white_square : king_black_square;
    }

    inline void set_attack_square(Square square, bool value)
    {
        attack_mask &= ~square.mask();
        attack_mask |= static_cast<uint64_t>(value) << square;
    }

    inline void set_pinned_square(Square square, bool value)
    {
        pin_mask &= ~square.mask();
        pin_mask |= static_cast<uint64_t>(value) << square;
    }

    inline void set_kingDanger_square(Square square, bool value)
    {
        kingDanger_mask &= ~square.mask();
        kingDanger_mask |= static_cast<uint64_t>(value) << square;
    }

    inline void set_push_square(Square square, bool value)
    {
        push_mask &= ~square.mask();
        push_mask |= static_cast<uint64_t>(value) << square;
    }

    inline void set_capture_square(Square square, bool value)
    {
        capture_mask &= ~square.mask();
        capture_mask |= static_cast<uint64_t>(value) << square;
    }

    inline void add_checker(Square square)
    {
        checker_square = square;
        checkers_number++;
    }

    inline void set_king_square(Square square, ChessColor color)
    {
        color == ChessColor::WHITE ? king_white_square = square : king_black_square = square;
    }

    inline void clear()
    {
        attack_mask = 0U;
        pin_mask = 0U;
        kingDanger_mask = 0U;
        checker_square = Square::SQ_INVALID;
        checkers_number = 0U;
        king_white_square = Square::SQ_INVALID;
        king_black_square = Square::SQ_INVALID;
    }

private:
    uint64_t attack_mask;
    uint64_t pin_mask;
    uint64_t kingDanger_mask;
    uint64_t push_mask;
    uint64_t capture_mask;
    Square checker_square;
    Square king_white_square;
    Square king_black_square;
    uint8_t checkers_number;
};

static void calculate_dangers(const Board& board, MoveGenInfo& moveGenInfo);
static void calculate_pawn_dangers(const Board& board, Square sq, MoveGenInfo& moveGenInfo);
static void calculate_knight_dangers(const Board& board, Square sq, MoveGenInfo& moveGenInfo);
static void calculate_king_dangers(const Board& board, Square sq, MoveGenInfo& moveGenInfo);
static void calculate_queen_dangers(const Board& board, Square sq, MoveGenInfo& moveGenInfo);
static void calculate_rook_dangers(const Board& board, Square sq, MoveGenInfo& moveGenInfo);
static void calculate_bishop_dangers(const Board& board, Square sq, MoveGenInfo& moveGenInfo);

static inline bool is_enemy_king(const Board& board, Square king_square, ChessColor friend_color);
static inline bool is_enemy_piece(const Board& board, Square piece_square, ChessColor friend_color);

static void calculate_danger_to_square(const Board& board, Square attacker_sq, Square defender_sq,
                                       ChessColor friend_color, MoveGenInfo& moveGenInfo);

static void calculate_danger_to_direction(const Board& board, Direction dir, Square attacker_sq,
                                          ChessColor friend_color, MoveGenInfo& moveGenInfo);

static void calculate_pawn_moves(const Board& board, Square sq, MoveGenInfo& moveGenInfo,
                                 MoveList& moves);
static void calculate_knight_moves(const Board& board, Square sq, MoveGenInfo& moveGenInfo,
                                   MoveList& moves);
static void calculate_king_moves(const Board& board, Square sq, MoveGenInfo& moveGenInfo,
                                 MoveList& moves);
static void calculate_queen_moves(const Board& board, Square sq, MoveGenInfo& moveGenInfo,
                                  MoveList& moves);
static void calculate_rook_moves(const Board& board, Square sq, MoveGenInfo& moveGenInfo,
                                 MoveList& moves);
static void calculate_bishop_moves(const Board& board, Square sq, MoveGenInfo& moveGenInfo,
                                   MoveList& moves);
static void calculate_castle_moves(const Board& board, Square king_sq, ChessColor friendly_color,
                                   MoveGenInfo& moveGenInfo, MoveList& moves);

static void calculate_moves_to_direction(const Board& board, Direction dir, Square attacker_sq,
                                         ChessColor friend_color, MoveGenInfo& moveGenInfo,
                                         MoveList& moves);

static bool is_valid_move_pinned(const Board& board, Square piece_sq, Square end_sq,
                                 MoveGenInfo& moveGenInfo);

static bool is_valid_move_en_passant(const Board& board, Square piece_sq, Square end_sq,
                                     MoveGenInfo& moveGenInfo);

static void calculate_checkers(const Board& board, MoveGenInfo& moveGenInfo);
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

    MoveGenInfo moveGenInfo;
    moves.clear();

    const ChessColor side_to_move = board.state().side_to_move();
    calculate_dangers(board, moveGenInfo);

    calculate_checkers(board, moveGenInfo);

    if (moveGenInfo.get_checkers_number() >= 2) {

        calculate_king_moves(board, moveGenInfo.get_king_square(side_to_move), moveGenInfo, moves);
        return;   // when double check only king moves allowed
    }

    for (Square square = Square::SQ_A1; square <= Square::SQ_H8; square++) {

        const Piece piece = board.get_piece(square);
        const ChessColor color = get_color(piece);

        if (board.is_empty(square) || color == side_to_move) continue;

        switch (piece_to_pieceType(piece)) {
        case PieceType::PAWN: calculate_pawn_moves(board, square, moveGenInfo, moves); break;
        case PieceType::KNIGHT: calculate_knight_moves(board, square, moveGenInfo, moves); break;
        case PieceType::KING: calculate_king_moves(board, square, moveGenInfo, moves); break;
        case PieceType::QUEEN: calculate_queen_moves(board, square, moveGenInfo, moves); break;
        case PieceType::ROOK: calculate_rook_moves(board, square, moveGenInfo, moves); break;
        case PieceType::BISHOP: calculate_bishop_moves(board, square, moveGenInfo, moves); break;
        default: break;
        }
    }


    //numMoves = numLegalMoves;
    //stateInfo.check = checkersNum > 0;
}

static void calculate_dangers(const Board& board, MoveGenInfo& moveGenInfo)
{

    moveGenInfo.clear();

    for (Square square = Square::SQ_A1; square <= Square::SQ_H8; square++) {

        const Piece piece = board.get_piece(square);
        const ChessColor color = get_color(piece);

        if (board.is_empty(square) || color == board.state().side_to_move()) continue;

        switch (piece_to_pieceType(piece)) {
        case PieceType::PAWN: calculate_pawn_dangers(board, square, moveGenInfo); break;
        case PieceType::KNIGHT: calculate_knight_dangers(board, square, moveGenInfo); break;
        case PieceType::KING: calculate_king_dangers(board, square, moveGenInfo); break;
        case PieceType::QUEEN: calculate_queen_dangers(board, square, moveGenInfo); break;
        case PieceType::ROOK: calculate_rook_dangers(board, square, moveGenInfo); break;
        case PieceType::BISHOP: calculate_bishop_dangers(board, square, moveGenInfo); break;
        default: break;
        }
    }
};

static void calculate_pawn_dangers(const Board& board, Square square, MoveGenInfo& moveGenInfo)
{
    const ChessColor side_to_move = board.state().side_to_move();

    const Square defender_square_left =
        side_to_move == ChessColor::WHITE ? square.northEast() : square.southEast();

    const Square defender_square_right =
        side_to_move == ChessColor::WHITE ? square.northWest() : square.southWest();

    calculate_danger_to_square(board, square, defender_square_left, side_to_move, moveGenInfo);
    calculate_danger_to_square(board, square, defender_square_right, side_to_move, moveGenInfo);
}

static void calculate_knight_dangers(const Board& board, Square sq, MoveGenInfo& moveGenInfo)
{
    const ChessColor side_to_move = board.state().side_to_move();

    calculate_danger_to_square(board, sq, sq.north().northWest(), side_to_move, moveGenInfo);
    calculate_danger_to_square(board, sq, sq.north().northEast(), side_to_move, moveGenInfo);
    calculate_danger_to_square(board, sq, sq.east().northEast(), side_to_move, moveGenInfo);
    calculate_danger_to_square(board, sq, sq.east().southEast(), side_to_move, moveGenInfo);
    calculate_danger_to_square(board, sq, sq.south().southEast(), side_to_move, moveGenInfo);
    calculate_danger_to_square(board, sq, sq.south().southWest(), side_to_move, moveGenInfo);
    calculate_danger_to_square(board, sq, sq.west().northWest(), side_to_move, moveGenInfo);
    calculate_danger_to_square(board, sq, sq.west().southWest(), side_to_move, moveGenInfo);
}

static void calculate_king_dangers(const Board& board, Square sq, MoveGenInfo& moveGenInfo)
{
    const ChessColor side_to_move = board.state().side_to_move();

    moveGenInfo.set_king_square(sq, side_to_move);

    calculate_danger_to_square(board, sq, sq.north(), side_to_move, moveGenInfo);
    calculate_danger_to_square(board, sq, sq.northEast(), side_to_move, moveGenInfo);
    calculate_danger_to_square(board, sq, sq.east(), side_to_move, moveGenInfo);
    calculate_danger_to_square(board, sq, sq.southEast(), side_to_move, moveGenInfo);
    calculate_danger_to_square(board, sq, sq.south(), side_to_move, moveGenInfo);
    calculate_danger_to_square(board, sq, sq.southWest(), side_to_move, moveGenInfo);
    calculate_danger_to_square(board, sq, sq.west(), side_to_move, moveGenInfo);
    calculate_danger_to_square(board, sq, sq.northWest(), side_to_move, moveGenInfo);
}

static void calculate_queen_dangers(const Board& board, Square sq, MoveGenInfo& moveGenInfo)
{
    calculate_rook_dangers(board, sq, moveGenInfo);
    calculate_bishop_dangers(board, sq, moveGenInfo);
}

static void calculate_rook_dangers(const Board& board, Square sq, MoveGenInfo& moveGenInfo)
{
    const ChessColor side_to_move = board.state().side_to_move();

    calculate_danger_to_direction(board, Direction::NORTH, sq, side_to_move, moveGenInfo);
    calculate_danger_to_direction(board, Direction::SOUTH, sq, side_to_move, moveGenInfo);
    calculate_danger_to_direction(board, Direction::EAST, sq, side_to_move, moveGenInfo);
    calculate_danger_to_direction(board, Direction::WEST, sq, side_to_move, moveGenInfo);
}

static void calculate_bishop_dangers(const Board& board, Square sq, MoveGenInfo& moveGenInfo)
{
    const ChessColor side_to_move = board.state().side_to_move();

    calculate_danger_to_direction(board, Direction::NORTH_EAST, sq, side_to_move, moveGenInfo);
    calculate_danger_to_direction(board, Direction::NORTH_WEST, sq, side_to_move, moveGenInfo);
    calculate_danger_to_direction(board, Direction::SOUTH_EAST, sq, side_to_move, moveGenInfo);
    calculate_danger_to_direction(board, Direction::SOUTH_WEST, sq, side_to_move, moveGenInfo);
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
                                       ChessColor friend_color, MoveGenInfo& moveGenInfo)
{
    if (defender_sq.is_valid()) {
        moveGenInfo.set_attack_square(defender_sq, true);
        moveGenInfo.set_kingDanger_square(defender_sq, true);

        if (is_enemy_king(board, defender_sq, friend_color)) {
            moveGenInfo.add_checker(attacker_sq);
        }
    }
}

static void calculate_danger_to_direction(const Board& board, Direction dir, Square attacker_sq,
                                          ChessColor friend_color, MoveGenInfo& moveGenInfo)
{
    bool attack = true;
    bool danger = true;
    bool pin = false;
    Square pinned_piece_square;

    Square defender_sq;
    Square ini_sq = attacker_sq;
    ini_sq.to_direction(dir);

    for (defender_sq = ini_sq; defender_sq.is_valid(); defender_sq.to_direction(dir)) {

        if (attack) moveGenInfo.set_attack_square(defender_sq, true);

        if (danger) moveGenInfo.set_kingDanger_square(defender_sq, true);

        if (!board.is_empty(defender_sq)) {
            if (is_enemy_king(board, defender_sq, friend_color)) {
                if (attack) {
                    moveGenInfo.add_checker(attacker_sq);
                }
            }
            else {
                danger = false;
            }

            if (pin && is_enemy_king(board, defender_sq, friend_color)) {
                pin = false;
                moveGenInfo.set_pinned_square(pinned_piece_square, true);
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

static void calculate_pawn_moves(const Board& board, Square sq, MoveGenInfo& moveGenInfo,
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
        if (moveGenInfo.is_square_in_push(push_sq) &&
            is_valid_move_pinned(board, sq, push_sq, moveGenInfo)) {

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
        if (moveGenInfo.is_square_in_capture(capture_sq1) &&
            is_valid_move_pinned(board, sq, capture_sq1, moveGenInfo)) {

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
        if (moveGenInfo.is_square_in_capture(capture_sq2) &&
            is_valid_move_pinned(board, sq, capture_sq2, moveGenInfo))
            moves.add(Move(sq, capture_sq2));
    }


    if (sq.row() == start_row) {

        // double push
        Square double_push_sq = push_sq;
        double_push_sq.to_direction(dir);

        if (board.is_empty(push_sq) && board.is_empty(double_push_sq)) {
            if (moveGenInfo.is_square_in_push(double_push_sq) &&
                is_valid_move_pinned(board, sq, double_push_sq, moveGenInfo))
                moves.add(Move(sq, double_push_sq));
        }
    }

    if (sq.row() == en_passant_row) {
        if (board.state().en_passant_square() == capture_sq1) {
            if (moveGenInfo.is_square_in_capture(capture_sq1) &&
                is_valid_move_en_passant(board, sq, capture_sq1, moveGenInfo))
                moves.add(Move(sq, capture_sq1, MoveType::EN_PASSANT));
        }

        if (board.state().en_passant_square() == capture_sq2) {
            if (moveGenInfo.is_square_in_capture(capture_sq2) &&
                is_valid_move_en_passant(board, sq, capture_sq2, moveGenInfo))
                moves.add(Move(sq, capture_sq2, MoveType::EN_PASSANT));
        }
    }
}

static void calculate_knight_moves(const Board& board, Square sq, MoveGenInfo& moveGenInfo,
                                   MoveList& moves)
{
    auto calculate_knight_move_to_square = [&](Square end_sq, ChessColor friend_color) {
        if (end_sq.is_valid() && is_valid_move_pinned(board, sq, end_sq, moveGenInfo)) {
            if (board.is_empty(end_sq) && moveGenInfo.is_square_in_push(end_sq)) {
                moves.add(Move(sq, end_sq));
            }
            else if (is_enemy_piece(board, end_sq, friend_color) &&
                     moveGenInfo.is_square_in_capture(end_sq)) {
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
static void calculate_king_moves(const Board& board, Square sq, MoveGenInfo& moveGenInfo,
                                 MoveList& moves)
{
    auto calculate_king_move_to_square = [&](Square end_sq, ChessColor friend_color) {
        if (end_sq.is_valid() && !moveGenInfo.is_square_in_king_danger(end_sq)) {
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

    calculate_castle_moves(board, sq, friend_color, moveGenInfo, moves);
}
static void calculate_queen_moves(const Board& board, Square sq, MoveGenInfo& moveGenInfo,
                                  MoveList& moves)
{
    calculate_rook_moves(board, sq, moveGenInfo, moves);
    calculate_bishop_moves(board, sq, moveGenInfo, moves);
}
static void calculate_rook_moves(const Board& board, Square sq, MoveGenInfo& moveGenInfo,
                                 MoveList& moves)
{
    const ChessColor side_to_move = board.state().side_to_move();

    calculate_moves_to_direction(board, Direction::NORTH, sq, side_to_move, moveGenInfo, moves);
    calculate_moves_to_direction(board, Direction::SOUTH, sq, side_to_move, moveGenInfo, moves);
    calculate_moves_to_direction(board, Direction::EAST, sq, side_to_move, moveGenInfo, moves);
    calculate_moves_to_direction(board, Direction::WEST, sq, side_to_move, moveGenInfo, moves);
}
static void calculate_bishop_moves(const Board& board, Square sq, MoveGenInfo& moveGenInfo,
                                   MoveList& moves)
{
    const ChessColor side_to_move = board.state().side_to_move();


    calculate_moves_to_direction(board, Direction::NORTH_EAST, sq, side_to_move, moveGenInfo,
                                 moves);
    calculate_moves_to_direction(board, Direction::NORTH_WEST, sq, side_to_move, moveGenInfo,
                                 moves);
    calculate_moves_to_direction(board, Direction::SOUTH_EAST, sq, side_to_move, moveGenInfo,
                                 moves);
    calculate_moves_to_direction(board, Direction::SOUTH_WEST, sq, side_to_move, moveGenInfo,
                                 moves);
}

static void calculate_castle_moves(const Board& board, Square king_sq, ChessColor friendly_color,
                                   MoveGenInfo& moveGenInfo, MoveList& moves)
{
    if (moveGenInfo.get_checkers_number() > 0) {
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

        if (board.is_empty(between_sq1) && !moveGenInfo.is_square_in_king_danger(between_sq1) &&
            board.is_empty(between_sq2) && !moveGenInfo.is_square_in_king_danger(between_sq2)) {

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
        if (board.is_empty(between_sq1) && !moveGenInfo.is_square_in_king_danger(between_sq1) &&
            board.is_empty(between_sq2) && !moveGenInfo.is_square_in_king_danger(between_sq2) &&
            board.is_empty(between_sq3)) {

            moves.add(friendly_color == ChessColor::WHITE ? Move::castle_white_queen()
                                                          : Move::castle_black_queen());
        }
    }
}

static void calculate_moves_to_direction(const Board& board, Direction dir, Square attacker_sq,
                                         ChessColor friend_color, MoveGenInfo& moveGenInfo,
                                         MoveList& moves)
{

    Square defender_sq;
    Square ini_sq = attacker_sq;
    ini_sq.to_direction(dir);

    for (defender_sq = ini_sq; defender_sq.is_valid(); defender_sq.to_direction(dir)) {

        if (!defender_sq.is_valid() ||
            !is_valid_move_pinned(board, attacker_sq, defender_sq, moveGenInfo)) {
            break;
        }

        if (board.is_empty(defender_sq)) {
            if (moveGenInfo.is_square_in_push(defender_sq)) {
                moves.add(Move(attacker_sq, defender_sq));
            }
        }
        else {
            if (is_enemy_piece(board, defender_sq, friend_color) &&
                moveGenInfo.is_square_in_capture(defender_sq)) {
                moves.add(Move(attacker_sq, defender_sq));
            }
            break;
        }
    }
}

static bool is_valid_move_pinned(const Board& board, Square piece_sq, Square end_sq,
                                 MoveGenInfo& moveGenInfo)
{

    if (!moveGenInfo.is_square_pinned(piece_sq)) return true;   // piece is not pinned

    bool is_legal = false;

    const ChessColor color = board.state().side_to_move();
    Square king_sq = moveGenInfo.get_king_square(color);

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
                                     MoveGenInfo& moveGenInfo)
{
    if (!is_valid_move_pinned(board, piece_sq, end_sq, moveGenInfo)) {
        return false;
    }

    bool is_legal = true;

    const ChessColor color = board.state().side_to_move();
    Square king_sq = moveGenInfo.get_king_square(color);

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

static void calculate_checkers(const Board& board, MoveGenInfo& moveGenInfo)
{
    if (moveGenInfo.get_checkers_number() != 1) {
        // if there is no check or there is double check there is nothing to calculate
        return;
    }
    const ChessColor friend_color = board.state().side_to_move();

    const Square king_sq = moveGenInfo.get_king_square(friend_color);
    const Square checker_sq = moveGenInfo.get_checker_square();

    // insert checker in capture mask, when in check it is legal to capture the checker.
    moveGenInfo.set_capture_square(checker_sq, true);

    // insert in between squares in push mask, when in check it is legal to block check with a piece.
    if (is_slider(piece_to_pieceType(board.get_piece(checker_sq)))) {

        const Direction direction =
            get_direction(king_sq.row(), king_sq.col(), checker_sq.row(), checker_sq.col());

        Square aux_sq = king_sq;
        aux_sq.to_direction(direction);

        while (aux_sq != checker_sq) {
            moveGenInfo.set_push_square(aux_sq, true);
            aux_sq.to_direction(direction);
        }
    }
}
