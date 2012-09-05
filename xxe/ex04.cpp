#include <iostream>
#include <cstddef>
#include <memory>

// This exercise doesn't seem very useful. There are a few ways to make it "crash" - e.g. by
// allocating tons of huge elements and running out of memory. But surely that's not what the
// author meant. I also assume the author isn't talking about destructors throwing exceptions
// or bogus values supplied to the constructor (like zero) or allocator copy ctors throwing.
// All of these things could make it "crash" if unhandled, but those aren't errors in the ctor.
//
// If there is an error in the constructor, I can't find it.
namespace xxe {
    template<class T, class A = std::allocator<T> >
    class vector {
    private:
        T* v;
        T* space;
        T* last;
        A alloc;

        void destroy_all();
    public:
        typedef size_t size_type;

        explicit vector(size_type n, const T& val = T(), const A& = A());
        vector(const vector& a);
        vector& operator=(const vector& a);
        ~vector() { destroy_all(); alloc.deallocate(v, last-v); }
        size_type size() const { return space-v; }
        size_type capacity() const { return last-v; }
        void push_back(const T&);
    };

    template<class T, class A>
    void vector<T,A>::destroy_all() {
        for(T* p = v; p != last; p++)
            alloc.destroy(p);
    }

    template<class T, class A>
    vector<T,A>::vector(size_type n, const T& val, const A& a) : alloc(a) {
        v = alloc.allocate(n);
        try {
            std::uninitialized_fill(v, v + n, val);
            space = last = v + n;
        }
        catch(...) {
            alloc.deallocate(v, n);
            throw;
        }
    }

    class Test_error { };
    struct Test {
    };
}

int main() {
    using namespace xxe;
    try {
        vector<Test> v(10);
    }
    catch(...) {
        Test_error();
    }
}
