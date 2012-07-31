#include <cmath>

#include "Shape.h"
#include "Surface.h"
#include "Line.h"

namespace GraphicsLib
{
	bool Shape::Dash(int x) const
	{
		return x % 5 < 3;
	}

	bool Shape::Dot(int x) const
	{
		return x % 2 < 1;
	}

	double GetDistance(Point p1, Point p2)
	{
		double w = static_cast<double>(p2.Y()-p1.Y());
		double h = static_cast<double>(p2.X()-p1.X());
		return abs(sqrt(w*w+h*h));
	}

	Point GetPointByNum(const Shape* s, int n)
	{
		switch(n)
		{
		case 0: return s->NW(); break;
		case 1: return s->NE(); break;
		case 2: return s->SW(); break;
		case 3: return s->SE(); break;
		case 4: return s->N(); break;
		case 5: return s->S(); break;
		case 6: return s->W(); break;
		case 7: return s->E(); break;
		case 8: return s->C(); break;
		default: throw;
		}
	}

	Line Shape::Connect(Shape* s) const
	{
		Point p1, p2;
		double dist = -1;

		// Brute force, but could be optimized based on shape's positions relative to one another
		for(int i = 0; i < 9; i++)
		{
			for(int j = 0; j < 9; j++)
			{
				Point tmpp1 = GetPointByNum(this,i);
				Point tmpp2 = GetPointByNum(s,j);
				double tmp = GetDistance(tmpp1,tmpp2);
				if(dist < 0 || tmp < dist)
				{
					dist = tmp;
					p1 = GetPointByNum(this,i);
					p2 = GetPointByNum(s,j);
				}
			}
		}
		Line l(p1,p2);
		return l;
	}
}
