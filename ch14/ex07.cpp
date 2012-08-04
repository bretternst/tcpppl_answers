#include <iostream>

#include "Vector.h"

// This is an abuse of exceptions. Exceptions are meant for exceptional circumstances,
// and reaching the end of a vector is most definitely not exceptional (every vector has
// an end). This is actually a logic error in the calling code.
//
// It is also unclear to other developers what is going on.
//
// Finally, it adds overhead. A new exception object must be created and copied, and the
// process of stack unwinding is most likely less performant in most implementations.
int main()
{
    using namespace std;
    using namespace Exercises;

    Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    int i = 0;
    try
    {
        for(;;)
        {
            cout << v.at(i++) << endl;
        }
    }
    catch(Vector<int>::RangeError)
    {
    }

    return 0;
}
