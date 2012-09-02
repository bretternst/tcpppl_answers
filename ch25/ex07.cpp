#include <iostream>

namespace ch25 {
    template<class X, class A>
    inline void Assert(A assertion) {
        if(!assertion)
            throw X();
    }

    template<class T, T low, T high>
    class Range {
        T val;
    public:
        class Error { };
        Range(T i) { Assert<Error>(low <= i && i < high); val = i; }
        Range operator=(T i) { return *this=Range(i); }
        operator T() { return val; }
    };
}

int main() {
    using namespace ch25;
    using namespace std;

    Range<int, 0, 10> r(5);
    try {
        r = 11;
    }
    catch(Range<int, 0, 10>::Error) {
        cout << "caught Range::Error" << endl;
    }
}
