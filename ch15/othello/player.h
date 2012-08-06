#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "board.h"

namespace othello
{
    class Player
    {
        Board& b;
        Board::Color c;
        std::string n;
        int points;

    protected:
        Board& get_board() { return b; }

    public:
        Player(Board& board, Board::Color color, std::string name) : b(board), c(color), n(name), points(0) {}

        Board::Color color() const { return c; }
        const std::string& name() const { return n; }
        virtual void take_turn() = 0;
        void add_points(int n) { points += n; }
        int score() { return points; }
    };
}

#endif
