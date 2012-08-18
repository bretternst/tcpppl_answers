#include <iterator>
#include <list>
#include <iostream>
#include <list>
#include <vector>

namespace ch18 {
    using namespace std;

    template<class In1, class In2, class Pred>
    In1 __search(In1 first1, In1 last1, In2 first2, In2 last2, Pred pred, random_access_iterator_tag, random_access_iterator_tag) {
        while(first1 != last1) {
            // early out; remainder is not long enough to contain search range
            // are there other optimizations that can be made?
            if((last2 - first2) > (last1 - first1))
                return last1;
            In1 i = first1;
            In2 j = first2;
            while(i != last1 && j != last2 && pred(*i, *j)) {
                i++;
                j++;
            }
            if(j == last2)
                break;
            first1++;
        }
        return first1;
    }

    template<class In1, class In2, class Pred>
    In1 __search(In1 first1, In1 last1, In2 first2, In2 last2, Pred pred, forward_iterator_tag, forward_iterator_tag) {
        while(first1 != last1) {
            In1 i = first1;
            In2 j = first2;
            while(i != last1 && j != last2 && pred(*i, *j)) {
                i++;
                j++;
            }
            if(j == last2)
                break;
            first1++;
        }

        return first1;
    }

    template<class In1, class In2, class Pred>
    In1 search(In1 first1, In1 last1, In2 first2, In2 last2, Pred pred) {
        return __search(first1, last1, first2, last2, pred, typename iterator_traits<In1>::iterator_category(),
                        typename iterator_traits<In2>::iterator_category());
    }

    template<class In1, class In2>
    In1 search(In1 first1, In1 last1, In2 first2, In2 last2) {
        return search(first1, last1, first2, last2, std::equal_to<typename iterator_traits<In1>::value_type>());
    }
}

int main() {
    using namespace std;

    int pattern[] = { 4, 5, 6 };
    list<int> l;
    for(int i = 0; i < 10; i++)
        l.push_back(i);

    list<int>::iterator lres = ch18::search(l.begin(), l.end(), pattern, pattern + 3);
    cout << *lres << endl;

    vector<int> v;
    for(int i = 0; i < 10; i++)
        v.push_back(i);

    vector<int>::iterator vres = ch18::search(v.begin(), v.end(), pattern, pattern + 3);
    cout << *vres << endl;
}