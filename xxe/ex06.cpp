#include <iostream>
#include <iterator>
#include <cstddef>
#include <cassert>

namespace xxe {
    // This is much the same as the previous exercise, except for multi-element insert and erase.
    // These should both provide the strong guarantee, if I've implemented them correctly.
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
        void insert(iterator, size_t n, const T& x);
        iterator erase(iterator);
        iterator erase(iterator, iterator);
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
    void List<T,A>::insert(iterator pos, size_t n, const T& x) {
        if(n < 1)
            return;

        Node* head = 0;
        Node* tail = 0;
        for(int i = 0; i < n; i++) {
            Node* p;
            // this is a bit messy; maybe there's a cleaner way to do it?
            try {
                p = alloc.allocate(1);
                try {
                    alloc.construct(p, Node(x));
                }
                catch(...) {
                    alloc.deallocate(p, 1);
                }
            }
            catch(...) {
                p = tail;
                while(p) {
                    Node* pp = p->prev;
                    alloc.destroy(p);
                    alloc.deallocate(p, 1);
                }
                throw;
            }
            if(!head) {
                head = p;
                tail = p;
            } else {
                tail->next = p;
                p->prev = tail;
                tail = p;
            }
        }
        if(pos.n == first) {
            first->prev = tail;
            tail->next = first;
            first = head;
        } else if (pos.n == last) {
            last->prev->next = head;
            head->prev = last->prev;
            last->prev = tail;
            tail->next = last;
        } else {
            Node* prev = pos.n->prev;
            Node* next = pos.n;
            prev->next = head;
            head->prev = prev;
            next->prev = tail;
            tail->next = next;
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

    template<class T, class A>
    typename List<T,A>::iterator List<T,A>::erase(iterator start, iterator end) {
        if(start == end)
            return end;

        Node* next = end.n;
        Node* tail = next->prev;
        if(end.n != first) {
            Node* prev = start.n->prev;
            prev->next = next;
            next->prev = prev;
        } else {
            next->prev = 0;
            first = next;
        }

        Node* p = tail;
        do {
            Node* pp = p->prev;
            alloc.destroy(p);
            alloc.deallocate(p, 1);
            p = pp;
        } while (p != start.n);
        return end;
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
    i++; i++; i++;
    i = l.erase(i);
    l.insert(i, 5, 0);
    i = l.begin(); i++; i++; i++; i++; i++;
    List<int>::iterator j = i;
    j++; j++; j++; j++; j++;
    l.erase(i, j);
    copy(l.begin(), l.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
}
