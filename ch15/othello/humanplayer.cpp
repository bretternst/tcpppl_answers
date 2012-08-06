#include "humanplayer.h"

namespace othello
{
    void HumanPlayer::take_turn()
    {
        int flips = 0;
        while(!flips)
        {
            Position p = input.get_move();
            if ((flips = get_board().play(color(), p)) < 1)
            {
                input.alert("That is not a valid move.");
            }
        }
        add_points(flips+1);
    }
}
