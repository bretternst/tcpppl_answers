#include <iostream>

namespace ch15
{
    class A
    {
    public:
        int a;
        A() : a(1) {}
        virtual void do_something(A* o) {
            std::cout << "A::DoSomething called" << std::endl;
        }
    };

    class B : virtual public A
    {
    public:
        int b;
        B() : b(2) {}

        // Since the argument differs, this does not actually override
        // A::do_something. However, in main, we'll use a cast to act
        // as if it did.
        void do_something(B* o) {
            std::cout << "B::DoSomething called" << std::endl; o->b = 15;
        }
    };
}

int main()
{
    using namespace ch15;
    using namespace std;

    B* b = new B();
    A* a = b;

    // Pretend we called A::do_something() virtually - which would supply a
    // pointer to A.  The pointer would not be adjusted to work for B because
    // the necessary shift would not be known at compile time.
    b->do_something(reinterpret_cast<B*>(a));

    // Corrupted! This should output 1 but outputs 15 instead in GCC. This
    // would be implementation-specific as it depends on the layout of the
    // objects in memory.
    // Notice how the field "a" is only ever assigned the value 1. Oops.
    cout << a->a << endl;

    return 0;

    // In order for this kind of thing to work, there would have to be an extra
    // layer of information in the vtable to indicate what arguments each
    // overloaded function takes and then perform a dynamic_cast<> to modify
    // the pointer accordingly. However, this would add significant runtime
    // overhead, both in terms of performance and space. As is, virtual
    // functions are extremely fast because they only require one level of
    // indirection while retrieving the function pointer from the vtable. If
    // the argument types cannot be assumed to be consistent, this would need
    // to be corrected.
}
