#include <iostream>
#include <fstream>
#include <ctime>
#include <string>

#include "Map.h"

// For this exercise, I implement a red-black tree in Map.h (thanks to Wikipedia for the
// general algorithm). It is many, many times faster than the previous exercise.
int main(int argc, char* argv[])
{
	using namespace std;
	using namespace Exercises;

	// Approach 1: using the associative array from section 11.8
	if(argc != 2)
	{
		cerr << "Must supply a filename." << endl;
		return -1;
	}

	ifstream inFile(argv[1]);

	clock_t t = clock();

	Map<string, int> wordCounts;

	string w;
	while(inFile)
	{
		inFile >> w;
		wordCounts[w]++;
	}

	cout << "Unique words: " << wordCounts.Count() << endl;
	cout << "Tree depth: " << wordCounts.Depth() << endl;
	cout << "Elapsed time: " << clock() - t << endl;

	cout << "Press any key to iterate:" << endl;
	cin.get();

	for(Map<string,int>::Iterator i = wordCounts.Begin(); i != wordCounts.End(); i++)
	{
		cout << i->key << ' ' << i->val << endl;
	}

	cout << "Press any key to reverse iterate: " << endl;
	cin.get();

	Map<string,int>::Iterator beg = wordCounts.Begin();
	Map<string,int>::Iterator i = wordCounts.End();
	i--;

	for(;;i--)
	{
		cout << i->key << ' ' << i->val << endl;
		if(i == beg) break;
	}

	return 0;
}
