#include "circle.h"
#include "window.h"

namespace gfxlib
{
    void fill_run(Window& s, Color c, int y, int x1, int x2)
    {
        for(int i = x1; i <= x2; i++)
            s.set_pixel(Point(i,y), c);
    }

    void Circle::draw(Window& s) const
    {
        int f = 1 - m_r;
        int ddfx = 1;
        int ddfy = -2 * m_r;
        int x = 0;
        int y = m_r;
        int thickness = this->thickness();
        LineStyle outline = this->outline();
        Color stroke = this->stroke();
        Color fill = this->fill();

        s.draw_dot(Point(m_c.x(),m_c.y()+m_r),thickness,stroke);
        s.draw_dot(Point(m_c.x(),m_c.y()-m_r),thickness,stroke);
        s.draw_dot(Point(m_c.x()+m_r,m_c.y()),thickness,stroke);
        s.draw_dot(Point(m_c.x()-m_r,m_c.y()),thickness,stroke);
        fill_run(s, fill, m_c.y(), m_c.x()-m_r+1, m_c.x()+m_r-1);

        while(x < y)
        {
            if(f>=0)
            {
                y--;
                ddfy += 2;
                f += ddfy;
            }
            x++;
            ddfx += 2;
            f += ddfx;
            
            fill_run(s, fill, m_c.y()+y, m_c.x()-x+1, m_c.x()+x-1);
            fill_run(s, fill, m_c.y()-y, m_c.x()-x+1, m_c.x()+x-1);
            fill_run(s, fill, m_c.y()+x, m_c.x()-y+1, m_c.x()+y-1);
            fill_run(s, fill, m_c.y()-x, m_c.x()-y+1, m_c.x()+y-1);

            if(outline == LINE_SOLID || (outline == LINE_DASHED && dash(x)) || (outline == LINE_DOTTED && dot(x)))
            {
                s.draw_dot(Point(m_c.x()+x, m_c.y()+y),thickness,stroke);
                s.draw_dot(Point(m_c.x()-x, m_c.y()+y),thickness,stroke);

                s.draw_dot(Point(m_c.x()+x, m_c.y()-y),thickness,stroke);
                s.draw_dot(Point(m_c.x()-x, m_c.y()-y),thickness,stroke);

                s.draw_dot(Point(m_c.x()+y, m_c.y()+x),thickness,stroke);
                s.draw_dot(Point(m_c.x()-y, m_c.y()+x),thickness,stroke);

                s.draw_dot(Point(m_c.x()+y, m_c.y()-x),thickness,stroke);
                s.draw_dot(Point(m_c.x()-y, m_c.y()-x),thickness,stroke);
            }
        }
    }
}
