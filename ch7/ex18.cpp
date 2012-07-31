#include <iostream>
#include <stdlib.h>

using namespace std;

int factorial_(int num)
{
	int res = num;
	for(int i = 2; i < num; i++)
	{
		res *= i;
	}
	return res;
}

int main(int argc, char* argv[])
{
	if(argc<2) return -1;
	int num = atoi(argv[1]);
	cout << "factorial for " << num << " is " << factorial_(num) << endl;
}
