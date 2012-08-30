#include <iostream>
#include <valarray>
#include <iostream>
#include <iterator>

// The exercise says to "take special care when defining the destructor" but I don't
// understand how this applies. Slice_iter holds a pointer to a valarray, and I don't
// know of any iterator that would destroy the container it iterates. Is there something
// else the destructor should do? Am I missing a vital part of the question?
//
// I checked a couple of other implementations of Slice_iter via Google and none of them
// had a destructor.
namespace ch22 {
    using namespace std;

    template<class T>
    class Slice_iter : public iterator<random_access_iterator_tag, T> {
        valarray<T>* v;
        slice s;
        size_t curr;

        T& ref(size_t i) const { return (*v)[s.start()+i*s.stride()]; }
    public:
        Slice_iter(valarray<T>* vv, slice ss) : v(vv), s(ss), curr(0) { }

        Slice_iter end() const {
            Slice_iter t = *this;
            t.curr = s.size();
            return t;
        }

        Slice_iter& operator++() { curr++; return *this; }
        Slice_iter operator++(int) { Slice_iter t = *this; curr++; return t; }

        T& operator[](size_t i) { return ref(i); }
        T& operator()(size_t i) { return ref(i); }
        T& operator*() { return ref(curr); }

        size_t operator-(const Slice_iter& q) {
            return curr - q.curr;
        }

        friend bool operator==(const Slice_iter& p, const Slice_iter& q) {
            return p.curr == q.curr && p.s.stride()==q.s.stride() && p.s.start()==q.s.start();
        }
        friend bool operator!=(const Slice_iter& p, const Slice_iter& q) {
            return !(p==q);
        }
        friend bool operator<(const Slice_iter& p, const Slice_iter& q) {
            return p.curr < q.curr && p.s.stride()==q.s.stride() && p.s.start()==q.s.start();
        }
    };
}

int main() {
    using namespace std;
    using namespace ch22;

    int arr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    valarray<int> va(arr, 10);
    Slice_iter<int> si(&va, slice(1, 5, 2));
    copy(si, si.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
}
