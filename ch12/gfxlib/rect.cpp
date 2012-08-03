#include "rect.h"
#include "surface.h"
#include "line.h"

namespace gfxlib
{
	void Rect::Draw(Surface& s) const
	{
		Color stroke = Stroke();

		DrawSide(s,NW(),sw);
		DrawSide(s,NW(),ne);
		DrawSide(s,ne,SE());
		DrawSide(s,sw,SE());

		DrawFill(s);
	}

	void Rect::DrawSide(Surface& s, Point p1, Point p2) const
	{
		Line side(p1,p2);
		side.Thickness(Thickness());
		side.Outline(Outline());
		side.Stroke(Stroke());
		side.Draw(s);
	}

	void Rect::DrawFill(Surface& s) const
	{
		int fill = Fill();
		for(int i = sw.X()+Thickness(); i < ne.X(); i++)
		{
			for(int j = ne.Y()+Thickness(); j < sw.Y(); j++)
			{
				s.SetPixel(Point(i,j),fill);
			}
		}
	}
}
