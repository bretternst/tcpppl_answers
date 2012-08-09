#include <iostream>
#include <ctime>
#include <sstream>
#include <vector>
#include <algorithm>

namespace solitaire
{
    class Card
    {
    public:
        enum SuitType { Spades, Clubs, Diamonds, Hearts };

    private:
        SuitType s;
        unsigned char n;

    public:
        Card(SuitType suit, unsigned char number) : s(suit), n(number) {}
        SuitType suit() const { return s; }
        char symbol() const
        {
            switch(n) {
                case 10: return 'T'; case 11: return 'J'; case 12: return 'Q'; case 13: return 'K'; 
                default: return n+'0';
            }
        }
        unsigned char number() { return n; }
        bool is_black() { return s == Spades || s == Clubs; }
        bool is_red() { return s == Diamonds || s == Hearts; }
        std::string to_string() const
        {
            std::string ret;
            ret.push_back(symbol());
            switch(s) {
                case Spades: ret.push_back('S'); break;
                case Clubs: ret.push_back('C'); break;
                case Diamonds: ret.push_back('D'); break;
                case Hearts: ret.push_back('H'); break;
            }
            return ret;
        }
    };

    std::ostream& operator<<(std::ostream& output, const Card& x)
    {
        output << x.to_string();
        return output;
    }

    class Deck : public std::vector<Card>
    {
    public:
        Deck();
        void shuffle() { std::random_shuffle(this->begin(), this->end()); }
    };

    class Stack : public std::vector<Card*>
    {
    public:
        Stack(Deck& deck)
        {
            for(Deck::iterator i = deck.begin(); i != deck.end(); i++)
                this->push_back(&(*i));
        }

        template<class Tcoll> Stack(const Tcoll& cards)
        {
            for(typename Tcoll::const_iterator i = cards.begin(); i != cards.end(); i++)
                this->push_back(*i);
        }

        Stack() {}

        virtual Card* draw() { Card* c = this->back(); this->pop_back(); return c; }
    };

    class Tableau : public Stack
    {
        unsigned int m_shown;
    public:
        Tableau() : m_shown(0) {}
        void add_hidden(Card* c) { this->push_back(c); }
        void add_shown(int n) { m_shown += n; }
        void add_shown(Card* c) { this->push_back(c); m_shown++; }
        void remove_shown(int n) { m_shown -= n; }
        unsigned int shown() const { return m_shown; }
        Card* draw() { m_shown--; return Stack::draw(); }
    };
    
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

    bool get_col(std::istream& input, int& col)
    {
        col = 0;
        input >> col;
        col--;
        return col >= 0 && col <= 6;
    }
    
    bool get_foundation(std::istream& input, int& f)
    {
        f = 0;
        input >> f;
        f--;
        return f >= 0 && f <= 3;
    }

    bool get_num_of_cards(std::istream& input, const Tableau& t, unsigned int& n)
    {
        n = 0;
        input >> n;
        if(n < 1) n = 1;
        return n <= t.shown();
    }

    bool is_valid_under(Card* c1, Card* c2)
    {
        return ((c1->is_black() && c2->is_red()) || (c1->is_red() && c2->is_black())) && c2->number() == c1->number() - 1;
    }
}


int main()
{
    using namespace std;
    using namespace solitaire;

    // seed rand() once per execution
    srand(static_cast<unsigned int>(time(0)));

    // game elements
    Deck deck;
    Stack stock(deck);
    Stack waste;
    Stack foundation[4];
    Tableau tableau[7];

    // deal cards
    deck.shuffle();
    for(int i = 0; i < 7; i++)
    {
        for(int j = 0; j < i; j++)
            tableau[i].add_hidden(stock.draw());
        tableau[i].add_shown(stock.draw());
    }
    for(int i = 0; i < 3; i++)
        waste.push_back(stock.draw());

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
                    if(i <= tableau[j].size()-tableau[j].shown())
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

                    int n = min((int)stock.size(),3);
                    for(int i = 0; i < n; i++)
                        waste.push_back(stock.draw());
                    break;
                }
            case 't':
                {
                    // top card to column
                    int col;
                    if(!get_col(cmd, col))
                    {
                        cout << "invalid column" << endl;
                        break;
                    }
                    if(waste.size() == 0)
                    {
                        cout << "no card to move" << endl;
                        break;
                    }
                    if(!(tableau[col].size() == 0 && waste.back()->number() == 13) &&
                        !is_valid_under(tableau[col].back(), waste.back()))
                    {
                        cout << "move is not valid" << endl;
                    }
                    else
                    {
                        tableau[col].add_shown(waste.draw());
                    }
                    break;
                }
            case 'm':
                {
                    int col1;
                    int col2;
                    unsigned int n;
                    if(!get_col(cmd, col1) ||
                        !get_col(cmd, col2))
                    {
                        cout << "invalid column" << endl;
                        break;
                    }
                    if(!get_num_of_cards(cmd, tableau[col1], n))
                    {
                        cout << "invalid number of cards" << endl;
                        break;
                    }
                    Tableau::iterator i = tableau[col1].end() - n;

                    // if target is empty and top card is a king, permit the move
                    // otherwise, if the card is of an alternating color and one number lower, permit the move
                    if((tableau[col2].empty() && (*i)->number() == 13) ||
                        (is_valid_under(tableau[col2].back(),*i)))
                    {
                        tableau[col2].insert(tableau[col2].end(), i, tableau[col1].end());
                        tableau[col2].add_shown(n);
                        tableau[col1].erase(i, tableau[col1].end());
                        tableau[col1].remove_shown(n);
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
                    if(!get_foundation(cmd, f))
                    {
                        cout << "invalid foundation" << endl;
                        break;
                    }

                    // move from top to foundation
                    if(!get_col(cmd, col))
                    {
                        Card* c = waste.back();
                        if((foundation[f].empty() && c->number()==1) ||
                            (foundation[f].back()->suit() == c->suit() && foundation[f].back()->number() == c->number()-1))
                        {
                            foundation[f].push_back(waste.draw());
                        }
                        else
                        {
                            cout << "invalid move" << endl;
                            break;
                        }
                    }
                    else
                    {
                        if(tableau[col].empty() || tableau[col].shown() < 1)
                        {
                            cout << "invalid column" << endl;
                            break;
                        }
                        Card* c = tableau[col].back();

                        // ace can move into an empty foundation stack
                        // otherwise, card must be the same suit and n+1
                        if((foundation[f].empty() && c->number()==1) ||
                            (foundation[f].back()->suit() == c->suit() && foundation[f].back()->number() == c->number()-1))
                        {
                            foundation[f].push_back(tableau[col].draw());
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
                        if(foundation[i].empty() || foundation[i].back()->number() != 13) gameover = false;

                    break;
                }
            case 'f':
                {
                    // flip bottom card in column
                    int col;
                    if(!get_col(cmd, col) ||
                        tableau[col].empty() ||
                        tableau[col].shown() != 0)
                    {
                        cout << "invalid column" << endl;
                        break;
                    }
                    tableau[col].add_shown(1);
                    break;
                }
            }
        }
    }
    cout << "congratulations!" << endl;
}
