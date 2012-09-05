#include <iostream>
#include <iterator>
#include <cstddef>
#include <cassert>

namespace xxe {
    // All operations provide the strong guarantee, and there are no special requirements.
    // In each operation, no modifications to node pointers are made until allocation
    // or deallocation was successful and copying or assignment has been done.
    // As usual, memory leaks are likely if destructors throw.
    // Adding elements requires operator=
    template<class T, class A = std::allocator<T> >
    class List {
        struct Node {
            T val;
            Node* next;
            Node* prev;
            Node() : next(0), prev(0) { }
            Node(const T& x) : next(0), prev(0), val(x) { }
        };

    public:
        typedef typename A::template rebind<Node>::other alloc_type;
        struct iterator : std::iterator<std::bidirectional_iterator_tag, T> {
            Node* n;
            iterator(Node* n) : n(n) { }
            T& operator*() { return n->val; }
            T* operator->() { return &n->val; }
            iterator& operator++() { n = n->next; }
            iterator operator++(int) { iterator tmp = *this; ++*this; return tmp; }
            iterator& operator--() { n = n->prev; }
            iterator operator--(int) { iterator tmp = *this; --*this; return tmp; }
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
        iterator insert(iterator, const T&);
        iterator erase(iterator);
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
        alloc.construct(p, Node(x));
        first->prev = p;
        p->next = first;
        first = p;
    }

    template<class T, class A>
    void List<T,A>::pop_front() {
        if(first != last) {
            Node* p = first;
            first->next->prev = 0;
            first = first->next;
            alloc.destroy(p);
            alloc.deallocate(p, 1);
        }
    }

    template<class T, class A>
    void List<T,A>::push_back(const T& x) {
        if(first == last)
            push_front(x);
        else {
            Node* p = alloc.allocate(1);
            alloc.construct(p, Node(x));
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
            last->prev = prev;
            prev->next = last;
            alloc.destroy(p);
            alloc.deallocate(p, 1);
        }
    }
    
    template<class T, class A>
    typename List<T,A>::iterator List<T,A>::insert(iterator pos, const T& x) {
        if(pos == end()) {
            push_back(x);
            return iterator(last->prev);
        }
        else if (pos == begin()) {
            push_front(x);
            return begin();
        }
        else {
            Node* p = alloc.allocate(1);
            alloc.construct(p, Node(x));
            p->val = x;
            Node* prev = pos.n->prev;
            pos.n->prev = p;
            prev->next = p;
            p->prev = prev;
            p->next = pos.n;
            return iterator(p);
        }
    }

    template<class T, class A>
    typename List<T,A>::iterator List<T,A>::erase(iterator pos) {
        if(pos == end()) {
            pop_back();
            return end();
        }
        else if (pos == begin()) {
            pop_front();
            return begin();
        }
        else {
            Node* prev = pos.n->prev;
            Node* next = pos.n->next;
            prev->next = next;
            next->prev = prev;
            alloc.destroy(pos.n);
            alloc.deallocate(pos.n, 1);
            return next;
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
    l.push_back(5);
    l.push_back(6);
    l.push_front(1);
    l.push_front(0);
    assert(l.front() == 0);
    l.pop_front();
    l.pop_back();

    l.insert(l.end(), 6);
    l.insert(l.begin(), 0);
    List<int>::iterator i = l.begin();
    i++;
    i++;
    i = l.insert(i, 2);
    i++;
    i++;
    i++;
    l.erase(i);
    copy(l.begin(), l.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
}
