#include <iostream>

namespace Exercises
{
	template<class T>
	class List
	{
		class Node
		{
		public:
			Node* next;
			Node* prev;
			T* item;
			Node(T* i, Node* n, Node* p) : item(i), next(n), prev(p) {}
			~Node() { delete item; }
		};

		Node* head;
		Node* tail;

	public:
		class UnderflowError {};

		List() : head(0), tail(0) {}
		~List() { Node* n = head; while(n) { Node* d = n; n = n->next; delete d; } }

		void push_front(T* x) { head = new Node(x,head,0); if(head->next) head->next->prev = head; if(!tail) tail=head; }
		void push_back(T* x) { tail = new Node(x,0,tail); if(tail->prev) tail->prev->next = tail; if(!head) head=tail; }
		void pop_front() { if(!head) throw UnderflowError(); Node* n = head; head = head->next; if(head) head->prev = 0; else tail = 0; delete n; }
		void pop_back() { if(!tail) throw UnderflowError(); Node* n = tail; tail = tail->prev; if(tail) tail->next = 0; else head = 0; delete n; }
		T* front() { if(head) return head->item; else throw UnderflowError(); }
		T* back() { if(tail) return tail->item; else throw UnderflowError(); }
	};

	class ListItem
	{
		int x;
	public:
		ListItem(int xx) : x(xx) {}
		~ListItem() { std::cout << "destructor called: " << x << std::endl; }
	};
}

int main()
{
	using namespace std;
	using namespace Exercises;

	// destruct sequence should read: 4 3 5 1 2
	List<ListItem> l;
	l.push_back(new ListItem(1));
	l.push_back(new ListItem(2));
	l.push_front(new ListItem(4));
	l.push_back(new ListItem(3));
	l.pop_front();
	l.push_front(new ListItem(5));
	l.pop_back();
	l.pop_front();
}
