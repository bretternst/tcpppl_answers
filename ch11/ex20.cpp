#include <iostream>

// It's not clear to me exactly what the exercise was after.
// This seems to be a reasonable implementation, as long as
// an instance of C doesn't outlive the instances of S and T.
namespace Exercises
{
    struct S { int x, y; };
    struct T { const char* p; const char* q; };

    class C
    {
    public:
        int& x;
        const char*& p;

        C(S s, T t) : x(s.x), p(t.p)
        {
        }
    };
}

int main()
{
    using namespace std;
    using namespace Exercises;

    S s = { 1, 2 };
    T t = { "hello", "" };
    C c(s, t);

    cout << c.x << ' ' << c.p << endl;

    return 0;
}
