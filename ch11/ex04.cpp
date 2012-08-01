#include <iostream>

using std::ostream;

class RINT
{
    int m_i;
public:
    explicit RINT(int i) : m_i(i) {}

    RINT operator-() { return RINT(-m_i); }
    RINT operator+() { return RINT(+m_i); }
    RINT operator-(const RINT& x) { return RINT(m_i-x.m_i); }
    RINT operator+(const RINT& x) { return RINT(m_i+x.m_i); }
    RINT operator/(const RINT& x) { return RINT(m_i/x.m_i); }
    RINT operator*(const RINT& x) { return RINT(m_i*x.m_i); }
    RINT operator%(const RINT& x) { return RINT(m_i%x.m_i); }
    RINT operator-(const int x) { return RINT(m_i-x); }
    RINT operator+(const int x) { return RINT(m_i+x); }
    RINT operator/(const int x) { return RINT(m_i/x); }
    RINT operator*(const int x) { return RINT(m_i*x); }
    RINT operator%(const int x) { return RINT(m_i%x); }

    int val() const { return m_i; }
};

RINT operator-(const int x, const RINT& y) { return RINT(x - y.val()); }
RINT operator+(const int x, const RINT& y) { return RINT(x + y.val()); }
RINT operator/(const int x, const RINT& y) { return RINT(x / y.val()); }
RINT operator*(const int x, const RINT& y) { return RINT(x * y.val()); }
RINT operator%(const int x, const RINT& y) { return RINT(x % y.val()); }

ostream& operator<<(ostream& out, const RINT& x) {
    return out << x.val();
}

int main()
{
    using namespace std;

    RINT x(2);
    RINT y(3);
    int z = 4;
    cout << (-x) << endl;
    cout << (+y) << endl;
    cout << (y-x) << endl;
    cout << (y+x) << endl;
    cout << (y/x) << endl;
    cout << (y*x) << endl;
    cout << (y%x) << endl;

    cout << (z-x) << endl;
    cout << (x-z).val() << endl;
    cout << (z+x) << endl;
    cout << (x+z) << endl;
    cout << (z/x) << endl;
    cout << (x/z) << endl;
    cout << (z*x) << endl;
    cout << (x*z) << endl;
    cout << (z%x) << endl;
    cout << (x%z) << endl;
}
