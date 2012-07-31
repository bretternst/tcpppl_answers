#include <queue>

#include "Error.hpp"
#include "Math.hpp"
#include "Lexer.hpp"

using namespace Exercises;

Token Lexer::ReadToken()
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

Token Lexer::Pop()
{
	if(queue.size() > 0)
	{
		Token tok = queue.front();
		queue.pop();
		return tok;
	}
	else
	{
		return ReadToken();
	}
}

Token Lexer::Peek()
{
	if(queue.size() < 1)
	{
		queue.push(ReadToken());
	}
	return queue.front();
}

void Lexer::Push(Token tok)
{
	queue.push(tok);
}
