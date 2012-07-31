#include "Circle.h"
#include "Surface.h"

namespace GraphicsLib
{
	void FillRun(Surface& s, Color c, int y, int x1, int x2)
	{
		for(int i = x1; i <= x2; i++)
			s.SetPixel(Point(i,y), c);
	}

	void Circle::Draw(Surface& s) const
	{
		int f = 1 - r;
		int ddfx = 1;
		int ddfy = -2 * r;
		int x = 0;
		int y = r;
		int thickness = Thickness();
		LineStyle outline = Outline();
		Color stroke = Stroke();
		Color fill = Fill();

		s.DrawDot(Point(c.X(),c.Y()+r),thickness,stroke);
		s.DrawDot(Point(c.X(),c.Y()-r),thickness,stroke);
		s.DrawDot(Point(c.X()+r,c.Y()),thickness,stroke);
		s.DrawDot(Point(c.X()-r,c.Y()),thickness,stroke);
		FillRun(s, fill, c.Y(), c.X()-r+1, c.X()+r-1);

		while(x < y)
		{
			if(f>=0)
			{
				y--;
				ddfy += 2;
				f += ddfy;
			}
			x++;
			ddfx += 2;
			f += ddfx;
			
			FillRun(s, fill, c.Y()+y, c.X()-x+1, c.X()+x-1);
			FillRun(s, fill, c.Y()-y, c.X()-x+1, c.X()+x-1);
			FillRun(s, fill, c.Y()+x, c.X()-y+1, c.X()+y-1);
			FillRun(s, fill, c.Y()-x, c.X()-y+1, c.X()+y-1);

			if(outline == LINE_SOLID || (outline == LINE_DASHED && Dash(x)) || (outline == LINE_DOTTED && Dot(x)))
			{
				s.DrawDot(Point(c.X()+x, c.Y()+y),thickness,stroke);
				s.DrawDot(Point(c.X()-x, c.Y()+y),thickness,stroke);

				s.DrawDot(Point(c.X()+x, c.Y()-y),thickness,stroke);
				s.DrawDot(Point(c.X()-x, c.Y()-y),thickness,stroke);

				s.DrawDot(Point(c.X()+y, c.Y()+x),thickness,stroke);
				s.DrawDot(Point(c.X()-y, c.Y()+x),thickness,stroke);

				s.DrawDot(Point(c.X()+y, c.Y()-x),thickness,stroke);
				s.DrawDot(Point(c.X()-y, c.Y()-x),thickness,stroke);
			}
		}
	}
}
