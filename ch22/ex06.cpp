#include <valarray>

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

    template<class T>
    class Cslice_iter : public iterator<random_access_iterator_tag, T> {
        const valarray<T>* v;
        slice s;
        size_t curr;

        const T& ref(size_t i) const { return (*v)[s.start()+i*s.stride()]; }
    public:
        Cslice_iter(const valarray<T>* vv, slice ss) : v(vv), s(ss), curr(0) { }
        template<class U>
        Cslice_iter(const Slice_iter<U>& x) : v(x.v), s(x.s), curr(x.curr) { }

        Cslice_iter end() const {
            Cslice_iter t = *this;
            t.curr = s.size();
            return t;
        }

        Cslice_iter& operator++() { curr++; return *this; }
        Cslice_iter operator++(int) { Cslice_iter t = *this; curr++; return t; }

        const T& operator[](size_t i) { return ref(i); }
        const T& operator()(size_t i) { return ref(i); }
        const T& operator*() { return ref(curr); }

        size_t operator-(const Cslice_iter& q) {
            return curr - q.curr;
        }

        friend bool operator==(const Cslice_iter& p, const Cslice_iter& q) {
            return p.curr == q.curr && p.s.stride()==q.s.stride() && p.s.start()==q.s.start();
        }
        friend bool operator!=(const Cslice_iter& p, const Cslice_iter& q) {
            return !(p==q);
        }
        friend bool operator<(const Cslice_iter& p, const Cslice_iter& q) {
            return p.curr < q.curr && p.s.stride()==q.s.stride() && p.s.start()==q.s.start();
        }
    };


    class Matrix {
        valarray<double>* v;
        size_t d1, d2;
    public:
        Matrix(size_t x, size_t y);
        Matrix(const Matrix&);
        Matrix& operator=(const Matrix&);
        ~Matrix() { delete v; }

        size_t size() const { return d1 * d2; }
        size_t dim1() const { return d1; }
        size_t dim2() const { return d2; }

        Slice_iter<double> row(size_t i);
        Cslice_iter<double> row(size_t i) const;

        Slice_iter<double> column(size_t i);
        Cslice_iter<double> column(size_t i) const;

        double& operator()(size_t x, size_t y);
        double operator()(size_t x, size_t y) const;

        Slice_iter<double> operator()(size_t i) { return column(i); }
        Cslice_iter<double> operator()(size_t i) const { return column(i); }

        Slice_iter<double> operator[](size_t i) { return column(i); }
        Cslice_iter<double> operator[](size_t i) const { return column(i); }

        Matrix& operator*=(double);
        valarray<double>& array() { return *v; }
    };
}

int main() {
    using namespace std;
    using namespace ch22;
}
