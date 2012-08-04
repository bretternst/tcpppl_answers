#include <iostream>
#include <time.h>

// In terms of performance, the non-intrusive list takes (over many iterations)
// about 75%-90% of the time that an intrusive list takes (under GCC).
//
// I think the non-intrusive one is the clear winner from the perspective of
// ease of use and clarity. The consumer of the list shouldn't have to worry
// about deriving the element type from a certain class that contains linked
// list housekeeping. Those details should be part of the list itself and not
// bleed out into the rest of the program. The user should only be concerned
// about the actual element type that they want to use.
namespace ch13
{
    using std::cout;
    using std::endl;

    struct Link
    {
        Link* next;
        Link() : next(0) {}
    };

    struct LinkInt : public Link
    {
        int val;
        LinkInt(int val) : val(val) {}
    };

    struct LinkChar : public Link
    {
        char val;
        LinkChar(int val) : val(val) {}
    };

    template<class T> class List_intrusive
    {       
        Link* head;
        Link* tail;
    
    public:
        class RangeError {};

        List_intrusive() : head(0), tail(0) {}
        List_intrusive(const T& t) : head(new T(t)), tail(head) {}
        ~List_intrusive();

        void add(const T& t);
        T& operator[](int idx);
    };

    template<class T> List_intrusive<T>::~List_intrusive()
    {
        Link* p = head;
        while(p) { Link* n = p->next; delete p; p = n; }
    }

    template<class T> void List_intrusive<T>::add(const T& t)
    {
        if(!head)
        {
            tail = head = new T(t);
        }
        else
        {
            tail->next = new T(t);
            tail = tail->next;
        }
    }

    template<class T> T& List_intrusive<T>::operator[](int idx)
    {
        int i = 0;
        Link* p = head;
        if(!p) throw RangeError();
        while(i++ < idx)
        {
            p = p->next;
            if(!p) throw RangeError();
        }
        return *(static_cast<T*>(p));
    }

    template<class T> class List_nonintrusive
    {
        struct Link
        {
            Link* next;
            T val;
            Link(const T& val) : val(val), next(0) {}
        };
        Link* head;
        Link* tail;
    
    public:
        class RangeError {};

        List_nonintrusive() : head(0), tail(0) {}
        List_nonintrusive(const T& t) : head(new Link(t)), tail(head) {}
        ~List_nonintrusive();

        void add(const T& t);
        T& operator[](int idx);
    };

    template<class T> List_nonintrusive<T>::~List_nonintrusive()
    {
        Link* p = head;
        while(p) { Link* n = p->next; delete p; p = n; }
    }

    template<class T> void List_nonintrusive<T>::add(const T& t)
    {
        if(!head)
        {
            tail = head = new Link(t);
        }
        else
        {
            tail->next = new Link(t);
            tail = tail->next;
        }
    }

    template<class T> T& List_nonintrusive<T>::operator[](int idx)
    {
        int i = 0;
        Link* p = head;
        while(i++ < idx)
        {
            if(!p) throw RangeError();
            p = p->next;
        }
        return p->val;
    }
}

int main()
{
    using namespace std;
    using namespace ch13;

    cout << "using intrusive linked list:" << endl;
    int start = clock();
    List_intrusive<LinkInt> l1;
    for(int i = 0; i < 10000000; i++)
        l1.add(LinkInt(i));
    cout << "elapsed time: " << clock()-start << endl;

    cout << "using non-intrusive linked list:" << endl;
    start = clock();
    List_nonintrusive<int> l2;
    for(int i = 0; i < 10000000; i++)
        l2.add(i);
    cout << "elapsed time: " << clock()-start << endl;
    return 0;
}
