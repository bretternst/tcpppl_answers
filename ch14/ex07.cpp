#include <iostream>

#include "vector.h"

// Exceptions should be used to report circumstances where some library function (or
// other code) can't do what it is meant to do. In this case, reading past the end
// of a vector can't be done, so an exception is rightly thrown. However, the calling
// code can easily avoid asking the library to do something it can't. It is both
// clearer and faster to avoid a completely avoidable exception.
int main()
{
    using namespace std;
    using namespace ch14;

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
