#include <list>
#include <algorithm>
#include <vector>
#include <iostream>

namespace ch18 {
    void print_int(const int& x) {
        std::cout << x << std::endl;
    }
}

// The question may be hinting at something else (some use of inplace_merge?)
int main() {
    using namespace std;
    using namespace ch18;

    list<int> lst;
    lst.push_back(4);
    lst.push_back(2);
    lst.push_back(3);
    lst.push_back(1);
    lst.push_back(5);

    vector<int> v;
    copy(lst.begin(), lst.end(), back_inserter(v));
    sort(v.begin(), v.end());
    copy(v.begin(), v.end(), lst.begin());
    for_each(lst.begin(), lst.end(), print_int);
}