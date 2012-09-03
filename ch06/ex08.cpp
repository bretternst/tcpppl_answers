#include <iostream>
#include <limits>

using namespace std;

int main()
{
    // compiled with g++
    unsigned int ui = numeric_limits<unsigned int>::max() + 1;
    cout << ui << endl; // 0
    ui = 0 - 1;
    cout << ui << endl; // 4 billion

    signed int i = numeric_limits<signed int>::max() + 1;
    cout << i << endl; // -2 billion
    i = 0 - 1;
    cout << i << endl; // -1

    cout << i / 0 << endl; // compiler warning; floating point exception at runtime
}
