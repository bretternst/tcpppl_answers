#include <iostream>
#include <cstddef>
#include <memory>

// I'd really love to know what the error is here; I must be missing something stupidly obvious!
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
