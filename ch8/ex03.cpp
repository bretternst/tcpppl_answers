#include <iostream>
#include <string>
#include <map>
#include <cctype>
#include <sstream>
#include <queue>
#include <cmath>

namespace Error
{
    using std::string;

    double error(const string&);
    int no_of_errors = 0;
}

namespace Lexer
{
    using std::string;
    using std::istream;
    using std::queue;
    using Error::error;

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

    // Globals
    Token curr_sym;
    queue<Token> token_queue;
    int curr_line = 0;

    Token_value get_token(istream*);
}

namespace Parser
{
    using std::string;
    using std::vector;
    using std::map;
    using std::istream;
    using Lexer::Token;
    using Lexer::curr_sym;
    using Lexer::get_token;
    using Error::error;

    typedef vector<string> ParamList;
    typedef vector<double> ArgumentList;
    typedef vector<Token> TokenList;
    typedef double (*Function_ptr)(double);

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

    map<string, double> table;
    map<string, Function> functions;

    void createFunc(string, Function_ptr);

    ParamList parameters(istream*);
    ArgumentList arguments(istream*);
    double call(istream*, string);
    double expr(istream*, bool);
    double func(istream*, bool);
    double prim(istream*, bool);
    double term(istream*, bool);
}


// Prints a syntax error message
double Error::error(const string& s)
{
    using std::cerr;
    using std::endl;
    no_of_errors++;
    cerr << "error(" << Lexer::curr_line+1 << "): " << s << endl;
    return 1;
}

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
        error("bad token");
        return curr_sym.curr_tok = PRINT;
    }
}

Parser::ParamList Parser::parameters(istream* input)
{
    ParamList list;
    for(;;)
    {
        get_token(input);
        if(curr_sym.curr_tok == Lexer::NAME)
        {
            list.push_back(curr_sym.string_value);
        }
        else if (curr_sym.curr_tok == Lexer::SEP)
        {
            continue;
        }
        else if (curr_sym.curr_tok == Lexer::RP)
        {
            return list;
        }
        else if (curr_sym.curr_tok == Lexer::PRINT || curr_sym.curr_tok == Lexer::END)
        {
            error(") expected");
            return list;
        }
    }
}

Parser::ArgumentList Parser::arguments(istream* input)
{
    ArgumentList list;
    get_token(input);
    for(;;)
    {
        if(curr_sym.curr_tok == Lexer::RP)
        {
            return list;
        }
        else if (curr_sym.curr_tok == Lexer::SEP)
        {
            get_token(input);
            continue;
        }
        else if (curr_sym.curr_tok == Lexer::PRINT || curr_sym.curr_tok == Lexer::END)
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

double Parser::call(istream* input, string name)
{
    if (functions.count(name) < 1) return error("undefined function "+name);

    ArgumentList args = arguments(input);
    Function func = functions[name];
    if(func.params.size() != args.size()) return error("incorrect number of arguments");

    if(func.type == FUNC_BUILTIN)
    {
        return func.ptr(args[0]);
    }

    map<string,double> argMap;
    for(int i = 0; i < func.params.size(); i++)
    {
        argMap[func.params[i]] = args[i];
    }

    for(TokenList::const_iterator i = func.tokens.begin(); i != func.tokens.end(); i++)
    {
        Token tok = *i;
        if(tok.curr_tok == Lexer::NAME && argMap.count(tok.string_value) > 0)
        {
            Token val;
            val.curr_tok = Lexer::NUMBER;
            val.number_value = argMap[tok.string_value];
            Lexer::token_queue.push(val);
        }
        else
            Lexer::token_queue.push(*i);
    }
    return expr(input, true);
}

double Parser::func(istream* input, bool get)
{
    if (get) get_token(input);
    if(curr_sym.curr_tok != Lexer::NAME) return error("function name expected");
    string name = curr_sym.string_value;
    if (get_token(input) != Lexer::LP) return error("( expected");
    ParamList params = parameters(input);
    if (get_token(input) != Lexer::ASSIGN) return error("= expected");
    Function func;

    get_token(input);
    while(curr_sym.curr_tok != Lexer::PRINT && curr_sym.curr_tok != Lexer::END)
    {
        func.tokens.push_back(curr_sym);
        get_token(input);
    }
    func.params = params;
    func.type = FUNC_STANDARD;
    functions[name] = func;
    return 0;
}

double Parser::prim(istream* input, bool get)
{
    if (get) get_token(input);

    switch(curr_sym.curr_tok)
    {
    case Lexer::FUNC:
        return func(input, true);
    case Lexer::NUMBER:
        {
            double v = curr_sym.number_value;
            get_token(input);
            return v;
        }
    case Lexer::NAME:
        {
            string name = curr_sym.string_value;
            if(get_token(input) == Lexer::LP)
            {
                double v = call(input, name);
                return v;
            }
            else
            {
                double& v = table[name];
                if (curr_sym.curr_tok == Lexer::ASSIGN) v = expr(input, true);
                return v;
            }
        }
    case Lexer::MINUS:
        return -prim(input, true);
    case Lexer::LP:
        {
            double e = expr(input, true);
            if (curr_sym.curr_tok != Lexer::RP) return error("')' expected");
            get_token(input);
            return e;
        }
    default:
        return error("primary expected");
    }
}

double Parser::term(istream* input, bool get)
{
    double left = prim(input, get);

    for (;;)
    {
        switch(curr_sym.curr_tok)
        {
        case Lexer::MUL:
            left *= prim(input, true);
            break;
        case Lexer::DIV:
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

double Parser::expr(istream* input, bool get)
{
    double left = term(input, get);

    for (;;)
    {
        switch(curr_sym.curr_tok)
        {
        case Lexer::PLUS:
            left += term(input, true);
            break;
        case Lexer::MINUS:
            left -= term(input, true);
            break;
        default:
            return left;
        }
    }
}

void Parser::createFunc(string name, Function_ptr fPtr)
{
    Function func;
    func.params.push_back("d");
    func.type = FUNC_BUILTIN;
    func.ptr = fPtr;
    functions[name] = func;
}


int main(int argc, char* argv[])
{
    using namespace std;

    using Lexer::curr_sym;
    using Lexer::get_token;
    using Parser::expr;

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
        Error::error("too many arguments");
        return 1;
    }

    // Create built-in variables
    Parser::table["pi"] = 3.1415926535897932385;
    Parser::table["e"] = 2.72818284590452354;

    // Create built-in functions
    Parser::createFunc("sqrt", sqrt);
    Parser::createFunc("log", log);
    Parser::createFunc("sin", sin);

    while (*input)
    {
        get_token(input);
        if (curr_sym.curr_tok == Lexer::END) break;
        if (curr_sym.curr_tok == Lexer::PRINT) continue;
        cout << expr(input, false) << endl;
    }

    if (input != &cin) delete input;
    return Error::no_of_errors;
}
