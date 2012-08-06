#ifndef __BOARD_H__
#define __BOARD_H__

#include <string>

#include "common.h"
#include "iplayerinput.h"

namespace othello
{
    class Board : public IPlayerInput
    {
    public:
        enum Color
        {
            Empty,
            Black,
            White
        };
        class OutOfRangeError {};

        static const int NumberOfRows = 8;
        static const int NumberOfColumns = 8;

    private:
        Color spaces[NumberOfRows][NumberOfColumns];
        std::string m_prompt;

        void check_range(Position p) const;

    protected:
        const std::string& prompt() const { return m_prompt; }

    public:
        Board();
        virtual ~Board() {}
        Color space(Position p) const;
        Color& space(Position p);
        void update(const std::string& m_prompt);
        virtual void draw() = 0;

        int find_path(Color c, Position p, int xDelta, int yDelta) const;
        int flip_path(Color c, Position p, int xDelta, int yDelta);
        int simulate_play(Color c, Position p) const;
        int play(Color c, Position p);
        bool has_legal_move(Color c);
        virtual void end_game() {}
    };

    inline void Board::check_range(Position p) const
    {
        if(p.y < 0 || p.y >= NumberOfRows || p.x < 0 || p.x >= NumberOfRows)
            throw OutOfRangeError();
    }
}

#endif
