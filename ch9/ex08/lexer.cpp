#include <queue>

#include "error.h"
#include "math.h"
#include "lexer.h"

using Lexer::Token;

Token Lexer::curr_sym;
std::queue<Token> Lexer::token_queue;
int Lexer::curr_line = 0;

// Acquires a token
Lexer::Token_value Lexer::get_token(istream* input)
{
    if(token_queue.size() > 0)
    {
        curr_sym = token_queue.front();
        token_queue.pop();
        return curr_sym.curr_tok;
    }

    char ch = 0;

    do
    {
        if(!input->get(ch)) return curr_sym.curr_tok = END;
    } while (ch != '\n' && isspace(ch));

    switch(ch)
    {
    case '\n':
        curr_line++;
    case ';':
        return curr_sym.curr_tok = PRINT;
    case 0:
        return curr_sym.curr_tok = END;
    case '*': case '/': case '+': case '-': case '(': case ')': case '=': case ',':
        return curr_sym.curr_tok = Token_value(ch);
    case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7':
    case '8': case '9': case '.':
        input->putback(ch);
        *input >> curr_sym.number_value;
        return curr_sym.curr_tok = NUMBER;
    case '%':
        return curr_sym.curr_tok = FUNC;
    default:
        if (isalpha(ch))
        {
            curr_sym.string_value = ch;
            while(input->get(ch) && isalnum(ch)) curr_sym.string_value.push_back(ch);
            input->putback(ch);
            return curr_sym.curr_tok = NAME;
        }
        throw Error::SyntaxError("bad token");
    }
}
