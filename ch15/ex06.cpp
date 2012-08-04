#include <iostream>

namespace Exercises
{
    class Arena
    {
    public:
        virtual void* Alloc(size_t sz) = 0;
        virtual void Free(void*) = 0;

        template<class T> void Destroy(T* o)
        {
            o->~T();
            Free(o);
        }
    };

    // a very simplistic arena that just wraps the C malloc and free functions, meant
    // simply to illustrate custom allocation.
    class CArena : public Arena
    {
    public:
        void* Alloc(size_t sz)
        {
            return malloc(sz);
        }

        void Free(void* p)
        {
            free(p);
        }
    };

    class A
    {
        int x;
    public:
        A() : x(0) {}
        A(int x) : x(x) {}

        A* Clone(Arena* a)
        {
            return new(a) A(*this);
        }

        void* operator new(size_t, Arena* a)
        {
            return a->Alloc(sizeof(A));
        }

        int X() { return x; }
    };
}

int main()
{
    using namespace std;
    using namespace Exercises;

    CArena arena;

    A a(5);
    A* a1 = a.Clone(&arena);
    cout << a1->X() << endl; // should print 5
    arena.Destroy(a1);

    return 0;
}
