#include <iostream>
#include <ctime>
#include <sstream>

#include "ex04.h"

namespace Solitaire
{
    Deck::Deck()
    {
        // fill the deck
        this->reserve(52);
        for(int i = 1; i <= 13; i++)
        {
            this->push_back(Card(Card::Spades, i));
            this->push_back(Card(Card::Clubs, i));
            this->push_back(Card(Card::Diamonds, i));
            this->push_back(Card(Card::Hearts, i));
        }
    }

    bool getCol(std::istream& input, int& col)
    {
        col = 0;
        input >> col;
        col--;
        return col >= 0 && col <= 6;
    }
    
    bool getFoundation(std::istream& input, int& f)
    {
        f = 0;
        input >> f;
        f--;
        return f >= 0 && f <= 3;
    }

    bool getNumberOfCards(std::istream& input, const Tableau& t, unsigned int& n)
    {
        n = 0;
        input >> n;
        if(n < 1) n = 1;
        return n <= t.Shown();
    }

    bool isValidUnder(Card* c1, Card* c2)
    {
        return ((c1->IsBlack() && c2->IsRed()) || (c1->IsRed() && c2->IsBlack())) && c2->Number() == c1->Number() - 1;
    }
}


int main()
{
    using namespace std;
    using namespace Solitaire;

    // seed rand() once per execution
    srand(static_cast<unsigned int>(time(0)));

    // game elements
    Deck deck;
    Stack stock(deck);
    Stack waste;
    Stack foundation[4];
    Tableau tableau[7];

    // deal cards
    deck.Shuffle();
    for(int i = 0; i < 7; i++)
    {
        for(int j = 0; j < i; j++)
            tableau[i].AddHidden(stock.Draw());
        tableau[i].AddShown(stock.Draw());
    }
    for(int i = 0; i < 3; i++)
        waste.push_back(stock.Draw());

    bool gameover = false;
    while(!gameover)
    {
        // draw the board
        cout << "type a command:" << endl;
        cout << "  d                       draw from stock" << endl;
        cout << "  t <col>                 move top drawn card to column" << endl;
        cout << "  m <col1> <col2> [num]   move num (default 1) cards from col1 to col2" << endl;
        cout << "  f <col>                 flip face-down card in col" << endl;
        cout << "  x <fs> <col>            move bottom card in col to foundation stack fs" << endl;
        cout << "  x <fs>                  move top card from waste to foundation stack fs" << endl;

        cout << "Stock: [" << stock.size() << "]" << endl;
        cout << "Waste: ";
        int n = static_cast<int>(waste.size()) - 3;
        for(unsigned int i = max(n, 0); i < waste.size(); i++)
        {
            cout << *waste[i] << ' ';
        }
        if(!stock.empty()) cout << "<-";
        cout << endl << "Foundations: ";
        for(int i = 0; i < 4; i++)
        {
            if(foundation[i].empty()) 
                cout << "[  ] ";
            else
                cout << '[' << *foundation[i].back() << "] ";
        }
        cout << endl;

        for(int i = 1; i <= 7; i++) cout << ' ' << i << ' ';
        cout << endl;

        bool finished = false;
        for(unsigned int i=1;!finished;i++)
        {
            finished = true;
            for(unsigned int j = 0; j < 7; j++)
            {
                if(tableau[j].size() >= i)
                {
                    if(i <= tableau[j].size()-tableau[j].Shown())
                        cout << "** ";
                    else
                        cout << *tableau[j][i-1] << ' ';
                    finished = false;
                }
                else
                    cout << "   ";
            }
            cout << endl;
        }

        cout << "command: ";
        string scmd;
        getline(cin, scmd);
        istringstream cmd(scmd);
        char ch;
        if(cmd.get(ch))
        {
            ch = tolower(ch);
            switch(ch)
            {
            case 'd':
                {
                    // draw
                    if(stock.size() == 0)
                    {
                        if(waste.size() == 0) break;
                        stock.assign(waste.rbegin(), waste.rend());
                        waste.clear();
                    }

                    int n = min(stock.size(),static_cast<unsigned int>(3));
                    for(int i = 0; i < n; i++)
                        waste.push_back(stock.Draw());
                    break;
                }
            case 't':
                {
                    // top card to column
                    int col;
                    if(!getCol(cmd, col))
                    {
                        cout << "invalid column" << endl;
                        break;
                    }
                    if(waste.size() == 0)
                    {
                        cout << "no card to move" << endl;
                        break;
                    }
                    if(!(tableau[col].size() == 0 && waste.back()->Number() == 13) &&
                        !isValidUnder(tableau[col].back(), waste.back()))
                    {
                        cout << "move is not valid" << endl;
                    }
                    else
                    {
                        tableau[col].AddShown(waste.Draw());
                    }
                    break;
                }
            case 'm':
                {
                    int col1;
                    int col2;
                    unsigned int n;
                    if(!getCol(cmd, col1) ||
                        !getCol(cmd, col2))
                    {
                        cout << "invalid column" << endl;
                        break;
                    }
                    if(!getNumberOfCards(cmd, tableau[col1], n))
                    {
                        cout << "invalid number of cards" << endl;
                        break;
                    }
                    Tableau::iterator i = tableau[col1].end() - n;

                    // if target is empty and top card is a king, permit the move
                    // otherwise, if the card is of an alternating color and one number lower, permit the move
                    if((tableau[col2].empty() && (*i)->Number() == 13) ||
                        (isValidUnder(tableau[col2].back(),*i)))
                    {
                        tableau[col2].insert(tableau[col2].end(), i, tableau[col1].end());
                        tableau[col2].AddShown(n);
                        tableau[col1].erase(i, tableau[col1].end());
                        tableau[col1].RemoveShown(n);
                    }
                    else
                    {
                        cout << "invalid move" << endl;
                        break;
                    }
                }
            case 'x':
                {
                    // move card from tableau to foundation
                    int col;
                    int f;
                    if(!getFoundation(cmd, f))
                    {
                        cout << "invalid foundation" << endl;
                        break;
                    }

                    // move from top to foundation
                    if(!getCol(cmd, col))
                    {
                        Card* c = waste.back();
                        if((foundation[f].empty() && c->Number()==1) ||
                            (foundation[f].back()->Suit() == c->Suit() && foundation[f].back()->Number() == c->Number()-1))
                        {
                            foundation[f].push_back(waste.Draw());
                        }
                        else
                        {
                            cout << "invalid move" << endl;
                            break;
                        }
                    }
                    else
                    {
                        if(tableau[col].empty() || tableau[col].Shown() < 1)
                        {
                            cout << "invalid column" << endl;
                            break;
                        }
                        Card* c = tableau[col].back();

                        // ace can move into an empty foundation stack
                        // otherwise, card must be the same suit and n+1
                        if((foundation[f].empty() && c->Number()==1) ||
                            (foundation[f].back()->Suit() == c->Suit() && foundation[f].back()->Number() == c->Number()-1))
                        {
                            foundation[f].push_back(tableau[col].Draw());
                        }
                        else
                        {
                            cout << "invalid move" << endl;
                            break;
                        }
                    }

                    // check for endgame condition
                    gameover = true;
                    for(int i = 0; i < 4; i++)
                        if(foundation[i].empty() || foundation[i].back()->Number() != 13) gameover = false;

                    break;
                }
            case 'f':
                {
                    // flip bottom card in column
                    int col;
                    if(!getCol(cmd, col) ||
                        tableau[col].empty() ||
                        tableau[col].Shown() != 0)
                    {
                        cout << "invalid column" << endl;
                        break;
                    }
                    tableau[col].AddShown(1);
                    break;
                }
            }
        }
    }
    cout << "congratulations!" << endl;
}
