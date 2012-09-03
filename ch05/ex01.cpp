#include <iostream>
#include <string>

using namespace std;

int main()
{
    const char* a = "a";
    int b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int (&c)[10] = b;

    char *(*d)[10];
    char *d1[10];
    d = &d1;

    char** e = 0;
    const int f = 1;
    const int* g;

    int h;
    int *const h1 = &h;
}
