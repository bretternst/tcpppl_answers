#include <iostream>

using namespace std;

int i = 1, j = 1;
int f1() { return ::i++; }
int f2() { return ::i = 0; }
void f3(int a, int b) {}

int main()
{
    ++::i = ::i++; cout << ::i << endl;
    f3(f1(),f2()); cout << ::i << endl;

    int a[4] = {1, 2, 3, 4};
    int x = 0,y = 0;
    a[x] = x++; cout << a[x] << endl;

    int a1[4], *p = a1;
    *p++ = *p++ = 0; cout << a1[0] << endl;

    x = ++x + 1; cout << x << endl;
}
