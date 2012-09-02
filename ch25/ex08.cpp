#include <iostream>

namespace ch25 {
    template<class X, class A>
    inline void Assert(A assertion) {
        if(!assertion)
            throw X();
    }

    template<class T>
    class Range {
        T low, high;
        T val;
    public:
        class Error { };
        Range(T low, T high, T i = T()) { Assert<Error>(low <= i && i < high); val = i; }
        Range operator=(T i) { return *this=Range(low, high, i); }
        operator T() { return val; }
    };
}

int main() {
    using namespace ch25;
    using namespace std;

    Range<int> r(0, 10, 5);
    try {
        r = 11;
    }
    catch(Range<int>::Error) {
        cout << "caught Range::Error" << endl;
    }
}
