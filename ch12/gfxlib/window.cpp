#include <cstdlib>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "window.h"
#include "line.h"

namespace gfxlib
{
    Window::Window(Display *display, int w, int h, Color background = 0xFFFFFFFF) : m_current(0,0), m_display(display), m_w(w), m_h(h)
    {
        int s = DefaultScreen(display);
        m_window = XCreateSimpleWindow(display, RootWindow(display,s), 0, 0, w, h, 0, 0, 0xffffff);
        XMapWindow(display, m_window);

        // supposed to be destroyed by XDestroyImage
        char *bitmap = (char*)malloc(3 * w * h);
        m_img = XCreateImage(display, DefaultVisual(display, s), 24, XYPixmap, 0, bitmap, w, h, 32, 0);

        // clear the bitmap (probably a faster way to do this, but whatever)
        for(int i = 0; i < w; i++)
            for(int j = 0; j < h; j++)
                XPutPixel(m_img, i, j, background);

        XSync(display, s);
    }

    Window::~Window()
    {
        XDestroyImage(m_img);
        XDestroyWindow(m_display, m_window);
    }

    void Window::paint() {
        GC gc = XCreateGC(m_display, m_window, 0, 0);
        XPutImage(m_display, m_window, gc, m_img, 0, 0, 0, 0, m_w, m_h);
        XFreeGC(m_display, gc);
    }

    void Window::draw_dot(const Point p, int thickness, Color c)
    {
        if(thickness==0) return;

        // Added in exercise 5
        if(p.x() < 0 || p.y() < 0 || p.x() >= m_w || p.y() >= m_h) return;

        for(int i = p.x()-((thickness-1)/2); i < p.x()+(thickness/2)+1; i++)
        {
            for(int j = p.y()-((thickness-1)/2); j < p.y()+(thickness/2)+1; j++)
            {
                set_pixel(Point(i,j),c);
            }
        }
    }
    
    void Window::set_pixel(Point p, Color c) {
        XPutPixel(m_img, p.x(), p.y(), c);
    }

    void Window::draw_text(Point p1, Point p2, const std::wstring& str, Color c)
    {
        // Perform clipping, to maintain consistency with exercise 5
        int height = p2.y()-p1.y();
        if(p1.x() < 0) p1.x(0);
        if(p1.y() < 0) p1.y(0);
        if(p2.x() > m_w) p2.x(m_w);
        if(p2.y() > m_h) p2.y(m_h);

        /*
        Graphics g(&bmp);
        Font f(&FontFamily(L"Tahoma"), static_cast<REAL>(height), Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
        RectF bounds(static_cast<REAL>(p1.X()),static_cast<REAL>(p1.Y()),
            static_cast<REAL>(p2.X()-p1.X()),static_cast<REAL>(p2.Y()-p1.Y()));

        g.DrawString(&str[0], str.size(), &f, bounds,
                NULL,&SolidBrush(Gdiplus::Color(c)));
                */
    }
}
