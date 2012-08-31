#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>

// Uniform distribution between 0 and n.
int main() {
    using namespace std;

    srand(57);
    int n = 10;
    vector<int> v(10);
    for(int i = 0; i < 1000000; i++) {
        double d = (double(rand())/RAND_MAX) * n;
        if(d < 10.0)
            v[d]++;
    }
    for(int i = 0; i < 10; i++) {
        cout.width(6);
        cout << i << ' ' << string(v[i] / 10000,'#') << '\n';
    }
}