#include <iostream>
#include <cmath>
#include "dc.h"
#include "lexer.h"
#include "parser.h"
#include "error.h"

int DeskCalc::run(std::istream* input, std::ostream* output, std::ostream* error)
{
	using namespace std;
	using Lexer::curr_sym;
	using Lexer::get_token;
	using Parser::expr;

	int no_of_errors = 0;

	// Create built-in variables
	Parser::table["pi"] = 3.1415926535897932385;
	Parser::table["e"] = 2.72818284590452354;

	// Create built-in functions
	Parser::createFunc("sqrt", sqrt);
	Parser::createFunc("log", log);
	Parser::createFunc("sin", sin);

	while (*input)
	{
		try
		{
			get_token(input);
			if (curr_sym.curr_tok == Lexer::END) break;
			if (curr_sym.curr_tok == Lexer::PRINT) continue;
			*output << expr(input, false) << endl;
		}
		catch(Error::DivideByZeroError)
		{
			no_of_errors++;
			*error << "Line " << Lexer::curr_line << ": attempt to divide by zero" << endl;
		}
		catch(Error::SyntaxError err)
		{
			no_of_errors++;
			*error << "Line " << Lexer::curr_line << ": syntax error: " << err.errMsg << endl;
			Error::skip(input);
		}
		catch(Error::OverflowError)
		{
			no_of_errors++;
			*error << "Line " << Lexer::curr_line << ": overflow" << endl;
		}
	}

	return no_of_errors;
}
