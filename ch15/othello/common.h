#ifndef __COMMON_H__
#define __COMMON_H__

namespace othello
{
    struct Position
    {
        int x;
        int y;
        Position() : x(0), y(0) {}
        Position(int x, int y) : x(x), y(y) {}
    };
}

#endif
