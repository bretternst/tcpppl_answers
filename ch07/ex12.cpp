#include <iostream>

// Some people prefix the type with func_ or F.
typedef int (*func_compare)(int a, int b);
typedef int (*Fcompare)(int a, int b);

// To make it clear that it's a type, some people
// postfix _t.
typedef int (*Fcomparer_t)(int a, int b);

using namespace std;

int main(int argc, char* argv[])
{
}
