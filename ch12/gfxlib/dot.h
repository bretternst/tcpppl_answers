#ifndef __DOT_H__
#define __DOT_H__

#include "shape.h"
#include "window.h"

namespace gfxlib
{
    class Dot : public Shape
    {
        Point pos;
    public:
        Dot(const Point& pos) : pos(pos) {}
        virtual void draw(Window&) const;
        virtual const Point n() const { return pos; }
        virtual const Point s() const { return pos; }
        virtual const Point w() const { return pos; }
        virtual const Point e() const { return pos; }
        virtual const Point nw() const { return pos; }
        virtual const Point ne() const { return pos; }
        virtual const Point sw() const { return pos; }
        virtual const Point se() const { return pos; }
        virtual const Point c() const { return pos; }
    };
}

#endif
