#include <iostream>
#include <iterator>
#include <cstddef>
#include <cassert>

namespace xxe {
    // All operations provide the strong guarantee, and there are no special requirements.
    // In each operation, no modifications to node pointers are mode until allocation
    // or deallocation was successful.
    // As usual, memory leaks are likely if destructors throw.
    // Adding elements requires operator=
    template<class T, class A = std::allocator<T> >
    class List {
        struct Node {
            T val;
            Node* next;
            Node* prev;
            Node() : next(0), prev(0) { }
        };

    public:
        typedef typename A::template rebind<Node>::other alloc_type;
        struct iterator : std::iterator<std::bidirectional_iterator_tag, T> {
            Node* n;
            iterator(Node* n) : n(n) { }
            T& operator*() { return n->val; }
            T* operator->() { return &n->val; }
            iterator& operator++() { n = n->next; }
            iterator operator++(int) { iterator tmp = *this; *this++; return tmp; }
            iterator& operator--() { n = n->prev; }
            iterator operator--(int) { iterator tmp = *this; *this--; return tmp; }
            bool operator==(const iterator& x) { return n == x.n; }
            bool operator!=(const iterator& x) { return !(*this == x); }
        };

    private:
        alloc_type alloc;
        Node* first;
        Node* last;
        size_t sz;

        void destroy_all();

    public:
        List(const alloc_type&);
        ~List() { destroy_all(); }

        size_t size() const { return sz; }
        bool empty() const { return first == last; }
        iterator begin() { return iterator(first); }
        iterator end() { return iterator(last); }
        T& front() { return first->val; }
        T& back() { return last->prev ? last->prev->val : last->val; }
        void push_front(const T& x);
        void pop_front();
        void push_back(const T& x);
        void pop_back();
    };

    template<class T, class A>
    List<T,A>::List(const alloc_type& a = alloc_type()) : alloc(a), sz(0) {
        last = first = alloc.allocate(1);
        try {
            alloc.construct(first, Node());
        }
        catch(...) {
            alloc.deallocate(first, 1);
            throw;
        }
    }

    template<class T, class A>
    void List<T,A>::destroy_all() {
        Node* p = first;
        while(p) {
            Node* next = p->next;
            alloc.destroy(p);
            alloc.deallocate(p, 1);
            p = next;
        }
    }

    template<class T, class A>
    void List<T,A>::push_front(const T& x) {
        Node* p = alloc.allocate(1);
        alloc.construct(p, Node());
        p->val = x;
        first->prev = p;
        p->next = first;
        first = p;
    }

    template<class T, class A>
    void List<T,A>::pop_front() {
        if(first != last) {
            Node* next = first->next;
            alloc.destroy(first);
            alloc.deallocate(first, 1);
            first = next;
        }
    }

    template<class T, class A>
    void List<T,A>::push_back(const T& x) {
        if(first == last)
            push_front(x);
        else {
            Node* p = alloc.allocate(1);
            alloc.construct(p, Node());
            p->val = x;
            Node* prev = last->prev;
            p->next = last;
            p->prev = prev;
            prev->next = p;
            last->prev = p;
        }
    }

    template<class T, class A>
    void List<T,A>::pop_back() {
        if(first != last) {
            Node* p = last->prev;
            Node* prev = p->prev;
            alloc.destroy(p);
            alloc.deallocate(p, 1);
            last->prev = prev;
            prev->next = last;
        }
    }
}

int main() {
    using namespace std;
    using namespace xxe;

    List<int> l;
    l.push_back(3);
    l.push_back(4);
    l.push_back(5);
    l.push_back(6);
    l.push_front(2);
    l.push_front(1);
    l.push_front(0);
    assert(l.front() == 0);
    l.pop_front();
    l.pop_back();

    copy(l.begin(), l.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
}
