#ifndef __HUMANPLAYER_H__
#define __HUMANPLAYER_H__

#include "player.h"
#include "iplayerinput.h"

namespace othello
{
    class HumanPlayer : public Player
    {
        IPlayerInput& input;
    public:
        HumanPlayer(Board& board, Board::Color color, std::string name, IPlayerInput& input)
            : Player(board, color, name), input(input) {}

        void take_turn();
    };
}

#endif
