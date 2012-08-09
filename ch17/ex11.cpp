#include <iostream>
#include <bitset>
#include <limits>

void print_bits(int n)
{
    std::bitset<sizeof(int)*8> b(n);
    std::cout << n << " = " << b << std::endl;
}

int main()
{
    using namespace std;

    print_bits(0);
    print_bits(1);
    print_bits(-1);
    print_bits(18);
    print_bits(-18);
    print_bits(numeric_limits<int>::max());
}
