#include <iostream>

namespace ch16
{
    // This exercise is really starting to lose me. I'm going to implement only "common" functionality
    // for "standard containers" which means mainly just a begin() and end() accessor for iterators.
    // The goal is to have the same loop process both kinds of vectors to print out its elements.
    // I don't even support [] because that is not an operation common to all "standard containers."

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

    class Vector_16_2_2 : public Container
    {
        static const int DefaultCapacity = 16;
        size_t sz;
        size_t capacity;
        Object** elements;
    public:
        explicit Vector_16_2_2() : sz(0), capacity(DefaultCapacity), elements(new Object*[DefaultCapacity]) {}
        ~Vector_16_2_2() { delete[] elements; }
        Object* get() { return sz > 0 ? elements[--sz] : 0; }
        void put(Object* o);
        int size() { return sz; }
        Object*& operator[](int idx) { return elements[idx]; }
    };

    void Vector_16_2_2::put(Object* o)
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

    template<class T> class Vector_16_2_1
    {
        T* elements;
        size_t sz;
    public:
        explicit Vector_16_2_1(size_t n) : sz(n), elements(new T[n]) {}
        ~Vector_16_2_1() { delete[] elements; }
        T& operator[](size_t i) { return elements[i]; }
        size_t size() { return sz; }
    };

    template<class T> class Vector_16_2_1_wrapper
    {
        Vector_16_2_1<T>& v;
    public:
        typedef T* iterator;

        Vector_16_2_1_wrapper(Vector_16_2_1<T>& v) : v(v) {}
        iterator begin() { return &v[0]; }
        iterator end() { return &v[v.size()]; }     
    };

    template<class T> class Vector_16_2_2_wrapper
    {
        Vector_16_2_2& v;
    public:
        class Holder : public Object
        {
            T x;
        public:
            Holder(T x) : x(x) {}
            T& value() { return x; }
        };

        class Itor
        {
            Vector_16_2_2& v;
            int i;
        public:
            Itor(Vector_16_2_2& v, int i) : v(v), i(i) {}
            bool operator==(Itor& x) { return x.i == i; }
            bool operator!=(Itor& x) { return !(x==*this); }
            Itor& operator++(int) { i++; return *this; }
            Itor& operator--(int) { i--; return *this; }
            T& operator*() { Object* o = v[i]; Holder* h = static_cast<Holder*>(o); return h->value(); }
        };

        typedef Itor iterator;
        Vector_16_2_2_wrapper(Vector_16_2_2& v) : v(v) {}
        Itor begin() { return Itor(v, 0); }
        Itor end() { return Itor(v, v.size()); }
    };
}

int main()
{
    using namespace ch16;
    using namespace std;

    Vector_16_2_1<int> v(3);
    v[0] = 1;
    v[1] = 2;
    v[2] = 3;
    Vector_16_2_1_wrapper<int> vw(v);

    for(Vector_16_2_1_wrapper<int>::iterator i = vw.begin(); i != vw.end(); i++)
    {
        cout << *i << endl;
    }

    Vector_16_2_2 v2;
    v2.put(new Vector_16_2_2_wrapper<int>::Holder(1));
    v2.put(new Vector_16_2_2_wrapper<int>::Holder(2));
    v2.put(new Vector_16_2_2_wrapper<int>::Holder(3));
    Vector_16_2_2_wrapper<int> vw2(v2);

    for(Vector_16_2_2_wrapper<int>::iterator i = vw2.begin(); i != vw2.end(); i++)
    {
        cout << *i << endl;
    }

    return 0;
}
