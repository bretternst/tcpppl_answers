#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <iostream>
#include <queue>

namespace Lexer
{
	using std::string;
	using std::istream;
	using std::queue;

	// Token definition
	enum Token_value
	{
		NAME, NUMBER, END, FUNC = '%',
		PLUS='+', MINUS='-', MUL='*', DIV='/',
		PRINT=';', ASSIGN='=', LP='(', RP=')',
		SEP=','
	};

	struct Token
	{
		Token_value curr_tok;
		double number_value;
		string string_value;
	};

	Token_value get_token(istream*);

	extern Token curr_sym;
	extern queue<Token> token_queue;
	extern int curr_line;
}

#endif
