#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <vector>
#include <map>
#include <string>
#include <X11/Xlib.h>

#include "point.h"
#include "shape.h"

namespace gfxlib
{
    class Window
    {
        Point m_current;
        Color m_background;

        int m_w;
        int m_h;
        Display *m_display;
        ::Window m_window;
        ::GC m_gc;
        ::XImage *m_img;

    public:
        Window(Display *display, int w, int h, Color background);
        ~Window();

        ::Window handle() const { return m_window; }
        void paint();
        const Point& current() const { return m_current; }
        void current(const Point& p) { m_current = p; }
        void draw(Shape* s){s->draw(*this); m_current = s->se();}
        Line connect(Shape* s1, Shape* s2);
        void set_pixel(Point p, Color c);
        void draw_dot(Point p, int thickness, Color c);
        void draw_text(Point p1, Point p2, const std::wstring& str, Color c);
    };
}
#endif
