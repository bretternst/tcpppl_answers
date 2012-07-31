#include <iostream>
#include <string>
#include <ctime>

using namespace std;

// Using an exception to return the value is far slower than returning it directly.

int throwOrReturn(int x, bool throwIt)
{
	if(throwIt) throw x;
	else return x;
}

int main(int argc, char* argv[])
{
	int x = 1, y;

	int start = clock();
	for(int i = 0; i < 1000000; i++)
	{
		y = throwOrReturn(x, false);
	}
	int end = clock();
	cout << "direct return, elapsed clock ticks: " << end-start << endl;

	start = clock();
	for(int i = 0; i < 1000000; i++)
	{
		try
		{
			throwOrReturn(x, true);
		}
		catch(int err)
		{
			y = err;
		}
	}
	end = clock();
	cout << "exceptions, elapsed clock ticks: " << end-start << endl;
}
