#include <iostream>
#include <sstream>
#include <windows.h>

#include "ConsoleBoard.h"

namespace Othello
{
	using namespace std;

	void ConsoleBoard::DrawSpace(Color s)
	{
		static HANDLE stdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		static WORD attrWhite = BACKGROUND_GREEN | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		static WORD attrBlack = BACKGROUND_GREEN;

		SetConsoleTextAttribute(stdOutHandle, s == White ? attrWhite : attrBlack);

		cout << (s == Empty ? ' ' : '*');
	}

	void ConsoleBoard::Draw()
	{
		static HANDLE stdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);

		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
		GetConsoleScreenBufferInfo(stdOutHandle, &consoleInfo);
		WORD attr = consoleInfo.wAttributes;
		COORD home = {0, 0};
		DWORD dummy;
		FillConsoleOutputCharacter(stdOutHandle, L' ', consoleInfo.dwSize.X * consoleInfo.dwSize.Y, home, &dummy);
		FillConsoleOutputAttribute(stdOutHandle, attr, consoleInfo.dwSize.X * consoleInfo.dwSize.Y, home, &dummy);
		SetConsoleCursorPosition(stdOutHandle, home);

		cout << ' ';
		for(int col = 0; col < NumberOfColumns; col++)
		{
			cout << col+1;
		}
		cout << endl;

		for(int row = 0; row < NumberOfRows; row++)
		{
			SetConsoleTextAttribute(stdOutHandle,attr);
			cout << row+1;
			for(int col = 0; col < NumberOfColumns; col++)
			{
				DrawSpace(Space(Position(col,row)));
			}
			cout << endl;
		}
		SetConsoleTextAttribute(stdOutHandle,attr);
		cout << Prompt() << endl;
	}

	Position ConsoleBoard::GetMove()
	{
		Position p(0,0);

		for(;;)
		{
			cout << "enter move in the format col,row: ";

			char ch = 0;

			std::string line;
			getline(cin, line);
			std::istringstream rdr(line);

			rdr.get(ch);
			if(isdigit(ch))
			{
				rdr.putback(ch);
				rdr >> p.X;
			}
			else
			{
				continue;
			}

			rdr.get(ch);
			if(ch != ',') continue;

			rdr.get(ch);
			if(isdigit(ch))
			{
				rdr.putback(ch);
				rdr >> p.Y;
			}
			else
			{
				continue;
			}

			if(p.X < 1 || p.X > NumberOfRows || p.Y < 1 || p.Y > NumberOfRows)
			{
				cout << "invalid row or column #." << endl;
				continue;
			}
			p.X--; p.Y--;

			return p;
		}
	}

	void ConsoleBoard::Alert(std::string msg)
	{
		cout << msg << endl;
	}
}
