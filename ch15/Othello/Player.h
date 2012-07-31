#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Board.h"

namespace Othello
{
	class Player
	{
		Board& b;
		Board::Color c;
		std::string n;
		int points;

	protected:
		Board& GetBoard() { return b; }

	public:
		Player(Board& board, Board::Color color, std::string name) : b(board), c(color), n(name), points(0) {}

		Board::Color Color() const { return c; }
		const std::string& Name() const { return n; }
		virtual void TakeTurn() = 0;
		void AddPoints(int n) { points += n; }
		int Points() { return points; }
	};
}

#endif
