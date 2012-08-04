#include <cmath>

#include <cstdlib>
#include "shape.h"
#include "line.h"

namespace gfxlib
{
    bool Shape::dash(int x) const
    {
        return x % 5 < 3;
    }

    bool Shape::dot(int x) const
    {
        return x % 2 < 1;
    }

    double get_distance(Point p1, Point p2)
    {
        double w = static_cast<double>(p2.y()-p1.y());
        double h = static_cast<double>(p2.x()-p1.x());
        return abs(sqrt(w*w+h*h));
    }

    Point get_point_by_num(const Shape* s, int n)
    {
        switch(n)
        {
        case 0: return s->nw(); break;
        case 1: return s->ne(); break;
        case 2: return s->sw(); break;
        case 3: return s->se(); break;
        case 4: return s->n(); break;
        case 5: return s->s(); break;
        case 6: return s->w(); break;
        case 7: return s->e(); break;
        case 8: return s->c(); break;
        default: throw;
        }
    }

    Line Shape::connect(Shape* s) const
    {
        Point p1, p2;
        double dist = -1;

        // Brute force, but could be optimized based on shape's positions relative to one another
        for(int i = 0; i < 9; i++)
        {
            for(int j = 0; j < 9; j++)
            {
                Point tmpp1 = get_point_by_num(this,i);
                Point tmpp2 = get_point_by_num(s,j);
                double tmp = get_distance(tmpp1,tmpp2);
                if(dist < 0 || tmp < dist)
                {
                    dist = tmp;
                    p1 = get_point_by_num(this,i);
                    p2 = get_point_by_num(s,j);
                }
            }
        }
        Line l(p1,p2);
        return l;
    }
}
