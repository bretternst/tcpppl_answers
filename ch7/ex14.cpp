#include <iostream>

using namespace std;

// There should not be an = sign in there. It would be awkward to use
// this macro: double d PI;
#define PI = 3.141593;

// This one creates problems with operator precedence. It should be
// wrapped in parens. If used like so: cout << 3 + MAX(x,y);
// it will result in a compile time error.
#define MAX(a,b) a>b?a:b

// Firstly, it would be a good idea to wrap the whole thing in parens.
// However, the main problem is that macros cannot call themselves
// recursively. Using this macro e.g. fac(5) will result in a compiler
// error that the symbol fac is not found.
#define fac(a) (a)*fac((a)-1)

int main(int argc, char* argv[])
{
}
