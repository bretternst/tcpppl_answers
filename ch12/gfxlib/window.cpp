#include <windows.h>
#include <gdiplus.h>

#include "window.h"
#include "line.h"
#include "color.h"

namespace gfxlib
{
	using namespace Gdiplus;

	std::map<HWND,Window*> Window::hWindows;

	Window::Window(int w, int h, Color background = 0xFFFFFFFF) : current(0,0), surface(w,h,background)
	{
		hWnd = CreateWindow("Canvas", "Canvas", WS_CAPTION | WS_SYSMENU, 
			100, 100, 640, 480, 0, 0, GetModuleHandle(NULL), NULL);
		ShowWindow(hWnd, SW_SHOW);
		hWindows[hWnd] = this;
	}

	Window::~Window()
	{
	}

	LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		HDC hdc;

		switch(message)
		{
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			hWindows[hWnd]->OnPaint(hdc);
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			delete hWindows[hWnd];
			hWindows.erase(hWnd);
			if(hWindows.size()<1)
			{
				PostQuitMessage(0);
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}

	void Window::OnPaint(HDC hdc) const
	{
		using namespace Gdiplus;
		Gdiplus::Graphics g(hdc);

		surface.Paint(g);
	}
}
