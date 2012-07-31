#include <string>
#include <windows.h>
#include <gdiplus.h>

#include "Surface.h"
#include "Color.h"

namespace GraphicsLib
{
	using Gdiplus::REAL;
	using Gdiplus::Graphics;

	Surface::Surface(int w, int h, Color background) : w(w), h(h), bmp(w, h)
	{
		Graphics g(&bmp);
		g.FillRectangle(&Gdiplus::SolidBrush(Gdiplus::Color(background)), 0, 0, w, h);
	}

	void Surface::Paint(Graphics& g) const
	{
		g.DrawImage(const_cast<Gdiplus::Bitmap*>(&bmp), 0, 0);
	}

	void Surface::DrawDot(const Point p, int thickness, Color c)
	{
		if(thickness==0) return;

		// Added in exercise 5
		if(p.X() < 0 || p.Y() < 0 || p.X() >= w || p.Y() >= h) return;

		for(int i = p.X()-((thickness-1)/2); i < p.X()+(thickness/2)+1; i++)
		{
			for(int j = p.Y()-((thickness-1)/2); j < p.Y()+(thickness/2)+1; j++)
			{
				SetPixel(Point(i,j),c);
			}
		}
	}

	void Surface::DrawText(Point p1, Point p2, const std::wstring& str, Color c)
	{
		using namespace Gdiplus;

		// Perform clipping, to maintain consistency with exercise 5
		int height = p2.Y()-p1.Y();
		if(p1.X() < 0) p1.X(0);
		if(p1.Y() < 0) p1.Y(0);
		if(p2.X() > w) p2.X(w);
		if(p2.Y() > h) p2.Y(h);

		Graphics g(&bmp);
		Font f(&FontFamily(L"Tahoma"), static_cast<REAL>(height), Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
		RectF bounds(static_cast<REAL>(p1.X()),static_cast<REAL>(p1.Y()),
			static_cast<REAL>(p2.X()-p1.X()),static_cast<REAL>(p2.Y()-p1.Y()));

		g.DrawString(&str[0], str.size(), &f, bounds,
				NULL,&SolidBrush(Gdiplus::Color(c)));
	}
}
