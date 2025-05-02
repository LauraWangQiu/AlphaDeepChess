#pragma once

/**
 * @file transposition_table.hpp
 * @brief transposition table utilities declaration.
 *
 * https://www.chessprogramming.org/Transposition_Table
 * 
 */

#include <vector>
#include <bit_utilities.hpp>
#include "move.hpp"

/**
 * @brief TranspositionTable
 *
 * Transposition table used as cache to store evaluation of chess positions
 * 
 */
class TranspositionTable
{
public:
    enum class SIZE : int;
    enum class NodeType : uint8_t;
    class Entry;

    /**
     * @brief get_entry(uint64_t)
     * 
     * returns the entry in the table indexed by the zobrist hash key
     * 
     * @param[in] zobrist_key zobrist hash key of the position 
     * 
     * @return valid entry or failed entry
     * 
     */
    static inline constexpr Entry get_entry(uint64_t zobrist_key)
    {
        const Entry& entry = entries[index_in_table(zobrist_key)];

        return entry.is_valid() && entry.key == zobrist_key ? entry : Entry::failed_entry();
    }

    /**
     * @brief store_entry(uint64_t)
     * 
     * stores an entry in the transposition table
     * 
     * @param[in] zobrist zobrist hash key of the position 
     * @param[in] eval evaluation of the position
     * @param[in] move best mode of the position
     * @param[in] node_type node type 
     * @param[in] depth depth 
     * 
     */
    static inline constexpr void store_entry(uint64_t zobrist, int eval, Move move, NodeType node_type, int8_t depth)
    {
        entries[index_in_table(zobrist)] = Entry(zobrist, eval, move, node_type, depth);
        assert(entries[index_in_table(zobrist)].is_valid());
        assert(move.is_valid());
    }

    /**
     * @brief store_entry(const Entry&)
     * 
     * stores an entry in the transposition table
     * 
     * @param[in] entry entry to store 
     * 
     */
    static inline constexpr void store_entry(const Entry& entry)
    {
        entries[index_in_table(entry.key)] = entry;
        assert(entries[index_in_table(entry.key)].is_valid());
        assert(entry.move.is_valid());
    }

    /**
     * @brief resize(SIZE)
     * 
     * resize the transposition table
     * 
     * @note new_size_mb must be power of two
     * 
     * @param[in] new_size_mb new size of the transposition table in mb, must be power of two
     * 
     */
    static void resize(SIZE new_size_mb);

    /**
     * @brief int_to_tt_size(int)
     * 
     * returns the corresponding enum SIZE.
     * @note size must be power of two, if invalid or too large size, MB_INVALID will be returned
     * 
     * @param[in] size_mb_power_of_two int with size in mb, must be a power of two
     * 
     * @return SIZE
     * 
     */
    static constexpr inline SIZE int_to_tt_size(int size_mb_power_of_two)
    {
        return is_power_of_two(size_mb_power_of_two) && 1 <= size_mb_power_of_two && size_mb_power_of_two <= 2048
            ? static_cast<SIZE>(size_mb_power_of_two)
            : SIZE::INVALID;
    }

    /**
     * @brief get_num_entries()
     *
     * return num of entries in transposition table
     * 
     * @return entries.size
     */
    static inline constexpr uint32_t get_num_entries() { return entries.size(); }

    /**
     * @brief returns the memory address of the entry in the tt
     *
     * @param[in] zobrist_key hash of the position
     * 
     * @return &(entries[zobrist_key])
     */
    static inline constexpr const void* get_address_of_entry(uint64_t zobrist_key)
    {
        return &(entries[index_in_table(zobrist_key)]);
    }

    TranspositionTable() = delete;
    ~TranspositionTable() = delete;

private:
    /**
     * @brief index_in_table(uint64_t)
     * 
     * calculates the real index key in the transposition table of a position, 
     * it crops the zobrist key with % operator so key fits in table.
     * 
     * @param[in] zobrist_key zobrist hash key of the position 
     * 
     * @return zobrist_key % num_entries
     * 
     */
    static inline constexpr uint64_t index_in_table(uint64_t zobrist_key)
    {
        assert(is_power_of_two(entries.size()));

        return zobrist_key & (uint64_t(entries.size()) - 1ULL);
    }

    /**
     * @brief initialization(std::vector<Entry>)
     * 
     * initliazes the entries vector
     * 
     * @return std::vector<Entry> entries
     * 
     */
    static std::vector<Entry> initialization();

    /**
     * @brief entries
     * 
     * vector of entries
     * 
     */
    static std::vector<TranspositionTable::Entry> entries;

public:
    /**
     * @brief TranspositionTable::SIZE
     *
     *  Size in MB of the transposition table, only powers of two are permitted.
     *    
     */
    enum class SIZE : int
    {
        MB_1 = 1 << 0,
        MB_2 = 1 << 1,
        MB_4 = 1 << 2,
        MB_8 = 1 << 3,
        MB_16 = 1 << 4,
        MB_32 = 1 << 5,
        MB_64 = 1 << 6,
        MB_128 = 1 << 7,
        MB_256 = 1 << 8,
        MB_512 = 1 << 9,
        MB_1024 = 1 << 10,
        MB_2048 = 1 << 11,
        INVALID
    };

    /**
     * @brief TranspositionTable::NodeType
     *
     * We usually do not find the exact value of a position but we can know that the
     * value is either too low or too high for us to be concerned with searching any further. 
     * If we have the exact value, of course, we store that in the transposition table. 
     * But if the value of our position is either high enough to set the lower bound, 
     * or low enough to set the upper bound, it is good to store that information also.
     * 
     */
    enum class NodeType : uint8_t
    {
        FAILED,        // failed entry
        EXACT,         // PV-Node, Score is Exact
        UPPER_BOUND,   // All-Node, Score is Upper Bound
        LOWER_BOUND,   // Cut-Node, Score is Lower Bound
        PERFT,         // Used only for perft command (ignored in the search)

    };

    /**
     * @brief TranspositionTable::Entry
     *
     * @note Entry in the transposition table
     * 
     * Each entry stores information about a specific chess position, including its
     * Zobrist key, evaluation score, best move, node type, and search depth.
     * 
     *   - key.        
     *   - evaluation. 
     *   - move.       
     *   - node_type.  
     *   - depth. 
     */
    class Entry
    {
    public:
        /**
         * @brief Zobrist key of the chess position.
         *
         * This key uniquely identifies the position in the transposition table.
         */
        uint64_t key;

        /**
         * @brief Evaluation score of the chess position.
         *
         * This score represents the evaluation of the position from the perspective
         * of the side to move.
         */
        int evaluation;

        /**
         * @brief Best move found for the chess position.
         *
         * This move represents the best move calculated for the position.
         */
        Move move;

        /**
         * @brief Type of the node in the search tree.
         *
         * Indicates whether the node is an exact value, a lower bound, or an upper bound.
         */
        NodeType node_type;

        /**
         * @brief Depth at which the position was evaluated.
         *
         * This value represents the search depth at which the evaluation was performed.
         */
        int8_t depth;

        /**
         * @brief Default constructor for an entry.
         *
         * Initializes the entry with default values, marking it as invalid.
         */
        constexpr Entry() : key(0ULL), evaluation(0), move(), node_type(NodeType::FAILED), depth(0U) { }

        /**
         * @brief Copy constructor for an entry.
         *
         * Creates a new entry by copying the values from another entry.
         *
         * @param[in] entry The entry to copy.
         */
        constexpr Entry(const Entry& entry)
            : key(entry.key), evaluation(entry.evaluation), move(entry.move), node_type(entry.node_type),
            depth(entry.depth)
        { }

        /**
         * @brief Parameterized constructor for an entry.
         *
         * Initializes the entry with the given values.
         *
         * @param[in] key Zobrist key of the position.
         * @param[in] evaluation Evaluation score of the position.
         * @param[in] move Best move found for the position.
         * @param[in] node_type Type of the node in the search tree.
         * @param[in] depth Depth at which the position was evaluated.
         */
        constexpr Entry(uint64_t key, int evaluation, Move move, NodeType node_type, uint8_t depth)
            : key(key), evaluation(evaluation), move(move), node_type(node_type), depth(depth)
        { }

        /**
         * @brief Checks if the entry is valid.
         *
         * An entry is considered valid if its node type is not `NodeType::FAILED`.
         *
         * @return True if the entry is valid, false otherwise.
         */
        inline constexpr bool is_valid() const { return node_type != NodeType::FAILED; }

        /**
         * @brief Returns a failed entry.
         *
         * A failed entry is an entry with default values, marked as invalid.
         *
         * @return A failed entry.
         */
        static inline constexpr Entry failed_entry() { return Entry(); }

        /**
         * @brief Equality operator.
         *
         * Compares two entries for equality based on their key, evaluation, move,
         * node type, and depth.
         *
         * @param[in] other The entry to compare with.
         * @return True if the entries are equal, false otherwise.
         */
        constexpr bool operator==(const Entry& other) const
        {
            return key == other.key && evaluation == other.evaluation && move == other.move &&
                node_type == other.node_type && depth == other.depth;
        }

        /**
         * @brief Inequality operator.
         *
         * Compares two entries for inequality.
         *
         * @param[in] other The entry to compare with.
         * @return True if the entries are not equal, false otherwise.
         */
        constexpr bool operator!=(const Entry& other) const { return !(*this == other); }

        /**
         * @brief Assignment operator.
         *
         * Assigns the values of another entry to this entry.
         *
         * @param[in] other The entry to assign from.
         * @return A reference to this entry.
         */
        constexpr Entry& operator=(const Entry& other)
        {
            if (this != &other)   // not a self-assignment
            {
                this->key = other.key;
                this->evaluation = other.evaluation;
                this->move = other.move;
                this->node_type = other.node_type;
                this->depth = other.depth;
            }
            return *this;
        }
    };
};
