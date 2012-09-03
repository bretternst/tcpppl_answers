#include <iostream>
#include <functional>

namespace ch25 {
    template<class F, class G>
    class Compose : public std::unary_function<typename G::argument_type, typename F::result_type> {
        F f;
        G g;
    public:
        Compose(const F& f, const G& g) : f(f), g(g) { }
        typename F::result_type operator()(const typename G::argument_type& x) {
            return f(g(x));
        }
    };

    template<class F, class G>
    Compose<F,G> compose(const F& f, const G& g) {
        return Compose<F,G>(f, g);
    }

    class Func_incr : public std::unary_function<int, int> {
    public:
        int operator()(int x) {
            return x + 1;
        }
    };

    class Func_mult : public std::unary_function<int, int> {
        int f;
    public:
        Func_mult(int factor) : f(factor) { }
        int operator()(int x) {
            return x * f;
        }
    };
}

int main() {
    using namespace std;
    using namespace ch25;

    cout << compose(Func_mult(2),Func_incr())(7) << endl;
}
