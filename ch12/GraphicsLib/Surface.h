#ifndef __SURFACE_H__
#define __SURFACE_H__

#include <string>
#include <windows.h>
#include <gdiplus.h>

#include "Point.h"
#include "Color.h"

namespace GraphicsLib
{
	class Surface
	{
		int w;
		int h;
		Gdiplus::Bitmap bmp;

	public:
		Surface(int w, int h, Color background);
		void Paint(Gdiplus::Graphics& g) const;
		void SetPixel(Point p, Color c) { bmp.SetPixel(p.X(),p.Y(), Gdiplus::Color(c)); }
		void DrawDot(Point p, int thickness, Color c);
		void DrawText(Point p1, Point p2, const std::wstring& str, Color c);
	};
}

#endif
