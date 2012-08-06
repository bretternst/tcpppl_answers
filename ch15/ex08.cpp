#include <iostream>

// my first attempt was this:
class A
{
};

extern const unsigned long volatile int operator +(A a, int y);

// But I couldn't get to 10 keywords. Here's an actual solution stolen from the
// web:

// Copied from http://www.vandevoorde.com/C++Solutions/challenge.shtml
int f()
{
    if(true)
        return 1;
    else return throw sizeof new unsigned long int const volatile(), 0;
}

int main()
{
}
