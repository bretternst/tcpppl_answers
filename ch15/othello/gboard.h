#ifndef __GUIBOARD_H__
#define __GUIBOARD_H__

#include <map>
#include <X11/Xlib.h>

#include "board.h"
#include "iplayerinput.h"

namespace othello
{
    class GuiBoard : public Board
    {
        static const int WinWidth = 480;
        static const int WinHeight = 500;
        static const int TextHeight = 20;
        
        Display* m_display;
        Window m_window;
        int m_screen;
        XFontStruct* m_font;

    public:
        GuiBoard();
        ~GuiBoard();
        void draw();
        Position get_move();
        void alert(std::string msg);
        void end_game();
    };
}

#endif
