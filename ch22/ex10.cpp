#include <vector>
#include <iostream>

namespace ch22 {
    using namespace std;

    // Much harder to slice rows with this implementation...
    class Matrix {
        vector<vector<double> > v;
        size_t d1, d2;
    public:
        Matrix(size_t x, size_t y) : d1(x), d2(y), v(x) { for(int i = 0; i < d1; i++) v[i].resize(d2); }
        Matrix(const Matrix& x) : d1(x.d1), d2(x.d2), v(x.d1) { for(int i = 0; i < d1; i++) v[i].resize(d2); }
        Matrix& operator=(const Matrix& x);
        ~Matrix() { }

        size_t size() const { return d1 * d2; }
        size_t dim1() const { return d1; }
        size_t dim2() const { return d2; }

        vector<double>& column(size_t i) { return v[i]; }
        vector<double> column(size_t i) const { return vector<double>(v[i]); }

        double& operator()(size_t x, size_t y) { return column(x)[y]; }
        double operator()(size_t x, size_t y) const { return column(x)[y]; }
        vector<double>& operator()(size_t i) { return column(i); }
        vector<double> operator()(size_t i) const { return column(i); }
        vector<double>& operator[](size_t i) { return column(i); }
        vector<double> operator[](size_t i) const { return column(i); }

        Matrix& operator*=(double d);
    };

    Matrix& Matrix::operator=(const Matrix& x) {
        d1 = x.d1;
        d2 = x.d2;
        v.resize(d1);
        for(int i = 0; i < d1; i++)
            v[i].assign(x.v[i].begin(), x.v[i].end());
    }

    Matrix& Matrix::operator*=(double d) {
        for(int i = 0; i < d1; i++) {
            for(int j = 0; j < d2; j++) {
                v[i][j] *= d;
            }
        }
    }
}

int main() {
    using namespace std;
    using namespace ch22;

    Matrix m(3,3);
    int count = 0;
    for(int y = 0; y < 3; y++) {
        for(int x = 0; x < 3; x++) {
        cerr << x << ',' << y << endl;
            m(x,y) = count++;
        }
    }

    m *= 2;

    for(int y = 0; y < 3; y++) {
        for(int x = 0; x < 3; x++) {
            cout.width(6);
            cout << m(x,y);
        }
        cout << endl;
    }
}
