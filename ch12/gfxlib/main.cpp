#include <windows.h>
#include <gdiplus.h>

#include "tests.h"
#include "window.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void OnPaint(HDC);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmd, int nCmd)
{
	WNDCLASSEX wndClsEx;
	MSG messages;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;

	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	wndClsEx.hInstance = hInstance;
	wndClsEx.lpszClassName = "Canvas";
	wndClsEx.lpfnWndProc = gfxlib::Window::WndProc;
	wndClsEx.style = 0;
	wndClsEx.cbSize = sizeof(WNDCLASSEX);
	wndClsEx.lpszMenuName = NULL;
	wndClsEx.cbClsExtra = 0;
	wndClsEx.cbWndExtra = 0;
	HBRUSH hBrush = CreateSolidBrush(RGB(255,255,255));
	wndClsEx.hbrBackground = hBrush;
    wndClsEx.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wndClsEx.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wndClsEx.hCursor = LoadCursor (NULL, IDC_ARROW);

	if (!RegisterClassEx(&wndClsEx))
		return -1;

	RunTests();

	while(GetMessage(&messages, NULL, 0, 0))
	{
		TranslateMessage(&messages);
		DispatchMessage(&messages);
	}

	DeleteObject(hBrush);
	Gdiplus::GdiplusShutdown(gdiplusToken);

	return 0;
}
