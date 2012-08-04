#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>

// The second method called for in the exercise is faster, but only marginally
// on GCC. This is probably due to the fact that it uses a simple for loop
// instead of iterators. However, the iterators are a close second, probably
// helped by function inlining.
namespace ch13
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

//      for(ItemList::iterator i = items.begin(); i != items.end(); i++)
//      {
//          if(i->key == key) return i->val;
//      }
        items.push_back(Pair(key,0));
        return items.back().val;
    }
}

int main(int argc, char* argv[])
{
    using namespace std;
    using namespace ch13;

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
