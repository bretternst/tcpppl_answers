#include <iostream>
#include <stack>

namespace Exercises
{
	template<class T>
	class Queue
	{
	private:
		std::stack<T> s1;
		std::stack<T> s2;
	public:
		class UnderflowError {};

		Queue() {}
		void PushBack(T x) { s1.push(x); }
		T PopFront();
		bool Empty() const { return s1.empty() && s2.empty(); }
	};

	template<class T>
	T Queue<T>::PopFront()
	{
		if(s2.empty())
		{
			if(s1.empty()) throw UnderflowError();
			for(int i = s1.size(); i > 0; --i)
			{
				s2.push(s1.top());
				s1.pop();
			}
		}

		T x = s2.top();
		s2.pop();
		return x;
	}
}

int main()
{
	using namespace std;
	using namespace Exercises;

	Queue<int> q;
	q.PushBack(1);
	q.PushBack(2);
	q.PushBack(3);

	cout << q.PopFront() << endl;
	q.PushBack(4);
	q.PushBack(5);

	while(!q.Empty())
	{
		cout << q.PopFront() << endl;
	}

	return 0;
}
