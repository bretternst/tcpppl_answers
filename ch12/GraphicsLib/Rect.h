#ifndef __RECT_H__
#define __RECT_H__

#include "Shape.h"
#include "Surface.h"

namespace GraphicsLib
{
	class Rect : public Shape
	{
		Point sw;
		Point ne;

		void DrawSide(Surface&, Point, Point) const;
		void DrawFill(Surface&) const;
	public:
		Rect(const Point& sw, const Point& ne) : sw(sw), ne(ne) {}
		virtual void Draw(Surface&) const;
		virtual const Point N() const { return Point(C().X(),ne.Y()); }
		virtual const Point S() const { return Point(C().X(),sw.Y()); }
		virtual const Point W() const { return Point(sw.X(),C().Y()); }
		virtual const Point E() const { return Point(ne.X(),C().Y()); }
		virtual const Point NW() const { return Point(sw.X(),ne.Y()); }
		virtual const Point NE() const { return ne; }
		virtual const Point SW() const { return sw; }
		virtual const Point SE() const { return Point(ne.X(),sw.Y()); }
		virtual const Point C() const { return Point((sw.X()+ne.X())/2,(sw.Y()+ne.Y())/2); }
	};
}

#endif
