#include <iostream>
#include <cmath>

namespace ch11
{
    // For this implementation, I use a wrapper class that responds to multiplication
    // by a double. The wrapper class is returned by the unary * operator on Index.
    // I could have done this with only one class with Index's * operator returning a
    // reference to itself, but I wanted to guard against someone doing simply 2 * I,
    // which would perform the same operation.
    class Index;
    double mypow(const double x, const Index y);

    class Index
    {
        double v;
    public:
        class IndexWrapper;

        Index(const double v) : v(v) {}
        double val() const { return v; }
        IndexWrapper operator*() const;
    };

    class Index::IndexWrapper
    {
        friend class Index;

        Index i;
        IndexWrapper(Index i) : i(i) {}
        friend double operator*(const double, const IndexWrapper&);
    };

    Index::IndexWrapper Index::operator*() const
    {
        return IndexWrapper(*this);
    }

    double operator*(const double n, const Index::IndexWrapper& i)
    {
        return mypow(n, i.i);
    }

    double mypow(const double x, const Index y)
    {
        return pow(x, y.val());
    }
}

int main()
{
    using namespace std;
    using namespace ch11;

    Index I(8);
    cout << 2 ** I << endl; // expect: 256

    return 0;
}
