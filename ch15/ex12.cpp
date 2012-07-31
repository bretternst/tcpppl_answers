#include <iostream>

namespace Exercises
{
	class A
	{
	public:
		int a;
		A() : a(1) {}
		virtual void DoSomething(A* o) { std::cout << "A::DoSomething called" << std::endl; o->a = 10; }
	};

	class B : virtual public A
	{
	public:
		int b;
		B() : b(2) {}

		// we'll pretend that this method actually overrides A::DoSomething. We simulate this
		// using reinterpret_cast in main().
		void DoSomething(B* o) { std::cout << "B::DoSomething called" << std::endl; o->b = 15; }
	};
}

int main()
{
	using namespace Exercises;
	using namespace std;

	B* b = new B();
	A* a = b;

	// Pretend we called A::DoSomething() virtually - which would supply a pointer to A*.
	// The pointer would not be adjusted to work for B* because the necessary shift would 
	// not be known at compile time.
	b->DoSomething(reinterpret_cast<B*>(a));

	// Corrupted! This should output 1 but outputs 10 instead in MSVC. This would be implementation-
	// specific as it depends on the layout of the objects.
	cout << a->a;

	return 0;

	// In order for this kind of thing to work, there would have to be an extra layer of information
	// in the vtable to indicate what arguments each overloaded function takes and then perform a
	// dynamic_cast<> to modify the pointer accordingly. However, this would add significant runtime
	// overhead, both in terms of performance and space. As is, virtual functions are extremely fast
	// because they only require one level of indirection while retrieving the function pointer from
	// the vtable. If the argument types cannot be assumed to be consistent, this would need to be
	// corrected.
}
