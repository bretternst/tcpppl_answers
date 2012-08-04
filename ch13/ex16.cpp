#include <iostream>
#include <map>
#include <string>

namespace Exercises
{
    // A quick and dirty check, see below for a better one.
    // This may or may not result in runtime overhead (a smart
    // compiler will optimize out the if (false) {...} but this
    // is not guaranteed, especially if optimization is off.
    // If there are other constructors, the logic would need to
    // be copied or moved into another method.
    template<class T>
    class Comparer1
    {
    public:
        Comparer1()
        {
            // Enforce constraints: a < operator is required.
            if(false)
            {
                T* x;
                *x < *x ? 1 : 1;
            }
        }
    };

    // This is the method that Bjarne recommends on his web page here:
    // http://www.research.att.com/~bs/bs_faq2.html#constraints
    // With either of these methods, anything that can be expressed in
    // C++ can be checked without actually executing it, including
    // checking of a type is derived from some other type.
    template<class T>
    struct HasLessThan
    {
        static void constraints(T x) { x < x ? 1 : 1; }
        HasLessThan() { void(*p)(T) = constraints; }
    };

    template<class T>
    class Comparer2 : private HasLessThan<T>
    {
    };

    struct Dummy
    {
    };
}

int main()
{
    using namespace std;
    using namespace Exercises;

    Comparer1<int> c1;

    // Won't compile:
//  Comparer1<Dummy> c2;

    Comparer2<int> c3;

    // Won't compile:
//  Comparer2<Dummy> c4;

    return 0;
}
