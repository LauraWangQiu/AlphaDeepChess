#pragma once
#include <string>
#include <memory>
#include <board/board.hpp>

/**
 * @brief Main loop class  
 * @brief User can select options to interact with the program  
 * @brief Options:  
 * @brief q - Quit the program  
 * @brief v - Show version  
 * @brief p - Print board  
 * @brief h - Show options
*/
class Loop {
private:
    bool exit;                      // Condition to exit the loop
    std::unique_ptr<Board> board;   // Unique game board

public:
    /**
     * @brief Construct a new Loop object
     * @brief exit is set to false
     */
    Loop();
    /**
     * @brief Destroy the Loop object
     */
    ~Loop();

    /**
    * @brief Initialize board
    * @return true if the loop was initialized successfully  
    * @return false if the loop was not initialized successfully
    */
    bool init();
    /**
     * @brief Run the loop
     */
    void run();

    /**
     * @brief Quit the loop
     */
    inline void quit() { exit = true; }
    /**
     * @brief Set the exit value
     * @param value the value to set
     */
    inline void setExit(bool value) { exit = value; }
    /**
     * @brief Get the exit value
     * @return true if the loop should exit  
     * @return false if the loop should not exit
     */
    inline bool getExit() const { return exit; }

    /**
     * @brief Print the version
     */
    void printVersion() const;
    /**
     * @brief Print the board
     */
    void printBoard() const;
    /**
     * @brief Print the options
     */
    void printOptions() const;
};
