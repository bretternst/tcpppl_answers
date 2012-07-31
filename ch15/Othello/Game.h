#ifndef __GAME_H__
#define __GAME_H__

#include "Board.h"
#include "Player.h"

namespace Othello
{
	class Game
	{
		Board& b;
		Player* p1;
		Player* p2;
		Player* currentPlayer;

		void ChangePlayer();

	public:
		class PlayerColorError {};

		Game(Board& board, Player& player1, Player& player2);

		void Play();
	};
}

#endif
