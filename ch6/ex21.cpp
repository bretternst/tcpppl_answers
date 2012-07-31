#include <iostream>
#include <string>
#include <map>
#include <cctype>
#include <sstream>
#include <queue>

using namespace std;

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

typedef vector<string> ParamList;
typedef vector<double> ArgumentList;
typedef vector<Token> TokenList;

struct Function
{
	ParamList params;
	TokenList tokens;
};

// Globals
Token curr_sym;
queue<Token> token_queue;
int no_of_errors = 0;
int curr_line = 0;
map<string, double> table;
map<string, Function> functions;

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
		error("bad token");
        cout << ch << endl;
		return curr_sym.curr_tok = PRINT;
	}
}

// Expressions

double expr(istream* input, bool get);

ParamList parameters(istream* input)
{
	ParamList list;
	for(;;)
	{
		get_token(input);
		if(curr_sym.curr_tok == NAME)
		{
			list.push_back(curr_sym.string_value);
		}
		else if (curr_sym.curr_tok == SEP)
		{
			continue;
		}
		else if (curr_sym.curr_tok == RP)
		{
			return list;
		}
		else if (curr_sym.curr_tok == PRINT || curr_sym.curr_tok == END)
		{
			error(") expected");
			return list;
		}
	}
}

ArgumentList arguments(istream* input)
{
	ArgumentList list;
	get_token(input);
	for(;;)
	{
		if(curr_sym.curr_tok == RP)
		{
			return list;
		}
		else if (curr_sym.curr_tok == SEP)
		{
			get_token(input);
			continue;
		}
		else if (curr_sym.curr_tok == PRINT || curr_sym.curr_tok == END)
		{
			error(") expected");
			return list;
		}
		else
		{
			list.push_back(expr(input, false));
		}
	}
}

double call(istream* input, string name)
{
	if (functions.count(name) < 1) return error("undefined function "+name);

	ArgumentList args = arguments(input);
	Function func = functions[name];
	if(func.params.size() != args.size()) return error("incorrect number of arguments");

	map<string,double> argMap;
	for(int i = 0; i < func.params.size(); i++)
	{
		argMap[func.params[i]] = args[i];
	}

	for(TokenList::const_iterator i = func.tokens.begin(); i != func.tokens.end(); i++)
	{
		Token tok = *i;
		if(tok.curr_tok == NAME && argMap.count(tok.string_value) > 0)
		{
			Token val;
			val.curr_tok = NUMBER;
			val.number_value = argMap[tok.string_value];
			token_queue.push(val);
		}
		else
			token_queue.push(*i);
	}
	return expr(input, true);
}

double func(istream* input, bool get)
{
	if (get) get_token(input);
	if(curr_sym.curr_tok != NAME) return error("function name expected");
	string name = curr_sym.string_value;
	if (get_token(input) != LP) return error("( expected");
	ParamList params = parameters(input);
	if (get_token(input) != ASSIGN) return error("= expected");
	Function func;

	get_token(input);
	while(curr_sym.curr_tok != PRINT && curr_sym.curr_tok != END)
	{
		func.tokens.push_back(curr_sym);
		get_token(input);
	}
	func.params = params;
	cout << "function created: " << name << endl;
	functions[name] = func;
	return 0;
}

double prim(istream* input, bool get)
{
	if (get) get_token(input);

	switch(curr_sym.curr_tok)
	{
	case FUNC:
		return func(input, true);
	case NUMBER:
		{
			double v = curr_sym.number_value;
			get_token(input);
			return v;
		}
	case NAME:
		{
			string name = curr_sym.string_value;
			if(get_token(input) == LP)
			{
				double v = call(input, name);
				return v;
			}
			else
			{
				double& v = table[name];
				if (curr_sym.curr_tok == ASSIGN) v = expr(input, true);
				return v;
			}
		}
	case MINUS:
		return -prim(input, true);
	case LP:
		{
			double e = expr(input, true);
			if (curr_sym.curr_tok != RP) return error("')' expected");
			get_token(input);
			return e;
		}
	default:
		return error("primary expected");
	}
}

double term(istream* input, bool get)
{
	double left = prim(input, get);

	for (;;)
	{
		switch(curr_sym.curr_tok)
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
		switch(curr_sym.curr_tok)
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
		if (curr_sym.curr_tok == END) break;
		if (curr_sym.curr_tok == PRINT) continue;
		cout << expr(input, false) << endl;
	}

	if (input != &cin) delete input;
	return no_of_errors;
}
