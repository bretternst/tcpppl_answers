#ifndef __AIPLAYER_H__
#define __AIPLAYER_H__

#include "player.h"

namespace othello
{
    class AIPlayer : public Player
    {
    public:
        AIPlayer(Board& board, Board::Color color)
            : Player(board, color, "computer") {}

        void take_turn();
    };
}

#endif
