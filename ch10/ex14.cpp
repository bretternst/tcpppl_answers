#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <map>

namespace Exercises
{
    using std::istream;
    using std::ostream;
    using std::string;
    using std::istringstream;
    using std::queue;
    using std::map;

    struct Token
    {
        enum TokenValue
        {
            NUMBER, END, UNKNOWN, SYMBOL, PLUS='+', MINUS='-', MUL='*', DIV='/', LP='(', RP=')', EQ='='
        };
        TokenValue token;
        int value;
        string symbol;

        Token(TokenValue token) : token(token), value(0) {}
        Token(TokenValue token, int value) : token(token), value(value) {}
        Token(TokenValue token, string& symbol) : token(token), symbol(symbol) {}
    };
    typedef queue<Token> TokenQueue;

    struct SymEntry
    {
        enum SymType {
            NONE, VAR
        };
        SymType type;

        SymEntry() : type(NONE) {}
        SymEntry(SymType type) : type(type) {}
    };

    struct SymVariable : SymEntry
    {
        int value;

        SymVariable(int value) : SymEntry(VAR), value(value) {}
    };

    class SymbolTable
    {
        map<string, SymEntry*> items;

    public:
        bool exists(const string& name) const { return items.count(name) > 0; }
        void set(string& name, SymEntry* symbol) { items[name] = symbol; }
        SymEntry* Get(string& name) { return items[name]; }
        ~SymbolTable() { for(map<string,SymEntry*>::const_iterator i = items.begin(); i != items.end(); i++)delete i->second; }
    };
    
    // Now that the expression class needs to maintain a symbol table, we cannot
    // create a new instance of the class every time we want to evaluate a new expression.
    // Otherwise, there would be no point in setting variables, as their values would
    // immediately be forgotten.
    //
    // I could also have had some kind of "state table" that the caller passes in, but
    // it seemed cleaner simply to modify Expr to evaluate a number of expressions in
    // sequence. Therefore, eval now takes a string.
    //
    // I have also streamlined some of the internals, so that tokens are not stored as
    // a field, and passed using a queue instead of a linked list.
    //
    // The symbol table itself is simplified because there is only one type of symbol: a
    // variable.
    class Expr
    {
        SymbolTable symTab;

        Token get_token(istream* input);
        int prim(TokenQueue& tq);
        int term(TokenQueue& tq);
        int expr(TokenQueue& tq);

    public:
        class SyntaxError
        {
        public:
            const char* message;
            SyntaxError() : message("") {}
            SyntaxError(const char* message) : message(message) {}
        };

        class DivideByZeroError {};

        int eval(string expr);
    };

    int Expr::eval(string expr)
    {
        istringstream istr(expr);

        TokenQueue tq;

        for(;;)
        {
            Token tok = get_token(&istr);
            tq.push(tok);
            if(tok.token == Token::END) break;
        }

        int i = this->expr(tq);
        if(tq.front().token != Token::END) throw SyntaxError("expected end of expression");
        return i;
    }

    Token Expr::get_token(istream* input)
    {
        char ch = 0;
        do
        {
            if(!input->get(ch)) return Token(Token::END);
        } while (isspace(ch));

        switch(ch)
        {
        case 0:
            return Token(Token::END);
        case '*': case '/': case '+': case '-': case '(': case ')': case '=':
            return Token(Token::TokenValue(ch));
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            input->putback(ch);
            int i;
            *input >> i;
            return Token(Token::NUMBER, i);
        default:
            if(isalpha(ch))
            {
                string ssym;
                ssym.push_back(ch);
                while(input->get(ch))
                {
                    if(!isalnum(ch)){ input->putback(ch); break; }
                    ssym.push_back(ch);
                }
                return Token(Token::SYMBOL, ssym);
            }
            else
                return Token(Token::UNKNOWN);
        }
    }

    int Expr::prim(TokenQueue& tq)
    {
        Token tok = tq.front();
        tq.pop();

        switch(tok.token)
        {
        case Token::NUMBER:
            {
                int i = tok.value;
                return i;
            }
        case Token::MINUS:
            return -prim(tq);
        case Token::LP:
            {
                int i = expr(tq);
                tok = tq.front();
                if(tok.token != Token::RP) throw SyntaxError("')' expected");
                tq.pop();
                return i;
            }
        case Token::SYMBOL:
            {
                if(tq.front().token == Token::EQ)
                {
                    tq.pop();
                    int i = expr(tq);
                    if(symTab.exists(tok.symbol) && symTab.Get(tok.symbol)->type != SymEntry::VAR)
                        throw SyntaxError("symbol is not a variable");

                    symTab.set(tok.symbol, new SymVariable(i));
                    return i;
                }
                else
                {
                    if(!symTab.exists(tok.symbol)) 
                        throw SyntaxError("symbol not found");

                    SymEntry* sym = symTab.Get(tok.symbol);

                    if(sym->type != SymEntry::VAR)
                        throw SyntaxError("symbol is not a variable");

                    return static_cast<SymVariable*>(sym)->value;
                }
            }
        default:
            throw SyntaxError("primary expected");
        }
    }

    int Expr::term(TokenQueue& tq)
    {
        int left = prim(tq);

        for(;;)
        {
            switch(tq.front().token)
            {
            case Token::MUL:
                tq.pop();
                left *= prim(tq);
                break;
            case Token::DIV:
                tq.pop();
                if(int i = prim(tq))
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

    int Expr::expr(TokenQueue& tq)
    {
        int left = term(tq);

        for (;;)
        {
            switch(tq.front().token)
            {
            case Token::PLUS:
                tq.pop();
                left += term(tq);
                break;
            case Token::MINUS:
                tq.pop();
                left -= term(tq);
                break;
            default:
                return left;
            }
        }
        return left;
    }
}


int main(int argc, char* argv[])
{
    using namespace std;
    using namespace Exercises;

    Expr e;
    while(cin)
    {
        string s;
        cin >> s;
        try
        {
            cout << e.eval(s) << endl;
        }   
        catch(Expr::SyntaxError err)
        {
            cerr << "Syntax error: " << err.message << endl;
        }
        catch(Expr::DivideByZeroError)
        {
            cerr << "Divide by zero error" << endl;
        }
    }
}
