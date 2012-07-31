#include <iostream>

namespace Exercises
{
	template<class T> class Ptr_to_T
	{
		T* p;
		T* array;
		int size;
	public:
		class NotAnArrayError {};
		class OutOfRangeError {};
		class NullError {};

		Ptr_to_T(T* p, T* v, int s) : p(p), array(v), size(s) {}; 
		Ptr_to_T(T* p) : p(p), array(0), size(0) {};

		Ptr_to_T& operator++();
		Ptr_to_T operator++(int);

		Ptr_to_T& operator--();
		Ptr_to_T operator--(int);

		T& operator*();
	};

	template<class T>
	Ptr_to_T<T>& Ptr_to_T<T>::operator++()
	{
		if(!array) throw NotAnArrayError();
		p++;
		return *this;
	}

	template<class T>
	Ptr_to_T<T> Ptr_to_T<T>::operator++(int)
	{
		Ptr_to_T<T> copy = *this;
		++*this;
		return copy;
	}

	template<class T>
	Ptr_to_T<T>& Ptr_to_T<T>::operator--()
	{
		if(!array) throw NotAnArrayError();
		p--;
		return *this;
	}

	template<class T>
	Ptr_to_T<T> Ptr_to_T<T>::operator--(int)
	{
		Ptr_to_T<T> copy = *this;
		--*this;
		return copy;
	}

	template<class T>
	T& Ptr_to_T<T>::operator*()
	{
		if(!p) throw NullError();
		if(p >= array + size || p < array) throw OutOfRangeError();
		return *p;
	}
}

int main()
{
	using namespace std;
	using namespace Exercises;

	// test null error
	Ptr_to_T<int> t1(0);
	try
	{
		cout << *t1 << endl;
	}
	catch(Ptr_to_T<int>::NullError)
	{
		cout << "caught null error" << endl;
	}

	// test out of range error
	int x[2] = { 0, 1 };
	Ptr_to_T<int> t2(x, x, 2);
	t2++;
	t2++;
	try
	{
		cout << *t2 << endl;
	}
	catch(Ptr_to_T<int>::OutOfRangeError)
	{
		cout << "caught out of range error (1)" << endl;
	}

	// test out of range error 2
	t2--;
	t2--;
	t2--;
	try
	{
		cout << *t2 << endl;
	}
	catch(Ptr_to_T<int>::OutOfRangeError)
	{
		cout << "caught out of range error (2)" << endl;
	}

	// test not an array error
	try
	{
		t1++;
	}
	catch(Ptr_to_T<int>::NotAnArrayError)
	{
		cout << "caught not an array error" << endl;
	}

	// test normal condition
	t2++;
	t2++;
	cout << *t2 << endl; // should print 1

	return 0;
}
