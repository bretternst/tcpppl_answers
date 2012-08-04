#include <iostream>
#include <ctime>
#include <cmath>
#include <algorithm>

// To try out this solution, we re-use the histogram from the previous exercise.
#include "ex05.h"

namespace ch10
{
    const double PI = 3.14159265358979323846;

    double generate()
    {
        static bool seeded = false;
        if(!seeded)
        {
            srand(time(0));
            seeded = true;
        }
        return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
    }

    //
    // Uniform random numbers
    //
    class UniformRandomGenerator
    {
        int lowerBound;
        int upperBound;

    public:
        UniformRandomGenerator(int lowerBound, int upperBound) : lowerBound(lowerBound), upperBound(upperBound) {}
        int draw();
    };

    int UniformRandomGenerator::draw()
    {
        return generate() * (upperBound-lowerBound) + lowerBound;
    }

    //
    // Exponentially distributed random numbers
    //
    class ExponentialRandomGenerator
    {
        int offset;
        int lambda;

    public:
        ExponentialRandomGenerator(int offset, int lambda) : offset(offset), lambda(lambda) {}
        int draw();
    };

    int ExponentialRandomGenerator::draw()
    {
        // found this somewhere on the web
        double r = -log(generate());
        return r*lambda + offset;
    }

    //
    // Normally distributed random numbers
    //
    class NormalRandomGenerator
    {
        int mean;
        int sigma;

    public:
        NormalRandomGenerator(int mean, int sigma) : mean(mean), sigma(sigma) {}
        int draw();
    };

    int NormalRandomGenerator::draw()
    {
        // again, found somewhere on the web
        double u1, u2, z;
        u1 = generate();
        u2 = generate();
        z = sqrt(-2.0 * log(u1)) * cos(2.0 * PI * u2);
        return mean + sigma*z;
    }
}

int main()
{
    using namespace ch10;
    using namespace std;

    IntervalSet intervals;
    intervals.insert(1);
    intervals.insert(100);
    intervals.insert(200);
    intervals.insert(300);
    intervals.insert(400);
    intervals.insert(500);
    intervals.insert(600);
    intervals.insert(700);
    intervals.insert(800);
    intervals.insert(900);
    intervals.insert(1000);

    Histogram hist(intervals);

    cout << "Uniform distribution:" << endl;
    UniformRandomGenerator gen1(1, 1000);
    for(int i = 0; i < 100; i++)
    {
        hist.add(gen1.draw());
    }
    hist.print_formatted(cout);

    cout << "Exponential distribution:" << endl;
    ExponentialRandomGenerator gen2(1,200);
    hist = Histogram(intervals);
    for(int i = 0; i < 100; i++)
    {
        int result = gen2.draw();
        if(result >= 1 && result < 1000) hist.add(result);
    }
    hist.print_formatted(cout);

    cout << "Normal distribution:" << endl;
    NormalRandomGenerator gen3(500, 200);
    hist = Histogram(intervals);
    for(int i = 0; i < 100; i++)
    {
        int result = gen3.draw();
        if(result >= 1 && result < 1000) hist.add(result);
    }
    hist.print_formatted(cout);
}
