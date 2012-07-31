#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>

// This exercise asks for a performance comparison between using the associative
// array defined in Chapter 8 with a solution not using an associative array
// (in this case, I just used two separate vectors of identical size). I was
// surprised to find that, initially, the non-assoc version outperformed the
// assoc version by a wide margin, whereas I expected them to be roughly the same
// because the bulk of the work would be the O(n) search for keys.
//
// However, I found that because the assoc array used an iterator whereas the
// non-assoc solution used a simple for loop on the index, it ran much slower.
// I modified the assoc array to use a simple for loop on index and found that
// it now outperformed the non-assoc version by a small margin.
//
// Therefore, I must conclude that, at least in the implementation I'm using,
// iterators have a fairly sizable overhead. Is it due to the iterators
// themselves or the additional function calls required? If the functions are
// inlined, it is likely not the function calls.
namespace Exercises
{
	using std::string;

	class Assoc
	{
		struct Pair
		{
			string key;
			int val;
			Pair(const string& key, int val) : key(key), val(val) {}
		};

		// prevent copying
		Assoc(const Assoc&) {}
		void operator=(const Assoc&) {}

		typedef std::vector<Pair> ItemList;
		ItemList items;

	public:
		class MissingKeyError {};

		Assoc() {}
		int& operator[](const string& key);
		int count() const { return items.size(); }
	};

	int& Assoc::operator[](const string& key)
	{
		for(int i = 0; i < items.size(); i++)
		{
			if(items[i].key == key)
				return items[i].val;
		}

//		for(ItemList::iterator i = items.begin(); i != items.end(); i++)
//		{
//			if(i->key == key) return i->val;
//		}
		items.push_back(Pair(key,0));
		return items.back().val;
	}
}

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

	Assoc wordCounts;
	string w;
	while(inFile)
	{
		inFile >> w;
		wordCounts[w]++;
	}

	cout << "Unique words: " << wordCounts.count() << endl;
	cout << "Elapsed time: " << clock() - t << endl;

	// Approach 2: not using an associative array, just two vectors
	inFile.close();
	inFile.clear();
	inFile.open(argv[1]);

	t = clock();

	vector<string> words;
	vector<int> counts;

	while(inFile)
	{
		inFile >> w;
		int i = -1;
		bool found = false;
		for(i = 0; i < words.size(); i++)
		{
			if(words[i] == w)
			{
				counts[i]++;
				found = true;
				break;
			}
		}
		if(!found)
		{
			words.push_back(w);
			counts.push_back(1);
		}
	}

	cout << "Unique words: " << wordCounts.count() << endl;
	cout << "Elapsed time: " << clock() - t << endl;

	return 0;
}
