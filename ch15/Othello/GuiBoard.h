#ifndef __GUIBOARD_H__
#define __GUIBOARD_H__

#include <map>
#include <windows.h>

#include "Board.h"
#include "IPlayerInput.h"

namespace Othello
{
	class GuiBoard : public Board
	{
		static std::map<HWND,GuiBoard*> hWindows;
		static const int WinWidth = 480;
		static const int WinHeight = 500;
		static const int TextHeight = 20;
		HANDLE guiThread;
		HANDLE clickEvent;
		HWND windowHandle;
		volatile int clickPos;

		static DWORD __stdcall s_ThreadStart(LPVOID p);
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		void StartThread();
		void OnPaint(HDC hdc) const;
		void OnClick(int x, int y);
	public:
		class WindowsApiError {};

		GuiBoard();
		~GuiBoard();
		void Draw();
		Position GetMove();
		void Alert(std::string msg);
		void EndGame();
	};
}

#endif
