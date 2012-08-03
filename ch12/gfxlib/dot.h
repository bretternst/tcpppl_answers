#ifndef __DOT_H__
#define __DOT_H__

#include "shape.h"
#include "surface.h"

namespace gfxlib
{
	class Dot : public Shape
	{
		Point pos;
	public:
		Dot(const Point& pos) : pos(pos) {}
		virtual void Draw(Surface&) const;
		virtual const Point N() const { return pos; }
		virtual const Point S() const { return pos; }
		virtual const Point W() const { return pos; }
		virtual const Point E() const { return pos; }
		virtual const Point NW() const { return pos; }
		virtual const Point NE() const { return pos; }
		virtual const Point SW() const { return pos; }
		virtual const Point SE() const { return pos; }
		virtual const Point C() const { return pos; }
	};
}

#endif
