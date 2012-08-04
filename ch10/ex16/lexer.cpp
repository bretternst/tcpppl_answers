#include <queue>

#include "error.h"
#include "math.h"
#include "lexer.h"

using namespace ch10;

Token Lexer::read_token()
{
    char ch = 0;

    do
    {
        if(!input->get(ch)) return Token(Token::END);
    } while (ch != '\n' && isspace(ch));

    switch(ch)
    {
    case '\n':
        line++;
    case ';':
        return Token(Token::PRINT);
    case 0:
        return Token(Token::END);
    case '*': case '/': case '+': case '-': case '(': case ')': case '=': case ',':
        return Token(Token::TokenValue(ch));
    case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7':
    case '8': case '9': case '.':
        {
            double d;
            input->putback(ch);
            *input >> d;
            return Token(Token::NUMBER, d);
        }
    case '%':
        return Token(Token::FUNC);
    default:
        if (isalpha(ch))
        {
            string ssym;
            ssym.push_back(ch);
            while(input->get(ch) && isalnum(ch)) ssym.push_back(ch);
            input->putback(ch);
            return Token(Token::NAME, ssym);
        }
        throw Error::SyntaxError("bad token");
    }
}

Token Lexer::pop()
{
    if(m_queue.size() > 0)
    {
        Token tok = m_queue.front();
        m_queue.pop();
        return tok;
    }
    else
    {
        return read_token();
    }
}

Token Lexer::peek()
{
    if(m_queue.size() < 1)
    {
        m_queue.push(read_token());
    }
    return m_queue.front();
}

void Lexer::push(Token tok)
{
    m_queue.push(tok);
}
