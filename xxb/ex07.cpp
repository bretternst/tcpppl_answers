#include <iostream>

template<class T, class T2 = int>
class X {
    T x;
    T2 y;

public:
    template<class T3>
    void f(T3 z) {
    }
};

namespace test {
    class Y { };
    void f(Y y) {
    }
}

int main() {
    for(int i = 0; i < 10; i++) {
    }
    // Uncomment the following to test for-loop variable scoping.
    // On a compliant compiler, it will produce an error. With GCC,
    // you can suppress the error using -fpermissive
    // std::cout << i << '\n';

    // Test default template args.
    X<int> x;

    // Test member templates.
    x.f(0.0);

    // Test argument namespace lookup
    f(test::Y());
}
