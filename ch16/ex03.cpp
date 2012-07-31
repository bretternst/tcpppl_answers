#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class initial
{
	char c;
public:
	initial(char c) : c(c) {}
	bool operator()(std::string s) { return s.size() > 0 && s[0] == c; }
};

int main()
{
	using namespace std;

	typedef vector<string> StringVec;

	string fruit;
	StringVec fruits;

	cout << "enter some types of fruit, blank line to end:" << endl;
	getline(cin,fruit);
	while(fruit.size() > 0)
	{
		fruits.push_back(fruit);
		getline(cin, fruit);
	}

	sort(fruits.begin(),fruits.end());
	initial pred('a');
	StringVec::const_iterator i = find_if(fruits.begin(),fruits.end(),pred);
	while(i != fruits.end() && pred(*i))
	{
		cout << *i << endl;
		i++;
	}

	return 0;
}
