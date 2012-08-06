#include <iostream>
#include <sstream>
#include <ncurses.h>
#include "cboard.h"

namespace othello
{
    using std::cout;
    using std::cin;
    using std::endl;
    
    ConsoleBoard::ConsoleBoard() {
        initscr();
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_GREEN);
        init_pair(2, COLOR_WHITE, COLOR_GREEN);
    }

    ConsoleBoard::~ConsoleBoard() {
        endwin();
    }

    void ConsoleBoard::draw_space(Color s)
    {
        int pair = (s == White ? COLOR_PAIR(2) : COLOR_PAIR(1));
        attron(pair);
        addch(s == Empty ? ' ' : '*');
        attroff(pair);
    }

    void ConsoleBoard::draw()
    {
        clear();

        addch(' ');
        for(int col = 0; col < NumberOfColumns; col++)
        {
            printw("%d", col+1);
        }

        for(int row = 0; row < NumberOfRows; row++)
        {
            mvprintw(row + 1, 0, "%d", row + 1);
            for(int col = 0; col < NumberOfColumns; col++)
            {
                draw_space(space(Position(col,row)));
            }
        }
        mvprintw(NumberOfRows + 2, 0, "%s\n", prompt().c_str());
        refresh();
    }

    Position ConsoleBoard::get_move()
    {
        Position p(0,0);

        for(;;)
        {
            printw("enter move in the format row,col: ");
            refresh();

            char ch = 0;

            char c[80];
            getstr(c);
            std::string line(c);
            std::istringstream rdr(line);

            rdr.get(ch);
            if(isdigit(ch))
            {
                rdr.putback(ch);
                rdr >> p.x;
            }
            else
            {
                continue;
            }

            rdr.get(ch);
            if(ch != ',') continue;

            rdr.get(ch);
            if(isdigit(ch))
            {
                rdr.putback(ch);
                rdr >> p.y;
            }
            else
            {
                continue;
            }

            if(p.x < 1 || p.y > NumberOfRows || p.y < 1 || p.y > NumberOfRows)
            {
                printw("invalid row or column #.\n");
                refresh();
                continue;
            }
            p.x--; p.y--;

            return p;
        }
    }

    void ConsoleBoard::alert(std::string msg)
    {
        printw("%s\n", msg.c_str());
        refresh();
    }
}
