#include <iostream>

enum Type {S, I};

class Entry
{
	char* name;
	Type t;
	union {
		char* s;
		int i;
	};
public:
	class InvalidOperationError {};

	Entry() {}
	Entry(char* s) : t(S), s(s) {}
	Entry(int i) : t(I), i(i) {}
	char* GetS()
	{
		if(t != S)
			throw InvalidOperationError();
		return s;
	}
	int GetI()
	{
		if(t != I)
			throw InvalidOperationError();
		return i;
	}
	void Set(char* str)
	{
		t = S;
		s = str;
	}
	void Set(int num)
	{
		t = I;
		i = num;
	}
};

void main()
{
	using namespace std;

	Entry e1("hello");
	Entry e2(4);
	cout << e1.GetS() << endl;
	cout << e2.GetI() << endl;
	e2.Set("goodbye");
	cout << e2.GetS() << endl;
}
