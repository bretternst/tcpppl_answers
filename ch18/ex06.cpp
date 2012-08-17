#include <iostream>
#include <functional>
#include <algorithm>
#include <iterator>
#include <sstream>

namespace ch18 {
    using namespace std;

    template<class In>
    struct Iseq : public pair<In,In> {
        Iseq(In i1, In i2) : pair<In,In>(i1, i2) { }
    };

    template<class C>
    Iseq<typename C::iterator> iseq(C& c) {
        return Iseq<typename C::iterator>(c.begin(), c.end());
    }

    // for arrays
    template<class T>
    Iseq<T*> iseq(T* p, size_t n) {
        return Iseq<T*>(p, p + n);
    }

    // for istream
    template<class T>
    Iseq<istream_iterator<T> > iseq(istream& in) {
        return Iseq<istream_iterator<T> >(istream_iterator<T>(in), istream_iterator<T>());
    }

    // for iterator pairs
    template<class In>
    Iseq<In> iseq(In in1, In in2) {
        return Iseq<In>(in1, in2);
    }

    template<class In, class Pred>
    Pred for_each(Iseq<In> r, Pred pred) {
        return std::for_each(r.first, r.second, pred);
    }

    template<class In, class T>
    In find(Iseq<In> r, const T& v) {
        return std::find(r.first, r.second, v);
    }

    template<class In, class Pred>
    In find_if(Iseq<In> r, Pred pred) {
        return std::find_if(r.first, r.second, pred);
    }

    template<class In, class T>
    ptrdiff_t count(Iseq<In> r, const T& v) {
        return std::count(r.first, r.second, v);
    }

    template<class In, class Pred>
    ptrdiff_t count_if(Iseq<In> r, Pred pred) {
        return std::count_if(r.first, r.second, pred);
    }

    template<class In, class In2>
    bool equal(Iseq<In> r, In2 in2) {
        return std::equal(r.first, r.second, in2);
    }

    template<class In, class In2, class BinPred>
    bool equal(Iseq<In> r, In2 in2, BinPred pred) {
        return std::equal(r.first, r.second, in2, pred);
    }

    template<class In, class In2>
    pair<In,In2> mismatch(Iseq<In> r, In2 in2) {
        return std::mismatch(r.first, r.second, in2);
    }

    template<class In, class In2, class BinPred>
    pair<In,In2> mismatch(Iseq<In> r, In2 in2, BinPred pred) {
        return std::mismatch(r.first, r.second, in2, pred);
    }

    template<class For, class For2>
    For search(Iseq<For> r, Iseq<For2> r2) {
        return std::search(r.first, r.second, r2.first, r2.second);
    }

    template<class For, class For2, class BinPred>
    For search(Iseq<For> r, Iseq<For2> r2, BinPred pred) {
        return std::search(r.first, r.second, r2.first, r2.second, pred);
    }

    template<class For, class For2>
    For find_end(Iseq<For> r, Iseq<For2> r2) {
        return std::find_end(r.first, r.second, r2.first, r2.second);
    }

    template<class For, class For2, class BinPred>
    For find_end(Iseq<For> r, Iseq<For2> r2, BinPred pred) {
        return std::find_end(r.first, r.second, r2.first, r2.second, pred);
    }

    template<class For, class For2>
    For search_n(Iseq<For> r, Iseq<For2> r2) {
        return std::search_n(r.first, r.second, r2.first, r2.second);
    }

    template<class For, class For2, class BinPred>
    For search_n(Iseq<For> r, Iseq<For2> r2, BinPred pred) {
        return std::search_n(r.first, r.second, r2.first, r2.second, pred);
    }    

    class print_int : unary_function<int, int> {
    public:
        int operator()(int x) {
            cout << x << endl;
            return x;
        }
    };
}

int main() {
    using namespace ch18;
    using namespace std;

    int arr[] = { 1, 2, 3, 4, 4 };
    int* p = find(iseq(arr, 5), 3);
    for_each(iseq(arr, 5), print_int());
    cout << *p << endl;
    cout << count(iseq(arr, 5), 4) << endl;
    cout << equal(iseq(arr, 5), arr) << endl;
    int arr2[] = { 1, 2, 3, 4, 5 };
    cout << *mismatch(iseq(arr, 5), arr2).second << endl;
    int arr3[] = { 3, 4 };
    cout << *search(iseq(arr, 5), iseq(arr3, 2)) << endl;

    cout << "---" << endl;

    p = find(iseq(arr, arr + 5), 3);
    cout << *p << endl;

    string str = "abcdefgh";
    istringstream is(str);
    istream_iterator<char> c = find(iseq<char>(is), 'd');
    cout << *c << endl;
}
