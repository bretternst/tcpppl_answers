#include <iostream>

using namespace std;

void f(int a, int b)
{
    // this assigns 3 to a and always evaluates true
    if ( a = 3 ) cout << "test 1: true" << endl;

    // always evaluates false. 077 == 0 is false (0). No matter what a is, a & 0 is 0.
    if (a&077==0) cout << "test 2: true" << endl;
    
    // a := b + 1; // compiler error
}

int main()
{
    f(1, 2);
}
