#include <iostream>
#include <cctype>
#include <stdlib.h>

using namespace std;

void rev(char* s)
{
    char *p=s;
    while(*p)p++;
    while(--p > s)
    {
        *p ^= *s;
        *s ^= *p;
        *p ^= *s++;
    }
}

char* itoa_(int i, char result[], int base)
{
    if (!base) base = 10;

    int idx = 0;
    do
    {
        int digit = i % base;
        result[idx++] = digit > 10 ? 'A' + digit - 10 : '0' + digit;
        i /= base;
    } while (i > 0);
    result[idx] = 0;
    rev(result);
    return result;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
        return -1;

    char b[255];

    int i = atoi(argv[1]);
    cout << "0x" << itoa_(i, b, 16) << endl;
    cout << itoa_(i, b, 10) << endl;
    cout << "0" << itoa_(i, b, 8) << endl;
    cout << itoa_(i, b, 2) << endl;
}
