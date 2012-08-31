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

    class Nrand : public Randint {
        int32_t mean;
        double stddev;
    public:
        Nrand(int32_t mean, double stddev) : mean(mean), stddev(stddev) { }

        // Box-Muller algorithm found here:
        // http://stackoverflow.com/questions/75677/converting-a-uniform-distribution-to-a-normal-distribution
        int32_t operator()() {
            double u = 2*fdraw()-1;
            double v = 2*fdraw()-1;
            double r = u*u + v*v;
            if(r == 0 || r > 1)
                return (*this)();
            double c = sqrt(-2*log(r)/r);
            return u * c * stddev + mean;
        }
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

    Nrand nr(5, 1.5);
    crude_test(nr, 0, 10);
}
