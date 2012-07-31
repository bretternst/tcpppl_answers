#include <iostream>
#include <string>

struct MyException
{
	std::string msg;
	MyException(std::string s)
	{
		msg = s;
	}
};

void doThrow()
{
	throw MyException("My exception!");
}

int main()
{
	try
	{
		doThrow();
	}
	catch(MyException ex)
	{
		std::cout << "Caught exception: " << ex.msg << std::endl;
	}
}
