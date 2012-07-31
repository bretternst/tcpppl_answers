#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <algorithm>

int main(int argc, char* argv[])
{
	using namespace std;

	if(argc < 2)
	{
		cerr << "please supply a filename" << endl;
		return -1;
	}

	ifstream f(argv[1]);
	if(!f.is_open())
	{
		cerr << "could not open file" << endl;
		return -1;
	}

	set<string> words;
	string word;

	while(f)
	{
		word.clear();

		char ch;
		do {
			f.get(ch);
		} while (f && !isalpha(ch));

		if(!f) break;

		word.push_back(ch);
		while(f.get(ch) && isalpha(ch))
			word.push_back(ch);

		// commented out simplistic approach in favor of new approach above
//		f >> word;

		// transform all words to lowercase
		transform(word.begin(), word.end(), word.begin(), tolower);

		words.insert(word);
	}

	for(set<string>::const_iterator i = words.begin(); i != words.end(); i++)
		cout << *i << endl;

	return 0;
}
