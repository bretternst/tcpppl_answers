#include <iostream>
#include <algorithm>
#include <iterator>

namespace ch18 {
    using std::less;
    using std::ostream_iterator;
    using std::iterator_traits;
    using std::cout;
    using std::endl;
    using std::swap;

    template<class Ran, class Cmp>
    void sort(Ran first, Ran last, Cmp cmp) {
        const size_t n = last - first;
        for(int gap = n/2; 0 < gap; gap /= 2) {
            for(int i = gap; i < n; i++) {
                for(int j = i-gap; 0 <= j; j -= gap) {
                    if (cmp(*(first + (j+gap)), *(first + j))) {
                        swap(*(first + j), *(first + (j+gap)));
                        copy(first, last, ostream_iterator<typename iterator_traits<Ran>::value_type>(cout, " "));
                        cout << endl;
                    }
                    else
                        break;
                }
            }
        }
    }

    template<class Ran>
    void sort(Ran first, Ran last) {
        ch18::sort(first, last, less<typename iterator_traits<Ran>::value_type>());
    }
}

int main() {
    using namespace ch18;

    int arr[] = { 4, 6, 3, 2, 5, 1 };
    ch18::sort(arr, arr + 6);
}
