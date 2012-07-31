#ifndef __BOARD_H__
#define __BOARD_H__

#include <string>

#include "Common.h"
#include "IPlayerInput.h"

namespace Othello
{
	class Board : public IPlayerInput
	{
	public:
		enum Color
		{
			Empty,
			Black,
			White
		};
		class OutOfRangeError {};

		static const int NumberOfRows = 8;
		static const int NumberOfColumns = 8;

	private:
		Color spaces[NumberOfRows][NumberOfColumns];
		std::string prompt;

		void CheckRange(Position p) const;

	protected:
		const std::string& Prompt() const { return prompt; }

	public:
		Board();
		virtual ~Board() {}
		Color Space(Position p) const;
		Color& Space(Position p);
		void Update(const std::string& prompt);
		virtual void Draw() = 0;

		int FindPath(Color c, Position p, int xDelta, int yDelta) const;
		int FlipPath(Color c, Position p, int xDelta, int yDelta);
		int SimulatePlay(Color c, Position p) const;
		int Play(Color c, Position p);
		bool HasLegalMove(Color c);
		virtual void EndGame() {}
	};

	inline void Board::CheckRange(Position p) const
	{
		if(p.Y < 0 || p.Y >= NumberOfRows || p.X < 0 || p.X >= NumberOfRows)
			throw OutOfRangeError();
	}
}

#endif
