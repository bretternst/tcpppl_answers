#include <iostream>
#include <vector>

// comment out to use the linked list implementation
#define USE_VECTOR

namespace Exercises
{
#ifdef USE_VECTOR
	using std::vector;

	class CharQueue
	{
		vector<char> queue;

	public:
		void enqueue(char ch);
		char dequeue();
	};

	void CharQueue::enqueue(char ch)
	{
		queue.insert(queue.begin(), ch);
	}

	char CharQueue::dequeue()
	{
		if(queue.begin() == queue.end()) return 0;
		char ch = *(queue.end()-1);
		queue.pop_back();
		return ch;
	}
#else
	class CharQueue
	{
		struct Node
		{
			char ch;
			Node* next;

			Node(char ch) : ch(ch), next(0) {}
		};

		Node* head;
		Node* tail;

	public:
		CharQueue() : head(new Node(0)), tail(head) {}
		~CharQueue();

		void enqueue(char ch);
		char dequeue();
	};

	CharQueue::~CharQueue()
	{
		Node* p = head->next;
		delete head;
		while (p)
		{
			Node* q = p->next;
			delete p;
			p = q;
		}
	}

	void CharQueue::enqueue(char ch)
	{
		tail->next = new Node(ch);
		tail = tail->next;
	}

	char CharQueue::dequeue()
	{
		Node* n = head->next;
		if(!n) return 0;
		char ch = n->ch;
		head->next = n->next;
		delete n;
		return ch;
	}
#endif
}

int main(int argc, char* argv[])
{
	using namespace std;
	using namespace Exercises;

	if(argc != 2)
	{
		cerr << "invalid number of arguments" << endl;
		return 1;
	}

	CharQueue queue;
	char* p = argv[1];
	while(*p)
	{
		queue.enqueue(*(p++));
	}

	char ch;
	while(ch = queue.dequeue())
		cout << ch << endl;
}
