#include <vector>
#include <iostream>
#include <functional>
#include <algorithm>
#include <vector>
#include <cmath>

namespace ch18 {
    double square(double x) {
        return x * x;
    }

    double square_root(double x) {
        return sqrt(x);
    }

    class Print_nth : public std::unary_function<double,int> {
        int i;
    public:
        Print_nth() : i(1) { }
        int operator()(double x) {
            std::cout << i++ << ':' << x << ' ';
            return x;
        }
    };
}

int main() {
    using namespace ch18;
    using namespace std;

    vector<double> v(100);
    for(int i = 1; i <= 100; i++)
        v[i-1] = i;

    transform(v.begin(), v.end(), v.begin(), square);
    for_each(v.begin(), v.end(), Print_nth());
    cout << endl;
    transform(v.begin(), v.end(), v.begin(), square_root);
    for_each(v.begin(), v.end(), Print_nth());
    cout << endl;
}