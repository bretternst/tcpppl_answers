#include <iostream>
#include <string>
#include <typeinfo>

// included only for comparison testing
#include <list>

namespace ch17
{
    class integral_type {};
    class not_integral_type {};

    template<class T> struct is_integral { typedef not_integral_type type; };
    template<> struct is_integral<char> { typedef integral_type type; };
    template<> struct is_integral<short> { typedef integral_type type; };
    template<> struct is_integral<int> { typedef integral_type type; };
    template<> struct is_integral<long> { typedef integral_type type; };
    template<> struct is_integral<unsigned char> { typedef integral_type type; };
    template<> struct is_integral<unsigned short> { typedef integral_type type; };
    template<> struct is_integral<unsigned int> { typedef integral_type type; };
    template<> struct is_integral<unsigned long> { typedef integral_type type; };

    template<class T, class A = std::allocator<T> >
    class list {
    public:
        typedef T value_type;
        typedef A allocator_type;
        typedef typename A::size_type size_type;

        typedef typename A::pointer pointer;
        typedef typename A::const_pointer const_pointer;
        typedef typename A::reference reference;
        typedef typename A::const_reference const_reference;

    private:
        struct Node {
            T val;
            Node* next;
            Node* prev;
            Node(const T& v, Node* n, Node* p) : val(v),next(n),prev(p) {}
        };

        class const_list_iterator;

        // iterator classes
        class list_iterator {
            friend class list;

            Node* n;
            explicit list_iterator(Node* nn) : n(nn) {}
        public:
            typedef std::bidirectional_iterator_tag iterator_category;
            typedef typename list::value_type value_type;
            typedef typename A::difference_type difference_type;
            typedef typename list::pointer pointer;
            typedef typename list::reference reference;

            list_iterator() : n(0) {}
            list_iterator(const list_iterator& x) : n(x.n) {}
            list_iterator& operator=(const list_iterator& x) {
                n = x.n;
                return *this;
            }

            bool operator==(const list_iterator& x) const { return n == x.n; }
            bool operator!=(const list_iterator& x) const { return n != x.n; }
            bool operator==(const const_list_iterator& x) const { return n == x.n; }
            bool operator!=(const const_list_iterator& x) const { return n != x.n; }
            reference operator*() const { return n->val; }
            pointer operator->() const { return &(n->val); }

            list_iterator& operator++() {
                n = n->next; return *this;
            }

            list_iterator operator++(int) {
                list_iterator res = *this;
                ++*this;
                return res;
            }

            list_iterator& operator--() {
                n = n->prev;
                return *this;
            }

            list_iterator operator--(int) {
                list_iterator res = *this;
                --*this;
                return res;
            }
        };

        class const_list_iterator {
            friend class list;

            Node* n;
            explicit const_list_iterator(Node* nn) : n(nn) {}
        public:
            typedef std::bidirectional_iterator_tag iterator_category;
            typedef typename list::value_type value_type;
            typedef typename A::difference_type difference_type;
            typedef typename list::const_pointer pointer;
            typedef typename list::const_reference reference;

            const_list_iterator() : n(0) {}
            const_list_iterator(const const_list_iterator& x) : n(x.n) {}
            const_list_iterator(const list_iterator& x) : n(x.n) {}
            const_list_iterator& operator=(const const_list_iterator& x) {
                n = x.n;
                return *this;
            }

            bool operator==(const const_list_iterator& x) const { return n == x.n; }
            bool operator!=(const const_list_iterator& x) const { return n != x.n; }
            const_reference operator*() const { return n->val; }
            const_pointer operator->() const { return &(n->val); }

            const_list_iterator& operator++() {
                n = n->next;
                return *this;
            }
            
            const_list_iterator operator++(int) {
                const_list_iterator res = *this;
                ++*this;
                return res;
            }

            const_list_iterator& operator--() {
                n = n->prev;
                return *this;
            }

            const_list_iterator& operator--(int) {
                const_list_iterator res = *this;
                --*this;
                return res;
            }
        };

        typename A::template rebind<Node>::other alloc_node;
        A alloc_value;
        Node* head;
        size_type sz;

        Node* create_node(Node* next, Node* prev) {
            Node* p = alloc_node.allocate(1);
            p->next = next ? next : p;
            p->prev = prev ? prev : p;
            return p;
        }

        void construct_node(Node* n, const T& x) {
            alloc_value.construct(&n->val, x);
        }

        void destroy_node(Node* p) {
            alloc_node.destroy(p);
            alloc_node.deallocate(p, 1);
        }

        template<class In>
        void do_insert(const_list_iterator pos, In first, In last, integral_type) {
            insert(pos, (size_type)first, last);
        }

        template<class In>
        void do_insert(const_list_iterator pos, In first, In last, not_integral_type) {
            while(first!=last) {
                pos = insert(pos, *first++);
                ++pos;
            }
        }

    public:
        typedef list_iterator iterator;
        typedef const_list_iterator const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

        // constructors/destructor/assignment
        explicit list(const A& a = A()) : sz(0),head(create_node(0,0)),alloc_value(a) {}

        explicit list(size_type n, const T& val = T(), const A& a = A()) : sz(0),head(create_node(0,0)),alloc_value(a) {
            insert(end(), n, val);
        }

        template<class In>
        list(In first, In last, const A& a = A()) : sz(0),head(create_node(0,0)),alloc_value(a) {
            assign(first, last);
        }

        list(const list& x) : sz(0),head(create_node(0,0)) {
            assign(x.begin(),x.end());
        }

        ~list() {
            clear();
        }

        list& operator=(const list& x) {
            assign(x.begin(),x.end());
            return *this;
        }

        // iterators
        iterator begin() {
            return list_iterator(head->next);
        }

        const_iterator begin() const {
            return const_list_iterator(head->next);
        }

        iterator end() {
            return list_iterator(head);
        }

        const_iterator end() const {
            return const_list_iterator(head);
        }

        reverse_iterator rbegin() {
            return reverse_iterator(list_iterator(head));
        }

        const_reverse_iterator rbegin() const {
            return const_reverse_iterator(list_iterator(head));
        }

        reverse_iterator rend() {
            return reverse_iterator(list_iterator(head->next));
        }

        const_reverse_iterator rend() const {
            return const_reverse_iterator(const_list_iterator(head->next));
        }

        // capacity
        size_type size() const {
            return sz;
        }

        bool empty() const {
            return size()==0;
        }

        size_type max_size() const {
            return alloc_node.max_size();
        }

        void resize(size_type n, T val = T());

        // element access
        reference front() {
            return head->next->val;
        }

        const_reference front() const {
            return head->next->val;
        }

        reference back() {
            return head->prev->val;
        }

        const_reference back() const {
            return head->prev->val;
        }

        // modifiers
        template<class In>
        void assign(In first, In last) {
            clear();
            insert(begin(), first, last);
        }

        void assign(size_type n, const T& val) {
            clear();
            insert(begin(), n, val);
        }

        void push_front(const T& x) {
            insert(begin(), x);
        }

        void pop_front() {
            erase(begin());
        }

        void push_back(const T& x) {
            insert(end(), x);
        }

        void pop_back() {
            erase(iterator(head->prev));
        }

        iterator insert(const_iterator pos, const T& x);

        void insert(const_iterator pos, size_type n, const T& x) {
            while(n--) {
                pos = insert(pos, x);
            }
        }

        template<class In>
        void insert(const_iterator pos, In first, In last) {
            do_insert(pos, first, last, typename is_integral<In>::type());
        }

        iterator erase(const_iterator pos);

        iterator erase(const_iterator first, const_iterator last) {
            while(first!=last) {
                first = erase(first);
            }
            return iterator(first.n);
        }

        void swap(list& x) {
            std::swap(head, x.head); std::swap(sz, x.sz);
        }

        void clear() { 
            erase(begin(),end());
        }

        // operations
        void splice(const_iterator pos, list& x) {
            splice(pos, x, x.begin(), x.end());
        }

        void splice(const_iterator pos, list& x, iterator p) {
            iterator p1 = p; ++p1; splice(pos, x, p, p1);
        }

        void splice(const_iterator pos, list& x, iterator first, iterator last);

        void remove(const T& val) {
            remove_if(std::bind2nd(std::equal_to<T>(),val));
        }

        template<class Pred> void remove_if(Pred p) {
            for(iterator i=begin();i!=end();) {
                if(p(*i)) {
                    i = erase(i);
                }
                else {
                    ++i;
                }
            }
        }

        void unique() {
            unique(std::equal_to<T>());
        }

        template<class BinPred>
        void unique(BinPred b);

        void merge(list& x) {
            merge(x, std::less<T>());
        }

        template<class Cmp> void merge(list&, Cmp);

        void sort() {
            sort(std::less<T>());
        }

        template<class Cmp>
        void sort(Cmp);
        
        void reverse();

        // allocator
        allocator_type get_allocator() const {
            return alloc_value;
        }
    };

    template<class T, class A>
    void list<T,A>::resize(size_type n, T val) {
        if(n<sz) {
            iterator i = begin();
            while(n--) ++i;
            erase(i, end());
        }
        else if(n>sz) {
            insert(end(), n-sz, val);
        }
    };

    template<class T, class A>
    typename list<T,A>::iterator list<T,A>::insert(const_iterator pos, const T& x) {
        Node* n = create_node(pos.n, pos.n->prev);
        construct_node(n, x);
        pos.n->prev->next = n;
        pos.n->prev = n;
        sz++;
        return list_iterator(n);
    }

    template<class T, class A>
    typename list<T,A>::iterator list<T,A>::erase(const_iterator pos) {
        Node* n = pos.n->prev->next = pos.n->next;
        n->prev = pos.n->prev;
        destroy_node(pos.n);
        sz--;
        return iterator(n);
    }

    template<class T, class A>
    void list<T,A>::splice(const_iterator pos, list& x, iterator first, iterator last) {
        first.n->prev->next = last.n;
        last.n->prev->next = pos.n;

        Node* lastNode = last.n->prev;
        last.n->prev = first.n->prev;
        first.n->prev = pos.n->prev;

        pos.n->prev->next = first.n;
        pos.n->prev = lastNode;

        while(first++ != pos) {
            x.sz--;sz++;
        }
    }

    template<class T, class A>
    template<class BinPred>
    void list<T,A>::unique(BinPred b) {
        if(sz < 2) return;

        for(iterator i = begin(), prev = i++; i != end();) {
            if(b(*i,*prev)) {
                i = erase(i);
            }
            else {
                prev = i++;
            }
        }
    }

    template<class T, class A>
    template<class Cmp>
    void list<T,A>::merge(list& x, Cmp cmp) {
        iterator ipos = begin();
        iterator first = x.begin();
        iterator last = first;
        while(first != x.end())
        {
            while(ipos != end() && cmp(*ipos,*first)) {
                ++ipos;
            }
            do { 
                ++last;
            } while (last != x.end() && cmp(*last,*ipos));
            splice(ipos, x, first, last);
            first = last;
        }
    }

    template<class T, class A>
    template<class Cmp>
    void list<T,A>::sort(Cmp cmp) {
        if(sz > 1)
        {
            iterator pos = begin();
            for(list<T,A>::size_type i = 0; i < sz/2; i++) {
                ++pos;
            }
            list other;
            other.splice(other.begin(),*this,begin(),pos);
            other.sort();
            sort();
            merge(other,cmp);
        }
    }

    template<class T, class A>
    void list<T,A>::reverse() {
        Node* p = head;
        do
        {
            std::swap(p->next,p->prev);
            p = p->prev;
        } while (p != head);
    }

    template<class T, class A>
    bool operator==(const list<T,A>& x, const list<T,A>& y) {
        if(x.size() != y.size()) {
            return false;
        }

        for(typename list<T,A>::const_iterator ix = x.begin(), iy = y.begin(); ix != x.end(); ++ix,++iy) {
            if(*ix != *iy) return false;
        }
        return true;
    }

    template<class T, class A>
    bool operator<(const list<T,A>& x, const list<T,A>& y) {
        return std::lexicographical_compare(x.begin(),x.end(),y.begin(),y.end());
    }
}

// end list implementation
// begin test cases

struct int_holder {
    int i;
    int_holder() : i(0) {}
    int_holder(int ii) : i(ii) {}
};
bool operator==(const int_holder& x, const int_holder& y) { return x.i==y.i; }
bool operator!=(const int_holder& x, const int_holder& y) { return x.i!=y.i; }
bool operator<(const int_holder& x, const int_holder& y) { return x.i<y.i; }

template<class L1, class L2>
void compare_result(const char* name, const L1& l1, const L2& l2) {
    std::cout << name << ": ";
    if(l1.size() != l2.size()) std::cout << "FAIL - size differs" << std::endl;

    else if(l1.size() > 0 && (l1.front() != l2.front() || l1.back() != l2.back()))
        std::cout << "FAIL - front or back differs" << std::endl;

    else if(std::lexicographical_compare(l1.begin(),l1.end(),l2.begin(),l2.end()) ||
        std::lexicographical_compare(l2.begin(),l2.end(),l1.begin(),l1.end()))
        std::cout << "FAIL - difference in forward iteration" << std::endl;

    else if(std::lexicographical_compare(l1.rbegin(),l1.rend(),l2.rbegin(),l2.rend()) ||
        std::lexicographical_compare(l2.rbegin(),l2.rend(),l1.rbegin(),l1.rend()))
        std::cout << "FAIL - difference in reverse iteration" << std::endl;

    else std::cout << "PASS" << std::endl;
}

template<class L>
void test_fill1(L& l) {
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    l.push_back(4);
    l.push_back(5);
    l.push_front(5);
    l.push_front(4);
    l.push_front(3);
    l.push_front(2);
    l.push_front(1);
}

template<class L>
void test_fill2(L& l) {
    l.push_back(100);
    l.push_back(200);
    l.push_back(300);
}

template<class L>
void test_fill3(L& l) {
    l.push_back(1);
    l.push_back(1);
    l.push_back(4);
    l.push_back(4);
    l.push_back(7);
    l.push_back(7);
    l.push_back(7);
    l.push_back(9);
    l.push_back(15);
    l.push_back(15);
}

template<class L>
void test_fill4(L& l) {
    l.push_back(0);
    l.push_back(3);
    l.push_back(3);
    l.push_back(7);
    l.push_back(8);
    l.push_back(14);
    l.push_back(17);
}

template<class L>
void test_resize(L& l) {
    l.clear();
    test_fill1(l);
    l.resize(l.size()-3,999);
    l.resize(l.size()+5,999);
}

template<class L>
void test_pop_front_empty(L& l) {
    l.clear();
    l.push_front(1);
    l.pop_front();
}

template<class L>
void test_pop_back_empty(L& l) {
    l.clear();
    l.push_front(1);
    l.pop_back();
}

template<class L>
void test_insert(L& l) {
    l.clear();
    test_fill1(l);
    typename L::iterator i = l.insert(l.begin(),1000);
    l.insert(i, 1001);
    i = l.insert(l.end(),1002);
    l.insert(i, 1003);
    i = l.begin();
    ++i; ++i;
    i = l.insert(i, 1004);
    l.insert(i, 1005);

    l.insert(i, 5, 1006);

    std::list<int> x;
    x.push_back(1007);
    x.push_back(1008);
    x.push_back(1009);
    l.insert(i,x.begin(),x.end());
}

template<class L>
void test_erase(L& l) {
    l.clear();
    test_fill1(l);
    l.erase(l.begin());
    l.erase(--l.end());
    typename L::iterator i = l.begin();
    ++i; ++i;
    i = l.erase(i);
    typename L::iterator i2 = i;
    ++i2; ++i2;
    i = l.erase(i,i2);
    l.erase(i);
}

template<class L>
void test_swap(L& l) {
    l.clear();
    L l2;
    test_fill1(l2);
    l.swap(l2);
}

template<class L>
void test_splice(L& l) {
    l.clear();
    test_fill1(l);
    L l2;
    test_fill2(l2);
    l.splice(++l.begin(), l2);
    test_fill2(l2);
    l.splice(l.begin(), l2);
    test_fill2(l2);
    l.splice(l.end(), l2);
    test_fill2(l2);
    l.splice(l.begin(), l2, l2.begin());
    l.splice(l.end(), l2, --l2.end());
    test_fill1(l2);
    l.splice(l.begin(), l2, ++++l2.begin(), ++++++++l2.begin());
    l.splice(l.end(), l2, --------l2.end(), ----l2.end());
    l.splice(++++++l.begin(), l2, ++l2.begin(), ++++++l2.begin());
}

template<class L>
void test_remove(L& l) {
    l.clear();
    test_fill3(l);
    l.remove(1);
    l.remove(7);
    l.remove(100);
}

template<class L>
void test_unique(L& l) {
    l.clear();
    test_fill3(l);
    l.unique();
}

template<class L>
void test_merge(L& l) {
    l.clear();
    test_fill3(l);
    L l2;
    test_fill4(l2);
    l.merge(l2);
    l2.push_back(2);
    l2.push_back(4);
    l2.push_back(6);
    l2.push_back(7);
    l2.push_back(8);
    l2.push_back(10);
    l2.push_back(12);
    l.merge(l2);
}


template<class L>
void test_sort1(L& l) {
    l.clear();
    l.push_back(3);
    l.push_back(2);
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    l.push_back(11);
    l.push_back(4);
    l.sort();
}

template<class L>
void test_sort2(L& l) {
    l.clear();
    l.push_back(2);
    l.push_back(4);
    l.push_back(6);
    l.push_back(7);
    l.push_back(8);
    l.push_back(10);
    l.push_back(12);
    l.sort();
}

template<class L>
void test_sort3(L& l) {
    l.clear();
    l.push_back(3);
    l.push_back(2);
    l.push_back(1);
    l.sort();
}

template<class L>
void test_reverse(L& l) {
    l.push_back(2);
    l.reverse();
    l.push_back(1);
    l.reverse();
    test_fill1(l);
    l.reverse();
    test_fill2(l);
    l.reverse();
    test_fill3(l);
    l.reverse();
}

bool test_equality() {
    ch17::list<int> l1, l2;
    test_fill1(l1);
    test_fill1(l2);
    if(!(l1==l2)) return false;
    l2.push_back(2);
    l1.push_back(1);
    if(l1==l2) return false;
    l1.pop_back();
    if(l1==l2) return false;
    if(l2<l1) return false;
    l2.pop_back();
    if(l1<l2) return false;
    l1.push_back(1);
    l2.push_back(2);
    if(!(l1<l2)) return false;
    return true;
}

template<class T>
void run_tests() {
    std::cout << "test pass T=" << typeid(T).name() << std::endl;

    ch17::list<T> l1;
    std::list<T> l2;

    test_fill1(l1);
    test_fill1(l2);
    compare_result("push_back",l1,l2);

    test_resize(l1);
    test_resize(l2);
    compare_result("resize",l1,l2);

    l1.pop_front();
    l2.pop_front();
    l1.pop_back();
    l2.pop_back();
    compare_result("pop",l1,l2);

    test_pop_front_empty(l1);
    test_pop_front_empty(l2);
    compare_result("pop_front_empty",l1,l2);

    test_pop_back_empty(l1);
    test_pop_back_empty(l2);
    compare_result("pop_back_empty",l1,l2);

    test_insert(l1);
    test_insert(l2);
    compare_result("insert",l1,l2);

    test_erase(l1);
    test_erase(l2);
    compare_result("erase",l1,l2);

    test_swap(l1);
    test_swap(l2);
    compare_result("swap",l1,l2);

    test_splice(l1);
    test_splice(l2);
    compare_result("splice",l1,l2);

    test_remove(l1);
    test_remove(l2);
    compare_result("remove",l1,l2);

    test_unique(l1);
    test_unique(l2);
    compare_result("unique",l1,l2);

    test_merge(l1);
    test_merge(l2);
    compare_result("merge",l1,l2);

    test_sort1(l1);
    test_sort1(l2);
    compare_result("sort1",l1,l2);

    test_sort2(l1);
    test_sort2(l2);
    compare_result("sort2",l1,l2);

    test_sort3(l1);
    test_sort3(l2);
    compare_result("sort3",l1,l2);

    test_reverse(l1);
    test_reverse(l2);
    compare_result("reverse",l1,l2);

    std::cout << "equality: " << (test_equality() ? "PASS" : "FAIL") << std::endl;

    const ch17::list<T> l3(l1);
    compare_result("const_iterator",l1,l3);

    std::list<T> stdlctor1(10, 10);
    ch17::list<T> exlctor1(10, 10);
    compare_result("ctor1",stdlctor1,exlctor1);

    std::list<T> stdlctor2(l1.begin(),l1.end());
    ch17::list<T> exlctor2(l1.begin(),l1.end());
    compare_result("ctor2",stdlctor2,exlctor2);

    ch17::list<T> exlctor3(l1);
    compare_result("ctor3",exlctor3,l1);
};

int main() {
    run_tests<int>();
    run_tests<int_holder>();
}
