#ifndef __CONSOLEBOARD_H__
#define __CONSOLEBOARD_H__

#include "board.h"
#include "iplayerinput.h"

namespace othello
{
    class ConsoleBoard : public Board
    {
        void draw_space(Color c);
    public:
        ConsoleBoard();
        ~ConsoleBoard();
        void draw();
        Position get_move();
        void alert(std::string msg);
    };
}

#endif
