#include <valarray>
#include <iostream>
#include <cstdarg>
#include <numeric>

// Using variadic arguments is probably a poor solution; in a C++11 program, variadic templates
// may be a better way to do this. Alternatively, with some helper classes, the m[1][2]...[n]
// notation could be achieved.
namespace ch22 {
    using namespace std;

    inline double mul(double x, double y) {
        return x * y;
    }

    template<int N>
    class Matrix {
        valarray<double>* v;
        size_t dims[N];
    public:
        explicit Matrix(size_t* sizes);
        Matrix(const Matrix& x) : v(new valarray<double>(*x.v)) {
            copy(x.dims, x.dims + N, dims);            
        }
        Matrix& operator=(const Matrix& x) {
            delete v;
            v = new valarray<double>(*x.v);
            copy(x.dims, x.dims + N, dims);
        }
        ~Matrix() { delete v; }

        size_t size() const { v->size(); }
        size_t dim(size_t n) { return dims[n]; }

        double& operator()(size_t d1, ...);
        double operator()(size_t d1, ...) const;

        Matrix& operator*=(double d) { *v *= d; return *this; }
        valarray<double>& array() { return *v; }
    };

    template<int N>
    Matrix<N>::Matrix(size_t* sizes) {
        copy(sizes, sizes + N, dims);
        size_t sz = accumulate(dims, dims + N, 1, mul);
        v = new valarray<double>(sz);
    }

    template<int N>
    double& Matrix<N>::operator()(size_t d1, ...) {
        va_list ap;
        va_start(ap, d1);

        size_t idx = d1;
        for(int i = 1; i < N; i++) {
            int d = va_arg(ap, int);
            idx += d * accumulate(dims, dims + i, 1, mul);
        }

        va_end(ap);
        return (*v)[idx];
    };

    template<int N>
    double Matrix<N>::operator()(size_t d1, ...) const {
        va_list ap;
        va_start(ap, d1);

        size_t idx = d1;
        for(int i = 1; i < N; i++) {
            int d = va_arg(ap, int);
            idx += d * accumulate(dims, dims + i, 1, mul);
        }

        va_end(ap);
        return (*v)[idx];
    };
}

int main() {
    using namespace std;
    using namespace ch22;

    size_t dims[] = {3, 3, 2};
    Matrix<3> m(dims);
    int count = 0;
    for(int z = 0; z < 2; z++) {
        for(int y = 0; y < 3; y++) {
            for(int x = 0; x < 3; x++) {
                m(x,y,z) = count++;
            }
        }
    }

    m *= 2;

    for(int z = 0; z < 2; z++) {
        cout << "Plane " << z << endl;
        for(int y = 0; y < 3; y++) {
            for(int x = 0; x < 3; x++) {
                cout.width(6);
                cout << m(x,y,z);
            }
            cout << endl;
        }
        cout << endl;
    }
}
