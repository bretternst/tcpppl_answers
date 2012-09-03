#include <map>
#include <string>
#include "parser.h"
#include "math.h"

using std::map;
using std::string;
map<string, double> Parser::table;
map<string, Parser::Function> Parser::functions;

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
            throw Error::SyntaxError(") expected");
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
            throw Error::SyntaxError(") expected");
        }
        else
        {
            list.push_back(expr(input, false));
        }
    }
}

double Parser::call(istream* input, string name)
{
    if (functions.count(name) < 1) 
        throw Error::SyntaxError("undefined function "+name);

    ArgumentList args = arguments(input);
    Function func = functions[name];
    if(func.params.size() != args.size()) 
        throw Error::SyntaxError("incorrect number of arguments");

    if(func.type == FUNC_BUILTIN)
    {
        return func.ptr(args[0]);
    }

    map<string,double> argMap;
    for(unsigned int i = 0; i < func.params.size(); i++)
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
    if(curr_sym.curr_tok != Lexer::NAME) throw Error::SyntaxError("function name expected");
    string name = curr_sym.string_value;
    if (get_token(input) != Lexer::LP) throw Error::SyntaxError("( expected");
    ParamList params = parameters(input);
    if (get_token(input) != Lexer::ASSIGN) throw Error::SyntaxError("= expected");
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
            if (curr_sym.curr_tok != Lexer::RP) throw Error::SyntaxError("')' expected");
            get_token(input);
            return e;
        }
    default:
        throw Error::SyntaxError("primary expected");
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
            left = Math::mul(left, prim(input, true));
            break;
        case Lexer::DIV:
            if (double d = prim(input, true))
            {
                left = Math::div(left, d);
                break;
            }
            throw Error::DivideByZeroError();
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
            left = Math::plus(left, term(input, true));
            break;
        case Lexer::MINUS:
            left = Math::minus(left, term(input, true));
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
