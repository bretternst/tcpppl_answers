#include <iostream>
#include <vector>
#include <time.h>

namespace Exercises
{
	class Object {};

	class Container : public Object
	{
	public:
		struct Bad_op
		{
			const char* p;
			Bad_op(const char* pp) : p(pp) {}
		};

		virtual Object* get() { throw Bad_op("get"); }
		virtual void put(Object*) { throw Bad_op("put"); }
		virtual Object*& operator[](size_t) { throw Bad_op("[]"); }
	};

	class Vector_16_2_2 : public Container
	{
		static const int DefaultCapacity = 16;
		size_t sz;
		size_t capacity;
		Object** elements;
	public:
		explicit Vector_16_2_2() : sz(0), capacity(DefaultCapacity), elements(new Object*[DefaultCapacity]) {}
		~Vector_16_2_2() { delete[] elements; }
		Object* get() { return sz > 0 ? elements[--sz] : 0; }
		void put(Object* o);
		int size() { return sz; }
		Object*& operator[](int idx) { return elements[idx]; }
	};

	void Vector_16_2_2::put(Object* o)
	{
		if(sz == capacity)
		{
			capacity *= 2;
			Object** newArr = new Object*[capacity];
			for(size_t i = 0; i < sz; i++) newArr[i] = elements[i];
			delete[] elements;
			elements = newArr;
		}
		elements[sz++] = o;
	}

	class IntHolder : public Object
	{
	public:
		int x;
		IntHolder() : x(0) {}
		IntHolder(int xx) : x(xx) {}
		int value() { return x; }
	};

	template<class T> class Itor : public Object
	{
		int i;
		Vector_16_2_2& c;
	public:
		Itor(Vector_16_2_2& c) : c(c) {}
		T* first() { return c.size() ? static_cast<T*>(c[i = 0]) : 0; }
		T* next() { if(i >= c.size()-1) return 0; Object* o = c[++i]; return static_cast<T*>(o); }
 	};
}

int main()
{
	using namespace Exercises;
	using namespace std;

	IntHolder numbers[3];
	numbers[0] = IntHolder(1);
	numbers[1] = IntHolder(2);
	numbers[2] = IntHolder(3);

	Vector_16_2_2 v;
	v.put(&numbers[0]);
	v.put(&numbers[1]);
	v.put(&numbers[2]);

	Itor<IntHolder> i(v);
	IntHolder* ih = i.first();
	while(ih)
	{
		cout << ih->value() << endl;
		ih = i.next();
	}

	return 0;
}
