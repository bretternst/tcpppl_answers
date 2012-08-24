#include <iostream>
#include <vector>

namespace ch19 {
    using namespace std;

    class out_of_bounds { };

    // Implementation of Checked_iter brought over from previous exercise.
    template<class Cont, class Iter = typename Cont::iterator>
    class Checked_iter : public iterator_traits<Iter> {
        Iter curr;
        Cont* c;
    public:
        typedef iterator_traits<Iter> traits_type;
        typedef typename traits_type::reference reference;
        typedef typename traits_type::pointer pointer;
        typedef typename traits_type::difference_type difference_type;

        void valid(Iter p, random_access_iterator_tag) const {
            if (p > c->end() || p < c->begin())
                throw out_of_bounds();
        }

        void valid(Iter p, forward_iterator_tag) const {
            if(c->end() == p)
                return;
            for(Iter pp = c->begin(); pp != c->end(); ++pp) {
                if(pp == p)
                    return;
            }
            throw out_of_bounds();
        }

        void valid(Iter p) const {
            valid(p, typename traits_type::iterator_category());
        }

        void check() const {
            valid(curr);
        }

        friend bool operator==(const Checked_iter& i, const Checked_iter& j) {
            return i.c==j && i.curr==j.curr;
        }

        Checked_iter(Cont& x, Iter p) : c(&x), curr(p) { valid(p); }
        Checked_iter(Cont& x) : c(&x), curr(x.begin()) { }

        reference operator*() const {
            valid(curr);
            if(curr==c->end()) throw out_of_bounds();
            return *curr;
        }

        pointer operator->() const {
            valid(curr);
            if(curr==c->end()) throw out_of_bounds();
            return &*curr;
        }

        Checked_iter operator+(difference_type d) const {
            Iter newCurr = curr + d;
            valid(newCurr);
            return Checked_iter(*c,newCurr);
        }

        reference operator[](difference_type d) const {
            Iter newCurr = curr + d;
            valid(newCurr);
            if(newCurr == c->end()) throw out_of_bounds();
            return curr[d];
        }

        Checked_iter& operator++() {
            Iter newCurr = curr;
            ++newCurr;
            valid(newCurr);
            curr = newCurr;
            return *this;
        }

        Checked_iter operator++(int) {
            Checked_iter tmp = *this;
            ++*this;
            return tmp;
        }

        Checked_iter& operator--() {
            Iter newCurr = curr;
            --newCurr;
            valid(newCurr);
            curr = newCurr;
            return *this;
        }

        Checked_iter operator--(int) {
            Checked_iter tmp = *this;
            --*this;
            return tmp;
        }

        difference_type index() const { return curr - c->begin(); }
        Iter unchecked() const { return curr; }

        Checked_iter operator-(difference_type d) const {
            return *this + -d;
        }

        difference_type operator-(const Checked_iter& x) const {
            return this->curr - x.curr;
        }

        bool operator<(const Checked_iter& x) const {
            return this->curr < x.curr;
        }

        friend bool operator!=(const Checked_iter& i, const Checked_iter& j) {
            return i.c!=j || i.curr!=j.curr;
        }

        bool operator>(const Checked_iter& x) const {
            return !(*this < x) && *this != x;
        }

        bool operator<=(const Checked_iter& x) const {
            return *this < x || *this == x;
        }

        bool operator>=(const Checked_iter& x) const {
            return !(*this < x);
        }
    };

    template<class Cont, class Iter>
    Checked_iter<Cont,typename Cont::iterator> make_checked(Cont& c, Iter i) {
        return Checked_iter<Cont,typename Cont::iterator>(c, i);
    }

    template<class Cont>
    Checked_iter<Cont,typename Cont::iterator> make_checked(Cont& c) {
        return Checked_iter<Cont,typename Cont::iterator>(c, c.begin());
    }

    // To make this simpler, I'm leaving out const operations and most
    // of the container operations that vector and list don't have in
    // common.
    template<class Cont>
    class Cont_handle {
        Cont* c;

    public:
        typedef Checked_iter<Cont, typename Cont::iterator> iterator;
        typedef typename Cont::size_type size_type;
        typedef typename Cont::reference reference;
        typedef typename Cont::value_type value_type;

        explicit Cont_handle(Cont& x) : c(&x) { }

        iterator begin() { return make_checked(*c, c->begin()); }
        iterator end() { return make_checked(*c, c->end()); }
        bool empty() const { return c->empty(); }
        size_type size() const { return c->size(); }
        reference front() { return c->front(); }
        reference back() { return c->back(); }
        void clear() { c->clear(); }

        void push_back(const value_type& x) { c->push_back(x); }
        void pop_back() { c->pop_back(); }

        iterator insert(iterator pos, const value_type& x) {
            pos.check();
            c->insert(pos.unchecked(), x);
        }

        iterator insert(iterator pos, size_type n, const value_type& x) {
            pos.check();
            c->insert(pos.unchecked(), n, x);
        }

        template<class In>
        iterator insert(iterator pos, In first, In last) {
            pos.check();
            c->insert(pos.unchecked(), first, last);
        }

        iterator erase(iterator pos) {
            pos.check();
            if(pos.unchecked() == c->end()) throw out_of_bounds();
            c->erase(pos.unchecked());
        }

        iterator erase(iterator first, iterator last) {
            first.check();
            if(first.unchecked() == c->end()) throw out_of_bounds();
            last.check();
            c->erase(first.unchecked(), last.unchecked());
        }
    };

    template<class Cont>
    Cont_handle<Cont> make_handle(Cont& x) {
        return Cont_handle<Cont>(x);
    }
}

int main() {
    using namespace ch19;

    vector<int> v;
    Cont_handle<vector<int> > h = make_handle(v);
    h.push_back(1);
    h.push_back(2);
    h.push_back(3);

    h.insert(h.end(), 4);

    h.erase(h.end() - 1);
    try {
        h.erase(h.end());
    }
    catch(out_of_bounds) {
        cout << "caught out_of_bounds" << endl;
    }

    Cont_handle<vector<int> >::iterator i = h.end();
    h.erase(h.begin());
    try {
        h.insert(i, 5);
    }
    catch(out_of_bounds) {
        cout << "caught out_of_bounds" << endl;
    }
    h.insert(h.begin(), 0);
}