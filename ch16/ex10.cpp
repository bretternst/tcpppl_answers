#include <iostream>
#include <vector>
#include <time.h>
#include <cstdlib>

// Compile with:
// g++ -o ex10 ex10.cpp -lrt
// Not too surprisingly, the 16.2.2 implementation had the worst for-loop
// performance, probably because of the overhead and indirection involved
// in casting and polymorphism.
//
// Interestingly, 16.2.1 outperformed standard vector when accessed with
// both a for-loop and an iterator. This may be due to the fact that the
// implementation is very simplistic. It is still a surprising result,
// given the virtualized calls to the iterator.
//
// The 16.2.2 implementation used by far the most memory because of the
// necessity of the Object base.
namespace ch16_2_1 {
    template<class T> class Vector
    {
        T* elements;
        size_t sz;
    public:
        explicit Vector(size_t n) : sz(n), elements(new T[n]) {}
        ~Vector() { delete[] elements; }
        T& operator[](size_t i) { return elements[i]; }
        size_t size() { return sz; }

        int get_mem_usage()
        {
            return sizeof(Vector) + sz * sizeof(T);
        }
    };

    template<class T> class Itor
    {
    public:
        virtual T* first() = 0;
        virtual T* next() = 0;
    };

    template<class T> class Vector_itor : public Itor<T>
    {
        Vector<T>& v;
        size_t index;
    public:
        Vector_itor(Vector<T>& vv) : v(vv), index(0) {}
        T* first() { return v.size() ? &v[index=0] : 0; }
        T* next() { return ++index<v.size() ? &v[index] : 0; }
    };
}

namespace ch16_2_2 {
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
        int size() { return sz; }
        Object*& operator[](int idx) { return elements[idx]; }

        int get_mem_usage()
        {
            return sizeof(Vector) + sizeof(Object*) * sz;
        }
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

    class IntHolder : public Object
    {
    public:
        int x;
        IntHolder() : x(0) {}
        IntHolder(int xx) : x(xx) {}
    };
}

// technique found on stackoverflow; for the short times
// we're using here, I'm not concerned about overflow
// of the final result.
unsigned long diff(timespec start, timespec end) {
    timespec tmp;
    if((end.tv_nsec-start.tv_nsec)<0) {
        tmp.tv_sec = end.tv_sec-start.tv_sec-1;
        tmp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
        tmp.tv_sec = end.tv_sec-start.tv_sec;
        tmp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return tmp.tv_sec * 1000000000 + tmp.tv_nsec;
}

int main()
{
    using namespace std;

    const int NumberOfElements = 10000;

    timespec start,end;
    vector<int> v;
    ch16_2_1::Vector<int> v1621(NumberOfElements);
    ch16_2_2::Vector v1622;

    ch16_2_2::IntHolder* numbersFor1622;
    numbersFor1622 = new ch16_2_2::IntHolder[NumberOfElements];

    for(int i = 0; i < NumberOfElements; i++)
    {
        int n = rand() % 1024;
        v.push_back(n);
        v1621[i] = n;
        numbersFor1622[i] = ch16_2_2::IntHolder(n);
        v1622.put(&numbersFor1622[i]);
    }

    cout << "std::vector" << endl;
    double num = 0;
    clock_gettime(CLOCK_REALTIME, &start);
    for(vector<int>::const_iterator i = v.begin(); i != v.end(); i++) {
        num += *i;
    }
    num /= v.size();
    clock_gettime(CLOCK_REALTIME, &end);
    cout << "iterator time: " << diff(start,end) << " mean = " << num << endl;
    num = 0;
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i < v.size(); i++)
    {
        num += v[i];
    }
    num /= v.size();
    clock_gettime(CLOCK_REALTIME, &end);
    cout << "for-loop time: " << diff(start,end) << " mean = " << num << endl;
    cout << "memory used (est): " << sizeof(std::vector<int>) + sizeof(int)*v.size() << endl; // may not represent the full size.
    cout << endl;

    cout << "Vector from 16.2.1" << endl;
    num = 0;
    clock_gettime(CLOCK_REALTIME, &start);
    ch16_2_1::Vector_itor<int> i1621(v1621);
    num = v1621[0];
    int* next;
    while(next = i1621.next())
        num += *next;
    num /= v.size();
    clock_gettime(CLOCK_REALTIME, &end);
    cout << "iterator time: " << diff(start,end) << " mean = " << num << endl;
    num = 0;
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i < v1621.size(); i++)
    {
        num += v1621[i];
    }
    num /= v1621.size();
    clock_gettime(CLOCK_REALTIME, &end);
    cout << "for-loop time: " << diff(start,end) << " mean = " << num << endl;
    cout << "memory used: " << v1621.get_mem_usage() << endl;
    cout << endl;

    cout << "Vector from 16.2.2" << endl;
    num = 0;
    int mem = v1622.get_mem_usage() + v.size()*sizeof(ch16_2_2::IntHolder);
    clock_gettime(CLOCK_REALTIME, &start);
    while(ch16_2_2::Object* o = v1622.get()) {
        ch16_2_2::IntHolder* i = static_cast<ch16_2_2::IntHolder*>(o);
        num += i -> x;
    }
    num /= v.size();
    clock_gettime(CLOCK_REALTIME, &end);
    cout << "for-loop time: " << diff(start,end) << " mean = " << num << endl;
    cout << "memory used: " << mem << endl;
    return 0;
}
