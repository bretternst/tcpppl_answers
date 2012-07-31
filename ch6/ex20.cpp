#include <iostream>
#include <string>
#include <map>
#include <cctype>
#include <sstream>

using namespace std;

// Token definition
enum Token_value
{
	NAME, NUMBER, END, FUNC = '%', CALL = '$',
	PLUS='+', MINUS='-', MUL='*', DIV='/',
	PRINT=';', ASSIGN='=', LP='(', RP=')'
};

// Globals
Token_value curr_tok = PRINT;
double number_value;
string string_value;
map<string, double> table;
int no_of_errors = 0;
int curr_line = 0;
map<string, string> functions;

// Prints a syntax error message
double error(const string& s)
{
	no_of_errors++;
	cerr << "error(" << curr_line+1 << "): " << s << endl;
	return 1;
}

// Acquires a token
Token_value get_token(istream* input)
{
	char ch = 0;

	do
	{
		if(!input->get(ch)) return curr_tok = END;
	} while (ch != '\n' && isspace(ch));

	switch(ch)
	{
	case '\n':
		curr_line++;
	case ';':
		return curr_tok = PRINT;
	case 0:
		return curr_tok = END;
	case '*': case '/': case '+': case '-': case '(': case ')': case '=':
		return curr_tok = Token_value(ch);
	case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7':
	case '8': case '9': case '.':
		input->putback(ch);
		*input >> number_value;
		return curr_tok = NUMBER;
	case '%':
		return curr_tok = FUNC;
	case '$':
		return curr_tok = CALL;
	default:
		if (isalpha(ch))
		{
			string_value = ch;
			while(input->get(ch) && isalnum(ch)) string_value.push_back(ch);
			input->putback(ch);
			return curr_tok = NAME;
		}
		error("bad token");
		return curr_tok = PRINT;
	}
}

// Expressions

double expr(istream* input, bool get);

double call(istream* input, bool get)
{
	if (get) get_token(input);
	if(curr_tok != NAME) return error("function name expected");

	string def;
	if(!(def = functions[string_value]).size()) return error("undefined function "+string_value);

    istringstream finput(def);
	double ret = expr(&finput, true);
	return ret;
}

double func(istream* input, bool get)
{
	if (get) get_token(input);
	if(curr_tok != NAME) return error("function name expected");
	string name = string_value;
	if (get_token(input) != ASSIGN) return error("= expected");
	string def;
	char ch;
	while (input->get(ch))
	{
		if(ch==PRINT || ch=='\n')
		{
			functions[name] = def;
			cout << "function created: " << name << ": " << def << endl;
			return 0;
		}
		else
		{
			def.push_back(ch);
		}
	}
	return error("unexpected end of input");
}

double prim(istream* input, bool get)
{
	if (get) get_token(input);

	switch(curr_tok)
	{
	case FUNC:
		return func(input, true);
	case CALL:
		{
		double v = call(input, true);
		get_token(input);
		return v;
		}
	case NUMBER:
		{
			double v = number_value;
			get_token(input);
			return v;
		}
	case NAME:
		{
			double& v = table[string_value];
			if (get_token(input) == ASSIGN) v = expr(input, true);
			return v;
		}
	case MINUS:
		return -prim(input, true);
	case LP:
		{
			double e = expr(input, true);
			if (curr_tok != RP) return error("')' expected");
			get_token(input);
			return e;
		}
	default:
		cout << curr_tok;
		return error("primary expected");
	}
}

double term(istream* input, bool get)
{
	double left = prim(input, get);

	for (;;)
	{
		switch(curr_tok)
		{
		case MUL:
			left *= prim(input, true);
			break;
		case DIV:
			if (double d = prim(input, true))
			{
				left /= d;
				break;
			}
			return error("divide by 0");
		default:
			return left;
		}
	}
}

double expr(istream* input, bool get)
{
	double left = term(input, get);

	for (;;)
	{
		switch(curr_tok)
		{
		case PLUS:
			left += term(input, true);
			break;
		case MINUS:
			left -= term(input, true);
			break;
		default:
			return left;
		}
	}
}

int main(int argc, char* argv[])
{
	istream* input;

	switch(argc)
	{
	case 1:
		input = &cin;
		break;
	case 2:
		input = new istringstream(argv[1]);
		break;
	default:
		error("too many arguments");
		return 1;
	}

	table["pi"] = 3.1415926535897932385;
	table["e"] = 2.72818284590452354;

	while (*input)
	{
		get_token(input);
		if (curr_tok == END) break;
		if (curr_tok == PRINT) continue;
		cout << expr(input, false) << endl;
	}

	if (input != &cin) delete input;
	return no_of_errors;
}
