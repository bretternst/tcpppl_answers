#include <iostream>
#include <vector>

namespace ch19 {
    using namespace std;

    class out_of_bounds { };

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

        void valid(Iter p) {
            valid(p, typename traits_type::iterator_category());
        }

        friend bool operator==(const Checked_iter& i, const Checked_iter& j) {
            return i.c==j && i.curr==j.curr;
        }

        Checked_iter(Cont& x, Iter p) : c(&x), curr(p) { valid(p); }
        Checked_iter(Cont& x) : c(&x), curr(x.begin()) { }

        reference operator*() const {
            if(curr==c->end()) throw out_of_bounds();
            return *curr;
        }

        pointer operator->() const {
            if(curr==c->end()) throw out_of_bounds();
            return &*curr;
        }

        Checked_iter operator+(difference_type d) const {
            if(c->end() - curr < d || d < -(curr - c->begin())) throw out_of_bounds();
            return Checked_iter(c,curr+d);
        }

        reference operator[](difference_type d) const {
            if(c->end() - curr <= d || d < -(curr - c->begin())) throw out_of_bounds();
            return curr[d];
        }

        Checked_iter& operator++() {
            if(curr == c->end()) throw out_of_bounds();
            ++curr;
            return *this;
        }

        Checked_iter operator++(int) {
            Checked_iter tmp = *this;
            ++*this;
            return tmp;
        }

        Checked_iter& operator--() {
            if(curr == c->begin()) throw out_of_bounds();
            --curr;
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
}

int main() {
    using namespace ch19;

    vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    Checked_iter<vector<int> > ci = make_checked(v);
    ci++;
    ci++;
    ci++;
    try {
        ci++;
    }
    catch(out_of_bounds) {
        cout << "caught out_of_bounds" << endl;        
    }
    ci--;
    ci--;
    ci--;
    try {
        ci--;
    }
    catch (out_of_bounds) {
        cout << "caught out_of_bounds" << endl;
    }
}
