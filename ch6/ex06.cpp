#include <iostream>

// 5
// #include <winnt.h>

using namespace std;

int main()
{
    // 1
    int a[4] = {1, 2, 3, 4};
    char* p = (char *)a;
    cout << (int)*(p+4) << endl; // two?

    // 2
    struct mystruct_t
    {
        char x;
        int y;
    } mystruct = {'a',2};
    p = (char*)&mystruct;
    cout << (int)*(p+4) << endl; // two?

    // 3
    char charr[4] = {2,0,0,0};
    int* pi = reinterpret_cast<int*>(charr);
    cout << *pi; // two?

    // 4
    signed char ch = 130;
    cout << ch;

    // 6
    // void _tmain(int argc, TCHAR* argv[]) { }

    // 7
    // Pre-compiled headers; e.g. #include "stdafx.h"

    // 8
    asm("nop");

    // 9
    // BOOL x = FALSE;

    // 10
    // void __cdecl func();
}
