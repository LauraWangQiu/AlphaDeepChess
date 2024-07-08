#pragma once
#include <types.hpp>
#include <string>
#include <cstdint>

/**
 * @brief Represents a square on the board
 */
class Square {
private:
    uint8_t value;   // The square value
    int row;         // The row
    int col;         // The column

public:
    /**
     * @brief Empty constructor. Initializes the square to Types::Square::SQ_NONE
     */
    Square() { setValue(); }
    /**
     * @brief Construct a new Square object with a value from uint8_t
     * @param value The square value
     */
    Square(uint8_t value) { setValue(value); }
    /**
     * @brief Construct a new Square object with a value from Types::Square
     * @param value The square value
     */
    Square(Types::Square value) { setValue(value); }
    /**
     * @brief Construct a new Square object from row and column.  
     * @brief The value is calculated as (row << 3) + col,  
     * @brief meaning (row * 8 + col)
     * @param row The row
     * @param col The column
     */
    Square(int row, int col) { setValue(row, col); }

    /**
     * @brief Overloads the << operator to print the coordinates of the square
     * @param os The output stream
     * @param square The square to print
     */
    friend std::ostream& operator<<(std::ostream& os, const Square& square) {
        os << (char)('a' + square.getCol()) << (char)('1' + square.getRow());
        return os;
    }

    /**
     * @brief Returns the square value
     * @return The square value
     */
    inline uint8_t getValue() const { return value; }
    /**
     * @brief Returns the square value
     * @return The square value
     */
    inline Types::Square getSquareValue() const { return static_cast<Types::Square>(value); }
    /**
     * @brief Sets the square value to Types::Square::SQ_NONE, row to -1 and col to -1
     */
    inline void setValue() {
        value = Types::Square::SQ_NONE;
        row = -1;
        col = -1;
    }
    /**
     * @brief Sets the square value to the given value, row and col are calculated from the value.  
     * @brief If the value is not valid, the square value is set to SQ_NONE
     * @param value The square value
     */
    inline void setValue(uint8_t value) {
        if (Types::validSquare(value)) {
            this->value = value;
            this->row = value >> 3;
            this->col = value & 7;
        }
        else
            setValue();
    }
    /**
     * @brief Sets the square value to the given value (Types::Square), row and col are calculated from the value
     * @param value The square value
     */
    inline void setValue(Types::Square value) {
        if (Types::validSquare(value)) {
            this->value = value;
            row = Types::row(value);
            col = Types::col(value);
        }
        else
            setValue();
    }
    /**
     * @brief Sets the square value to the given value (Types::Square), row and col are calculated from the value
     * @brief If the row and column are not valid, the square value is set to SQ_NONE
     * @param value The square value
     */
    inline void setValue(int row, int col) {
        if (Types::validCoord(row, col)) {
            this->row = row;
            this->col = col;
            value = (row << 3) + col;
        }
        else
            setValue();
    }

    /**
     * @brief Returns the square row
     * @return The square row
     */
    inline int getRow() const { return row; }
    /**
     * @brief Sets the square row
     * @param row The row
     */
    inline void setRow(int row) { setValue(row, this->col); }

    /**
     * @brief Returns the square column
     * @return The square column
     */
    inline int getCol() const { return col; }
    /**
     * @brief Sets the square column
     * @param col The column
     */
    inline void setCol(int col) { setValue(this->row, col); }
};
