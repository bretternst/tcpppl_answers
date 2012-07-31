#include <iostream>
#include <string>
#include <sstream>
#include <vector>

namespace Exercises
{
	using std::istream;
	using std::ostream;
	using std::string;
	using std::istringstream;

// When this is undefined, a node representation will be used internally, as opposed
// to a simple string.
//#define USE_STRING_REPRESENTATION

#ifdef USE_STRING_REPRESENTATION
	class Expr
	{
		enum TokenValue
		{
			NUMBER, END, PLUS='+', MINUS='-', MUL='*', DIV='/', LP='(', RP=')'
		};

		struct Token
		{
			TokenValue token;
			int value;
		};

		class SyntaxError {};
		class DivideByZeroError {};

		Token curr;
		string expression;

		TokenValue get_token(istream* input);
		int prim(istream* input);
		int term(istream* input);
		int expr(istream* input);

	public:
		Expr(const char* expr);
		int eval();
		void print(ostream& output);
	};

	Expr::Expr(const char* expr) : expression(expr)
	{
	}

	int Expr::eval()
	{
		istringstream input(expression);
		int i = expr(&input);
		if(get_token(&input) != Expr::END)
			throw SyntaxError();
		return i;
	}

	Expr::TokenValue Expr::get_token(istream* input)
	{
		char ch = 0;
		do
		{
			if(!input->get(ch)) return curr.token = Expr::END;
		} while (isspace(ch));

		switch(ch)
		{
		case 0:
			return curr.token = Expr::END;
		case '*': case '/': case '+': case '-': case '(': case ')':
			return curr.token = Expr::TokenValue(ch);
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
			input->putback(ch);
			*input >> curr.value;
			return curr.token = Expr::NUMBER;
		default:
			throw SyntaxError();
		}
	}

	int Expr::prim(istream* input)
	{
		get_token(input);

		switch(curr.token)
		{
		case Expr::NUMBER:
			{
				int i = curr.value;
				get_token(input);
				return i;
			}
		case Expr::MINUS:
			return -prim(input);
		case Expr::LP:
			{
				int i = expr(input);
				if(curr.token != Expr::RP) throw SyntaxError();
				get_token(input);
				return i;
			}
		default:
			throw SyntaxError();
		}
	}

	int Expr::term(istream* input)
	{
		int left = prim(input);

		for(;;)
		{
			switch(curr.token)
			{
			case Expr::MUL:
				left *= prim(input);
				break;
			case Expr::DIV:
				if(int i = prim(input))
				{
					left /= i;
					break;
				}
				throw DivideByZeroError();
			default:
				return left;
			}
		}
		return left;
	}

	int Expr::expr(istream* input)
	{
		int left = term(input);

		for (;;)
		{
			switch(curr.token)
			{
			case Expr::PLUS:
				left += term(input);
				break;
			case Expr::MINUS:
				left -= term(input);
				break;
			default:
				return left;
			}
		}
		return left;
	}

	#else

	class Expr
	{
		enum TokenValue
		{
			NUMBER, END, UNKNOWN, PLUS='+', MINUS='-', MUL='*', DIV='/', LP='(', RP=')'
		};

		struct Token
		{
			TokenValue token;
			int value;
			Token* next;

			Token(TokenValue token) : token(token), value(0), next(0) {}
			Token(TokenValue token, int value) : token(token), value(value), next(0) {}

			~Token()
			{
				if(next) delete next;
			}
		};

		Token* first;
		Token* last;

		Token* get_token(istream* input) const;
		int prim(Token*& tok) const;
		int term(Token*& tok) const;
		int expr(Token*& tok) const;

		void print_prim(Token*& tok, ostream& output) const;
		void print_term(Token*& tok, ostream& output) const;
		void print_expr(Token*& tok, ostream& output) const;

		void print_prim_postfix(Token*& tok, ostream& output) const;
		void print_term_postfix(Token*& tok, ostream& output) const;
		void print_expr_postfix(Token*& tok, ostream& output) const;

		void print_prim_il(Token*& tok, ostream& output) const;
		void print_term_il(Token*& tok, ostream& output) const;
		void print_expr_il(Token*& tok, ostream& output) const;

		void print_prim_asm(Token*& tok, ostream& output) const;
		void print_term_asm(Token*& tok, ostream& output) const;
		void print_expr_asm(Token*& tok, ostream& output) const;

	public:
		class SyntaxError {};
        class DivideByZeroError {};

		Expr(const char* expr);
		~Expr() { if(first) delete first; }
		int eval() const;
		void print_paren(ostream& output) const;
		void print_postfix(ostream& output) const;
		void print_il(ostream& output) const;
		void print_asm(ostream& output) const;
	};

	Expr::Expr(const char* expr) : first(new Token(Expr::UNKNOWN)), last(first)
	{
		istringstream input(expr);
		for(;;)
		{
			Token* tok = get_token(&input);
			last->next = tok;
			last = last->next;
			if(tok->token==Expr::END) break;
		}
	}

	int Expr::eval() const
	{
		Token* p = first;
		int i = expr(p);
		if(p->token != Expr::END) throw SyntaxError();
		return i;
	}

	Expr::Token* Expr::get_token(istream* input) const
	{
		char ch = 0;
		do
		{
			if(!input->get(ch)) return new Token(Expr::END);
		} while (isspace(ch));

		switch(ch)
		{
		case 0:
			return new Token(Expr::END);
		case '*': case '/': case '+': case '-': case '(': case ')':
			return new Token(Expr::TokenValue(ch));
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
			input->putback(ch);
			int i;
			*input >> i;
			return new Token(Expr::NUMBER, i);
		default:
			return new Token(Expr::UNKNOWN);
		}
	}

	int Expr::prim(Token*& tok) const
	{
		tok = tok->next;

		switch(tok->token)
		{
		case Expr::NUMBER:
			{
				int i = tok->value;
				tok = tok->next;
				return i;
			}
		case Expr::MINUS:
			return -prim(tok);
		case Expr::LP:
			{
				int i = expr(tok);
				if(tok->token != Expr::RP) throw SyntaxError();
				tok = tok->next;
				return i;
			}
		default:
			throw SyntaxError();
		}
	}

	int Expr::term(Token*& tok) const
	{
		int left = prim(tok);

		for(;;)
		{
			switch(tok->token)
			{
			case Expr::MUL:
				left *= prim(tok);
				break;
			case Expr::DIV:
				if(int i = prim(tok))
				{
					left /= i;
					break;
				}
				throw DivideByZeroError();
			default:
				return left;
			}
		}
		return left;
	}

	int Expr::expr(Token*& tok) const
	{
		int left = term(tok);

		for (;;)
		{
			switch(tok->token)
			{
			case Expr::PLUS:
				left += term(tok);
				break;
			case Expr::MINUS:
				left -= term(tok);
				break;
			default:
				return left;
			}
		}
		return left;
	}

	void Expr::print_prim(Token*& tok, ostream& output) const
	{
		tok = tok->next;

		switch(tok->token)
		{
		case Expr::NUMBER:
			{
				output << tok->value;
				tok = tok->next;
				break;
			}
		case Expr::MINUS:
			output << "-";
			print_prim(tok, output);
			break;
		case Expr::LP:
			{
				print_expr(tok, output);
				if(tok->token != Expr::RP) throw SyntaxError();
				tok = tok->next;
				break;
			}
		default:
			throw SyntaxError();
		}
	}

	void Expr::print_term(Token*& tok, ostream& output) const
	{
		output << "(";
		print_prim(tok, output);

		for(;;)
		{
			switch(tok->token)
			{
			case Expr::MUL:
			case Expr::DIV:
				output << static_cast<char>(tok->token);
				print_prim(tok, output);
				continue;
			default:
				output << ")";
				return;
			}
		}
	}

	void Expr::print_expr(Token*& tok, ostream& output) const
	{
		output << "(";
		print_term(tok, output);

		for(;;)
		{
			switch(tok->token)
			{
			case Expr::PLUS:
			case Expr::MINUS:
				output << static_cast<char>(tok->token);
				print_term(tok, output);
				continue;
			default:
				output << ")";
				return;
			}
		}
	}

	void Expr::print_paren(ostream& output) const
	{
		Token* p = first;
		print_expr(p, output);
		if(p->token != Expr::END) throw SyntaxError();
	}

	void Expr::print_prim_postfix(Token*& tok, ostream& output) const
	{
		tok = tok->next;

		switch(tok->token)
		{
		case Expr::NUMBER:
			{
				output << " " << tok->value;
				tok = tok->next;
				break;
			}
		case Expr::MINUS:
			print_prim_postfix(tok, output);
			output << " -";
			break;
		case Expr::LP:
			{
				print_expr_postfix(tok, output);
				if(tok->token != Expr::RP) throw SyntaxError();
				tok = tok->next;
				break;
			}
		default:
			throw SyntaxError();
		}
	}

	void Expr::print_term_postfix(Token*& tok, ostream& output) const
	{
		print_prim_postfix(tok, output);

		for(;;)
		{
			switch(tok->token)
			{
			case Expr::MUL:
			case Expr::DIV:
				{
					Token* t = tok;
					print_prim_postfix(tok, output);
					output << " " << static_cast<char>(t->token);
					continue;
				}
			default:
				return;
			}
		}
	}

	void Expr::print_expr_postfix(Token*& tok, ostream& output) const
	{
		print_term_postfix(tok, output);

		for(;;)
		{
			switch(tok->token)
			{
			case Expr::PLUS:
			case Expr::MINUS:
				{
					Token* t = tok;
					print_term_postfix(tok, output);
					output << " " << static_cast<char>(t->token);
					continue;
				}
			default:
				return;
			}
		}
	}

	void Expr::print_postfix(ostream& output) const
	{
		Token* p = first;
		print_expr_postfix(p, output);
		if(p->token != Expr::END) throw SyntaxError();
	}

	void Expr::print_prim_il(Token*& tok, ostream& output) const
	{
		tok = tok->next;

		switch(tok->token)
		{
		case Expr::NUMBER:
			{
				output << "push " << tok->value << std::endl;
				tok = tok->next;
				break;
			}
		case Expr::MINUS:
			print_prim_il(tok, output);
			output << "neg" << std::endl;
			break;
		case Expr::LP:
			{
				print_expr_il(tok, output);
				if(tok->token != Expr::RP) throw SyntaxError();
				tok = tok->next;
				break;
			}
		default:
			throw SyntaxError();
		}
	}

	void Expr::print_term_il(Token*& tok, ostream& output) const
	{
		print_prim_il(tok, output);

		for(;;)
		{
			switch(tok->token)
			{
			case Expr::MUL:
				{
					Token* t = tok;
					print_prim_il(tok, output);
					output << "mul" << std::endl;
					continue;
				}
			case Expr::DIV:
				{
					Token* t = tok;
					print_prim_il(tok, output);
					output << "div" << std::endl;
					continue;
				}
			default:
				return;
			}
		}
	}

	void Expr::print_expr_il(Token*& tok, ostream& output) const
	{
		print_term_il(tok, output);

		for(;;)
		{
			switch(tok->token)
			{
			case Expr::PLUS:
				{
					Token* t = tok;
					print_term_il(tok, output);
					output << "add" << std::endl;
					continue;
				}
			case Expr::MINUS:
				{
					Token* t = tok;
					print_term_il(tok, output);
					output << "sub" << std::endl;
					continue;
				}
			default:
				return;
			}
		}
	}

	void Expr::print_il(ostream& output) const
	{
		Token* p = first;
		print_expr_il(p, output);
		if(p->token != Expr::END) throw SyntaxError();
	}

	void Expr::print_prim_asm(Token*& tok, ostream& output) const
	{
		tok = tok->next;

		switch(tok->token)
		{
		case Expr::NUMBER:
			{
				output << "mov eax, " << tok->value << std::endl;
				output << "push eax" << std::endl;
				tok = tok->next;
				break;
			}
		case Expr::MINUS:
			print_prim_asm(tok, output);
			output << "pop eax" << std::endl;
			output << "neg eax" << std::endl;
			output << "push eax" << std::endl;
			break;
		case Expr::LP:
			{
				print_expr_asm(tok, output);
				if(tok->token != Expr::RP) throw SyntaxError();
				tok = tok->next;
				break;
			}
		default:
			throw SyntaxError();
		}
	}

	void Expr::print_term_asm(Token*& tok, ostream& output) const
	{
		print_prim_asm(tok, output);

		for(;;)
		{
		switch(tok->token)
		{
			case Expr::MUL:
				{
					Token* t = tok;
					print_prim_asm(tok, output);
					output << "pop eax" << std::endl;
					output << "pop ebx" << std::endl;
					output << "imul ebx" << std::endl;
					output << "push eax" << std::endl;
					continue;
				}
			case Expr::DIV:
				{
					Token* t = tok;
					print_prim_asm(tok, output);
					output << "pop eax" << std::endl;
					output << "pop ebx" << std::endl;
					output << "idiv ebx" << std::endl;
					output << "push eax" << std::endl;
					continue;
				}
			default:
				return;
			}
		}
	}

	void Expr::print_expr_asm(Token*& tok, ostream& output) const
	{
		print_term_asm(tok, output);

		for(;;)
		{
			switch(tok->token)
			{
			case Expr::PLUS:
				{
					Token* t = tok;
					print_term_asm(tok, output);
					output << "pop eax" << std::endl;
					output << "pop ebx" << std::endl;
					output << "add eax,ebx" << std::endl;
					output << "push eax" << std::endl;
					continue;
				}
			case Expr::MINUS:
				{
					Token* t = tok;
					print_term_asm(tok, output);
					output << "pop eax" << std::endl;
					output << "pop ebx" << std::endl;
					output << "sub eax,ebx" << std::endl;
					output << "push eax" << std::endl;
					continue;
				}
			default:
				return;
			}
		}
	}

	void Expr::print_asm(ostream& output) const
	{
		Token* p = first;
		print_expr_asm(p, output);
		if(p->token != Expr::END) throw SyntaxError();
	}

#endif
}


int main(int argc, char* argv[])
{
	using namespace std;
	using namespace Exercises;

	if(argc != 2)
	{
		cerr << "Incorrect number of arguments" << endl;
        return 1;
	}

#ifndef USE_STRING_REPRESENTATION
	try
	{
		Expr e(argv[1]);
		cout << "evaluates to: " << e.eval() << endl;
		cout << "Fully parenthesized: ";
		e.print_paren(cout);
		cout << endl;
		cout << "Postfix notation: ";
		e.print_postfix(cout);
		cout << endl;
		cout << "Converted to IL: " << endl;
		e.print_il(cout);
		cout << "Converted to x86 Assembly: " << endl;
		e.print_asm(cout);
	}
	catch(Expr::SyntaxError)
	{
		cerr << "Syntax error" << endl;
	}
	catch(Expr::DivideByZeroError)
	{
		cerr << "Divide by zero error" << endl;
	}
#endif
}
