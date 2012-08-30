#include <valarray>
#include <algorithm>
#include <iterator>
#include <iostream>

namespace ch22 {
    using std::valarray;

    template<class T, class Op>
    valarray<T> apply(const valarray<T>& src, Op f) {
        valarray<T> dst(src.size());
        std::transform(&src[0], &src[src.size()], &dst[0], f);
        return dst;
    }

    template<class T>
    struct Incr {
        T operator()(const T& x) { return x + 1; }
    };
}

int main() {
    using namespace std;
    using namespace ch22;

    int arr[] = { 0, 1, 2, 3, 4 };
    valarray<int> va(arr, 5);
    valarray<int> va2 = apply(va, Incr<int>());
    copy(&va2[0], &va2[va.size()], ostream_iterator<int>(cout, " "));
    cout << endl;
}
