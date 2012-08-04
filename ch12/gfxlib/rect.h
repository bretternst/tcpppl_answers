#ifndef __RECT_H__
#define __RECT_H__

#include "shape.h"
#include "window.h"

namespace gfxlib
{
    class Rect : public Shape
    {
        Point m_sw;
        Point m_ne;

        void draw_side(Window&, Point, Point) const;
        void draw_fill(Window&) const;
    public:
        Rect(const Point& sw, const Point& ne) : m_sw(sw), m_ne(ne) {}
        virtual void draw(Window&) const;
        virtual const Point n() const { return Point(c().x(),m_ne.y()); }
        virtual const Point s() const { return Point(c().x(),m_sw.y()); }
        virtual const Point w() const { return Point(m_sw.x(),c().y()); }
        virtual const Point e() const { return Point(m_ne.x(),c().y()); }
        virtual const Point nw() const { return Point(m_sw.x(),m_ne.y()); }
        virtual const Point ne() const { return m_ne; }
        virtual const Point sw() const { return m_sw; }
        virtual const Point se() const { return Point(m_ne.x(),m_sw.y()); }
        virtual const Point c() const { return Point((m_sw.x()+m_ne.x())/2,(m_sw.y()+m_ne.y())/2); }
    };
}

#endif
