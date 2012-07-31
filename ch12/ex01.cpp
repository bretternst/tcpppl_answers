#include <iostream>

namespace Exercises
{
	using std::cout;
	using std::endl;

	class Base
	{
	public:
		virtual void iam() { cout << "Base" << endl; }
	};

	class Derived1 : public Base
	{
	public:
		void iam() { cout << "Derived1" << endl; }
	};

	class Derived2 : public Base
	{
	public:
		void iam() { cout << "Derived2" << endl; }
	};
}

void main()
{
	using namespace std;
	using namespace Exercises;

	Base b;
	Derived1 d1;
	Derived2 d2;

	b.iam();
	d1.iam();
	d2.iam();

	Base* p;
	p = &b; p->iam();
	p = &d1; p->iam();
	p = &d2; p->iam();
}
