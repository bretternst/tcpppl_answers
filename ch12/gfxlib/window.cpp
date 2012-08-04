#include <cstdlib>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "window.h"
#include "line.h"
#include "text.h"
#include <iostream>

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

    void Window::draw(Shape *shape) {
        // since Xlib doesn't provide a way to draw text to an image, we have
        // to hack around that and store copies of all text objects to draw
        // whenever the window repaints.
        Text *t = dynamic_cast<Text*>(shape);
        if(t) {
            m_text.push_back(Text(*t));
        } else {
            shape->draw(*this);
        }
        m_current = shape->se();
    }

    void Window::paint() {
        GC gc = XCreateGC(m_display, m_window, 0, 0);
        XPutImage(m_display, m_window, gc, m_img, 0, 0, 0, 0, m_w, m_h);
        XFreeGC(m_display, gc);

        for(std::vector<Text>::iterator i = m_text.begin(); i != m_text.end(); i++) {
            i->draw(*this);
        }
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

    void Window::draw_text(Point p1, Point p2, const std::string& str, Color c)
    {
        // Perform clipping, to maintain consistency with exercise 5
        int height = p2.y()-p1.y();
        if(p1.x() < 0) p1.x(0);
        if(p1.y() < 0) p1.y(0);
        if(p2.x() > m_w) p2.x(m_w);
        if(p2.y() > m_h) p2.y(m_h);

        XFontStruct *font;
        font = XLoadQueryFont(m_display, "6x10");
        XGCValues values;
        values.font = font->fid;
        values.foreground = BlackPixel(m_display,DefaultScreen(m_display));
        values.background = WhitePixel(m_display,DefaultScreen(m_display));

        GC gc = XCreateGC(m_display, m_window, GCFont | GCForeground | GCBackground, &values);
        XRectangle clip;
        clip.x = p1.x();
        clip.y = p1.y();
        clip.width = p2.x() - p1.x();
        clip.height = p2.y() - p1.y();
        XSetClipRectangles(m_display, gc, 0, 0, &clip, 1, Unsorted);
        XDrawString(m_display, m_window, gc, p1.x(), p2.y(), str.c_str(), str.size());
    }
}
