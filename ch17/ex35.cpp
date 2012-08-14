#include <iostream>
#include "hash_map.h"

// This is ultra-simplistic, but matches the characteristics of the
// data specified in the question (uniformly distributed integers).
// Given integers that are already uniformly-distributed, the hash
// function doesn't really need to do much. In this case, we'll just
// uniformly distribute the integers to the buckets.
//
// Did I miss something in the question?
class SimpleIntHash {
public:
    int operator()(int x) {
        return x % 1024;
    }
};

int main() {
    using namespace ch17;
    using namespace std;

    hash_map<int,int,SimpleIntHash> hm(0, (size_t)1024);

    int j = 1;
    // Generates 1024 values that have the same hash value and check utilization
    // to confirm.
    for(int i = 0; i < 1024; i++) {
        hm[j] = i;
        j += 1024;
    }

    cout << hm.bucket_utilization() << endl;
}
