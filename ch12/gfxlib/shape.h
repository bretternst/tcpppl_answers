#ifndef __SHAPE_H__
#define __SHAPE_H__

#include <stdint.h>
#include "point.h"

namespace gfxlib
{
    class Line;
    class Window;

    typedef uint32_t Color;

    class Shape
    {
    public:
        enum LineStyle { LINE_SOLID, LINE_DASHED, LINE_DOTTED };

    private:
        int m_thickness;
        LineStyle m_outline;
        Color m_stroke;
        Color m_fill;

    protected:
        bool dash(int y) const;
        bool dot(int y) const;

    public:
        virtual void draw(Window&) const = 0;
        virtual const Point n() const = 0;
        virtual const Point s() const = 0;
        virtual const Point w() const = 0;
        virtual const Point e() const = 0;
        virtual const Point nw() const = 0;
        virtual const Point ne() const = 0;
        virtual const Point sw() const = 0;
        virtual const Point se() const = 0;
        virtual const Point c() const = 0;

        Shape() : m_thickness(2), m_outline(LINE_SOLID), m_stroke(0xff000000), m_fill(0xffffffff) {}
        int thickness() const { return m_thickness; }
        void thickness(int t) { m_thickness = t; }
        LineStyle outline() const { return m_outline; }
        void outline(LineStyle style) { m_outline = style; }
        int stroke() const { return m_stroke; }
        void stroke(int s) { m_stroke = s; }
        int fill() const { return m_fill; }
        void fill(int f) { m_fill = f; }

        Line connect(Shape* s) const;
    };
}

#endif
