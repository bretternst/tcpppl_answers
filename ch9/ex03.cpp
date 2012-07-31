#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// NOTE: The "INCLUDE" environment variable must contain the list of system
// include paths separated by colons. I'm not aware of an easy way to get the
// include paths at runtime.
namespace Lexer
{
	using std::istream;
	using std::string;

	enum TokenType
	{
		TOK_NONE = 0, TOK_NEWLINE, TOK_WHITESPACE, TOK_WORD, TOK_OTHER, TOK_END,
		TOK_POUND = '#', TOK_LT = '<', TOK_GT = '>', TOK_QUOT = '"'
	};

	struct Token
	{
		TokenType token;
		string value;

		Token(TokenType);
		Token(TokenType,string);
		Token(TokenType,char);
	};

	Token get_token(istream* input, bool ignoreWhitespace);
}

namespace Parser
{
	using std::string;
	using std::istream;
	using Lexer::Token;
	using Lexer::get_token;

	void init();
	void read_file(string fileName);
}

namespace Lexer
{
	Token::Token(TokenType t) { token = t; }
	Token::Token(TokenType t, string v) { token = t; value = v; }
	Token::Token(TokenType t, char c) { token = t; value.push_back(c); }

	Token get_token(istream* input, bool ignoreWhitespace)
	{
		char ch = 0;
		if(ignoreWhitespace)
		{
			for(;;)
			{
				if(!input->get(ch)) return Token(TOK_END);
				if(!(isspace(ch) && ch != '\n')) break;
			}
		}
		else if (!input->get(ch)) return Token(TOK_END);

		switch(ch)
		{
		case '\n': 
			return Token(TOK_NEWLINE, ch);
		case TOK_POUND: case TOK_LT: case TOK_GT: case TOK_QUOT:
			return Token(TokenType(ch), ch);
		default:
			if (isalnum(ch))
			{
				string s;
				s.push_back(ch);
				while(input->get(ch) && isalnum(ch)) s.push_back(ch);
				input->putback(ch);
				int x = 0;
				return Token(TOK_WORD, s);
			}
			return Token(TOK_OTHER, ch);
		}
	}
}

namespace Parser
{
	typedef std::vector<string> PathList;
	PathList includePaths;

	std::set<string> alreadyIncluded;
	int indent = 0;
	
	void init()
	{
		char* paths = getenv("INCLUDE");
		if(paths && *paths != 0)
		{
			char* part = strtok(paths, ":");
			while(part)
			{
				string s = part;
				if(*(s.end()-1) != '/') s.push_back('/');
				includePaths.push_back(s);
				part = strtok(0, ":");
			}
		}
	}

	string find_sys_include(string includeName)
	{
		for(PathList::const_iterator i = includePaths.begin(); i != includePaths.end(); i++)
		{
			string path = *i + includeName;
			std::ifstream test(path.c_str(), std::ifstream::in);
			bool found = test.is_open();
			test.close();
			if(found) return path;
		}
        return string();
	}

	void do_include_file(string includeName, bool sysFile)
	{
        string path = includeName;
		if(sysFile)
		{
			 path = find_sys_include(includeName);
		}
		if(path.size() > 0 && alreadyIncluded.count(path) == 0)
		{
			std::cout << string(indent, ' ') << includeName << " = " << (path.size() == 0 ? "???" : path) << std::endl;
            indent++;
            alreadyIncluded.insert(path);
            read_file(path);
            indent--;
		}
	}

	string read_simple_string(istream* input, char terminator)
	{
		char ch;
		string s;

		while(input->get(ch))
		{
			if(ch == terminator)
				break;
			else
				s.push_back(ch);
		}
		return s;
	}

	void read_include_filename(istream* input)
	{
		Token tok = get_token(input, true);
		char ch;
		if(tok.token == Lexer::TOK_LT || tok.token == Lexer::TOK_QUOT)
		{
			bool sysPath = tok.token == Lexer::TOK_LT;
			string fileName = read_simple_string(input, sysPath ? Lexer::TOK_GT : Lexer::TOK_QUOT);
			do_include_file(fileName, sysPath);
		}
	}

	void read_directive(istream* input)
	{
		Token tok = get_token(input, true);
		if(tok.token == Lexer::TOK_WORD && tok.value == "include")
		{
			read_include_filename(input);
		}

		// Eat the rest of the line.
		for(;;)
		{
			tok = get_token(input, false);
			if(tok.token == Lexer::TOK_END || tok.token == Lexer::TOK_NEWLINE)
				break;
		}
	}

	void read_normal_line(istream* input, Token tok)
	{
		for(;;)
		{
			switch(tok.token)
			{
			case Lexer::TOK_NEWLINE:
			case Lexer::TOK_END:
				return;
			default:
				break;
			}

			tok = get_token(input, false);
		}
	}

	void read_line(istream* input, Token tok)
	{
		if(tok.token == Lexer::TOK_NONE) tok = get_token(input, true);
		switch(tok.token)
		{
		case Lexer::TOK_END:
		case Lexer::TOK_NEWLINE:
			return;
		case Lexer::TOK_POUND:
			read_directive(input);
			break;
		default:
			read_normal_line(input, tok);
		}
	}

	void read_file(string fileName)
	{
		using std::ifstream;

        ifstream file(fileName.c_str(), ifstream::in);
		if(!file.good())
		{
			std::cerr << "ERROR: could not open file: " << fileName << std::endl;
		}

		while(file.good())
		{
			read_line(&file, Token(Lexer::TOK_NONE));
		}
	}
}

int main(int argc, char* argv[])
{
	using namespace std;

	istream* input;

	if (argc != 2)
	{
		cerr << "filename required" << endl;
		return -1;
	}

	Parser::init();
	Parser::read_file(argv[1]);
}
