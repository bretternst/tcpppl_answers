#ifndef __POINT_H__
#define __POINT_H__

namespace gfxlib
{
	class Point
	{
		int x;
		int y;

	public:
		int X() const { return x; }
		void X(int xx) { x = xx; }
		int Y() const { return y; }
		void Y(int yy) { y = yy; }
		Point() : x(0), y(0) {}
		Point(int x, int y) : x(x), y(y) {}
	};
}
#endif
