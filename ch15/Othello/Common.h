#ifndef __COMMON_H__
#define __COMMON_H__

namespace Othello
{
	struct Position
	{
		int X;
		int Y;
		Position() : X(0), Y(0) {}
		Position(int x, int y) : X(x), Y(y) {}
	};
}

#endif
