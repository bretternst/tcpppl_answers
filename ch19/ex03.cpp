#include <iostream>
#include <iterator>
#include <vector>

namespace ch19 {
    using namespace std;

    template<class Iter>
    class reverse_iterator : public iterator<typename iterator_traits<Iter>::iterator_category,
                                            typename iterator_traits<Iter>::value_type,
                                            typename iterator_traits<Iter>::difference_type,
                                            typename iterator_traits<Iter>::pointer,
                                            typename iterator_traits<Iter>::reference> {
    protected:
        Iter current;
        typedef iterator_traits<Iter> traits;
    public:
        typedef typename traits::value_type value_type;
        typedef typename traits::difference_type difference_type;
        typedef typename traits::pointer pointer;
        typedef typename traits::reference reference;
        typedef Iter iterator_type;

        reverse_iterator() : current() { }
        explicit reverse_iterator(Iter x) : current(x) { }
        template<class U> reverse_iterator(const reverse_iterator<U>& x) : current(x.base()) { }

        Iter base() const { return current; }

        reference operator*() const { Iter tmp = current; return *--tmp; }
        pointer operator->() const { Iter tmp = current; return &(*--tmp); }
        reference operator[](difference_type n) const;

        reverse_iterator& operator++() { --current; return *this; }
        reverse_iterator operator++(int) { reverse_iterator t = current; --current; return t; }
        reverse_iterator& operator--() { ++current; return *this; }
        reverse_iterator operator--(int) { reverse_iterator t = current; ++current; return t; }

        reverse_iterator operator+(difference_type n) const {
            reverse_iterator tmp = *this;
            tmp += n;
            return tmp;
        };
        reverse_iterator& operator+=(difference_type n) {
            current -= n;
            return *this;
        };
        reverse_iterator operator-(difference_type n) const {
            reverse_iterator tmp = *this;
            tmp -= n;
            return tmp;
        };
        reverse_iterator& operator-=(difference_type n) {
            current += n;
            return *this;
        }
        difference_type operator-(reverse_iterator& x) const {
            return x.base() - base();
        };
    };
}

int main() {
    using namespace std;

    vector<int> v;
    for(int i = 0; i < 10; i++)
        v.push_back(i);
    typedef ch19::reverse_iterator<vector<int>::iterator> ri;

    copy(ri(v.end()), ri(v.begin()), ostream_iterator<int>(cout, " "));
    cout << endl;

    ri r(v.end());
    r += 3;
    cout << *r << " " << *(r - 1) << endl; // 6 7
}
