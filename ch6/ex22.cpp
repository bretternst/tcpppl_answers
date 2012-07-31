#include <iostream>
#include <string>
#include <fstream>

using namespace std;

void readComment(istream *input)
{
	char ch;
	while (input->good())
	{
		input->get(ch);
		if(input->peek()=='/')
		{
			input->get();
			return;
		}
	}
}

void readString(istream *input, ostream *output, char endChar)
{
	char ch;
	while (input->good())
	{
		input->get(ch);
		switch(ch)
		{
		case '\\':
			output->put(ch);
			if(input->peek()==endChar)
			{
				output->put(input->get());
			}
			break;
		default:
			output->put(ch);
			if(ch==endChar)
			{
				return;
			}
			break;
		}
	}
}

void readLine(istream *input)
{
	char ch;
	while (input->good())
	{
		input->get(ch);
		if (ch=='\n') return;
	}
}

void stripComments(istream *input, ostream *output)
{
	char ch;
	while (input->good())
	{
		input->get(ch);
		switch(ch)
		{
		case '/':
			{
				char p = input->peek();
				if (p == '/')
				{
					readLine(input);
				}
				else if (p == '*')
				{
					input->get();
					readComment(input);
				}
				else
				{
					output->put(ch);
				}
				break;
			}
		case '"':
		case '\'':
			output->put(ch);
			readString(input, output, ch);
			break;
		default:
			output->put(ch);
			break;
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
		input = new ifstream(argv[1], ifstream::in);
		if (!input->good())
		{
			cerr << "could not open file" << endl;
			return 1;
		}
		break;
	default:
		cerr << "too many arguments" << endl;
		return 1;
	}

	stripComments(input, &cout);

	if (input != &cin) delete input;
}
