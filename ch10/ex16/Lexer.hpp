#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <iostream>
#include <queue>

namespace Exercises
{
	using std::string;
	using std::istream;
	using std::queue;

	struct Token
	{
		enum TokenValue {
			NAME, NUMBER, END, FUNC = '%',
			PLUS='+', MINUS='-', MUL='*', DIV='/',
			PRINT=';', ASSIGN='=', LP='(', RP=')',
			SEP=','
		};

		TokenValue Type;
		double NumberValue;
		string StringValue;

		Token(TokenValue type) : Type(type) {}
		Token(TokenValue type, double value) : Type(type), NumberValue(value) {}
		Token(TokenValue type, string value) : Type(type), StringValue(value) {}
	};

	class Lexer
	{
		istream* input;
		int line;
		queue<Token> queue;

		Token ReadToken();

	public:
		Lexer(istream* input) : input(input), line(1) {}
		Token Pop();
		Token Peek();
		void Push(Token);
		int GetLine() { return line; }
		void Clear() { queue = std::queue<Token>(); }
	};
}

#endif
