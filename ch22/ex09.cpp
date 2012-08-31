#include <iostream>
#include <algorithm>
#include <iterator>

// Array indexes start at 1.
// Added one of the slice types here for illustration.
namespace ch22 {
    class slice {
        size_t m_start;
        size_t m_size;
        size_t m_stride;
    public:
        slice() : m_start(0), m_size(0), m_stride(0) { }
        slice(size_t start, size_t size, size_t stride) : m_start(start), m_size(size), m_stride(stride) { }
        slice(const slice& x) : m_start(x.start()), m_size(x.size()), m_stride(x.stride()) { }
        size_t start() const { return m_start; }
        size_t size() const { return m_size; }
        size_t stride() const { return m_stride; }
    };

    template<class T>
    class Fort_array;

    template<class T>
    class slice_array {
        friend class Fort_array<T>;

        Fort_array<T>* v;
        slice s;
    public:
        typedef T value_type;
        void operator=(const T&);
    private:
        slice_array();
        slice_array(Fort_array<T>* v, slice s) : v(v), s(s) { };
        slice_array(const slice_array&);
        slice_array& operator=(const slice_array&);
    };

    template<class T>
    void slice_array<T>::operator=(const T& x) {
        for(int i = 0; i < s.size(); i++)
            (*v)[s.start() + i * s.stride()] = x;
    }

    template<class T>
    class Fort_array {
        T* p;
        size_t sz;
    public:
        Fort_array() : p(0), sz(0) { }
        Fort_array(size_t n) : sz(n), p(new T[n]) { }
        Fort_array(const Fort_array& x) : sz(x.sz), p(new T[x.sz]) { std::copy(x.p, x.p + x.sz, p); }
        Fort_array(const T* s, size_t n) : sz(n), p(new T[n]) { std::copy(s, s + n, p); }
        ~Fort_array() { delete[] p; }

        Fort_array& operator=(const Fort_array<T>& x);
        T& operator[] (size_t n) { return p[n - 1]; }
        const T& operator[] (size_t n) const { return p[n - 1]; }
        slice_array<T> operator[](slice s) {
            return slice_array<T>(this, s);
        }
        Fort_array<T> operator[](slice s) const {
            Fort_array<T> tmp(s.size());
            for(int i = 0; i < s.size(); i++)
                tmp[i+1] = *this[s.start() + i * s.stride()];
            return tmp;
        }

        size_t size() const { return sz; }
    };

    template<class T>
    inline Fort_array<T>& Fort_array<T>::operator=(const Fort_array<T>& x) {
        delete[] p;
        sz = x.sz;
        p = new T[sz];
        std::copy(x.p, x.p + sz, p);
        return *this;
    }

    template<class T>
    inline Fort_array<T> operator+(const Fort_array<T>& x, const Fort_array<T>& y) {
        Fort_array<T> res(x.size());
        for(int i = 1; i <= x.size(); i++)
            res[i] = x[i] + y[i];
        return res;
    }

    template<class T>
    inline Fort_array<T> operator+(const Fort_array<T>& x, const T& y) {
        Fort_array<T> res(x.size());
        for(int i = 1; i <= x.size(); i++)
            res[i] = x[i] + y;
        return res;
    }

    template<class T>
    inline Fort_array<T> operator+(const T& x, const Fort_array<T>& y) {
        return y + x;
    }

    template<class T>
    inline Fort_array<T> operator*(const Fort_array<T>& x, const Fort_array<T>& y) {
        Fort_array<T> res(x.size());
        for(int i = 1; i <= x.size(); i++)
            res[i] = x[i] * y[i];
        return res;
    }

    template<class T>
    inline Fort_array<T> operator*(const Fort_array<T>& x, const T& y) {
        Fort_array<T> res(x.size());
        for(int i = 1; i <= x.size(); i++)
            res[i] = x[i] * y;
        return res;
    }

    template<class T>
    inline Fort_array<T> operator*(const T& x, const Fort_array<T>& y) {
        return y * x;
    }

    template<class T>
    std::ostream& operator<<(std::ostream& out, const Fort_array<T>& x) {
        out << "[ ";
        std::copy(&x[1], &x[1] + x.size(), std::ostream_iterator<double>(out, " "));
        out << "]";
    }
}

int main() {
    using namespace ch22;
    using namespace std;

    double arr1[] = { 0, 1, 2, 3, 4 };

    ch22::Fort_array<double> x(arr1, 5);
    x[1] = 10;
    x[5] = 15;
    x[ch22::slice(2,3,1)] = 5;

    cout << x << endl; // 10 5 5 5 15
}
