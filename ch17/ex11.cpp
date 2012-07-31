#include <iostream>
#include <bitset>
#include <limits>

void printBits(int n)
{
	std::bitset<sizeof(int)*8> b(n);
	std::cout << n << " = " << b << std::endl;
}

int main()
{
	using namespace std;

	printBits(0);
	printBits(1);
	printBits(-1);
	printBits(18);
	printBits(-18);
	printBits(numeric_limits<int>::max());
}
