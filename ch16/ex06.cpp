#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::vector;
using std::string;

class contains
{
	vector<string>& names;
public:
	contains(vector<string>& names) : names(names) {}

	bool operator()(std::string s)
	{
		return find(names.begin(),names.end(),s) < names.end();
	}
};

int main()
{
	using namespace std;

	typedef vector<string> StringVec;

	string fruit;

	StringVec fruitsYouDontLike;
	cout << "enter the names of some fruits you don't like, blank line to end:" << endl;
	getline(cin,fruit);
	while(fruit.size() > 0)
	{
		fruitsYouDontLike.push_back(fruit);
		getline(cin,fruit);
	}

	StringVec fruits;
	cout << "enter some types of fruit, blank line to end:" << endl;
	getline(cin,fruit);
	while(fruit.size() > 0)
	{
		fruits.push_back(fruit);
		getline(cin, fruit);
	}

	// the sort is just carried over from the last few exercises
	sort(fruits.begin(),fruits.end());

	fruits.erase(remove_if(fruits.begin(),fruits.end(),contains(fruitsYouDontLike)),fruits.end());

	for(StringVec::const_iterator i = fruits.begin(); i != fruits.end(); i++)
	{
		cout << *i << endl;
	}

	return 0;
}
