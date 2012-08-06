#include "aiplayer.h"

namespace othello
{
    void AIPlayer::take_turn()
    {
        Board& b = get_board();

        Position p;
        int score = 0;

        // find the "best" move
        for(int y = 0; y < Board::NumberOfRows; y++)
        {
            for(int x = 0; x < Board::NumberOfColumns; x++)
            {
                Position candidate(x,y);
                int potentialScore = get_board().simulate_play(color(), candidate);
                if(potentialScore > score)
                {
                    score = potentialScore;
                    p = candidate;
                }
            }
        }

        add_points(b.play(color(), p) + 1);
    }
}
