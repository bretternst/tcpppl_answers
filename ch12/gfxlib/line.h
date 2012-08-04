#ifndef __LINE_H__
#define __LINE_H__

#include "shape.h"
#include "window.h"

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
        void draw_arrowhead(Window& s, const Point p1, const Point p2, ArrowStyle style) const;
    public:
        Line(const Point& p1, const Point& p2) : p1(p1), p2(p2), a1(ARROW_NONE), a2(ARROW_NONE) {}
        virtual void draw(Window&) const;
        virtual const Point n() const { return Point(c().x(),p1.y()<p2.y()?p1.y():p2.y()); }
        virtual const Point s() const { return Point(c().x(),p1.y()>p2.y()?p1.y():p2.y()); }
        virtual const Point w() const { return Point(p1.x()<p2.x()?p1.x():p2.x(),c().y()); }
        virtual const Point e() const { return Point(p1.x()>p2.x()?p1.x():p2.x(),c().y()); }
        virtual const Point nw() const { return Point(w().x(),n().y()); }
        virtual const Point ne() const { return Point(e().x(),n().y()); }
        virtual const Point sw() const { return Point(w().x(),s().y()); }
        virtual const Point se() const { return Point(e().x(),s().y()); }
        virtual const Point c() const { return Point((p1.x()+p2.x())/2, (p1.y()+p2.y())/2); }

        void arrowhead(ArrowPosition pos, ArrowStyle style);
    };
}

#endif
