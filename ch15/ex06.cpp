#include <iostream>
#include <cstdlib>

namespace ch15
{
    class Arena
    {
    public:
        virtual void* alloc(size_t sz) = 0;
        virtual void free(void*) = 0;

        template<class T> void destroy(T* o)
        {
            o->~T();
            free(o);
        }
    };

    // a very simplistic arena that just wraps the C malloc and free functions,
    // meant simply to illustrate custom allocation.
    class CArena : public Arena
    {
    public:
        void* alloc(size_t sz)
        {
            return malloc(sz);
        }

        void free(void* p)
        {
            ::free(p);
        }
    };

    class A
    {
        int m_x;
    public:
        A() : m_x(0) {}
        A(int x) : m_x(x) {}

        A* clone(Arena* a)
        {
            return new(a) A(*this);
        }

        void* operator new(size_t, Arena* a)
        {
            return a->alloc(sizeof(A));
        }

        int x() { return m_x; }
    };
}

int main()
{
    using namespace std;
    using namespace ch15;

    CArena arena;

    A a(5);
    A* a1 = a.clone(&arena);
    cout << a1->x() << endl; // should print 5
    arena.destroy(a1);

    return 0;
}
