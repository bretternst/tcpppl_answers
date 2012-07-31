#include "Game.h"

namespace Othello
{
	Game::Game(Board& board, Player& player1, Player& player2) : b(board), p1(&player1), p2(&player2), currentPlayer(0)
	{
		int midX = Board::NumberOfColumns / 2 - 1;
		int midY = Board::NumberOfRows / 2 - 1;
		board.Space(Position(midX,midY)) = Board::White;
		board.Space(Position(midX+1,midY)) = Board::Black;
		board.Space(Position(midX,midY+1)) = Board::Black;
		board.Space(Position(midX+1,midY+1)) = Board::White;

		if(!(player1.Color() == Board::White && player2.Color() == Board::Black) &&
			!(player1.Color() == Board::Black && player2.Color() == Board::White))
		{
			throw PlayerColorError();
		}
	}

	void Game::Play()
	{
		Player* currentPlayer = (p1->Color() == Board::Black ? p1 : p2);

		bool skipped = false;
		for(;;)
		{
			if(!b.HasLegalMove(currentPlayer->Color()))
			{
				if(skipped)
					break;
				else
				{
					skipped = true;
					continue;
				}
			}
			else
				skipped = false;

			b.Update("It is "+currentPlayer->Name()+"'s turn.");
			currentPlayer->TakeTurn();
			currentPlayer = (currentPlayer == p1 ? p2 : p1);
		}

		// count up total points
		int points1 = 0, points2 = 0;
		for(int y = 0; y < Board::NumberOfRows; y++)
		{
			for(int x = 0; x < Board::NumberOfColumns; x++)
			{
				if(b.Space(Position(x,y)) == p1->Color())
					points1++;
				else
					points2++;
			}
		}
		if(p1->Points() != p2->Points())
			b.Update("Game over - " + (p1->Points() > p2->Points() ? p1->Name() : p2->Name()) + " is the winner.");
		else
			b.Update("Game over - it was a tie.");

		b.EndGame();
	}
}
