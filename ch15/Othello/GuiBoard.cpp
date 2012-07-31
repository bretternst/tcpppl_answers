#include <iostream>
#include <sstream>
#include <map>
#include <windows.h>
#include <Gdiplus.h>

#include "GuiBoard.h"

namespace Othello
{
	using namespace std;

	//
	// Begin static stuff
	//

	std::map<HWND,GuiBoard*> GuiBoard::hWindows;

	class GdiInitWrapper
	{
		ULONG_PTR gdiToken;
	public:
		GdiInitWrapper()
		{
			Gdiplus::GdiplusStartupInput gdiplusStartupInput;
			Gdiplus::GdiplusStartup(&gdiToken, &gdiplusStartupInput, NULL);
		}
		~GdiInitWrapper()
		{
			Gdiplus::GdiplusShutdown(gdiToken);
		}
	};
	GdiInitWrapper gdiInitWrapper;

	// Marshal the callback back into the object context.
	DWORD __stdcall GuiBoard::s_ThreadStart(LPVOID p)
	{
		GuiBoard* b = reinterpret_cast<GuiBoard*>(p);
		b->StartThread();
		return 0;
	}

	LRESULT CALLBACK GuiBoard::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			hWindows.erase(hWnd);
			if(hWindows.size()<1)
			{
				PostQuitMessage(0);
			}
			break;
		case WM_LBUTTONUP:
			hWindows[hWnd]->OnClick(lParam & 0xffff, lParam >> 16);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}

	//
	// End static stuff
	//

	void GuiBoard::StartThread()
	{
		WNDCLASSEX wndClsEx;
		MSG messages;
		wndClsEx.hInstance = GetModuleHandle(NULL);
		wndClsEx.lpszClassName = "Board";
		wndClsEx.lpfnWndProc = &GuiBoard::WndProc;
		wndClsEx.style = 0;
		wndClsEx.cbSize = sizeof(WNDCLASSEX);
		wndClsEx.lpszMenuName = NULL;
		wndClsEx.cbClsExtra = 0;
		wndClsEx.cbWndExtra = 0;
		HBRUSH hBrush = CreateSolidBrush(RGB(0,127,0));
		wndClsEx.hbrBackground = hBrush;
		wndClsEx.hIcon = LoadIcon (NULL, IDI_APPLICATION);
		wndClsEx.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
		wndClsEx.hCursor = LoadCursor (NULL, IDC_ARROW);
		if (!RegisterClassEx(&wndClsEx))
			throw WindowsApiError();

		windowHandle = CreateWindow("Board","Board", WS_CAPTION | WS_SYSMENU,
			100, 100, WinWidth, WinHeight, 0, 0, GetModuleHandle(NULL), NULL);
		if(!windowHandle)
			throw WindowsApiError();
		hWindows[windowHandle] = this;

		ShowWindow(windowHandle, SW_SHOW);

		while(GetMessage(&messages, NULL, 0, 0))
		{
			TranslateMessage(&messages);
			DispatchMessage(&messages);
		}

		DeleteObject(hBrush);
	}

	void GuiBoard::OnPaint(HDC hdc) const
	{
		using namespace Gdiplus;

		Graphics g(hdc);
		Rect bounds;
		g.GetVisibleClipBounds(&bounds);

		int squareWidth = bounds.Width / NumberOfColumns;
		int squareHeight = (bounds.Height-TextHeight) / NumberOfRows;

		Pen p(Gdiplus::Color(255, 0, 0, 0), 1);
		for(int i = 1; i <= NumberOfColumns-1; i++)
		{
			int y = squareWidth * i;
			g.DrawLine(&p, y, 0, y, WinHeight);
		}
		for(int i = 1; i <= NumberOfRows-1; i++)
		{
			int x = squareHeight * i;
			g.DrawLine(&p, 0, x, WinWidth, x);
		}

		SolidBrush blackBrush(Gdiplus::Color(255, 0, 0, 0));
		SolidBrush whiteBrush(Gdiplus::Color(255, 255, 255, 255));

		g.FillRectangle(&whiteBrush, Rect(0,bounds.Height-TextHeight,bounds.Width,TextHeight));
		const std::string& input = Prompt();
		std::wstring output(input.begin(),input.end());
		g.DrawString(output.c_str(),
			output.size(),
			&Font(L"Arial",12),
			RectF(5,static_cast<REAL>(bounds.Height-TextHeight),static_cast<REAL>(bounds.Width-5),static_cast<REAL>(TextHeight)),
			0, 
			&blackBrush);

		for(int y = 0; y < NumberOfRows; y++)
		{
			for(int x = 0; x < NumberOfColumns; x++)
			{
				Color c = Space(Position(x,y));
				if(c != Empty)
				{
					g.FillEllipse(c == Black ? &blackBrush : &whiteBrush,
						squareWidth * x + 5, squareHeight * y + 5,
						squareWidth - 10, squareHeight - 10);
				}
			}
		}
	}

	void GuiBoard::OnClick(int mx, int my)
	{
		RECT client;
		GetClientRect(windowHandle, &client);
		int width = client.right-client.left;
		int height = client.bottom-client.top;

		int x = mx / (width / NumberOfColumns);
		int y = my / ((height - TextHeight) / NumberOfRows);

		clickPos = (y << 16) | x;
		SetEvent(clickEvent);
	}

	GuiBoard::GuiBoard()
	{
		DWORD threadId;
		guiThread = CreateThread(0, 0, &s_ThreadStart, this, 0, &threadId);
		clickEvent = CreateEvent(0, TRUE, FALSE, 0);
		if(!guiThread || !clickEvent)
			throw WindowsApiError();
	}

	GuiBoard::~GuiBoard()
	{
		DeleteObject(clickEvent);
	}

	void GuiBoard::Draw()
	{
		InvalidateRect(windowHandle, NULL, TRUE);
		UpdateWindow(windowHandle);
	}

	Position GuiBoard::GetMove()
	{
		ResetEvent(clickEvent);
		int x,y;
		do
		{
			WaitForSingleObject(clickEvent, INFINITE);
			x = clickPos & 0xffff;
			y = clickPos >> 16;
		}
		while(x < 0 || x >= NumberOfColumns || y < 0 || y >= NumberOfRows);

		return Position(x,y);
	}

	void GuiBoard::Alert(std::string msg)
	{
		MessageBox(0, msg.c_str(), "Alert", MB_OK);
	}

	void GuiBoard::EndGame()
	{
		WaitForSingleObject(guiThread, INFINITE);
	}
}
