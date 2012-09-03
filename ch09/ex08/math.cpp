#include <limits>
#include "math.h"
#include "error.h"

double Math::check(double x)
{
    if (x == std::numeric_limits<double>::infinity() ||
        x == -std::numeric_limits<double>::infinity())
    {
        throw Error::OverflowError();
    }
    return x;
}

double Math::plus(double a, double b)
{
    double x = a + b;
    return check(x);
}

double Math::minus(double a, double b)
{
    double x = a - b;
    return check(x);
}

double Math::mul(double a, double b)
{
    double x = a * b;
    return check(x);
}

double Math::div(double a, double b)
{
    double x = a / b;
    return check(x);
}
