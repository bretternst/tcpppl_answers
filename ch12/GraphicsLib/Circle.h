#ifndef __CIRCLE_H__
#define __CIRCLE_H__

#include "Shape.h"
#include "Surface.h"

namespace GraphicsLib
{
	class Circle : public Shape
	{
		Point c;
		int r;
	public:
		Circle(const Point& c, const int r) : c(c),r(r) {}
		virtual void Draw(Surface& s) const;
		virtual const Point N() const { return Point(c.X(),c.Y()-r); }
		virtual const Point S() const { return Point(c.X(),c.Y()+r); }
		virtual const Point W() const { return Point(c.X()-r,c.Y()); }
		virtual const Point E() const { return Point(c.X()+r,c.Y()); }
		virtual const Point NW() const { return Point(c.X()-r,c.Y()-r); }
		virtual const Point NE() const { return Point(c.X()+r,c.Y()-r); }
		virtual const Point SW() const { return Point(c.X()-r,c.Y()+r); }
		virtual const Point SE() const { return Point(c.X()+r,c.Y()+r); }
		virtual const Point C() const { return c; }
	};
}

#endif
