#include <iostream>
#include <algorithm>
#include <vector>

namespace ch18 {
    using namespace std;

    // Iseq from the previous exercise
    template<class In>
    struct Iseq : public pair<In,In> {
        Iseq(In i1, In i2) : pair<In,In>(i1, i2) { }
    };

    template<class C>
    Iseq<typename C::iterator> iseq(C& c) {
        return Iseq<typename C::iterator>(c.begin(), c.end());
    }

    template<class T>
    Iseq<T*> iseq(T* p, size_t n) {
        return Iseq<T*>(p, p + n);
    }

    template<class In>
    Iseq<In> iseq(In in1, In in2) {
        return Iseq<In>(in1, in2);
    }

    // Oseq from this exercise
    template<class Out>
    struct Oseq {
        Out seq;
        Oseq(Out o) : seq(o) { }
    };

    template<class Out>
    Oseq<Out> oseq(Out out) {
        return Oseq<Out>(out);
    }

    template<class T>
    Oseq<T*> oseq(T* arr) {
        return Oseq<T*>(arr);
    }

    // std function overloads
    template<class In, class Out>
    Out copy(Iseq<In> in, Oseq<Out> out) {
        std::copy(in.first, in.second, out.seq);
    }

    template<class In, class Out, class Op>
    Out transform(Iseq<In> in, Oseq<Out> out, Op op) {
        std::transform(in.first, in.second, out.seq, op);
    }

    template<class In, class Out>
    Out unique_copy(Iseq<In> in, Oseq<Out> out) {
        std::unique_copy(in.first, in.second, out.seq);
    }

    int add_one(int x) { return x + 1; }
}

int main() {
    using namespace std;
    using namespace ch18;

    int arr[] = { 1, 2, 3, 4, 4 };
    vector<int> v;

    copy(iseq(arr, 5), oseq(back_inserter(v)));
    for(vector<int>::iterator i = v.begin(); i != v.end(); i++)
        cout << *i << " ";
    cout << endl;
    v.clear();

    transform(iseq(arr, 5), oseq(back_inserter(v)), add_one);
    for(vector<int>::iterator i = v.begin(); i != v.end(); i++)
        cout << *i << " ";
    cout << endl;
    v.clear();

    unique_copy(iseq(arr, 5), oseq(back_inserter(v)));
    for(vector<int>::iterator i = v.begin(); i != v.end(); i++)
        cout << *i << " ";
    cout << endl;
    v.clear();
}