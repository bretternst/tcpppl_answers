#include <string>
#include <iostream>

#include "board.h"

namespace othello
{
    Board::Board()
    {
        for(int i = 0; i < NumberOfRows; i++)
            for(int j = 0; j < NumberOfColumns; j++)
                spaces[i][j] = Empty;
    }

    Board::Color& Board::space(Position p)
    {
        check_range(p);
        return spaces[p.y][p.x];
    }

    Board::Color Board::space(Position p) const
    {
        check_range(p);
        return spaces[p.y][p.x];
    }

    void Board::update(const std::string& msg)
    {
        m_prompt = msg;
        this->draw();
    }

    int Board::find_path(Color c, Position p, int xDelta, int yDelta) const
    {
        bool count = 0;

        for(;;)
        {
            p.x += xDelta;
            p.y += yDelta;
            if(p.x < 0 || p.y < 0 || 
                p.x >= Board::NumberOfColumns || 
                p.y >= Board::NumberOfRows || 
                space(p) == Board::Empty)
            {
                return 0;
            }

            if(space(p) == c)
                return count;
            else
                count++;
        }
    }

    int Board::flip_path(Color c, Position p, int xDelta, int yDelta)
    {
        int count = 0;
        if(find_path(c, p, xDelta, yDelta))
        {
            for(;;)
            {
                p.x += xDelta;
                p.y += yDelta;
                if(space(p) == c)
                    break;
                else
                {
                    space(p) = c;
                    count++;
                }
            }
        }
        return count;
    }

    int Board::play(Color c, Position p)
    {
        int flips = flip_path(c, p, -1, -1);
        flips += flip_path(c, p, -1, -1);
        flips += flip_path(c, p, 0, -1);
        flips += flip_path(c, p, 1, -1);
        flips += flip_path(c, p, 1, 0);
        flips += flip_path(c, p, 1, 1);
        flips += flip_path(c, p, 0, 1);
        flips += flip_path(c, p, -1, 1);
        flips += flip_path(c, p, -1, 0);
        if(flips > 0) space(p) = c;
        return flips;
    }

    int Board::simulate_play(Color c, Position p) const
    {
        if(space(p) != Empty) return 0;
        int flips = find_path(c, p, -1, -1);
        flips += find_path(c, p, -1, -1);
        flips += find_path(c, p, 0, -1);
        flips += find_path(c, p, 1, -1);
        flips += find_path(c, p, 1, 0);
        flips += find_path(c, p, 1, 1);
        flips += find_path(c, p, 0, 1);
        flips += find_path(c, p, -1, 1);
        flips += find_path(c, p, -1, 0);
        return flips;
    }

    bool Board::has_legal_move(Color c)
    {
        for(int y = 0; y < NumberOfRows; y++)
        {
            for(int x = 0; x < NumberOfColumns; x++)
            {
                if(simulate_play(c, Position(x,y)) > 0)
                {
                    return true;
                }
            }
        }
        return false;
    }
}
