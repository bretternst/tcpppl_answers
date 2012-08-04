#include "rect.h"
#include "line.h"

namespace gfxlib
{
    void Rect::draw(Window& s) const
    {
        Color stroke = this->stroke();

        draw_side(s,nw(),m_sw);
        draw_side(s,nw(),m_ne);
        draw_side(s,m_ne,se());
        draw_side(s,m_sw,se());

        draw_fill(s);
    }

    void Rect::draw_side(Window& s, Point p1, Point p2) const
    {
        Line side(p1,p2);
        side.thickness(thickness());
        side.outline(outline());
        side.stroke(stroke());
        side.draw(s);
    }

    void Rect::draw_fill(Window& s) const
    {
        int fill = this->fill();
        for(int i = m_sw.x()+thickness(); i < m_ne.x(); i++)
        {
            for(int j = m_ne.y()+thickness(); j < m_sw.y(); j++)
            {
                s.set_pixel(Point(i,j),fill);
            }
        }
    }
}
