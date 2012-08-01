#include <iostream>

using namespace std;

struct X
{
    int i;
    X(int i) : i(i) { }
    X operator+(int i) { return X(this->i + i); }
};

struct Y
{
    int i;
    Y(X x) : i(x.i) { };
    Y operator+(X x) { return Y(i+x.i); }
    operator int() { return i; };
};

X operator*(X x,Y y) { return X(x.i*y.i); }
int f(X x) { return x.i; }

X x = 1; 
Y y = x;
int i = 2;

int main()
{
    using namespace std;

    cout << (i + 10) << endl; // Built in int+int
//  cout << (y + 10) << endl; // Invalid: ambiguity, could mean (int)y + 10 or y + X(10)
//  cout << y + 10 * y << endl; // Invalid: ambiguity, could mean y + (10 * (int)y) or y + (X(10) * y)
    cout << (x + y + i).i << endl; // Y's int(), then built in int+int, then X operator+(int)
    cout << (x * x + i).i << endl; // Y(X), then X operator*(X,Y), then X operator+(int)
    cout << (f(7)) << endl; // X(int)
//  cout << (f(y)) << endl; // Invalid: no implicit conversion from type Y to X
    cout << (y + y) << endl; // Y's int(), Y's int(), built in int+int
    cout << (106 + y) << endl; // Y's int(), built in int+int
}
