#include <iostream>
#include <string>

using std::string;

template<class T> class NameValuePair
{
public:
	string name;
	T value;
};

template<class T> class Table
{
	NameValuePair<T>* p;
	int sz;
	int count;

public:
	Table(size_t sz) : count(0), sz(sz) { p = new NameValuePair<T>[sz]; }
	~Table() {delete[] p;}

	NameValuePair<T>* Lookup(char* name)
	{
		for(int i = 0; i < count; i++)
		{
			if(p[i].name==name) return &p[i];
		}
		return 0;
	}

	bool Insert(char* name, T value)
	{
		if(count > sz) return false;

		p[count].name = name;
		p[count++].value = value;
		return true;
	}
};



void main()
{
	using namespace std;

	Table<int> x(100);
	x.Insert("a", 1);
	x.Insert("b", 2);
	x.Insert("c", 3);

	cout << x.Lookup("a")->value << endl;
	cout << x.Lookup("b")->value << endl;
	cout << x.Lookup("c")->value << endl;
}
