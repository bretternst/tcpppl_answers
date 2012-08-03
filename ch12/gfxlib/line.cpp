#include <cmath>

#include "line.h"
#include "surface.h"

namespace gfxlib
{
	const double PI = 3.1415926;

	void Line::Draw(Surface& s) const
	{
		bool steep = abs(p2.Y()-p1.Y()) > abs(p2.X()-p1.X());
		Point pp1 = steep ? Point(p1.Y(),p1.X()) : Point(p1.X(),p1.Y());
		Point pp2 = steep ? Point(p2.Y(),p2.X()) : Point(p2.X(),p2.Y());
		int thickness = Thickness();
		LineStyle outline = Outline();
		Color stroke = Stroke();

		// Handle vertical lines as a special case.
		if(pp1.X()==pp2.X())
		{
			int x = pp1.X();
			int start = min(pp1.Y(),pp2.Y());
			int end = max(pp1.Y(),pp2.Y());
			for(int y = start; y <= end; y++)
			{
				if(outline == LINE_SOLID || (outline == LINE_DASHED && Dash(y)) || (outline == LINE_DOTTED && Dot(y)))
				{
					s.DrawDot(Point(x,y),thickness,stroke);
				}
			}
			return;
		}

		if(pp1.X() > pp2.X())
		{
			Point tmp = pp2;
			pp2 = pp1;
			pp1 = tmp;
		}
		int deltaX = pp2.X()-pp1.X();
		int deltaY = abs(pp2.Y()-pp1.Y());
		float error = 0;
		float deltaErr = deltaX == 0 ? 0 : static_cast<float>(deltaY) / static_cast<float>(deltaX);
		int yStep;
		int y = pp1.Y();
		yStep = pp1.Y() < pp2.Y() ? 1 : -1;
		for(int x = pp1.X(); x <= pp2.X(); x++)
		{
			if(outline == LINE_SOLID || (outline == LINE_DASHED && Dash(x)) || (outline == LINE_DOTTED && Dot(x)))
			{
				if(steep)
					s.DrawDot(Point(y,x),thickness,stroke);
				else
					s.DrawDot(Point(x,y),thickness,stroke);
			}
			error += deltaErr;
			if(error >= 0.5)
			{
				y += yStep;
				error -= 1.0;
			}
		}

		DrawArrowhead(s, p1, p2, a1);
		DrawArrowhead(s, p2, p1, a2);
	}

	void Line::DrawArrowhead(Surface& s, const Point ap1, const Point ap2, ArrowStyle style) const
	{
		double z = 0.0L;

		if(style == ARROW_IN)
		{
			z = PI / 4;
		}
		else if (style == ARROW_OUT)
		{
			z = PI * 0.75L;
		}
		else
			return;

		Point q(ap2.X()-ap1.X(),ap2.Y()-ap1.Y());
		double len = sqrt(static_cast<double>(q.X()*q.X() + q.Y()*q.Y()));
		Point q1(static_cast<int>(cos(z)*q.X()-sin(z)*q.Y()), static_cast<int>(sin(z)*q.X()+cos(z)*q.Y()));
		q1.X(static_cast<int>(q1.X() / len * Thickness()*4) + ap1.X());
		q1.Y(static_cast<int>(q1.Y() / len * Thickness()*4) + ap1.Y());
		Line a1(ap1, q1);
		a1.Thickness(Thickness());
		a1.Stroke(Stroke());
		a1.Draw(s);

		q1 = Point(static_cast<int>(cos(z)*q.X()+sin(z)*q.Y()), static_cast<int>(-sin(z)*q.X()+cos(z)*q.Y()));
		q1.X(static_cast<int>(q1.X() / len * Thickness()*4) + ap1.X());
		q1.Y(static_cast<int>(q1.Y() / len * Thickness()*4) + ap1.Y());
		a1 = Line(ap1, q1);
		a1.Thickness(Thickness());
		a1.Stroke(Stroke());
		a1.Draw(s);
	}

	void Line::Arrowhead(ArrowPosition pos, ArrowStyle style)
	{
		if(pos == ARROW_START)
			a1 = style;
		else if (pos == ARROW_END)
			a2 = style;
	}
}
