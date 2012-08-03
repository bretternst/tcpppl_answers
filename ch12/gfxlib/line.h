#ifndef __LINE_H__
#define __LINE_H__

#include "shape.h"
#include "soint.h"
#include "surface.h"

namespace gfxlib
{
	class Line : public Shape
	{
	public:
		enum ArrowStyle { ARROW_NONE, ARROW_IN, ARROW_OUT };
		enum ArrowPosition { ARROW_START, ARROW_END };

	private:
		Point p1;
		Point p2;
		ArrowStyle a1;
		ArrowStyle a2;
		void DrawArrowhead(Surface& s, const Point p1, const Point p2, ArrowStyle style) const;
	public:
		Line(const Point& p1, const Point& p2) : p1(p1), p2(p2), a1(ARROW_NONE), a2(ARROW_NONE) {}
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

		void Arrowhead(ArrowPosition pos, ArrowStyle style);
	};
}

#endif
