#include <valarray>
#include <iostream>

namespace ch22 {
    using namespace std;

    class Matrix {
        valarray<double>* v;
        size_t d1, d2, d3;
    public:
        Matrix(size_t x, size_t y, size_t z) : d1(x), d2(y), d3(z), v(new valarray<double>(x*y*z)) { }
        Matrix(const Matrix& x) : d1(x.d1), d2(x.d2), d3(x.d3), v(new valarray<double>(*x.v)) { }
        Matrix& operator=(const Matrix& x);
        ~Matrix() { delete v; }

        size_t size() const { return d1 * d2 * d3; }
        size_t dim1() const { return d1; }
        size_t dim2() const { return d2; }
        size_t dim3() const { return d3; }

        double& operator()(size_t x, size_t y, size_t z) { return (*v)[x + y * d1 + z * d1 * d2]; }
        double operator()(size_t x, size_t y, size_t z) const { return (*v)[x + y * d1 + z * d1 * d2]; }

        Matrix& operator*=(double d) { *v *= d; return *this; }
        valarray<double>& array() { return *v; }
    };

    Matrix& Matrix::operator=(const Matrix& x) {
        delete v;
        v = new valarray<double>(*x.v);
        d1 = x.d1;
        d2 = x.d2;
        d3 = x.d3;
    }
}

int main() {
    using namespace std;
    using namespace ch22;

    Matrix m(3,3,2);
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
