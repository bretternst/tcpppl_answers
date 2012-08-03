#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <map>

#include "point.h"
#include "shape.h"
#include "color.h"

namespace gfxlib
{
	class Window
	{
		Point current;
		Surface surface;
		Color background;

		HWND hWnd;

		static std::map<HWND,Window*> hWindows;

		void OnPaint(HDC hdc) const;
	public:
		Window(int w, int h, Color background);
		~Window();

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		const Point& Current() const { return current; }
		void Current(const Point& p) { current = p; }
		void Draw(Shape* s){s->Draw(surface); current = s->SE();}
		Line Connect(Shape* s1, Shape* s2);
	};
}
#endif
