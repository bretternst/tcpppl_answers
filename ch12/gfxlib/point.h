#ifndef __POINT_H__
#define __POINT_H__

namespace gfxlib
{
    class Point
    {
        int m_x;
        int m_y;

    public:
        int x() const { return m_x; }
        void x(int x) { m_x = x; }
        int y() const { return m_y; }
        void y(int y) { m_y = y; }
        Point() : m_x(0), m_y(0) {}
        Point(int x, int y) : m_x(x), m_y(y) {}
    };
}
#endif
