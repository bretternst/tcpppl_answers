#include <iostream>

using namespace std;

// Defines a reference to a function. It behaves the same way as
// any other reference. Once initialized, it cannot be changed or
// pointed to another function.
typedef int(&rifi) (int,int);

int func01(int a, int b)
{
    return a + b;
}

int main(int argc, char* argv[])
{
    rifi r = func01;
    cout << r(1,2) << endl;
}
