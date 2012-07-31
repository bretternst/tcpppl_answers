#include <iostream>
#include <vector>

int main()
{
	using namespace std;

	char* letters = "abcdefghijklmnopqrstuvwxyz";

	typedef vector<char> CharVec;

	CharVec v(&letters[0],&letters[26]);

	for(CharVec::const_iterator i = v.begin(); i != v.end(); i++)
	{
		cout << *i;
	}
	cout << endl;

	for(CharVec::const_reverse_iterator i = v.rbegin(); i != v.rend(); i++)
	{
		cout << *i;
	}
	cout << endl;

	return 0;
}
