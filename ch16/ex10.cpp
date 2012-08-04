#include <iostream>
#include <vector>
#include <time.h>

// std::vector was consistently the slowest implementation. However, I strongly suspect that
// this is simply becuase it is not as simplistic as the vectors described in chapter 16.
// The 16.2.1 version was the fastest, which is not surprising since it doesn't have the
// overhead of 16.2.2 which requires casts and the wrapping of native types inside an
// Object-derived object. However, 16.2.2 was on average only slightly slower than the
// 16.2.1 implementation.

// Not surprisingly, vector used a few more bytes (40024) probably do to increased housekeeping
// since it is a more complete implementation. The vector from 16.2.1 used 40008 and the
// vector from 16.2.2 used twice as much: 80016. The vector from 16.2.2 is the loser because
// of the extra level of indirection: storing pointers to Object instead of storing an Int
// directly.

namespace ch16
{
    template<class T> class Vector_16_2_1
    {
        T* elements;
        size_t sz;
    public:
        explicit Vector_16_2_1(size_t n) : sz(n), elements(new T[n]) {}
        ~Vector_16_2_1() { delete[] elements; }
        T& operator[](size_t i) { return elements[i]; }
        size_t size() { return sz; }

        int GetMemUsage()
        {
            return sizeof(Vector_16_2_1) + sz * sizeof(T);
        }
    };

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

        int GetMemUsage()
        {
            return sizeof(Vector_16_2_2) + sizeof(Object*) * sz;
        }
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

    class IntHolder : public Object
    {
    public:
        int x;
        IntHolder() : x(0) {}
        IntHolder(int xx) : x(xx) {}
    };
}

int main()
{
    using namespace ch16;
    using namespace std;

    const int NumberOfElements = 10000;

    vector<int> v;
    Vector_16_2_1<int> v1621(NumberOfElements);
    Vector_16_2_2 v1622;

    IntHolder* numbersFor1622;
    numbersFor1622 = new IntHolder[NumberOfElements];

    for(int i = 0; i < NumberOfElements; i++)
    {
        int n = rand() % 1024;
        v.push_back(n);
        v1621[i] = n;
        numbersFor1622[i] = IntHolder(n);
        v1622.put(&numbersFor1622[i]);
    }

    double num = 0;
    int start = clock();
    for(int i = 0; i < v.size(); i++)
    {
        num += v[i];
    }
    num /= v.size();
    cout << "mean for std::vector: " << num << " time: " << clock()-start << endl;
    cout << "memory used: " << sizeof(std::vector<int>) + sizeof(int)*v.size() << endl; // may not represent the full size.

    num = 0;
    start = clock();
    for(int i = 0; i < v1621.size(); i++)
    {
        num += v1621[i];
    }
    num /= v1621.size();
    cout << "mean for Vector_16_2_1: " << num << " time: " << clock()-start << endl;
    cout << "memory used: " << v1621.GetMemUsage() << endl;

    num = 0;
    int mem = v1622.GetMemUsage() + v.size()*sizeof(IntHolder);
    start = clock();
    while(Object* o = v1622.get())
    {
        IntHolder* i = static_cast<IntHolder*>(o);
        num += i->x;
    }
    num /= v.size();
    cout << "mean for Vector_16_2_2: " << num << " time: " << clock()-start << endl;
    cout << "memory used: " << mem << endl;
    return 0;
}
