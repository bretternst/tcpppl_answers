#include "error.h"

// Prints a syntax error message
void Error::skip(istream* input)
{
	while(char ch = static_cast<char>(input->get()))
	{
		if(ch == '\n' || ch == ';')
			return;
	}
}
