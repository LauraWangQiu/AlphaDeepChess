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
#include "move.hpp"

class Uci
{
public:
    Uci() {}

    ~Uci() {}

    void loop();

private:

    Board board;
    MoveList moves;
    
    void uciCommandAction();
    void isReadyCommandAction();
    void newgameCommandAction();
    void goCommandAction();
    void stopCommandAction();
    void evalCommandAction();
    void positionCommandAction();
    void diagramCommandAction();
    void helpCommandAction();
    void quitCommandAction();
    void unknownCommandAction();
};