#include <iostream>

namespace Exercises
{
	// I already implemented a complete Vector class in the previous chapter and don't
	// really feel like doing the same thing agian, so I will only cover the functions
	// explicitly mentioned in 16.2.1.
	template<class T> class Vector
	{
		T* elements;
		size_t sz;
	public:
		explicit Vector(size_t n) : sz(n), elements(new T[n]) {}
		~Vector() { delete[] elements; }
		T& operator[](size_t i) { return elements[i]; }
		size_t size() { return sz; }
	};

	template<class T> class List
	{
	public:
		class Link
		{
		public:
			T* x;
			Link* next;
			Link(T* x, Link* next) : x(x), next(next) {}
			~Link() { delete next; }
		};
		Link* head;

		List() : head(0) {}
		~List() { delete head; }
		void put(T* x) { head = new Link(x, head); }
		T* get() { return head; }
	};

	template<class T> class Itor
	{
	public:
		virtual T* first() = 0;
		virtual T* next() = 0;
	};

	template<class T> class Vector_itor : public Itor<T>
	{
		Vector<T>& v;
		size_t index;
	public:
		Vector_itor(Vector<T>& vv) : v(vv), index(0) {}
		T* first() { return v.size() ? &v[index=0] : 0; }
		T* next() { return ++index<v.size() ? &v[index] : 0; }
	};

	template<class T> class List_itor : public Itor<T>
	{
		List<T>& lst;
		typename List<T>::Link* p;
	public:
		List_itor(List<T>& l) : lst(l), p(lst.head) {}
		T* first() { p = lst.head; return p ? p->x : 0; }
		T* next() { p = p->next; return p ? p->x : 0; }
	};
}

int main()
{
	using namespace Exercises;
	using namespace std;

	Vector<int> v(5);
	v[0] = 0; v[1] = 1; v[2] = 2; v[3] = 3; v[4] = 4;
	List<int> l;
	l.put(&v[4]); l.put(&v[3]); l.put(&v[2]); l.put(&v[1]); l.put(&v[0]);

	Vector_itor<int> vi(v);
	int* p = vi.first();
	while(p)
	{
		cout << *p << ' ';
		p = vi.next();
	}
	cout << endl;

	List_itor<int> li(l);
	p = li.first();
	while(p)
	{
		cout << *p << ' ';
		p = li.next();
	}
	cout << endl;

	return 0;
}
