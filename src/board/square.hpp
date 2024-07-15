#pragma once
#include <types.hpp>
#include <cstdint>
#include <ostream>

/**
 * @brief Represents a square on the board
 */
class Square {
private:
    uint8_t value;   // The square value

public:
    /**
     * @brief Empty constructor. Initializes the square to Types::Square::SQ_NONE
     */
    constexpr Square() : value(Types::Square::SQ_NONE) { }
    /**
     * @brief Construct a new Square object with a value from uint8_t
     * @param value The square value
     */
    constexpr Square(uint8_t value) : value(Types::Square::SQ_NONE) { setValue(value); }
    /**
     * @brief Construct a new Square object with a value from Types::Square
     * @param value The square value
     */
    constexpr Square(Types::Square value) : value(Types::Square::SQ_NONE) { setValue(value); }
    /**
     * @brief Construct a new Square object from row and column.  
     * @brief The value is calculated as (row << 3) + col,  
     * @brief meaning (row * 8 + col)
     * @param row The row
     * @param col The column
     */
    constexpr Square(int row, int col) : value(Types::Square::SQ_NONE) { setValue(row, col); }

    /**
     * @brief Returns the square value
     * @return The square value
     */
    constexpr uint8_t getValue() const { return value; }
    /**
     * @brief Returns the square value
     * @return The square value
     */
    constexpr Types::Square getSquareValue() const { return static_cast<Types::Square>(value); }
    /**
     * @brief Sets the square value to the given value, row and col are calculated from the value.  
     * @brief If the value is not valid, the square value is set to SQ_NONE
     * @param value The square value
     */
    constexpr void setValue(uint8_t value) {
        if (Types::validSquare(value)) this->value = value;
        else
            debugerror("Invalid value");
    }
    /**
     * @brief Sets the square value to the given value (Types::Square), row and col are calculated from the value
     * @param value The square value
     */
    constexpr void setValue(Types::Square value) {
        if (Types::validSquare(value)) this->value = value;
        else
            debugerror("Invalid value");
    }
    /**
     * @brief Sets the square value to the given value (Types::Square), row and col are calculated from the value
     * @brief If the row and column are not valid, the square value is set to SQ_NONE
     * @param value The square value
     */
    constexpr void setValue(int row, int col) {
        if (Types::validCoord(row, col)) value = (row << 3) + col;
        else
            debugerror("Invalid row or column value");
    }

    /**
     * @brief Returns the square row
     * @return The square row
     */
    constexpr int getRow() const { return value >> 3; }
    /**
     * @brief Sets the square row
     * @param row The row
     */
    constexpr void setRow(Types::Row row) {
        if (Types::validRow(row)) value = (row << 3) + (value >> 3);
        else
            debugerror("Invalid row value");
    }

    /**
     * @brief Returns the square column
     * @return The square column
     */
    constexpr int getCol() const { return value & 7; }
    /**
     * @brief Sets the square column
     * @param col The column
     */
    constexpr void setCol(Types::Column col) {
        if (Types::validCol(col)) value = (value & ~7) + col;
        else
            debugerror("Invalid column value");
    }

    /**
     * @brief Overloads the << operator to print the coordinates of the square
     * @param os The output stream
     * @param square The square to print
     */
    friend std::ostream& operator<<(std::ostream& os, const Square& square) {
        if (Types::validSquare(square))
            os << (char)('a' + square.getCol()) << (char)('1' + square.getRow());
        else
            os << "null";
        return os;
    }
    /**
     * @brief Implicit conversion operator to uint8_t
     * @return uint8_t
     */
    constexpr operator uint8_t() const { return value; }
    /**
     * @brief Pre-increment operator overload
     * @return Square& incremented
     */
    constexpr Square& operator++() {
        ++value;
        return *this;
    }
    /**
     * @brief Post-increment operator overload
     * @return Square incremented
     */
    constexpr Square operator++(int) {
        Square temp = *this;
        ++(*this);
        return temp;
    }
    /**
     * @brief Pre-decrement operator overload
     * @return Square& decremented
     */
    constexpr Square& operator--() {
        --value;
        return *this;
    }
    /**
     * @brief Post-decrement operator overload
     * @return Square decrement
     */
    constexpr Square operator--(int) {
        Square temp = *this;
        --(*this);
        return temp;
    }
};
