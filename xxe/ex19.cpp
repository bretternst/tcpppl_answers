#include <iostream>
#include <cstddef>
#include <memory>
#include <algorithm>
#include <iterator>
#include <vector>

// Compile with:
// g++ -o ex19 ex19.cpp -lrt

// Includes the vector from exercise 10 and a copy of it without any try/catch blocks.
// Since there will be no throwing in assignment or copy, I expect perf to be roughly the same.
// The insert and erase methods are quite a bit simpler, however.
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
        T* end() { return this->space; }
        const T* begin() const { return this->v; }
        const T* end() const { return this->last; }

        void insert(T* p, size_type n, const T& x);
        T* insert(T* p, const T& x) { insert(p, 1, x); return p; }

        T* erase(T* p) { erase(p, p+1); }
        T* erase(T* first, T* last);

        void push_back(const T&);
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
    void vector<T,A>::push_back(const T& x) {
        if(this->space == this->last) {
            vector_base<T,A> b(this->alloc,size() ? 2 * size() : 2);
            b.space = b.v + size();
            std::uninitialized_copy(this->v, this->space, b.v);
            new(b.space) T(x);
            ++b.space;
            destroy_elements();
            xxe::swap(b, *this);
            return;
        }
        new(this->space) T(x);
        ++this->space;
    }

    template<class T, class A>
    void vector<T,A>::insert(T* p, size_type n, const T& x) {
        if(this->last - this->space < n) {
            // strong guarantee here; if anything throws, vector_base doesn't get swapped in
            vector_base<T,A> b(this->alloc, std::max(size() + n, size() * 2));
            std::uninitialized_copy(this->v, p, b.v);
            b.space = b.v + (p - this->v);
            for(int i = 0; i < n; i++) {
                new(b.v + (p - this->v) + i) T(x);
                b.space++;
            }
            if(p < this->space)
                std::uninitialized_copy(p, this->space, b.v + (p - this->v) + n);
            b.space = b.v + size() + n;
            destroy_elements();
            xxe::swap(b, *this);
            return;
        }
        // basic guarantee here
        for(T* q = this->space + n - 1; q >= p + n; --q) {
            try {
                this->alloc.construct(q, *(q - n));
            }
            catch(...) {
                // clean up constructed items and truncate vector to p
                for(T* r = this->space + n - 1; r >= this->space && r > q; --r)
                    this->alloc.destroy(r);
                for(T* r = q - n + 1; r < this->space; ++r)
                    this->alloc.destroy(r);
                this->space = p;
                throw;
            }
            this->alloc.destroy(q - n);
            try {
                this->alloc.construct(q - n, x);
            }
            catch(...) {
                // clean up constructed items and truncate vector to p
                for(T* r = this->space + n - 1; r >= this->space && r >= q; --r)
                    this->alloc.destroy(r);
                for(T* r = q - n; r < this->space; ++r)
                    this->alloc.destroy(r);
                this->space = p;
                throw;
            }
        }
        this->space += n;
    }

    template<class T, class A>
    T* vector<T,A>::erase(T* first, T* last) {
        int n = last - first;
        for(T* p = first; p != last; p++)
            this->alloc.destroy(p);
        for(T* p = first; p + n < this->space; p++) {
            try {
                this->alloc.construct(p, *(p + n));
            }
            catch(...) {
                // clean up constructed items and truncate vector to p
                for(T* r = p + n; r < this->last; r++)
                    this->alloc.destroy(r);
                this->last = p;
                throw;
            }
            this->alloc.destroy(p + n);
        }
        this->space -= n;
        return last;
    }

    template<class T, class A = std::allocator<T> >
    class vector_unsafe : private vector_base<T,A> {
        void destroy_elements() { for (T* p = this->v; p != this->space; ++p) p->~T(); this->space = this->v; }
    public:
        typedef typename A::size_type size_type;
        explicit vector_unsafe(size_type n, const T& val = T(), const A& = A());

        vector_unsafe(const vector_unsafe& a);
        vector_unsafe& operator=(const vector_unsafe& a);
        ~vector_unsafe() { destroy_elements(); }
        size_type size() const { return this->space - this->v; }
        size_type capacity() const { return this->last - this->v; }

        T* begin() { return this->v; }
        T* end() { return this->space; }
        const T* begin() const { return this->v; }
        const T* end() const { return this->last; }

        void insert(T* p, size_type n, const T& x);
        T* insert(T* p, const T& x) { insert(p, 1, x); return p; }

        T* erase(T* p) { erase(p, p+1); }
        T* erase(T* first, T* last);

        void push_back(const T&);
    };

    template<class T, class A>
    vector_unsafe<T,A>::vector_unsafe(size_type n, const T& val, const A& a) : vector_base<T,A>(a,n) {
        std::uninitialized_fill(this->v, this->v + n, val);
    }

    template<class T, class A>
    vector_unsafe<T,A>::vector_unsafe(const vector_unsafe<T,A>& a) : vector_base<T,A>(a.alloc, a.size()) {
        std::uninitialized_copy(a.begin(), a.end(), this->v);
    }

    template<class T, class A>
    void vector_unsafe<T,A>::push_back(const T& x) {
        if(this->space == this->last) {
            vector_base<T,A> b(this->alloc,size() ? 2 * size() : 2);
            b.space = b.v + size();
            std::uninitialized_copy(this->v, this->space, b.v);
            new(b.space) T(x);
            ++b.space;
            destroy_elements();
            xxe::swap(b, *this);
            return;
        }
        new(this->space) T(x);
        ++this->space;
    }

    template<class T, class A>
    void vector_unsafe<T,A>::insert(T* p, size_type n, const T& x) {
        if(this->last - this->space < n) {
            // strong guarantee here; if anything throws, vector_base doesn't get swapped in
            vector_base<T,A> b(this->alloc, std::max(size() + n, size() * 2));
            std::uninitialized_copy(this->v, p, b.v);
            b.space = b.v + (p - this->v);
            for(int i = 0; i < n; i++) {
                new(b.v + (p - this->v) + i) T(x);
                b.space++;
            }
            if(p < this->space)
                std::uninitialized_copy(p, this->space, b.v + (p - this->v) + n);
            b.space = b.v + size() + n;
            destroy_elements();
            xxe::swap(b, *this);
            return;
        }
        // basic guarantee here
        for(T* q = this->space + n - 1; q >= p + n; --q) {
            this->alloc.construct(q, *(q - n));
            this->alloc.destroy(q - n);
            this->alloc.construct(q - n, x);
        }
        this->space += n;
    }

    template<class T, class A>
    T* vector_unsafe<T,A>::erase(T* first, T* last) {
        int n = last - first;
        for(T* p = first; p != last; p++)
            this->alloc.destroy(p);
        for(T* p = first; p + n < this->space; p++) {
            this->alloc.construct(p, *(p + n));
            this->alloc.destroy(p + n);
        }
        this->space -= n;
        return last;
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

template<class T>
unsigned long perftest() {
    timespec start, end;

    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i < 1000000; i++) {
        T v(0);
        for(int i = 0; i <= 5; i++)
            v.push_back(i);

        v.insert(v.begin() + 2, 5, 0);
        v.erase(v.begin() + 2, v.begin() + 7);
        v.insert(v.begin() + 2, 0);
        v.erase(v.begin() + 2);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    return diff(start, end) / 1000000;
}

int main() {
    using namespace xxe;
    using namespace std;

    cout << "'Safe' vector: " << perftest<xxe::vector<int> >() << endl;
    cout << "'Less-safe' vector: " << perftest<xxe::vector_unsafe<int> >() << endl;
    cout << "Standard vector: " << perftest<std::vector<int> >() << endl;
}
