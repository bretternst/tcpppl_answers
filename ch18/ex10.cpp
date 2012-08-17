#include <functional>
#include <algorithm>
#include <vector>
#include <iostream>

// A bind3rd function is useful in the kind of scenario I've
// illustrated here; to simulate a trinary operation. Any
// time a third argument would be used to control the
// combination of another two arguments, a bind3rd function
// would be useful.
namespace ch18 {
    template<class Arg1, class Arg2, class Arg3, class R>
    struct trinary_function {
        typedef Arg1 first_argument_type;
        typedef Arg2 second_argument_type;
        typedef Arg3 third_argument_type;
        typedef R result_type;
    };

    template<class T>
    struct Op_trinary : public trinary_function<bool, T, T, T> {
        T operator()(bool arg1, const T& arg2, const T& arg3) {
            return arg1 ? arg2 : arg3;
        }
    };

    template<class Op>
    class binder3rd : public std::unary_function<typename Op::first_argument_type, typename Op::result_type> {
        typedef typename Op::first_argument_type Arg1;
        typedef typename Op::second_argument_type Arg2;
        typedef typename Op::third_argument_type Arg3;
        Op op;
        const Arg2& arg2;
        const Arg3& arg3;
    public:
        binder3rd(Op op, const Arg2& arg2, const Arg3& arg3) : op(op), arg2(arg2), arg3(arg3) { }
        typename Op::result_type operator()(const Arg1& x) {
            return op(x, arg2, arg3);
        }
    };

    template<class Op>
    binder3rd<Op> bind3rd(Op op,
        const typename Op::second_argument_type& arg2, const typename Op::third_argument_type& arg3) {
        return binder3rd<Op>(op, arg2, arg3);
    }

    void print_int(int x) {
        std::cout << x << std::endl;
    }
}

int main() {
    using namespace std;
    using namespace ch18;

    vector<bool> v1;
    v1.push_back(false);
    v1.push_back(true);
    v1.push_back(false);
    vector<int> vres;
    transform(v1.begin(), v1.end(), back_inserter(vres),
              bind3rd(Op_trinary<int>(), 1, 0));
    for_each(vres.begin(), vres.end(), print_int);
}