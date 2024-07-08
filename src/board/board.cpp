#include <board.hpp>
#include <defs.hpp>

Board::Board() : allBB(0), whiteBB(0), blackBB(0), piecesBB() { }

Board::~Board() { }

std::ostream& operator<<(std::ostream& os, const Board& board) {

    os << "\n+---+---+---+---+---+---+---+---+\n";

    // ...

    os << "\n+---+---+---+---+---+---+---+---+\n";

    os << '\n';
    return os;
}
