#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>
#include <list>

namespace ch18 {
    using namespace std;

    template<class I>
    I __move(I first, I last, I result, random_access_iterator_tag) {
        I res = result;
        if(result > first && result < last) {
            result += (last - first) - 1;
            last--;
            while(last >= first) {
                *result-- = *last--;
            }
        }
        else {
            while(first < last) {
                *result++ = *first++;
            }
        }
        return res;
    }

    template<class I>
    I __move(I first, I last, I result, bidirectional_iterator_tag) {
        I res = result;
        I i = first;
        int count = 0;
        bool reverse = false;
        while(i != last) {
            count++;
            if(i == result)
                reverse = true;
            i++;
        }
        if(reverse) {
            for(int j = 0; j < count - 1; j++)
                result++;
            last--;
            for(;;) {
                *result-- = *last;
                if(last == first)
                    break;
                last--;
            }
        }
        else {
            while(first != last) {
                *result++ = *first++;
            }
        }
        return res;
    }

    template<class I>
    I move(I first, I last, I result) {
        __move(first, last, result, typename iterator_traits<I>::iterator_category());
    }
}

int main() {
    using namespace std;
    using namespace ch18;

    vector<int> v;
    for(int i = 0; i < 10; i++)
        v.push_back(i);

    move(v.begin(), v.begin() + 5, v.begin() + 2);
    ostream_iterator<int> out(cout, " ");
    copy(v.begin(), v.end(), out);
    cout << endl;

    list<int> l;
    for(int i = 0; i < 10; i++)
        l.push_back(i);

    list<int>::iterator end = l.begin();
    for(int i = 0; i < 5; i++)
        end++;
    list<int>::iterator res = l.begin();
    res++;
    res++;
    move(l.begin(), end, res);
    copy(l.begin(), l.end(), out);
    cout << endl;

    int arr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    move(arr, arr + 5, arr + 2);
    copy(arr, arr + 10, out);
    cout << endl;
}
