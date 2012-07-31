#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "error.h"
#include "lexer.h"
#include "table.h"

namespace Parser
{
	using std::string;
	using std::vector;
	using std::map;
	using std::istream;
	using Lexer::Token;
	using Lexer::curr_sym;
	using Lexer::get_token;

	typedef vector<string> ParamList;
	typedef vector<double> ArgumentList;
	typedef vector<Token> TokenList;
	typedef double (*Function_ptr)(double);
	typedef Table::Table<double> ValueTable;

	enum Function_type
	{
		FUNC_STANDARD,
		FUNC_BUILTIN
	};

	struct Function
	{
		ParamList params;
		TokenList tokens;
		Function_type type;
		Function_ptr ptr;
	};

	extern ValueTable table;
	extern map<string, Parser::Function> functions;

	void createFunc(string, Function_ptr);

	ParamList parameters(istream*);
	ArgumentList arguments(istream*);
	double call(istream*, string);
	double expr(istream*, bool);
	double func(istream*, bool);
	double prim(istream*, bool);
	double term(istream*, bool);
}

#endif
