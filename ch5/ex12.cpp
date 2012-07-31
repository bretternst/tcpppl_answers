#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace std;

int main()
{
	const char* FIND_CHARS = "ab";

	string s;
	int count = 0;

	cout << "Enter a string, occurrences of 'ab' will be counted." << endl;
	cin >> s;

	if (s.length() == 0)
		return -1;

	for(string::const_iterator i = s.begin(); i + 1 != s.end(); i++)
	{
		if (*i == FIND_CHARS[0] && *(i+1) == FIND_CHARS[1])
			count++;
	}
	cout << count << endl;
	count = 0;

	const char* cstr = s.c_str();
	for(const char* c = cstr; *(c+1) != 0; c++)
	{
		if (*c == FIND_CHARS[0] && *(c+1) == FIND_CHARS[1])
			count++;
	}
	cout << count << endl;
}
