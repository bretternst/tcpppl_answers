#ifndef __SHAPE_H__
#define __SHAPE_H__

#include "point.h"
#include "surface.h"

namespace gfxlib
{
    class Line;

    class Shape
    {
    public:
        enum LineStyle { LINE_SOLID, LINE_DASHED, LINE_DOTTED };

    private:
        int thickness;
        LineStyle outline;
        Color stroke;
        Color fill;

    protected:
        bool Dash(int y) const;
        bool Dot(int y) const;

    public:
        virtual void Draw(Surface&) const = 0;
        virtual const Point N() const = 0;
        virtual const Point S() const = 0;
        virtual const Point W() const = 0;
        virtual const Point E() const = 0;
        virtual const Point NW() const = 0;
        virtual const Point NE() const = 0;
        virtual const Point SW() const = 0;
        virtual const Point SE() const = 0;
        virtual const Point C() const = 0;

        Shape() : thickness(2), outline(LINE_SOLID), stroke(0xff000000), fill(0xffffffff) {}
        int Thickness() const { return thickness; }
        void Thickness(int t) { thickness = t; }
        LineStyle Outline() const { return outline; }
        void Outline(LineStyle style) { outline = style; }
        int Stroke() const { return stroke; }
        void Stroke(int s) { stroke = s; }
        int Fill() const { return fill; }
        void Fill(int f) { fill = f; }

        Line Connect(Shape* s) const;
    };
}

#endif
