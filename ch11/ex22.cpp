#include <iostream>

namespace Exercises
{
    class Complex;

    class Imaginary
    {
        friend class Complex;

        double b;
    public:
        Imaginary(const double b) : b(b) {}

        Imaginary& operator+=(const Imaginary& x) { b += x.b; return *this; }
        Imaginary operator+(const Imaginary& x) const { return Imaginary(*this) += x; }

        Imaginary& operator-=(const Imaginary& x) { b -= x.b; return *this; }
        Imaginary operator-(const Imaginary& x) const { return Imaginary(*this) -= x; }

        Imaginary& operator*=(const Imaginary& x) { b *= x.b; return *this; }
        Imaginary operator*(const Imaginary& x) const { return Imaginary(*this) *= x; }

        Imaginary& operator/=(const Imaginary& x) { b /= x.b; return *this; }
        Imaginary operator/(const Imaginary& x) const { return Imaginary(*this) /= x; }

        friend std::ostream& operator<<(std::ostream& output, const Complex& c);
    };

    class Complex
    {
        double a;
        Imaginary i;
    public:
        Complex(const double a, const double b) : a(a), i(Imaginary(b)) {}

        Complex& operator+=(const Complex& x) { a += x.a; i += x.i; return *this; }
        Complex operator+(const Complex& x) const { return Complex(*this) += x; }

        Complex& operator-=(const Complex& x) { a -= x.a; i -= x.i; return *this; }
        Complex operator-(const Complex& x) const { return Complex(*this) -= x; }

        Complex& operator*=(const Complex& x);
        Complex operator*(const Complex& x) const { return Complex(*this) *= x; }

        Complex& operator/=(const Complex& x);
        Complex operator/(const Complex& x) const { return Complex(*this) /= x; }

        friend std::ostream& operator<<(std::ostream&, const Complex&);
    };

    Complex& Complex::operator*=(const Complex& x)
    {
        int a1 = a * x.a - i.b * x.i.b;
        i.b = i.b * x.a + a * x.i.b;
        a = a1;
        return *this;
    }

    Complex& Complex::operator/=(const Complex& x)
    {
        // This doesn't expclitly check for division by zero,
        // we'll rely on the platform's behavior to be consistent
        // with double.
        int a1 = (a * x.a + i.b * x.i.b) / (x.a*x.a + x.i.b*x.i.b);
        i.b = (i.b * x.a - a * x.i.b) / (x.a*x.a + x.i.b*x.i.b);
        a = a1;
        return *this;
    }

    std::ostream& operator<<(std::ostream& output, const Complex& c)
    {
        output << c.a << '+' << c.i.b << 'i';
        return output;
    }
}

int main()
{
    using namespace std;
    using namespace Exercises;

    Complex c1(2, 4);
    Complex c2(3, 1);

    cout << c1 << " + " << c2 << " = " << c1 + c2 << endl;
    cout << c1 << " - " << c2 << " = " << c1 - c2 << endl;
    cout << c1 << " * " << c2 << " = " << c1 * c2 << endl;
    cout << c1 << " / " << c2 << " = " << c1 / c2 << endl;

    return 0;
}
