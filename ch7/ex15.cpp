#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

enum Token_t
{
    TOK_NEWLINE,
    TOK_ID,
    TOK_LPAREN = '(',
    TOK_RPAREN = ')',
    TOK_DIRECTIVE = '#',
    TOK_END,
    TOK_OTHER,
    TOK_SEP = ','
};

struct Token
{
    Token_t token;
    string value;

    Token()
    {
    }
    Token(Token_t t)
    {
        this->token = t;
        switch(t) {
            case TOK_LPAREN:
            case TOK_RPAREN:
            case TOK_DIRECTIVE:
            case TOK_SEP:
                this->value.push_back(static_cast<char>(this->token));
                break;
        }
    }
    Token(Token_t t, string v)
    {
        this->token = t;
        this->value = v;
    }
    Token(Token_t t, char ch)
    {
        this->token = t;
        this->value.push_back(ch);
    }
};

typedef vector<Token> TokenList;
typedef vector<string> ParamList;
typedef vector<TokenList> ArgumentList;

struct Define
{
    ParamList params;
    TokenList tokens;

    Define()
    {
    }
    Define(ParamList p, TokenList t)
    {
        this->params = p;
        this->tokens = t;
    }
};

typedef map<string, Define> DefineList;
DefineList defines;

// Acquires a token
Token get_token(istream* input, bool ignoreWhitespace, bool echo)
{
    char ch = 0;

    if(ignoreWhitespace)
    {
        for(;;)
        {
            if(!input->get(ch)) return Token(TOK_END);

            if (!(isspace(ch) && ch != '\n'))
                break;
            else if (echo)
                cout << ch;
        }
    }
    else
    {
        if (!input->get(ch)) return Token(TOK_END);
    }

    switch(ch)
    {
    case '\n':
        if (echo)
            cout << ch;
        return Token(TOK_NEWLINE,ch);
    case '(': case ')': case '#': case ',':
        if (echo)
            cout << ch;
        return Token(Token_t(ch),ch);
    default:
        if (isalpha(ch))
        {
            string s;
            s.push_back(ch);
            while(input->get(ch) && isalnum(ch)) s.push_back(ch);
            input->putback(ch);
            int x = 0;
            return Token(TOK_ID, s);
        }
        if (echo)
            cout << ch;
        return Token(TOK_OTHER, ch);
    }
}

ParamList defineParams(istream* input)
{
    ParamList list;
    Token tok;
    while((tok = get_token(input, true, true)).token != TOK_NEWLINE && tok.token != TOK_END)
    {
        if(tok.token == TOK_ID)
        {
            cout << tok.value;
            list.push_back(tok.value);
            tok = get_token(input, true, true);
            if (tok.token == TOK_SEP) continue;
            else if (tok.token == TOK_RPAREN) break;
            cerr << "Unexpected token in macro arguments" << endl;
        }
    }
    return list;
}

TokenList defineDef(istream* input)
{
    TokenList list;
    Token tok;
    while((tok = get_token(input, true, true)).token != TOK_NEWLINE && tok.token != TOK_END)
    {
        list.push_back(tok);
        if (tok.token == TOK_ID)
            cout << tok.value;
    }
    return list;
}

void define(istream* input)
{
    Token tok = get_token(input, true, true);
    if (tok.token == TOK_ID)
    {
        cout << tok.value;
        string name = tok.value;
        tok = get_token(input, false, true);
        TokenList tokens;
        ParamList params;
        if (tok.token == TOK_LPAREN)
        {
            params = defineParams(input);
        }
        tokens = defineDef(input);
        defines[name] = Define(params,tokens);
    }
    else
    {
        cerr << "Define directive expects a name" << endl;
    }
}

void directive(istream* input)
{
    Token tok = get_token(input, true, true);
    switch(tok.token)
    {
    case TOK_ID:
        {
            cout << tok.value;
            if (tok.value == "define")
            {
                define(input);
            }
            else
            {
                cerr << "Unrecognized directive: " << tok.value << endl;
            }
            break;
        }
    default:
        cerr << "Syntax error in directive." << endl;
    }
}

ArgumentList arguments(istream* input)
{
    ArgumentList args;
    TokenList arg;
    Token tok;
    while((tok = get_token(input, true, false)).token != TOK_END)
    {
        if(tok.token == TOK_SEP || tok.token == TOK_RPAREN)
        {
            if(arg.size() < 1)
            {
                cerr << "Missing macro argument" << endl;
            }
            args.push_back(arg);
            arg.clear();
            if(tok.token == TOK_RPAREN) break;
        }
        else arg.push_back(tok);
    }
    return args;
}

void processMacro(istream* input, Define define)
{
    Token tok = get_token(input, true, false);
    ArgumentList args;
    if(tok.token == TOK_LPAREN)
    {
        args = arguments(input);
    }
    if (args.size() != define.params.size())
    {
        cerr << "Macro argument mismatch, expected " << define.params.size() << ", got " << args.size() << endl;
        return;
    }

    map<string,TokenList> argLookup;
    for(int i = 0; i < define.params.size(); i++)
    {
        if(argLookup.count(define.params[i]) > 0)
            cerr << "Warning: duplicate parameter name in macro" << endl;
        else
            argLookup[define.params[i]] = args[i];
    }

    for(TokenList::const_iterator i = define.tokens.begin(); i != define.tokens.end(); i++)
    {
        Token paramTok = *i;
        if(paramTok.token == TOK_ID && argLookup.count(paramTok.value) > 0)
        {
            TokenList argToks = argLookup[paramTok.value];
            for(TokenList::const_iterator j = argToks.begin(); j != argToks.end(); j++)
            {
                cout << (*j).value;
            }
        }
        else cout << paramTok.value;
    }

    if(tok.token != TOK_LPAREN)
        cout << tok.value;
}

void nonDirectiveLine(istream* input, Token* tok)
{
    Token t = *tok;
    for(;;)
    {
        switch(t.token)
        {
        case TOK_NEWLINE:
        case TOK_END:
            return;
        case TOK_ID:
            if(defines.count(t.value))
            {
                processMacro(input, defines[t.value]);
            }
            else
            {
                cout << t.value;
            }
            break;
        default:
            break;
        }
        t = get_token(input, false, true);
    }
}

void line(istream* input, Token* tok)
{
    Token t;
    if(tok == 0) {
        t = get_token(input, true, true);
        tok = &t;
    }
    switch(tok->token)
    {
    case TOK_END:
    case TOK_NEWLINE:
        return;
    case TOK_DIRECTIVE:
        directive(input);
        break;
    default:
        nonDirectiveLine(input, tok);
        return;
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
        input = new ifstream(argv[1]);
        break;
    default:
        cerr << "Too many arguments." << endl;
        return 1;
    }

    Token curToken;

    while (input->good())
    {
        curToken = get_token(input, true, true);
        line(input, &curToken);
    }
    cout << endl;

    if (input != &cin) delete input;
}
