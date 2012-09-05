#include <iostream>
#include <map>

namespace xxe {
    // just taking a wild guess here.
    template<class T>
    void safe_insert(T& container, const typename T::value_type& x) {
        try {
            container.insert(x);
        }
        catch(...) {
            container.erase(x.first);
        }
    }
}

int main() {
    using namespace std;

    int arr[] = { 0, 1, 2, 3, 4 };
    map<int,int> m;    

    for(int i = 0; i < 5; i++)
        xxe::safe_insert(m, std::make_pair(i, i));
    for(map<int,int>::iterator i = m.begin(); i != m.end(); i++) {
        cout << i->first << ' ';
    }
    cout << endl;
}