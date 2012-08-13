#include <iostream>
#include "hash_set.h"

int main() {
    using namespace ch17;
    using namespace std;

    hash_set<int> hs;
    for(int i = 0; i < 5; i++)
        hs.insert(i);

    hs.insert(4);
    hs.insert(10);

    for(hash_set<int>::iterator i = hs.begin(); i != hs.end(); i++) {
        cout << *i << endl;
    }
}