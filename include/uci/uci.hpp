#pragma once

/**
 * @file uci.hpp
 * @brief uci services.
 *
 * UCI(universal chess interface) declarations.
 * 
 * https://en.wikipedia.org/wiki/Universal_Chess_Interface
 * 
 * https://gist.github.com/DOBRO/2592c6dad754ba67e6dcaec8c90165bf#file-uci-protocol-specification-txt
 * 
 */

#include "board.hpp"
#include "search.hpp"
#include <atomic>
#include <array>
#include <string>
#include <thread>

/**
 * @brief MAX_UCI_TOKENS
 * 
 * max tokens that user can input in the console.
 * 
 */
constexpr int TOKEN_ARRAY_SIZE = 1024;

typedef std::array<std::string_view, TOKEN_ARRAY_SIZE> TokenArray;

/**
 * @brief Uci
 * 
 * Unified chess interface protocol
 * 
 */
class Uci
{
public:
    /**
     * @brief Uci
     * 
     * Uci constructor.
     * 
     */

    Uci() { }

    /**
     * @brief ~Uci
     * 
     * Uci destructor.
     * 
     */
    ~Uci()
    {
        stop_signal.store(true);
        if (searchThread.joinable()) searchThread.join();
        if (readerThread.joinable()) readerThread.join();
        if (timerThread.joinable()) timerThread.join();
    }

    /**
     * @brief loop
     * 
     * Uci main loop.
     * 
     */
    void loop();

private:
    /**
     * @brief stop_signal
     * 
     * @note thread safe
     * 
     * stop search signal
     * 
     */
    std::atomic<bool> stop_signal;

    /**
     * @brief board 
     * 
     * board with the chess position.
     * 
     */
    Board board;

    /**
     * @brief searchThread
     * 
     * thread in charge of searching for the best move.
     * 
     */
    std::thread searchThread;

    /**
     * @brief readerThread
     * 
     * thread in charge of reading the search output
     * 
     */
    std::thread readerThread;

    /**
     * @brief timerThread
     * 
     * thread in charge of stopping the search when time is up.
     * 
     */
    std::thread timerThread;

    /**
     * @brief timerMutex
     * 
     * mutex to protect the timerCv.
     * 
     */
    std::mutex timerMutex;

    /**
     * @brief timerCv
     * 
     * condition variable to signal the timer.
     * 
     */
    std::condition_variable timerCv;

    /**
     * @brief searchResults
     * 
     * array to store the search results.
     * 
     */
    SearchResults searchResults;

    /**
     * @brief uci_command_action
     * 
     * @note tell engine to use the uci (universal chess interface),
     *       this will be sent once as a first command after program boot.
     * 
     * respond with "uciok"
     * 
     */
    void uci_command_action() const;

    /**
     * @brief is_ready_command_action
     * 
     * @note  this command can be used to wait for the engine to be ready again or
     *        to ping the engine to find out if it is still alive.
     * 
     * Responds with "readyok"
     * 
     */
    void is_ready_command_action() const;

    /**
     * @brief new_game_command_action
     * 
     * Uci main loop.
     * 
     */
    void new_game_command_action();

    /**
     * @brief go_command_action
     * 
     * Starts the search in the searchThread.
     * 
     */
    void go_command_action(const TokenArray& tokens, uint32_t num_tokens);

    /**
     * @brief stop_command_action
     * 
     * Stops the search in the searchThread.
     * 
     */
    void stop_command_action();

    /**
     * @brief eval_command_action
     * 
     * Evaluate chess position.
     * 
     */
    void eval_command_action() const;

    /**
     * @brief position_command_action
     * 
     * Set the position in the board specified in fen notation.
     * 
     *  @note position [fen <fenstring> | startpos ]  moves <move1> .... <movei>
     * 
     *	set up the position described in fenstring on the internal board and
     *	play the moves on the internal chess board.
     *	if the game was played  from the start position the string "startpos" will be sent
     *	Note: no "new" command is needed. However, if this position is from a different game than
     *	the last position sent to the engine, the GUI should have sent a "ucinewgame" inbetween.
     *
     * @param[in] tokens buffer array with the user input tokens.
     * @param[in] num_tokens number of tokens.
     * 
     *  @return 
     *      - TRUE if success.
     *      - FALSE if error detected, probably error in user input.
     */
    bool position_command_action(const TokenArray& tokens, uint32_t num_tokens);

    /**
     * @brief diagram_command_action
     * 
     * Prints the board and fen of the position.
     * 
     */
    void diagram_command_action() const;

    /**
     * @brief help_command_action
     * 
     * Shows help information about commands.
     * 
     */
    void help_command_action() const;

    /**
     * @brief quit_command_action
     * 
     * Exits program.
     * 
     */
    void quit_command_action();

    /**
     * @brief perft_command_action
     * 
     * Executes perft test of the actual position.
     * 
     * @param[in] depth desired depth of the test.
     * 
     */
    void perft_command_action(uint64_t depth) const;

    /**
     * @brief setoption_command_action
     * 
     * Setoption command detected, use to change internal parameters of the engine.
     * 
     * @param[in] tokens buffer array with the user input tokens.
     * @param[in] num_tokens number of tokens.
     * 
     *  @return 
     *      - TRUE if success.
     *      - FALSE if error detected, probably error in user input.
     */
    bool setoption_command_action(const TokenArray& tokens, uint32_t num_tokens);

    /**
     * @brief unknown_command_action
     * 
     * Invalid command detected.
     * 
     */
    void unknown_command_action() const;

    /**
     * @brief create_move_from_string
     * 
     * Create the move from string representation and board actual position.
     * Check in the board if the move will be a capture, en passant, promotion.
     * 
     *  @note Move format:
     *
     *     The move format is in long algebraic notation.
     *     A nullmove from the Engine to the GUI should be sent as 0000.
     *     Examples:  e2e4, e7e5, e1g1 (white short castling), e7e8q (for promotion)
     *
     * @param[in] move_string string move representation (e.g a7a8Q : pawn move and queen promotion).
     * @param[in] board chess position.
     * 
     *  @return 
     *      - Move valid if success.
     *      - Move::null() if error detected, bad string move representation.
     */
    Move create_move_from_string(std::string_view move_string, const Board& board) const;

    /**
     * @brief calculate the time the bot has until stopping the search
     * 
     * @note pass 0 to not provide an argument
     * 
     * @param[in] us side to move
     * @param[in] movetime explicit think time (if provided it will be the think time)
     * @param[in] wtime white total time
     * @param[in] btime black total time
     * @param[in] winc white time increment per move
     * @param[in] binc black time increment per move
     * 
     *  @return (uint32_t) time to think
     * 
     */
    uint32_t think_time(ChessColor us, uint32_t movetime, uint32_t wtime, uint32_t btime, uint32_t winc, uint32_t binc);
};