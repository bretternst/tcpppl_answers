#ifndef __AIPLAYER_H__
#define __AIPLAYER_H__

#include "Player.h"

namespace Othello
{
    class AIPlayer : public Player
    {
    public:
        AIPlayer(Board& board, Board::Color color)
            : Player(board, color, "the computer") {}

        void TakeTurn();
    };
}

#endif
