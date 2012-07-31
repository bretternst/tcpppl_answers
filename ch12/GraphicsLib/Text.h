#ifndef __TEXT_H__
#define __TEXT_H__

#include <string>

#include "Shape.h"
#include "Point.h"
#include "Surface.h"

namespace GraphicsLib
{
	class Text : public Shape
	{
	private:
		Point p1;
		Point p2;
		std::wstring str;

	public:
		Text(const Point& p1, const Point& p2, const std::wstring str) : p1(p1), p2(p2), str(str) {}
		virtual void Draw(Surface&) const;
		virtual const Point N() const { return Point(C().X(),p1.Y()<p2.Y()?p1.Y():p2.Y()); }
		virtual const Point S() const { return Point(C().X(),p1.Y()>p2.Y()?p1.Y():p2.Y()); }
		virtual const Point W() const { return Point(p1.X()<p2.X()?p1.X():p2.X(),C().Y()); }
		virtual const Point E() const { return Point(p1.X()>p2.X()?p1.X():p2.X(),C().Y()); }
		virtual const Point NW() const { return Point(W().X(),N().Y()); }
		virtual const Point NE() const { return Point(E().X(),N().Y()); }
		virtual const Point SW() const { return Point(W().X(),S().Y()); }
		virtual const Point SE() const { return Point(E().X(),S().Y()); }
		virtual const Point C() const { return Point((p1.X()+p2.X())/2, (p1.Y()+p2.Y())/2); }
	};
}

#endif
