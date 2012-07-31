#include <iostream>

// This technique may or may not have been what the exercise
// was after (the chapter is about operator overloading, after
// all). However, it does achieve the same effect. Note that
// the s and t parameters passed into the constructor are copied,
// so changes to the x and p fields in class C won't affect
// the original objects that were passed in.
//
// This could be changed by having parameters passed by
// reference, but this would be dangerous because the objects
// being passed in may not live as long as the instance of C,
// and could thus result in accessing invalid memory.
namespace Exercises
{
	struct S { int x, y; };
	struct T { char* p; char* q; };

	class C
	{
	public:
		int& x;
		char*& p;

		C(S s, T t) : x(s.x), p(t.p)
		{
		}
	};
}

int main()
{
	using namespace std;
	using namespace Exercises;

	S s = { 1, 2 };
	T t = { "hello", "" };
	C c(s, t);

	cout << c.x << ' ' << c.p << endl;

	return 0;
}
