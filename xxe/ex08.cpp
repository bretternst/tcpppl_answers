#include <iostream>
#include <cstddef>
#include <memory>
#include <vector>
#include <algorithm>
#include <time.h>

// Compile with:
// g++ -o ex08 ex08.cpp -lrt

namespace xxe {
    template<class T, class A = std::allocator<T> >
    struct vector_base {
        A alloc;
        T* v;
        T* space;
        T* last;

        vector_base(const A& a, typename A::size_type n) : alloc(a), v(alloc.allocate(n)), space(v+n), last(v+n) { }
        ~vector_base() { alloc.deallocate(v,last-v); }
    };

    template<class T, class A>
    void swap(vector_base<T,A>& a, vector_base<T,A>& b) {
        std::swap(a.alloc, b.alloc);
        std::swap(a.v, b.v);
        std::swap(a.space, b.space);
        std::swap(a.last, b.last);
    }

    template<class T, class A = std::allocator<T> >
    class vector : private vector_base<T,A> {
        void destroy_elements() { for (T* p = this->v; p != this->space; ++p) p->~T(); this->space = this->v; }
    public:
        typedef typename A::size_type size_type;
        explicit vector(size_type n, const T& val = T(), const A& = A());

        vector(const vector& a);
        vector& operator=(const vector& a);
        ~vector() { destroy_elements(); }
        size_type size() const { return this->space - this->v; }
        size_type capacity() const { return this->last - this->v; }

        T* begin() { return this->v; }
        T* end() { return this->last; }
        const T* begin() const { return this->v; }
        const T* end() const { return this->last; }

        //void push_back(const T&);

        void assign1(const vector& a);
        void assign2(const vector& a);
        void assign3(const vector& a);

        template<class U, class B>
        friend void swap(vector<T,A>& a, vector<T,A>& b) {
            vector_base<T,A>& aa = a;
            vector_base<T,A>& bb = b;
            xxe::swap<vector_base<T,A> >(aa, bb);
        }
    };

    template<class T, class A>
    vector<T,A>::vector(size_type n, const T& val, const A& a) : vector_base<T,A>(a,n) {
        std::uninitialized_fill(this->v, this->v + n, val);
    }

    template<class T, class A>
    vector<T,A>::vector(const vector<T,A>& a) : vector_base<T,A>(a.alloc, a.size()) {
        std::uninitialized_copy(a.begin(), a.end(), this->v);
    }

    template<class T, class A>
    void vector<T,A>::assign1(const vector& a) {
        vector_base<T,A> b(a.alloc, a.size());
        std::uninitialized_copy(a.begin(), a.end(), b.v);
        destroy_elements();
        this->alloc.deallocate(this->v, this->last - this->v);
        vector_base<T,A>::operator=(b);
        b.v = 0;
    }

    template<class T, class A>
    void vector<T,A>::assign2(const vector& a) {
        vector temp(a);
        xxe::swap(*this, temp);
    }

    template<class T, class A>
    void vector<T,A>::assign3(const vector& a) {
        if(capacity() < a.size()) {
            vector temp(a);
            xxe::swap(*this, temp);
            return;
        }
        if(this == &a) return;
        size_type sz = size();
        size_type asz = a.size();
        if(asz <= sz) {
            std::copy(a.begin(), a.begin() + sz, this->v);
            std::uninitialized_copy(a.begin()+sz, a.end(), this->space);
        }
        this->space = this->v + asz;
    }

    template<class T, class A>
    void safe_assign1(std::vector<T,A>& a, const std::vector<T,A>& b) {
        std::vector<T,A> temp(b.get_allocator());
        temp.reserve(b.size());
        for(typename std::vector<T,A>::const_iterator p = b.begin(); p != b.end(); ++p)
            temp.push_back(*p);
        std::swap(a, temp);
    }

    template<class T, class A>
    void safe_assign2(std::vector<T,A>& a, std::vector<T,A> b) {
        std::swap(a, b);
    }
}

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

int main() {
    using namespace xxe;
    using namespace std;
    xxe::vector<int> to(1000, 1);
    xxe::vector<int> from(1000, 2);

    timespec start, end;

    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i < 100000; ++i)
        to.assign1(from);
    clock_gettime(CLOCK_REALTIME, &end);
    cout << "assign1: " << diff(start, end) / 100000 << endl;

    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i < 100000; ++i)
        to.assign2(from);
    clock_gettime(CLOCK_REALTIME, &end);
    cout << "assign2: " << diff(start, end) / 100000 << endl;

    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i < 100000; ++i)
        to.assign3(from);
    clock_gettime(CLOCK_REALTIME, &end);
    cout << "assign3: " << diff(start, end) / 100000 << endl;

    std::vector<int> vto(1000, 1);
    std::vector<int> vfrom(2000, 1);

    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i < 1000; ++i)
        xxe::safe_assign1(vto, vfrom);
    clock_gettime(CLOCK_REALTIME, &end);
    cout << "safe_assign1: " << diff(start, end) / 1000 << endl;

    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i < 1000; ++i)
        xxe::safe_assign2(vto, vfrom);
    clock_gettime(CLOCK_REALTIME, &end);
    cout << "safe_assign2: " << diff(start, end) / 1000 << endl;
}
