#include <iostream>

// The class generated for List<int> is functionally identical to
// List_int. They are also invoked in an identical way when viewing
// the disassembly in main().
namespace Exercises
{
	using std::cout;
	using std::endl;

	template<class T> class List
	{
		struct Link
		{
			Link* pre;
			Link* suc;
			T val;
			Link(Link* p, Link* s, const T& v) : pre(p), suc(s), val(v) { }
		};
		
		Link* head;
	
	public:
		List() : head(0) {}
		List(const T& t) : head(new Link(0,0,t)) {}
		void print_all() const { for(Link* p = head; p; p=p->suc) cout << p->val << endl; }
	};

	// This is what the compiler generates for List<int> below:
	class List_int
	{
		struct Link
		{
			Link* pre;
			Link* suc;
			int val;
			Link(Link* p, Link* s, const int& v) : pre(p), suc(s), val(v) { }
		};
		
		Link* head;
	
	public:
		List_int() : head(0) {}
		List_int(const int& t) : head(new Link(0,0,t)) {}
		void print_all() const { for(Link* p = head; p; p=p->suc) cout << p->val << endl; }
	};
}

int main()
{
	using namespace std;
	using namespace Exercises;

	List<int> i1(5);
	i1.print_all();

	List_int i2(5);
	i2.print_all();

	return 0;
}
