#include <string>
#include <iostream>

#include "Board.h"

namespace Othello
{
    Board::Board()
    {
        for(int i = 0; i < NumberOfRows; i++)
            for(int j = 0; j < NumberOfColumns; j++)
                spaces[i][j] = Empty;
    }

    Board::Color& Board::Space(Position p)
    {
        CheckRange(p);
        return spaces[p.Y][p.X];
    }

    Board::Color Board::Space(Position p) const
    {
        CheckRange(p);
        return spaces[p.Y][p.X];
    }

    void Board::Update(const std::string& msg)
    {
        prompt = msg;
        this->Draw();
    }

    int Board::FindPath(Color c, Position p, int xDelta, int yDelta) const
    {
        bool count = 0;

        for(;;)
        {
            p.X += xDelta;
            p.Y += yDelta;
            if(p.X < 0 || p.Y < 0 || 
                p.X >= Board::NumberOfColumns || 
                p.Y >= Board::NumberOfRows || 
                Space(p) == Board::Empty)
            {
                return 0;
            }

            if(Space(p) == c)
                return count;
            else
                count++;
        }
    }

    int Board::FlipPath(Color c, Position p, int xDelta, int yDelta)
    {
        int count = 0;
        if(FindPath(c, p, xDelta, yDelta))
        {
            for(;;)
            {
                p.X += xDelta;
                p.Y += yDelta;
                if(Space(p) == c)
                    break;
                else
                {
                    Space(p) = c;
                    count++;
                }
            }
        }
        return count;
    }

    int Board::Play(Color c, Position p)
    {
        int flips = FlipPath(c, p, -1, -1);
        flips += FlipPath(c, p, -1, -1);
        flips += FlipPath(c, p, 0, -1);
        flips += FlipPath(c, p, 1, -1);
        flips += FlipPath(c, p, 1, 0);
        flips += FlipPath(c, p, 1, 1);
        flips += FlipPath(c, p, 0, 1);
        flips += FlipPath(c, p, -1, 1);
        flips += FlipPath(c, p, -1, 0);
        if(flips > 0) Space(p) = c;
        return flips;
    }

    int Board::SimulatePlay(Color c, Position p) const
    {
        if(Space(p) != Empty) return 0;
        int flips = FindPath(c, p, -1, -1);
        flips += FindPath(c, p, -1, -1);
        flips += FindPath(c, p, 0, -1);
        flips += FindPath(c, p, 1, -1);
        flips += FindPath(c, p, 1, 0);
        flips += FindPath(c, p, 1, 1);
        flips += FindPath(c, p, 0, 1);
        flips += FindPath(c, p, -1, 1);
        flips += FindPath(c, p, -1, 0);
        return flips;
    }

    bool Board::HasLegalMove(Color c)
    {
        for(int y = 0; y < NumberOfRows; y++)
        {
            for(int x = 0; x < NumberOfColumns; x++)
            {
                if(SimulatePlay(c, Position(x,y)) > 0)
                {
                    return true;
                }
            }
        }
        return false;
    }
}
