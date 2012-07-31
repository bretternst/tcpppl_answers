#include "HumanPlayer.h"

namespace Othello
{
	void HumanPlayer::TakeTurn()
	{
		int flips = 0;
		while(!flips)
		{
			Position p = input.GetMove();
			if ((flips = GetBoard().Play(Color(), p)) < 1)
			{
				input.Alert("That is not a valid move.");
			}
		}
		AddPoints(flips+1);
	}
}
