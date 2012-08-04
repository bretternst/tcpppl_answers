#include "AIPlayer.h"

namespace Othello
{
    void AIPlayer::TakeTurn()
    {
        Board& b = GetBoard();

        Position p;
        int score = 0;

        // find the "best" move
        for(int y = 0; y < Board::NumberOfRows; y++)
        {
            for(int x = 0; x < Board::NumberOfColumns; x++)
            {
                Position candidate(x,y);
                int potentialScore = GetBoard().SimulatePlay(Color(), candidate);
                if(potentialScore > score)
                {
                    score = potentialScore;
                    p = candidate;
                }
            }
        }

        AddPoints(b.Play(Color(), p) + 1);
    }
}
