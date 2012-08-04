#include <iostream> 
#include "ex05.h"

int main()
{
    using namespace Exercises;
    using namespace std;

    try
    {
        IntervalSet intervals;
        intervals.insert(1);
        intervals.insert(10);
        intervals.insert(20);
        intervals.insert(30);
        intervals.insert(40);
        intervals.insert(50);
        intervals.insert(60);
        intervals.insert(70);
        intervals.insert(80);
        intervals.insert(90);
        intervals.insert(100);

        Histogram hist(intervals);

        cout << "Histogram output with fixed numbers:" << endl;
        hist.add(1).add(10).add(11).add(19).add(20).add(11).add(89).add(89).add(89).add(90).add(95).add(100);
        hist.print_formatted(std::cout);

        cout << endl << "Histogram with random numbers:" << endl;
        hist = Histogram(intervals);
        srand(clock());
        for(int i = 0; i < 100; i++)
        {
            hist.add(rand() % 100 + 1);
        }
        hist.print_formatted(std::cout);
    }
    catch(Histogram::OutOfRangeError)
    {
        cerr << "The value was out of range." << endl;
    }
    catch(Histogram::InvalidIntervalSetError)
    {
        cerr << "Invalid interval." << endl;
    }
}
