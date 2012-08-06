#include "game.h"

namespace othello
{
    Game::Game(Board& board, Player& player1, Player& player2) : b(board), p1(&player1), p2(&player2), currentPlayer(0)
    {
        int midX = Board::NumberOfColumns / 2 - 1;
        int midY = Board::NumberOfRows / 2 - 1;
        board.space(Position(midX,midY)) = Board::White;
        board.space(Position(midX+1,midY)) = Board::Black;
        board.space(Position(midX,midY+1)) = Board::Black;
        board.space(Position(midX+1,midY+1)) = Board::White;

        if(!(player1.color() == Board::White && player2.color() == Board::Black) &&
            !(player1.color() == Board::Black && player2.color() == Board::White))
        {
            throw PlayerColorError();
        }
    }

    void Game::play()
    {
        Player* currentPlayer = (p1->color() == Board::Black ? p1 : p2);

        bool skipped = false;
        for(;;)
        {
            if(!b.has_legal_move(currentPlayer->color()))
            {
                if(skipped)
                    break;
                else
                {
                    skipped = true;
                    continue;
                }
            }
            else
                skipped = false;

            b.update("It is "+currentPlayer->name()+"'s turn.");
            currentPlayer->take_turn();
            currentPlayer = (currentPlayer == p1 ? p2 : p1);
        }

        // count up total points
        int points1 = 0, points2 = 0;
        for(int y = 0; y < Board::NumberOfRows; y++)
        {
            for(int x = 0; x < Board::NumberOfColumns; x++)
            {
                if(b.space(Position(x,y)) == p1->color())
                    points1++;
                else
                    points2++;
            }
        }
        if(p1->score() != p2->score())
            b.update("Game over - " + (p1->score() > p2->score() ? p1->name() : p2->name()) + " is the winner.");
        else
            b.update("Game over - it was a tie.");

        b.end_game();
    }
}
