/*
    Uci protocol specifications
    https://gist.github.com/DOBRO/2592c6dad754ba67e6dcaec8c90165bf#file-uci-protocol-specification-txt
*/

#include "uci.hpp"

#include "evaluation.hpp"
#include "move_generator.hpp"
#include "search.hpp"

#include <iostream>
#include <sstream>

constexpr auto StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
constexpr auto EnPassantFEN = "rnbqkb1r/2pp2pn/1p6/pP1PppPp/8/2N5/P1P1PP1P/R1BQKBNR w KQkq f6 0 8";
constexpr auto PromotionFEN = "r3kb1r/pbpqn1P1/1pn4p/5Q2/2P5/2N5/PP1BN1pP/R3KB1R w KQkq - 2 13";
constexpr auto KiwipeteFEN = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";

/**
 * @brief loop
 * 
 * Uci main loop.
 * 
 */
void Uci::loop()
{
    std::string command;
    std::string line;

    TokenArray tokens;
    bool exit = false;

    board.load_fen(StartFEN);

    do {
        std::getline(std::cin, line);

        if (line.empty()) {
            continue;
        }

        std::istringstream iss(line);

        std::string token;

        int num_tokens = 0;

        while ((num_tokens < TOKEN_ARRAY_SIZE) && (iss >> token)) {
            tokens[num_tokens++] = token;
        }

        if (tokens.empty()) {
            continue;
        }

        command = tokens[0];

        if (command == "uci") {
            uci_command_action();
        }
        else if (command == "isready") {
            is_ready_command_action();
        }
        else if (command == "ucinewgame") {
            new_game_command_action();
        }
        else if (command == "g" || command == "go") {
            go_command_action();
        }
        else if (command == "s" || command == "stop") {
            stop_command_action();
        }
        else if (command == "e" || command == "eval") {
            eval_command_action();
        }
        else if (command == "p" || command == "position") {
            bool success = position_command_action(tokens, num_tokens);
        }
        else if (command == "d" || command == "diagram") {
            diagram_command_action();
        }
        else if (command == "h" || command == "help") {
            help_command_action();
        }
        else if (command == "q" || command == "quit") {
            quit_command_action();
            exit = true;
        }
        else {
            unknown_command_action();
        }

    } while (!exit);
}

/**
 * @brief uci_command_action
 * 
 * @note tell engine to use the uci (universal chess interface),
 *       this will be sent once as a first command after program boot.
 * 
 * respond with "uciok"
 * 
 */
void Uci::uci_command_action() const { std::cout << "uciok" << std::endl; }

/**
 * @brief is_ready_command_action
 * 
 * @note  this command can be used to wait for the engine to be ready again or
 *        to ping the engine to find out if it is still alive.
 * 
 * Responds with "readyok"
 * 
 */
void Uci::is_ready_command_action() const { std::cout << "readyok" << std::endl; }

/**
 * @brief new_game_command_action
 * 
 * Uci main loop.
 * 
 */
void Uci::new_game_command_action() { board.load_fen(StartFEN); }

/**
 * @brief go_command_action
 * 
 * Starts the search in the searchThread.
 * 
 */
void Uci::go_command_action()
{
    stopSearch.store(false);

    // Launch a new thread to search for the best move
    searchThread = std::thread(
        [this]() { bestMove = search_best_move(board, INFINITE_SEARCH_DEPTH_VALUE, stopSearch); });
}

/**
 * @brief stop_command_action
 * 
 * Stops the search in the searchThread.
 * 
 */
void Uci::stop_command_action()
{
    stopSearch.store(true);

    // Wait for the search thread to finish if it's running
    if (searchThread.joinable()) {
        searchThread.join();
    }

    std::cout << "Search stopped." << std::endl;
    std::cout << "Best move found : " << bestMove.to_string() << std::endl;
}

/**
 * @brief eval_command_action
 * 
 * Evaluate chess position.
 * 
 */
void Uci::eval_command_action() const
{
    std::cout << "Evaluation : " << evaluate_position(board) << std::endl;
}

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
bool Uci::position_command_action(const TokenArray& tokens, int num_tokens)
{
    if (num_tokens < 2) {
        return false;   // Not enough tokens
    }

    if (tokens[1] == "startpos") {
        board.load_fen(StartFEN); 
    }
    else if (tokens[1] == "fen") {
        if (num_tokens < 3) {
            return false;   // Not enough tokens for FEN
        }
        const int fen_string_pos = 2;

        board.load_fen(tokens[fen_string_pos]);
    }
    else {
        return false;   // Invalid format
    }

    // Handle any subsequent moves
    if (num_tokens > 3) {
        MoveList move_list;
        for (size_t i = 4; i < num_tokens; ++i) {
            Move move = create_move_from_string(tokens[i], board);
            if (!move.is_valid()) {
                return false;   // Invalid move
            }
            board.make_move(move);
        }
    }

    return true;
}

/**
 * @brief diagram_command_action
 * 
 * Prints the board and fen of the position.
 * 
 */
void Uci::diagram_command_action() const
{
    std::cout << board << std::endl;

    MoveList moves;

    generate_legal_moves(moves, board);

    std::cout << "Legal moves : " << moves.size() << std::endl << moves.to_string() << std::endl;
}

/**
 * @brief help_command_action
 * 
 * Shows help information about commands.
 * 
 */
void Uci::help_command_action() const
{
    std::cout
        << "Commands:\n"
           "----------------------------------------\n"
           "uci\n"
           "\tTell engine to use the UCI (Universal Chess Interface).\n"
           "\tThe engine must respond with 'uciok'.\n\n"

           "isready\n"
           "\tSynchronize the engine with the GUI. The engine must respond with 'readyok'.\n\n"

           "ucinewgame\n"
           "\tStart of a new game.\n\n"

           "position [fen <fenstring> | startpos ] moves <move1> .... <movei>\n"
           "\tSet up the position on the internal board.\n\n"
                "\t\tMove format:\n\n"
                "\t\tThe move format is in long algebraic notation.\n"
                "\t\tA nullmove from the Engine to the GUI should be sent as 0000.\n"
                "\t\tExamples:  e2e4, e7e5, e1g1 (white short castling), e7e8q (for promotion)\n\n"
           "go\n"
           "\tStart calculating.\n"
           "\tOptional parameters: searchmoves, ponder, wtime, btime, winc, binc, movestogo, "
           "depth, nodes, mate, movetime, infinite.\n\n"

           "stop\n"
           "\tStop calculating.\n\n"

           "quit\n"
           "\tQuit the program.\n\n"

           "d\n"
           "\tDisplay the current position on the board.\n\n"

        << std::endl;
}

/**
 * @brief quit_command_action
 * 
 * Exits program.
 * 
 */
void Uci::quit_command_action() const { std::cout << "goodbye" << std::endl; }


/**
 * @brief unknown_command_action
 * 
 * Invalid command detected.
 * 
 */
void Uci::unknown_command_action() const
{
    std::cout << "Unknown command, type help for more information" << std::endl;
}

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
Move Uci::create_move_from_string(const std::string& move_string, const Board& board) const
{
    return Move::null();
}