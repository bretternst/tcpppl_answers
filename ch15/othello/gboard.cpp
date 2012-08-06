#include <iostream>
#include <sstream>
#include <map>
#include <X11/Xlib.h>

#include "gboard.h"

namespace othello
{
    using std::cerr;
    using std::endl;

    class GuiError {};

    GuiBoard::GuiBoard()
    {
        if(!(m_display = XOpenDisplay(NULL)))
        {
            cerr << "Unable to open display" << endl;
            throw GuiError();
        }

        m_screen = DefaultScreen(m_display);
        m_window = XCreateSimpleWindow(m_display,
                RootWindow(m_display,m_screen),
                0, 0, 480, 500, 0, 0, 0x00ff00);
        if(!m_window)
            throw GuiError();
        Atom wm_delete = XInternAtom(m_display, "WM_DELETE_WINDOW",
                false);
        XSetWMProtocols(m_display, m_window, &wm_delete, 1);
        XSelectInput(m_display, m_window,
                StructureNotifyMask | ExposureMask | ButtonPressMask);
        XMapWindow(m_display, m_window);

        m_font = XLoadQueryFont(m_display, "10x20");

        for(;;) {
            XEvent e;
            XNextEvent(m_display, &e);
            if(e.type == MapNotify)
                break;
        }
    }

    GuiBoard::~GuiBoard()
    {
        XCloseDisplay(m_display);
    }

    void GuiBoard::draw()
    {
        int squareWidth = WinWidth / NumberOfColumns;
        int squareHeight = (WinHeight - TextHeight) / NumberOfRows;

        XGCValues values;
        values.font = m_font->fid;
        GC gc = XCreateGC(m_display, m_window, GCFont, &values);
        if(gc < 0)
            throw GuiError();
        XSetFillStyle(m_display, gc, FillSolid);
        XSetForeground(m_display, gc, BlackPixel(m_display, m_screen));
        XSetLineAttributes(m_display, gc, 2, LineSolid, CapRound, JoinRound);

        for(int i = 1; i <= NumberOfColumns-1; i++) {
            int x = squareWidth * i;
            XDrawLine(m_display, m_window, gc, x, 0, x, WinHeight);
        }
        for (int i = 1; i <= NumberOfRows-1; i++) {
            int y = squareHeight * i;
            XDrawLine(m_display, m_window, gc, 0, y, WinWidth, y);
        }
        XSetForeground(m_display, gc, WhitePixel(m_display, m_screen));
        XFillRectangle(m_display, m_window, gc, 0, WinHeight-TextHeight,
                WinWidth, TextHeight);

        for(int y = 0; y < NumberOfRows; y++) {
            for(int x = 0; x < NumberOfColumns; x++) {
                Color c = space(Position(x,y));
                if(c != Empty) {
                   XSetForeground(m_display, gc,
                           c == White ? WhitePixel(m_display, m_screen) :
                           BlackPixel(m_display, m_screen));
                   XFillArc(m_display, m_window, gc,
                           squareWidth * x + 5, squareWidth * y + 5,
                           squareWidth - 10, squareHeight - 10, 0, 360*64);
                }
            }
        }
        XSetForeground(m_display, gc, BlackPixel(m_display, m_screen));
        const std::string& input = prompt();
        XDrawString(m_display, m_window, gc, 5, WinHeight - 3,
                input.c_str(), input.size());
        XFlush(m_display);
        XFreeGC(m_display, gc);
    }

    Position GuiBoard::get_move() {
        XEvent event;
        for(;;) {
            XNextEvent(m_display, &event);
            if (event.type == Expose)
                this->draw();
            else if (event.type == ButtonPress && event.xbutton.button == Button1) {
                int x = event.xbutton.x;
                int y = event.xbutton.y;
                x = x / (WinWidth / NumberOfColumns);
                y = y / ((WinHeight - TextHeight) / NumberOfRows);
                return Position(x,y);
            }
        }
    }

    void GuiBoard::alert(std::string msg)
    {
        // for the gui versions, we'll just ignore bad moves
        // but display a warning on the console
        std::cout << msg << std::endl;
    }

    void GuiBoard::end_game() {
        // stick around until the user closes the window.
        Atom wm_delete = XInternAtom(m_display, "WM_DELETE_WINDOW",
                false);
        XSetWMProtocols(m_display, m_window, &wm_delete, 1);
        XEvent event;
        for(;;) {
            XNextEvent(m_display, &event);
            if(event.xclient.data.l[0] == wm_delete)
                break;
        }
    }
}
