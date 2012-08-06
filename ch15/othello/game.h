#ifndef __GAME_H__
#define __GAME_H__

#include "board.h"
#include "player.h"

namespace othello
{
    class Game
    {
        Board& b;
        Player* p1;
        Player* p2;
        Player* currentPlayer;

        void change_player();

    public:
        class PlayerColorError {};

        Game(Board& board, Player& player1, Player& player2);

        void play();
    };
}

#endif
