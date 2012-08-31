#include <valarray>
#include <numeric>
#include <iostream>

namespace ch22 {
    using namespace std;

    template<class T>
    T mul(const T& x, const T& y) {
        return x * y;
    }

    template<class T>
    class Multidim_array;

    template<class T>
    class subscript {
        Multidim_array<T>* v;
        valarray<size_t> s;
    public:
        explicit subscript(Multidim_array<T>* v, size_t n) : v(v), s(n, 1) { }
        explicit subscript(Multidim_array<T>* v, valarray<size_t> s) : v(v), s(s) { }
        subscript operator[](size_t i) {
            valarray<size_t> tmp(s.size() + 1);
            copy(&s[0], &s[0] + s.size(), &tmp[0]);
            tmp[s.size()] = i;
            return subscript(v, tmp);
        }
        operator T() {
            return (*v)[*this];
        }
        subscript& operator=(const T& x) {
            (*v)[*this] = x;
            return *this;
        }
        size_t size() const { return s.size(); }
        size_t idx(size_t i) const { return s[i]; }
    };

    template<class T>
    class Csubscript {
        const Multidim_array<T>* v;
        valarray<size_t> s;
    public:
        explicit Csubscript(const Multidim_array<T>* v, size_t n) : v(v), s(n, 1) { }
        explicit Csubscript(const Multidim_array<T>* v, valarray<size_t> s) : v(v), s(s) { }
        Csubscript operator[](size_t i) {
            valarray<size_t> tmp(s.size() + 1);
            copy(&s[0], &s[0] + s.size(), &tmp[0]);
            tmp[s.size()] = i;
            return Csubscript(v, tmp);
        }
        operator T() {
            return (*v)[*this];
        }
        size_t size() const { return s.size(); }
        size_t idx(size_t i) const { return s[i]; }
    };

    template<class T>
    class Multidim_array {
        valarray<T>* v;
        size_t* d;
        size_t n;
    public:
        Multidim_array(size_t* dims, size_t ndims);
        ~Multidim_array() { delete[] d; delete v; }        
        // copy ctor, operator=, etc.

        subscript<T> operator[](size_t i) {
            return subscript<T>(this, i);
        }

        Csubscript<T> operator[](size_t i) const {
            return Csubscript<T>(this, i);
        }

        T& operator[](const subscript<T>& s) {
            size_t idx = s.idx(0);
            for(int i = 1; i < n; i++)
                idx += s.idx(i) * accumulate(d, d + i, 1, mul<size_t>);
            return (*v)[idx];
        }

        T operator[](const Csubscript<T>& s) const {
            size_t idx = s.idx(0);
            for(int i = 1; i < n; i++)
                idx += s.idx(i) * accumulate(d, d + i, 1, mul<size_t>);
            return (*v)[idx];
        }
    };

    template<class T>
    Multidim_array<T>::Multidim_array(size_t* dims, size_t ndims) {
        n = ndims;
        d = new size_t[n];
        copy(dims, dims + n, d);
        v = new valarray<T>(accumulate(d, d + n, 1, mul<size_t>));
    }
}

int main() {
    using namespace std;
    using namespace ch22;

    size_t dims[] = { 3, 3 };
    Multidim_array<int> ma(dims, 2);
    int count = 0;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            ma[i][j] = count++;
        }
    }

    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            cout.width(6);
            cout << ma[i][j];
        }
        cout << endl;
    }
}
