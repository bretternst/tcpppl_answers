#include <iostream>
#include <cstddef>
#include <list>
#include <vector>
#include <limits>
#include <iterator>

// This could get really involved; since allocating multiple elements
// at once involves a contiguous block of memory that needs to be
// reserved, rather than single elements of fixed size in the previous
// exercise. For purposes of illustration, I just have the pool
// use the standard new and delete operators; but that's cheating
// since the pool isn't really a pool.
//
// I guess I'm not clear on the purpose of re-implementing (probably
// badly) a malloc-like algorithm since it would have to do similar
// scanning and stuff. It takes away the advantage of using a pool
// in the first place.
//
// So, the below is merely an approximation of what the default STL
// allocator does.
namespace ch19 {
    using std::numeric_limits;

    class Pool {
        const unsigned int esize;

        Pool(Pool&);
        void operator=(Pool&);
        void grow();
    public:
        Pool(size_t sz);
        ~Pool();

        void* alloc(size_t n);
        void free(void* b);
    };

    Pool::Pool(size_t sz) : esize(sz) {
    }

    Pool::~Pool() {
    }

    inline void* Pool::alloc(size_t n) {
        return new char[n * esize];
    }

    inline void Pool::free(void* b) {
        delete[] (char*)b;
    }

    template<class T>
    class Pool_alloc {
    private:
        static Pool mem;
    public:
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef const T* const_pointer;
        typedef const T& const_reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        Pool_alloc() { }
        Pool_alloc(const Pool_alloc&) { }
        template<class U>
        Pool_alloc(const Pool_alloc<U>&) { }

        pointer allocate(size_type n, void* = 0);
        void deallocate(pointer p, size_type n);
        void construct(pointer p, const_reference val);
        void destroy(pointer p);
        size_type max_size() const { return (size_type)numeric_limits<size_type>::max; }

        template<class U>
        struct rebind {
            typedef Pool_alloc<U> other;
        };
    };

    template<class T>
    Pool Pool_alloc<T>::mem(sizeof(T));

    template<class T>
    typename Pool_alloc<T>::pointer Pool_alloc<T>::allocate(size_type n, void *) {
        return static_cast<pointer>(mem.alloc(n));
    }

    template<class T>
    void Pool_alloc<T>::deallocate(pointer p, size_type n) {
        mem.free(p);
    }

    template<class T>
    void Pool_alloc<T>::construct(pointer p, const_reference val) {
        new ((void*)p) T(val);
    }

    template<class T>
    void Pool_alloc<T>::destroy(pointer p) {
        ((T*)p)->~T();
    }
}

int main() {
    using namespace ch19;
    using namespace std;

    list<int, Pool_alloc<int> > l;
    for(int i = 0; i < 50; i++)
        l.push_back(i);

    copy(l.begin(), l.end(), ostream_iterator<int>(cout, " "));
    cout << endl;

    vector<int, Pool_alloc<int> > v;
    v.reserve(50);
    for(int i = 0; i < 50; i++)
        v.push_back(i);

    copy(v.begin(), v.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
}
