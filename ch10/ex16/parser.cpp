#include <map>
#include <string>
#include <sstream>
#include <cmath>
#include "parser.h"
#include "math.h"

using std::map;
using std::string;

using namespace ch10;

Calculator::Calculator()
{
    register_variable("pi", 3.1415926535897932385);
    register_variable("e", 2.72818284590452354);

    register_function("sqrt", sqrt);
    register_function("log", log);
    register_function("sin", sin);
    register_function("cos", cos);
}

ParamList Calculator::parameters(Lexer& lexer)
{
    ParamList list;
    for(;;)
    {
        Token tok = lexer.pop();
        switch(tok.Type)
        {
        case Token::NAME:
            list.push_back(tok.StringValue);
            break;
        case Token::SEP:
            continue;
        case Token::RP:
            return list;
        default:
            throw Error::SyntaxError("')' expected");
        }
    }
}

ArgumentList Calculator::arguments(Lexer& lexer)
{
    ArgumentList list;
    for(;;)
    {
        Token tok = lexer.peek();
        switch(tok.Type)
        {
        case Token::RP:
            lexer.pop();
            return list;
        case Token::SEP:
            lexer.pop();
            continue;
        case Token::PRINT:
        case Token::END:
            throw Error::SyntaxError("')' expected");
        default:
            list.push_back(expr(lexer));
            break;
        }
    }
}

double Calculator::call(Lexer& lexer, string name)
{
    SymEntry* sym = symbols.Get(name);
    if(!sym)
    {
        throw Error::SyntaxError("undefined function "+name);
    }

    ArgumentList args = arguments(lexer);

    switch(sym->type)
    {
    case SymEntry::SYS_FUNC:
        {
            SymSysFunc* sFunc = static_cast<SymSysFunc*>(sym);
            if(args.size() != 1)
                throw Error::SyntaxError("incorrect number of arguments");
            return sFunc->func(args[0]);
        }
    case SymEntry::USR_FUNC:
        {
            SymUsrFunc* uFunc = static_cast<SymUsrFunc*>(sym);
            if(args.size() != uFunc->params.size())
                throw Error::SyntaxError("incorrect number of arguments");
            map<string,double> argMap;
            for(unsigned int i = 0; i < uFunc->params.size(); i++)
            {
                argMap[uFunc->params[i]] = args[i];
            }
            for(TokenList::const_iterator i = uFunc->func.begin(); i != uFunc->func.end(); i++)
            {
                Token tok = *i;
                if(tok.Type == Token::NAME && argMap.count(tok.StringValue) > 0)
                {
                    Token val(Token::NUMBER, argMap[tok.StringValue]);
                    lexer.push(val);
                }
                else
                    lexer.push(*i);
            }
            return expr(lexer);
        }
    default:
        throw Error::SyntaxError("not a function: "+name);
    }
}

double Calculator::func(Lexer& lexer)
{
    Token tok = lexer.pop();
    if(tok.Type != Token::NAME) throw Error::SyntaxError("function name expected");
    string name = tok.StringValue;
    tok = lexer.pop();
    if(tok.Type != Token::LP) throw Error::SyntaxError("'(' expected");
    ParamList params = parameters(lexer);
    tok = lexer.pop();
    if(tok.Type != Token::ASSIGN) throw Error::SyntaxError("'=' expected");

    TokenList tokens;
    tok = lexer.pop();
    while(tok.Type != Token::PRINT && tok.Type != Token::END)
    {
        tokens.push_back(tok);
        tok = lexer.pop();
    }
    lexer.push(tok);
    symbols.Set(name, new SymUsrFunc(tokens,params));
    return 0;
}

double Calculator::prim(Lexer& lexer)
{
    Token tok = lexer.peek();
    switch(tok.Type)
    {
    case Token::FUNC:
        lexer.pop();
        return func(lexer);
    case Token::NUMBER:
        return lexer.pop().NumberValue;
    case Token::NAME:
        {
            string name = tok.StringValue;
            lexer.pop();
            if(lexer.peek().Type == Token::LP)
            {
                lexer.pop();
                return call(lexer, name);
            }
            else if (lexer.peek().Type == Token::ASSIGN)
            {
                lexer.pop();
                SymEntry* sym = symbols.Get(name);
                if(sym && sym->type != SymEntry::VAR) throw Error::SyntaxError("not a variable: "+name);
                double v = expr(lexer);
                symbols.Set(name, new SymVariable(v));
                return v;
            }
            else
            {
                SymEntry* sym = symbols.Get(name);
                if(!sym) throw Error::SyntaxError("undefined symbol: "+name);
                if(sym->type != SymEntry::VAR) throw Error::SyntaxError("not a variable: "+name);
                SymVariable* sVar = static_cast<SymVariable*>(sym);
                return sVar->value;
            }
        }
    case Token::MINUS:
        lexer.pop();
        return -prim(lexer);
    case Token::LP:
        {
            lexer.pop();
            double e = expr(lexer);
            Token tok = lexer.pop();
            if(tok.Type != Token::RP) throw Error::SyntaxError("')' expected");
            return e;
        }
    default:
        throw Error::SyntaxError("primary expected");
    }
}

double Calculator::term(Lexer& lexer)
{
    double left = prim(lexer);

    for(;;)
    {
        switch(lexer.peek().Type)
        {
        case Token::MUL:
            lexer.pop();
            left = Math::mul(left, prim(lexer));
            break;
        case Token::DIV:
            lexer.pop();
            if(double d = prim(lexer))
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

double Calculator::expr(Lexer& lexer)
{
    double left = term(lexer);

    for(;;)
    {
        switch(lexer.peek().Type)
        {
        case Token::PLUS:
            lexer.pop();
            left = Math::plus(left, term(lexer));
            break;
        case Token::MINUS:
            lexer.pop();
            left = Math::minus(left, term(lexer));
            break;
        default:
            return left;
        }
    }
}

void Calculator::register_function(const string& name, FunctionPtr func)
{
    symbols.Set(name, new SymSysFunc(func));
}

void Calculator::register_variable(const string& name, double value)
{
    symbols.Set(name, new SymVariable(value));
}

double Calculator::eval(const string& expression)
{
    std::istringstream istr(expression);
    Lexer lex(&istr);
    return expr(lex);
}

int Calculator::eval(istream* input, ostream* output, ostream* error)
{
    using std::endl;

    int numErrors = 0;

    Lexer lexer(input);

    while(*input)
    {
        try
        {
            Token tok = lexer.peek();
            if(tok.Type == Token::END) break;
            if(tok.Type == Token::PRINT) { lexer.pop(); continue; }
            double v = expr(lexer);
            tok = lexer.peek();
            if(tok.Type != Token::END && tok.Type != Token::PRINT)
                throw Error::SyntaxError("unexpected token");
            *output << v << endl;
        }
        catch(Error::DivideByZeroError)
        {
            numErrors++;
            if(error)
            {
                *error << "Line " << lexer.get_line() << ": attempt to divide by zero" << endl;
            }
        }
        catch(Error::SyntaxError err)
        {
            numErrors++;
            if(error)
            {
                *error << "Line " << lexer.get_line() << ": syntax error: " << err.errMsg << endl;
            }
            if(lexer.peek().Type != Token::PRINT)
                Error::skip(input);
            lexer.clear();
        }
        catch(Error::OverflowError)
        {
            numErrors++;
            if(error)
            {
                *error << "Line " << lexer.get_line() << ": overflow" << endl;
            }
        }
    }

    return numErrors;
}
