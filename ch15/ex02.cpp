#include <iostream>

namespace Exercises
{
	using std::cout;
	using std::endl;

	class A
	{
	public:
		A() { cout << "constructing " << typeid(*this).name() << endl; }
		virtual ~A() { cout << "destructing " << typeid(*this).name() << endl; }
	};

	class B1 : virtual public A
	{
	public:
		B1() { cout << "constructing " << typeid(*this).name() << endl; }
		virtual ~B1() { cout << "destructing " << typeid(*this).name() << endl; }
	};

	class C1 : virtual public A
	{
	public:
		C1() { cout << "constructing " << typeid(*this).name() << endl; }
		virtual ~C1() { cout << "destructing " << typeid(*this).name() << endl; }
	};

	class D1 : public B1, public C1
	{
	public:
		D1() { cout << "constructing " << typeid(*this).name() << endl; }
		virtual ~D1() { cout << "destructing " << typeid(*this).name() << endl; }
	};

	class B2 : public A
	{
	public:
		B2() { cout << "constructing " << typeid(*this).name() << endl; }
		virtual ~B2() { cout << "destructing " << typeid(*this).name() << endl; }
	};

	class C2 : public A
	{
	public:
		C2() { cout << "constructing " << typeid(*this).name() << endl; }
		virtual ~C2() { cout << "destructing " << typeid(*this).name() << endl; }
	};

	class D2 : public B2, public C2
	{
	public:
		D2() { cout << "constructing " << typeid(*this).name() << endl; }
		virtual ~D2() { cout << "destructing " << typeid(*this).name() << endl; }
	};
}

int main()
{
	using namespace std;
	using namespace Exercises;

	cout << "creating an object of type D1..." << endl;
	A* d1 = new D1;
	cout << endl;

	cout << "creating an object of type D2..." << endl;
	D2* d2 = new D2;
	cout << endl;

	cout << "deleting an object of type D1..." << endl;
	delete d1;
	cout << endl;

	cout << "deleting an object of type D2..." << endl;
	delete d2;
	cout << endl;

	return 0;
}
