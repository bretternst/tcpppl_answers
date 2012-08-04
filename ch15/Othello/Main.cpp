#include <iostream>
#include <windows.h>

#include "Game.h"
#include "ConsoleBoard.h"
#include "GuiBoard.h"
#include "HumanPlayer.h"
#include "AIPlayer.h"

using namespace Othello;
using namespace std;

int main(int argc, char* argv[])
{
    using namespace std;

    Board* b;
    Player* p1;
    Player* p2;

    std::string p1name;
    std::string p2name;

    cout << "Enter player 1's name: ";
    getline(cin, p1name);
    cout << "Enter player 2's name (blank for computer): ";
    getline(cin, p2name);

    if(argc > 1 && strncmp(argv[1],"-g",2) == 0)
        b = new GuiBoard();
    else
        b = new ConsoleBoard();

    p1 = new HumanPlayer(*b, Board::Black, p1name, *b);
    if(p2name.size() == 0)
        p2 = new AIPlayer(*b, Board::White);
    else
        p2 = new HumanPlayer(*b, Board::White, p2name, *b);

    Game* g = new Game(*b, *p1, *p2);
    g->Play();

    delete g;
    delete b;
    delete p1;
    delete p2;

    return 0;
};
