#include <iostream>
#include <fstream>
#include <ctime>
#include <string>

#include "map.h"

// For this exercise, I implement a red-black tree in map.h (thanks to Wikipedia for the
// general algorithm). It is many, many times faster than the previous exercise.
int main(int argc, char* argv[])
{
    using namespace std;
    using namespace ch13;

    if(argc != 2)
    {
        cerr << "Must supply a filename." << endl;
        return -1;
    }

    ifstream inFile(argv[1]);

    clock_t t = clock();

    Map<string, int> wordcounts;

    string w;
    while(inFile)
    {
        inFile >> w;
        wordcounts[w]++;
    }

    cout << "Unique words: " << wordcounts.count() << endl;
    cout << "Tree depth: " << wordcounts.depth() << endl;
    cout << "Elapsed time: " << clock() - t << endl;

    cout << "Press any key to iterate:" << endl;
    cin.get();

    for(Map<string,int>::Iterator i = wordcounts.begin(); i != wordcounts.end(); i++)
    {
        cout << i->key << ' ' << i->val << endl;
    }

    cout << "Press any key to reverse iterate: " << endl;
    cin.get();

    Map<string,int>::Iterator beg = wordcounts.begin();
    Map<string,int>::Iterator i = wordcounts.end();
    i--;

    for(;;i--)
    {
        cout << i->key << ' ' << i->val << endl;
        if(i == beg) break;
    }

    return 0;
}
