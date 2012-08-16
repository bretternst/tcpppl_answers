#include <iostream>

namespace ch18 {
    template<class In1, class In2>
    std::pair<In1,In2> match(In1 first1, In1 last1, In2 first2) {
        while(first1 != last1 && *first1 != *first2) {
            first1++; first2++;
        }
        return std::make_pair(first1, first2);
    }

    template<class In1, class In2, class BinPred>
    std::pair<In1,In2> match(In1 first1, In1 last1, In2 first2, BinPred pred) {
        while(first1 != last1 && !pred(*first1, *first2)) {
            first1++; first2++;
        }
        return std::make_pair(first1, first2);
    }
}

int main() {
    using namespace std;

    int v1[] = { 1, 2, 3, 4, 5 };
    int v2[] = { 'a', 'b', 3, 4, 5 };

    int* p = ch18::match(v1, v1 + 5, v2).first;
    cout << *p << endl;
    p = ch18::match(v1, v1 + 5, v2, std::equal_to<int>()).first;
    cout << *p << endl;
}