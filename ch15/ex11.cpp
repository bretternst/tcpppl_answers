#include <iostream>

namespace Exercises
{
	using std::type_info;

	class DynamicBase
	{
		template<class Tto> friend Tto* DynamicCast(DynamicBase*);
		template<class Tto> friend Tto& DynamicCast(DynamicBase&);

		class TypeNode
		{
		public:
			void* p;
			const type_info& t;
			TypeNode* next;

			TypeNode(void* p, const type_info& t, TypeNode* next) : p(p), t(t), next(next) {}
			~TypeNode() { if(next) delete next; }
		};
		TypeNode* typeList;
	protected:
		void RegisterType(DynamicBase* o, void* p)
		{
			TypeNode* node = new TypeNode(p, typeid(*o), typeList);
			typeList = node;
		}
		DynamicBase() { typeList = new TypeNode(this, typeid(*this),0); }
		virtual ~DynamicBase() { delete typeList; }
	public:
		void PrintType() const
		{
			TypeNode* n = typeList;
			while(n)
			{
				std::cout << n->t.name() << std::endl;
				n = n->next;
			}
		}
	};

	template<class Tto>
	Tto* DynamicCast(DynamicBase* o)
	{
		if(!o) return 0;

		DynamicBase::TypeNode* n = o->typeList;
		while(n)
		{
			if(typeid(Tto) == n->t)
			{
				return reinterpret_cast<Tto*>(n->p);
			}
			n = n->next;
		}
		return 0;
	}

	template<class Tto>
	Tto& DynamicCast(DynamicBase& o)
	{
		DynamicBase::TypeNode* n = o.typeList;
		while(n)
		{
			if(typeid(Tto) == n->t)
			{
				return *(reinterpret_cast<Tto*>(n->p));
			}
			n = n->next;
		}
		throw std::bad_cast();
	}

	class A : public virtual DynamicBase
	{
	public:
		int a;
		A() { a = 1; RegisterType(this, this); }
	};

	class B : public virtual DynamicBase
	{
	public:
		int b;
		B() { b = 2; RegisterType(this, this); }
	};

	class C : public A, public B
	{
	public:
		int c;
		C() { c = 3; RegisterType(this, this); }
	};
}

int main()
{
	using namespace std;
	using namespace Exercises;

	C* c = new C();

	A* a = DynamicCast<A>(c);
	cout << typeid(*a).name() << endl;
	C* c2 = DynamicCast<C>(a);
	cout << typeid(*c2).name() << ',' << c2->a << ',' << c2->b << ',' << c2->c << endl;

	B* b = new B();
	A* a2 = DynamicCast<A>(b);
	if(!a2) cout << "got null as expected" << endl;

	C& cRef = *c;
	A& aRef = DynamicCast<A>(cRef);
	cout << aRef.a << endl; // should print 1
	C& cRef2 = DynamicCast<C>(aRef);
	cout << cRef2.c << endl; // should print 3

	try
	{
		B b2;
		A& aRef2 = DynamicCast<A>(b2);
	}
	catch(bad_cast)
	{
		cout << "caught bad_cast" << endl;
	}

	delete c;
	delete b;

	return 0;
}
