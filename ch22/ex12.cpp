#include <iostream>
#include <map>
#include <cmath>
#include <stdint.h>

namespace ch22 {
    class Randint {
        uint32_t randx;
    public:
        Randint(int32_t s = 0) : randx(s) { }
        void seed(int32_t s) { randx = s; }

        static int32_t abs(int32_t x) { return x & 0x7fffffff; }
        static double max() { return 2147483648.0; }
        int32_t draw() { return randx = randx * 1103515245 + 12345; }
        double fdraw() { return abs(draw())/max(); }
        int32_t operator()() { return abs(draw()); }
    };

    class Urand : public Randint {
        int32_t n;
    public:
        Urand(int32_t nn) : n(nn) { }
        int32_t operator()() { int32_t r = n*fdraw(); return (r == n) ? n-1 : r; }
    };

    class Erand : public Randint {
        int32_t mean;
    public:
        Erand(int32_t m) : mean(m) { }
        int32_t operator()() { return -mean * log((max()-draw())/max() + .5); }
    };
}

template<class T>
void crude_test(T& rand, int lower, int upper) {
    std::map<int,int> bucket;
    for(int i = 0; i < 1000000; i++) bucket[rand()]++;
    for(int j = lower; j < upper; j++) {
        std::cout.width(4);
        std::cout << j << ' ' << std::string(bucket[j] / 10000, '#') << '(' << bucket[j] << ")\n";
    }
}

int main() {
    using namespace std;
    using namespace ch22;

    Urand u(10);
    Erand e(10);
    cout << "Urand:\n";
    crude_test(u, 0, 10);
    cout << "\nErand:\n";
    crude_test(e, 0, 20);
}
