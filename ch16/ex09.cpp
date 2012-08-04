#include <iostream>

namespace Exercises
{
    class Object {};

    class Container : public Object
    {
    public:
        struct Bad_op
        {
            const char* p;
            Bad_op(const char* pp) : p(pp) {}
        };

        virtual Object* get() { throw Bad_op("get"); }
        virtual void put(Object*) { throw Bad_op("put"); }
        virtual Object*& operator[](size_t) { throw Bad_op("[]"); }
    };

    // Despite what the exercise says, this class isn't actually defined in 16.2.2.
    // So, as with exercise 7, I will define minimal capabilities as described in
    // section 16.2.1, but using this new style of inheritance instead.
    class Vector : public Container
    {
        static const int DefaultCapacity = 16;
        size_t sz;
        size_t capacity;
        Object** elements;
    public:
        explicit Vector() : sz(0), capacity(DefaultCapacity), elements(new Object*[DefaultCapacity]) {}
        ~Vector() { delete[] elements; }
        Object* get() { return sz > 0 ? elements[--sz] : 0; }
        void put(Object* o);
        Object*& operator[](int idx) { return elements[idx]; }
    };

    void Vector::put(Object* o)
    {
        if(sz == capacity)
        {
            capacity *= 2;
            Object** newArr = new Object*[capacity];
            for(size_t i = 0; i < sz; i++) newArr[i] = elements[i];
            delete[] elements;
            elements = newArr;
        }
        elements[sz++] = o;
    }

    class List : public Container
    {
        struct Link
        {
            Object* o;
            Link* next;
            Link(Object* o, Link* next) : o(o),next(next) {}
        };
        Link* head;
    public:
        List() : head(0) {}
        ~List() { while(head) { Link* tmp = head; head = head->next; delete tmp; } }
        Object* get() { if(!head) return 0; Object* ret = head->o; Link* tmp = head; head = head->next; delete tmp; return ret; }
        void put(Object* o) { head = new Link(o, head); }
    };

    class Something : public Object
    {
    public:
        int x;
        Something(int xx) : x(xx) {}
    };
}

int main()
{
    using namespace Exercises;
    using namespace std;

    Something o1(0);
    Something o2(1);
    Something o3(2);
    Something o4(3);
    Something o5(4);

    Vector v;
    v.put(&o5); v.put(&o4); v.put(&o3); v.put(&o2); v.put(&o1);
    List l;
    l.put(&o5); l.put(&o4); l.put(&o3); l.put(&o2); l.put(&o1);

    Object* p;
    while(p = v.get())
    {
        Something* s = static_cast<Something*>(p);
        cout << s->x << ' ';
    }
    cout << endl;

    while(p = l.get())
    {
        Something* s = static_cast<Something*>(p);
        cout << s->x << ' ';
    }
    cout << endl;

    return 0;
}
