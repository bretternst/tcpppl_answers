#ifndef __EX04_H__
#define __EX04_H__

#include <vector>
#include <stack>
#include <string>
#include <algorithm>

namespace Solitaire
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
        SuitType Suit() const { return s; }
        char Symbol() const
        {
            switch(n) {
                case 10: return 'T'; case 11: return 'J'; case 12: return 'Q'; case 13: return 'K'; 
                default: return n+'0';
            }
        }
        unsigned char Number() { return n; }
        bool IsBlack() { return s == Spades || s == Clubs; }
        bool IsRed() { return s == Diamonds || s == Hearts; }
        std::string ToString() const
        {
            std::string ret;
            ret.push_back(Symbol());
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
        output << x.ToString();
        return output;
    }

    class Deck : public std::vector<Card>
    {
    public:
        Deck();
        void Shuffle() { std::random_shuffle(this->begin(), this->end()); }
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

        virtual Card* Draw() { Card* c = this->back(); this->pop_back(); return c; }
    };

    class Tableau : public Stack
    {
        unsigned int shown;
    public:
        Tableau() : shown(0) {}
        void AddHidden(Card* c) { this->push_back(c); }
        void AddShown(int n) { shown += n; }
        void AddShown(Card* c) { this->push_back(c); shown++; }
        void RemoveShown(int n) { shown -= n; }
        unsigned int Shown() const { return shown; }
        Card* Draw() { shown--; return Stack::Draw(); }
    };
}

#endif
