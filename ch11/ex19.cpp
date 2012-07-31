#include <iostream>

namespace Exercises
{
	class T
	{
	public:
		int v;
		T() : v(0) {}
		T(int x) : v(x) {}
		operator int() {return v;}
	};

	class Ptr_to_T
	{
		T* p;
		T* array;
		int size;

		void Check() const;
	public:
		class InvalidPointerError{};

		Ptr_to_T(T* p, T* v, int s);
		Ptr_to_T(T* p);

		void operator=(const Ptr_to_T& other);
		Ptr_to_T& operator++();
		Ptr_to_T operator++(int);
		Ptr_to_T& operator--();
		Ptr_to_T operator--(int);
		T& operator*() const;
		T* operator->() const;
	};

	Ptr_to_T::Ptr_to_T(T* p, T* v, int s) : p(p), array(v), size(s)
	{
	}

	Ptr_to_T::Ptr_to_T(T* p) : p(p), array(p), size(1)
	{
	}

	void Ptr_to_T::Check() const
	{
		if(p < array || p >= array + size)
			throw InvalidPointerError();
	}

	void Ptr_to_T::operator=(const Ptr_to_T& other)
	{
		p = other.p;
		array = other.array;
		size = other.size;
	}

	Ptr_to_T& Ptr_to_T::operator++()
	{
		p++;
		return *this;
	}

	Ptr_to_T Ptr_to_T::operator++(int)
	{
		Ptr_to_T result = *this;
		p++;
		return result;
	}

	Ptr_to_T& Ptr_to_T::operator--()
	{
		p--;
		return *this;
	}

	Ptr_to_T Ptr_to_T::operator--(int)
	{
		Ptr_to_T result = *this;
		p--;
		return result;
	}

	T& Ptr_to_T::operator*() const
	{
		Check();
		return *p;
	}

	T* Ptr_to_T::operator->() const
	{
		Check();
		return p;
	}
}

int main()
{
	using namespace Exercises;
	using namespace std;

	T x(1);
	T y(2);

	T v[200];
	Ptr_to_T t(&v[0], v, 200);
	t--;
	try
	{
		x = *t;
	}
	catch(Ptr_to_T::InvalidPointerError)
	{
		cerr << "Caught exception!" << endl;
	}
	t++;
	*t = x;
	*(++t) = y;
	*(++t) = x;

	x = *(t--);
	y = *(--t);
	cout << x << ' ' << y << ' ' << t->v << endl; // expect: 1 1 1

	return 0;
}
