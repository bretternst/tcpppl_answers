#include "error.h"

void Error::skip(istream* input)
{
	char ch;
	while(input->get(ch))
	{
		if(ch == '\n' || ch == ';')
			return;
	}
}
