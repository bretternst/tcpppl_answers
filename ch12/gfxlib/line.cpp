#include <cmath>
#include <cstdlib>

#include "line.h"
#include "window.h"

namespace gfxlib
{
    const double PI = 3.1415926;

    void Line::draw(Window& s) const
    {
        bool steep = abs(p2.y()-p1.y()) > abs(p2.x()-p1.x());
        Point pp1 = steep ? Point(p1.y(),p1.x()) : Point(p1.x(),p1.y());
        Point pp2 = steep ? Point(p2.y(),p2.x()) : Point(p2.x(),p2.y());
        int thickness = this->thickness();
        LineStyle outline = this->outline();
        Color stroke = this->stroke();

        // Handle vertical lines as a special case.
        if(pp1.x()==pp2.x())
        {
            int x = pp1.x();
            int start = std::min(pp1.y(),pp2.y());
            int end = std::max(pp1.y(),pp2.y());
            for(int y = start; y <= end; y++)
            {
                if(outline == LINE_SOLID || (outline == LINE_DASHED && dash(y)) || (outline == LINE_DOTTED && dot(y)))
                {
                    s.draw_dot(Point(x,y),thickness,stroke);
                }
            }
            return;
        }

        if(pp1.x() > pp2.x())
        {
            Point tmp = pp2;
            pp2 = pp1;
            pp1 = tmp;
        }
        int deltaX = pp2.x()-pp1.x();
        int deltaY = abs(pp2.y()-pp1.y());
        float error = 0;
        float deltaErr = deltaX == 0 ? 0 : static_cast<float>(deltaY) / static_cast<float>(deltaX);
        int yStep;
        int y = pp1.y();
        yStep = pp1.y() < pp2.y() ? 1 : -1;
        for(int x = pp1.x(); x <= pp2.x(); x++)
        {
            if(outline == LINE_SOLID || (outline == LINE_DASHED && dash(x)) || (outline == LINE_DOTTED && dot(x)))
            {
                if(steep)
                    s.draw_dot(Point(y,x),thickness,stroke);
                else
                    s.draw_dot(Point(x,y),thickness,stroke);
            }
            error += deltaErr;
            if(error >= 0.5)
            {
                y += yStep;
                error -= 1.0;
            }
        }

        draw_arrowhead(s, p1, p2, a1);
        draw_arrowhead(s, p2, p1, a2);
    }

    void Line::draw_arrowhead(Window& s, const Point ap1, const Point ap2, ArrowStyle style) const
    {
        double z = 0.0L;

        if(style == ARROW_IN)
        {
            z = PI / 4;
        }
        else if (style == ARROW_OUT)
        {
            z = PI * 0.75L;
        }
        else
            return;

        Point q(ap2.x()-ap1.x(),ap2.y()-ap1.y());
        double len = sqrt(static_cast<double>(q.x()*q.x() + q.y()*q.y()));
        Point q1(static_cast<int>(cos(z)*q.x()-sin(z)*q.y()), static_cast<int>(sin(z)*q.x()+cos(z)*q.y()));
        q1.x(static_cast<int>(q1.x() / len * thickness()*4) + ap1.x());
        q1.y(static_cast<int>(q1.y() / len * thickness()*4) + ap1.y());
        Line a1(ap1, q1);
        a1.thickness(thickness());
        a1.stroke(stroke());
        a1.draw(s);

        q1 = Point(static_cast<int>(cos(z)*q.x()+sin(z)*q.y()), static_cast<int>(-sin(z)*q.x()+cos(z)*q.y()));
        q1.x(static_cast<int>(q1.x() / len * thickness()*4) + ap1.x());
        q1.y(static_cast<int>(q1.y() / len * thickness()*4) + ap1.y());
        a1 = Line(ap1, q1);
        a1.thickness(thickness());
        a1.stroke(stroke());
        a1.draw(s);
    }

    void Line::arrowhead(ArrowPosition pos, ArrowStyle style)
    {
        if(pos == ARROW_START)
            a1 = style;
        else if (pos == ARROW_END)
            a2 = style;
    }
}
