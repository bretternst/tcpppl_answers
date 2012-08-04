#include <iostream>
#include <vector>

namespace Exercises {
    template<class Coll1, class Coll2>
    void merge(Coll1& c1, Coll2& c2) {
        Coll1::iterator ipos = c1.begin();
        Coll2::iterator first = c2.begin();
        Coll2::iterator last = c2.begin();

        while(first != c2.end())
        {
            while(ipos != c1.end() && *ipos < *first) {
                ++ipos;
            }
            do {
                ipos = c1.insert(ipos, *first++);
//              first = c2.erase(first);
            } while (first != c2.end() && *first < *ipos);
        }
        c2.clear();
    }
}

int main() {
    using namespace std;
    using namespace Exercises;

    vector<int> c1;
    c1.push_back(1);
    c1.push_back(3);
    c1.push_back(5);
    c1.push_back(5);
    c1.push_back(6);
    c1.push_back(7);
    c1.push_back(9);
    c1.push_back(9);

    vector<int> c2;
    c2.push_back(2);
    c2.push_back(4);
    c2.push_back(6);
    c2.push_back(7);
    c2.push_back(8);
    c2.push_back(10);

    merge(c1,c2);

    // expect: 1 2 3 4 5 5 6 6 7 7 8 9 9 10
    for(vector<int>::const_iterator i = c1.begin(); i != c1.end(); i++) {
        cout << *i << endl;
    }
    if(c2.empty()) {
        cout << "c2 is empty as expected" << endl;
    }
}
