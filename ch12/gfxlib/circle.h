#ifndef __CIRCLE_H__
#define __CIRCLE_H__

#include "shape.h"
#include "window.h"

namespace gfxlib
{
    class Circle : public Shape
    {
        Point m_c;
        int m_r;
    public:
        Circle(const Point& c, const int r) : m_c(c),m_r(r) {}
        virtual void draw(Window& s) const;
        virtual const Point n() const { return Point(m_c.x(),m_c.y()-m_r); }
        virtual const Point s() const { return Point(m_c.x(),m_c.y()+m_r); }
        virtual const Point w() const { return Point(m_c.x()-m_r,m_c.y()); }
        virtual const Point e() const { return Point(m_c.x()+m_r,m_c.y()); }
        virtual const Point nw() const { return Point(m_c.x()-m_r,m_c.y()-m_r); }
        virtual const Point ne() const { return Point(m_c.x()+m_r,m_c.y()-m_r); }
        virtual const Point sw() const { return Point(m_c.x()-m_r,m_c.y()+m_r); }
        virtual const Point se() const { return Point(m_c.x()+m_r,m_c.y()+m_r); }
        virtual const Point c() const { return m_c; }
    };
}

#endif
