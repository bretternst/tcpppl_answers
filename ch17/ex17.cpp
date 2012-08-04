#include <iostream>
#include <string>

// included only for comparison testing
#include <list>

// this list implementation is highly stripped down. there are very few operations from the real
// list class that can be performed in less than O(n) time when a singly linked list is used. for
// example, inserting and erasing at arbitrary positions is more expensive because you do not
// have direct access to the previous node to "patch up" the link. as a result, i have removed
// most operations except for those which can be implemented efficiently. this class basically
// acts as a stack implemented using a linked list. iteration is forward-only.
namespace Exercises
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
            Node(const T& v, Node* n) : val(v),next(n) {}
        };

        class const_list_iterator;

        // iterator classes
        class list_iterator {
            friend class list;

            Node* n;
            explicit list_iterator(Node* nn) : n(nn) {}
        public:
            typedef std::forward_iterator_tag iterator_category;
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
        };

        class const_list_iterator {
            friend class list;

            Node* n;
            explicit const_list_iterator(Node* nn) : n(nn) {}
        public:
            typedef std::forward_iterator_tag iterator_category;
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
        };

        typename A::template rebind<Node>::other alloc_node;
        A alloc_value;
        Node* head;
        size_type sz;

        Node* create_node(Node* next) {
            Node* p = alloc_node.allocate(1);
            p->next = next;
            return p;
        }

        void construct_node(Node* n, const T& x) {
            alloc_value.construct(&n->val, x);
        }

        void destroy_node(Node* p) {
            alloc_node.destroy(p);
            alloc_node.deallocate(p, 1);
        }

    public:
        typedef list_iterator iterator;
        typedef const_list_iterator const_iterator;

        // constructors/destructor/assignment
        explicit list(const A& a = A()) : sz(0),head(0),alloc_value(a) {}

        ~list() {
            clear();
        }

        // iterators
        iterator begin() {
            return list_iterator(head);
        }

        const_iterator begin() const {
            return const_list_iterator(head);
        }

        iterator end() {
            return list_iterator(0);
        }

        const_iterator end() const {
            return const_list_iterator(0);
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
            return head->val;
        }

        const_reference front() const {
            return head->val;
        }

        void push_front(const T& x) {
            head = create_node(head);
            construct_node(head,x);
            sz++;
        }

        void pop_front() {
            Node* n = head->next;
            destroy_node(head);
            head = n;
            sz--;
        }

        void swap(list& x) {
            std::swap(head, x.head); std::swap(sz,x.sz);
        }

        void clear() { 
            while(head) {
                Node* n = head;
                head = head->next;
                destroy_node(n);
            }
            sz = 0;
        }

        // allocator
        allocator_type get_allocator() const {
            return alloc_value;
        }
    };

    template<class T, class A>
    bool operator==(const list<T,A>& x, const list<T,A>& y) {
        if(x.size() != y.size()) {
            return false;
        }

        for(list<T,A>::const_iterator ix = x.begin(), iy = y.begin(); ix != x.end(); ++ix,++iy) {
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

template<class L1, class L2>
void compare_result(char* name, const L1& l1, const L2& l2) {
    std::cout << name << ": ";
    if(l1.size() != l2.size()) std::cout << "FAIL - size differs" << std::endl;

    else if(l1.size() > 0 && (l1.front() != l2.front()))
        std::cout << "FAIL - front differs" << std::endl;

    else if(std::lexicographical_compare(l1.begin(),l1.end(),l2.begin(),l2.end()) ||
        std::lexicographical_compare(l2.begin(),l2.end(),l1.begin(),l1.end()))
        std::cout << "FAIL - difference in forward iteration" << std::endl;

    else std::cout << "PASS" << std::endl;
}

template<class L>
void test_fill1(L& l) {
    l.push_front(1);
    l.push_front(2);
    l.push_front(3);
    l.push_front(4);
    l.push_front(5);
}

int main() {
    Exercises::list<int> l1;
    std::list<int> l2;

    test_fill1(l1);
    test_fill1(l2);
    compare_result("fill", l1, l2);

    l1.pop_front();
    l2.pop_front();
    compare_result("pop_front", l1, l2);

    l1.clear();
    l2.clear();
    compare_result("clear", l1, l2);
}
