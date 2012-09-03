#include <iostream>

using namespace std;

void rev(char* s)
{
    char *p=s;
    while(*p)p++;
    while(--p > s)
    {
        // the old clever xor trick again, clearer to use a temp variable.
        *p ^= *s;
        *s ^= *p;
        *p ^= *s++;
    }
}

int main(int argc, char* argv[])
{
    if(argc < 2) return -1;
    rev(argv[1]);
    cout << argv[1] << endl;
}
