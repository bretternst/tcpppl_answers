#include <iostream>
#include <string>

namespace DoubleLinkedList
{
	using std::string;
	using std::cout;
	using std::endl;

	struct Node
	{
		string value;
		Node* prev;
		Node* next;

		Node(string v)
		{
			prev = 0;
			next = 0;
			value = v;
		}
	};

	Node* root;

	void init();
	void free();
	void add(string);
	void print();
	void reverse();
	void swap(Node*, Node*);
	Node* merge(Node*, Node*);
	Node* sort(Node*);
	void sort();
}

void DoubleLinkedList::init()
{
	root = new Node("");
}

void DoubleLinkedList::free()
{
	while (root != 0)
	{
		Node* next = root->next;
		delete root;
		root = next;
	}
}

void DoubleLinkedList::add(string value)
{
	Node* n = root;
	while (n->next) n = n->next;
	n->next = new Node(value);
	n->next->prev = n;
}

void DoubleLinkedList::reverse()
{
	if(!root->next) return;

	Node* first = root->next;
	Node* cur = first->next;
	if(!cur) return;
	Node* prev;

	first->next = 0;
	while (cur)
	{
		Node* next = cur->next;
		cur->next = cur->prev;
		cur->prev = next;

		prev = cur;
		cur = next;
	}
	prev->prev = root;
	root->next = prev;
}

void DoubleLinkedList::print()
{
	Node* n = root->next;
	while(n)
	{
		cout << n->value << endl;
		n = n->next;
	}
}

DoubleLinkedList::Node* DoubleLinkedList::merge(Node* a, Node* b)
{
	if(!a) return b;
	if(!b) return a;
	Node* head;
	if(a->value < b->value)
	{
		head = a;
		a = a->next;
	}
	else
	{
		head = b;
		b = b->next;
	}
	head->prev = 0;
	Node* cur = head;
	while(a && b)
	{
		if(a->value < b->value)
		{
			cur->next = a;
			a = a->next;
		}
		else
		{
			cur->next = b;
			b = b->next;
		}
		cur->next->prev = cur;
		cur = cur->next;
		cur->next = 0;
	}
	if(!a) a = b;
	while(a)
	{
		cur->next = a;
		cur->next->prev = cur;
		cur = cur->next;
		a = a->next;
	}
	cur->next = 0;
	return head;
}

DoubleLinkedList::Node* DoubleLinkedList::sort(Node* head)
{
	if(!head) return 0;
	if(!head->next) return head;

	Node* i1 = head, *i2 = head;
	while(i2 && i2->next && i2->next->next)
	{
		i1=i1->next;
		i2=i2->next->next;
	}
	i2 = i1->next;
	if(i2) i2->prev = 0;
	i1->next = 0;

	i1 = sort(i2);
	i2 = sort(head);
	return merge(i1, i2);
}

void DoubleLinkedList::sort()
{
	Node* sorted = sort(root->next);
	if(sorted) sorted->prev = root;
	root->next = sorted;
}

int main()
{
	using namespace DoubleLinkedList;
	using std::cout;
	using std::endl;

	init();
	add("BASIC");
	add("FORTRAN");
	add("C++");
	add("C");
	add("C#");
	add("ADA");
	add("COBOL");
	add("Pascal");
	add("LISP");
	add("Algol");
	add("Smalltalk");
	print();

	cout << endl << "Reversed:" << endl;
	reverse();
	print();

	cout << endl << "Sorted:" << endl;
	sort();
	print();
}
