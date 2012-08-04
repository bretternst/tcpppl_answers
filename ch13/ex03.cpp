#include <iostream>
#include <time.h>

namespace Exercises
{
    using std::cout;
    using std::endl;

    struct Link
    {
        Link* next;
        Link* prev;
        Link() : next(0), prev(0) {}
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
            tail->next->prev = tail;
            tail = tail->next;
        }
    }

    template<class T> T& List_intrusive<T>::operator[](int idx)
    {
        int i = 0;
        Link* p = head;
        while(i++ < idx)
        {
            if(!p) throw RangeError();
            p = p->next;
        }
        return *(static_cast<T*>(p));
    }

    template<class T> class List_nonintrusive
    {
        struct Link
        {
            Link* next;
            Link* prev;
            T val;
            Link(const T& val) : val(val), next(0), prev(0) {}
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
            tail->next->prev = tail;
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
    using namespace Exercises;

    List_intrusive<LinkInt> l1;
    l1.add(LinkInt(1));
    l1.add(LinkInt(2));
    l1.add(LinkInt(3));

    List_nonintrusive<int> l2;
    l2.add(4);
    l2.add(5);
    l2.add(6);

    return 0;
}
