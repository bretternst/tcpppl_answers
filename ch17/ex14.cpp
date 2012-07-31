#include <iostream>
#include <stack>

int main()
{
	using namespace std;

	stack<int> s;
	s.push(1);
	s.push(2);
	s.push(3);

	// create a copy of the stack so that we don't need to change it
	// then "reverse" that stack into yet another stack, and print out those values in order
	// this method is still O(n) but has a higher per-element cost because of the multiple
	// copies that need to be made. to make this faster, some implementations support the
	// ability to get the underlying collection. however, any technique that relied on this
	// would not be portable. so i'll do it the straightforward way here.
	stack<int> s2(s);
	stack<int> s3;
	while(!s2.empty())
	{
		s3.push(s2.top());
		s2.pop();
	}
	while(!s3.empty())
	{
		cout << s3.top() << endl;
		s3.pop();
	}
}
