#include <iostream>
#include <algorithm>
#include <iterator>
#include <valarray>
#include <time.h>

// Just implemented the interface members required for the test.
// Compile with:
// g++ -o ex08 ex08.cpp -lrt

// The standard valarray is FAR faster, especially in the large case.
// This is likely because the implementation on my system builds expressions
// out of operations on valarray and executes them in aggregate, reducing the
// amount of copying.
namespace ch22 {
    template<class T>
    class valarray {
        T* p;
        size_t sz;
    public:
        valarray() : p(0), sz(0) { }
        valarray(size_t n) : sz(n), p(new T[n]) { }
        valarray(const valarray& x) : sz(x.sz), p(new T[x.sz]) { std::copy(x.p, x.p + x.sz, p); }
        valarray(const T* s, size_t n) : sz(n), p(new T[n]) { std::copy(s, s + n, p); }
        ~valarray() { delete[] p; }

        valarray& operator=(const valarray<T>& x);
        T& operator[] (size_t n) { return p[n]; }
        const T& operator[] (size_t n) const { return p[n]; }

        size_t size() const { return sz; }
    };

    template<class T>
    inline valarray<T>& valarray<T>::operator=(const valarray<T>& x) {
        delete[] p;
        sz = x.sz;
        p = new T[sz];
        std::copy(x.p, x.p + sz, p);
        return *this;
    }

    template<class T>
    inline valarray<T> operator+(const valarray<T>& x, const valarray<T>& y) {
        valarray<T> res(x.size());
        for(int i = 0; i < x.size(); i++)
            res[i] = x[i] + y[i];
        return res;
    }

    template<class T>
    inline valarray<T> operator+(const valarray<T>& x, const T& y) {
        valarray<T> res(x.size());
        for(int i = 0; i < x.size(); i++)
            res[i] = x[i] + y;
        return res;
    }

    template<class T>
    inline valarray<T> operator+(const T& x, const valarray<T>& y) {
        return y + x;
    }

    template<class T>
    inline valarray<T> operator*(const valarray<T>& x, const valarray<T>& y) {
        valarray<T> res(x.size());
        for(int i = 0; i < x.size(); i++)
            res[i] = x[i] * y[i];
        return res;
    }

    template<class T>
    inline valarray<T> operator*(const valarray<T>& x, const T& y) {
        valarray<T> res(x.size());
        for(int i = 0; i < x.size(); i++)
            res[i] = x[i] * y;
        return res;
    }

    template<class T>
    inline valarray<T> operator*(const T& x, const valarray<T>& y) {
        return y * x;
    }

    template<class T>
    std::ostream& operator<<(std::ostream& out, const valarray<T>& x) {
        out << "[ ";
        std::copy(&x[0], &x[0] + x.size(), std::ostream_iterator<double>(out, " "));
        out << "]";
    }
}

unsigned long diff(timespec start, timespec end) {
    timespec tmp;
    if((end.tv_nsec-start.tv_nsec)<0) {
        tmp.tv_sec = end.tv_sec-start.tv_sec-1;
        tmp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
        tmp.tv_sec = end.tv_sec-start.tv_sec;
        tmp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return tmp.tv_sec * 1000000000 + tmp.tv_nsec;
}

template<class T>
unsigned long test_small() {    
    double arr1[] = { 0, 1, 2, 3, 4 };
    double arr2[] = { 1, 2, 1, 2, 1 };
    double arr3[] = { 2, 3, 2, 3, 2 };
    timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    T x(arr1, 5);
    T y(arr2, 5);
    T z(arr3, 5);
    T q;
    for(int i = 0; i < 100000; i++)
        q = 0.5 * (x + y) + z;
    clock_gettime(CLOCK_REALTIME, &end);
    return diff(start, end);
}

template<class T>
unsigned long test_large() {    
    double arr[1000];
    for(int i = 0; i < 1000; i++)
        arr[i] = i;
    timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    T x(arr, 1000);
    T y(arr, 1000);
    T z(arr, 1000);
    T q;
    for(int i = 0; i < 100000; i++)
        q = 0.5 * (x + y) + z;
    clock_gettime(CLOCK_REALTIME, &end);
    return diff(start, end);
}

int main() {
    using namespace ch22;
    using namespace std;

    double arr1[] = { 0, 1, 2, 3, 4 };
    double arr2[] = { 1, 2, 1, 2, 1 };
    double arr3[] = { 2, 3, 2, 3, 2 };

    ch22::valarray<double> x(arr1, 5);
    ch22::valarray<double> y(arr2, 5);
    ch22::valarray<double> z(arr3, 5);

    x = 0.5 * (x + y) + z;
    cout << x << endl; // [ 2.5 4.5 3.5 5.5 4.5]

    cout << "Small valarray (custom): " << test_small<ch22::valarray<double> >() << endl;
    cout << "Small valarray (std): " << test_small<std::valarray<double> >() << endl;
    cout << "Large valarray (custom): " << test_large<ch22::valarray<double> >() << endl;
    cout << "Large valarray (std): " << test_large<std::valarray<double> >() << endl;
}
