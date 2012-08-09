#include <iostream>
#include <vector>
#include <list>
#include <deque>

int main() {
    using namespace std;

    vector<int> v;
    deque<int> d;
    list<int> l;

    // Constant overhead: required no matter how many elements are present.
    // Note that there will be additional overhead for the backing store
    // if it is not used to capacity (empty storage not yet used).

    // My implementation is GCC on an x64 system
    cout << "Constant overhead:" << endl;
    cout << "vector: " << sizeof(v) << endl; // 24 bytes for me
    cout << "deque: " << sizeof(d) << endl; // 80 bytes for me
    cout << "list: " << sizeof(l) << endl; // 16 bytes for me

    // List has an additional overhead for each node. For me, this is 16 bytes
    // or two 64-bit pointers for the previous and next data node. Therefore,
    // each element will consume 16 bytes of additional overhead beyond the user
    // data.
}
