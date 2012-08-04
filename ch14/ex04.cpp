#include <iostream>
#include <limits>

namespace Exercises
{
    int IntMax = std::numeric_limits<int>::max();
    int IntMin = std::numeric_limits<int>::min();

    class Int
    {
        int i;

    public:
        class OverflowError {};
        class UnderflowError {};

        Int(int ii) : i(ii) {}
        operator int() {return i;}
        Int operator++(int) { Int copy = *this; ++i; return copy; }
        Int& operator++();
        Int operator--(int) { Int copy = *this; --i; return copy; }
        Int& operator--();
        Int& operator+=(const Int& x);
        Int operator+(const Int& x) { Int tmp = *this; tmp += x; return tmp; }
        Int operator+(int x) { return *this + Int(x); }
        Int& operator-=(const Int& x);
        Int operator-(const Int& x) { Int tmp = *this; tmp -= x; return tmp; }
        Int operator-(int x) { return *this - Int(x); }
        Int& operator*=(const Int& x);
        Int operator*(const Int& x) { Int tmp = *this; tmp *= x; return tmp; }
        Int operator*(int x) { return *this * Int(x); }
        Int& operator/=(const Int& x);
        Int operator/(const Int& x) { Int tmp = *this; tmp /= x; return tmp; }
        Int operator/(int x) { return *this / Int(x); }

        friend std::istream& operator>>(std::istream&, Int&);
        friend std::ostream& operator<<(std::ostream&, Int&);
    };

    Int& Int::operator--()
    {
        if(i == IntMin) throw UnderflowError();
        i--;
        return *this;
    }

    Int& Int::operator++()
    {
        if(i == IntMax) throw OverflowError();
        i++;
        return *this;
    }

    Int& Int::operator+=(const Int& x)
    {
        if(i > 0 && x.i > 0)
        {
            if(IntMax - i < x.i) throw OverflowError();
        }
        else if (i < 0 && x.i < 0)
        {
            if(IntMin - i > x.i) throw UnderflowError();
        }

        i += x.i;
        return *this;
    }

    Int& Int::operator-=(const Int& x)
    {
        if(i > 0 && x.i < 0)
        {
            if(IntMax - i < -x.i) throw OverflowError();
        }
        else if (i < 0 && x.i > 0)
        {
            if(IntMin - i > -x.i) throw UnderflowError();
        }
        i -= x.i;
        return *this;
    }

    Int& Int::operator*=(const Int& x)
    {
        if(x.i == 0 || i == 0)
        {
            i = 0;
        }
        else
        {
            int tmp = i * x.i;
            int check = tmp / x.i;
            if (check != i)
            {
                if((i < 0 && x.i > 0) || (i > 0 && x.i < 0))
                    throw UnderflowError();
                else
                    throw OverflowError();
            }
            i = tmp;
        }
        return *this;
    }

    Int& Int::operator/=(const Int& x)
    {
        i /= x.i;
        return *this;
    }

    std::istream& operator>>(std::istream& input, Int& x)
    {
        input >> x.i;
        return input;
    }

    std::ostream& operator<<(std::ostream& output, Int& x)
    {
        output << x.i;
        return output;
    }
}

void main()
{
    using namespace std;
    using namespace Exercises;

    Int x = 2;
    Int y = 3;
    cout << x + 1 << endl;
    cout << 1 + x << endl;
    cout << x + y << endl;
    cout << x * y << endl;
    cout << x / y << endl;
    cout << x % y << endl;
    cout << x << endl;
    x = 4 + 4;
    cout << x << endl;
    int z = 8;
    x = z;
    cout << x << endl;
    x++;
    z = x;
    cout << z << endl;
    cout << x++ << endl;
    cout << ++x << endl;
    cout << x-- << endl;
    cout << --x << endl;
    x += 1;
    cout << x << endl;
    x -= 1;
    cout << x << endl;
    cout << -x << endl;

    try
    {
        Int x = numeric_limits<int>::max() / 2 + 1;
        cout << x + numeric_limits<int>::max() / 2 + 1 << endl;
    }
    catch(Int::OverflowError)
    {
        cout << "overflow (1)" << endl;
    }

    try
    {
        Int x = numeric_limits<int>::min() / 2 - 1;
        cout << x + numeric_limits<int>::min() / 2 - 1 << endl;
    }
    catch(Int::UnderflowError)
    {
        cout << "underflow (2)" << endl;
    }

    try
    {
        Int x = numeric_limits<int>::max() / 5;
        cout << x * 6 << endl;
    }
    catch(Int::OverflowError)
    {
        cout << "overflow (3)" << endl;
    }
}
