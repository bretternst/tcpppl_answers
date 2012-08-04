#include <iostream>

namespace ch16
{   
    template<class T> class Vector_16_2_1
    {
        static const int DefaultCapacity = 16;
        size_t sz;
        size_t capacity;
        T* elements;
    public:
        explicit Vector_16_2_1() : sz(0), capacity(DefaultCapacity), elements(new T[DefaultCapacity]) {}
        ~Vector_16_2_1() { delete[] elements; }
        void push_back(T x);
        T pop_back() { return elements[--sz]; }
        T& operator[](int idx) { return elements[idx]; }
        size_t size() { return sz; }
    };

    template<class T>
    void Vector_16_2_1<T>::push_back(T x)
    {
        if(sz == capacity)
        {
            capacity *= 2;
            T* newArr = new T[capacity];
            for(size_t i = 0; i < sz; i++) newArr[i] = elements[i];
            delete[] elements;
            elements = newArr;
        }
        elements[sz++] = x;
    }

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
        virtual Object* operator[](size_t) { throw Bad_op("[]"); }
    };

    template<class T> class VectorWrapper : public Container
    {
    public:
        class Holder : public Object
        {
            T x;
        public:
            Holder(T x) : x(x) {}
            T value() { return x; }
        };

        Vector_16_2_1<T>& v;
        VectorWrapper(Vector_16_2_1<T>& v) : v(v) {}

        Object* get() { return v.size() > 0 ? new Holder(v.pop_back()) : 0; }
        void put(Holder* h) {v.push_back(h->value());}

        // Making this writable would have required too much contortion. This is messy enough
        // already.
        Object* operator[](size_t i) { return new Holder(v[i]); }
    };
}

int main()
{
    using namespace std;
    using namespace ch16;

    Vector_16_2_1<int> v;
    VectorWrapper<int> vw(v);
    vw.put(new VectorWrapper<int>::Holder(3));
    vw.put(new VectorWrapper<int>::Holder(2));
    vw.put(new VectorWrapper<int>::Holder(1));

    while(Object* o = vw.get())
    {
        VectorWrapper<int>::Holder* h = static_cast<VectorWrapper<int>::Holder*>(o);
        cout << h->value() << endl;
        delete h;
    }

    return 0;
}
