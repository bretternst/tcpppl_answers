#ifndef __CONSOLEBOARD_H__
#define __CONSOLEBOARD_H__

#include "Board.h"
#include "IPlayerInput.h"

namespace Othello
{
    class ConsoleBoard : public Board
    {
        void DrawSpace(Color s);
    public:
        void Draw();
        Position GetMove();
        void Alert(std::string msg);
    };
}

#endif
