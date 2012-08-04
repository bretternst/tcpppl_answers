#include <iostream>

using namespace std;

int f(int x, int y){return x+y;}

int main()
{
    // assumption: we're supposed to add parens to show the associativity of the different
    // operators, without changing the meaning of the statement. this shows that a number
    // of the expressions are nonsensical.
    int a, b, c, d, x, i;
    a = b = c = d = x = i = 1;

    a = b + c * d << 2 & 8;
    a = (((b + (c * d)) << 2) & 8);

    a & 077 != 3;
    (a & (077 != 3));

    a == b || a == c && c < 5;
    ((a == b) || ((a == c) && (c < 5)));

    c = x != 0;
    (c = (x != 0));

    0 <= i < 7;
    ((0 <= i) < 7);

    f(1,2)+3;
    ((f(1,2))+3);

    a = -1 + +b-- - 5;
    a = ((-1) + (+(b--)) - 5);

    a = b == c++;
    (a = (b == (c++)));

    a = b = c = 0;
    (a = (b = (c = 0)));

    int a1[4][2] = { { 1, 2 }, { 3, 4, }, { 5, 6, }, { 7, 8 } };
    int* b1 = &a, *d1 = &b;

    a1[3][1] *= *b1 ? c : *d1 * 2;
    (((a1[3])[1]) *= ((*b1) ? c : ((*d1) * 2)));
}
