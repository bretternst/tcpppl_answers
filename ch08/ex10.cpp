#include <iostream>
#include <limits>

using std::numeric_limits;
using std::cout;
using std::endl;
using std::cin;

struct OverflowError { };

double check(double x)
{
    if (x == numeric_limits<double>::infinity() ||
        x == -numeric_limits<double>::infinity())
    {
        throw OverflowError();
    }
    return x;
}

double plus(double a, double b)
{
    double x = a + b;
    return check(x);
}

double minus(double a, double b)
{
    double x = a - b;
    return check(x);
}

double mul(double a, double b)
{
    double x = a * b;
    return check(x);
}

double div(double a, double b)
{
    double x = a / b;
    return check(x);
}

int main(int argc, char* argv[])
{
    double a,b;
    cin >> a;
    cin >> b;

    try
    {
        double res = plus(a,b);
        cout << "plus: " << res << endl;
    }
    catch(OverflowError)
    {
        cout << "overflow" << endl;
    }

    try
    {
        double res = minus(a,b);
        cout << "minus: " << res << endl;
    }
    catch(OverflowError)
    {
        cout << "overflow" << endl;
    }

    try
    {
        double res = mul(a,b);
        cout << "mul: " << res << endl;
    }
    catch(OverflowError)
    {
        cout << "overflow" << endl;
    }

    try
    {
        double res = div(a,b);
        cout << "div: " << res << endl;
    }
    catch(OverflowError)
    {
        cout << "overflow" << endl;
    }
}

