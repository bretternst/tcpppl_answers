#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>

using std::vector;
using std::map;
using std::istream;
using std::string;

// The exercise description is a little unclear whether it is strings or
// words should be counted. That is, should whitespace be taken into account
// or should simple words be matched?
//
// I chose to match simple strings, which consequently can overlap. For
// example, if the string "bcd" and "def" are being searched, they would
// both be matched in a line containing "abcdef". It is case sensitive.
map<const string,int> countStringLines(istream* input, const vector<string>& strings)
{
    // Initialize the return value, so that it contains all strings
    // even if they were not found.
    map<const string,int> stringCounts;
    for(vector<string>::const_iterator i = strings.begin(); i != strings.end(); i++)
    {
        stringCounts[*i] = 0;
    }

    typedef map<string const*,string::const_iterator> Iterators;
    Iterators iter;

    while(*input)
    {
        // Reset all iterators
        for(vector<string>::const_iterator i = strings.begin(); i != strings.end(); i++)
        {
            iter[&(*i)] = i->begin();
        }

        char ch;

        // Loop until end of line or input
        while(input->get(ch) && ch != '\n')
        {
            for(Iterators::iterator i = iter.begin(); i != iter.end(); i++)
            {
                if(i->second != i->first->end())
                {
                    if (*(i->second) == ch)
                    {
                        i->second++;
                        if(i->second == i->first->end())
                            stringCounts[*(i->first)]++;
                    }
                    else
                    {
                        i->second = i->first->begin();
                    }
                }
            }
        }
    }
    return stringCounts;
}

// Usage:
// <executable> <fileName> <word> [<word>,...]
int main(int argc, char* argv[])
{
    using namespace std;

    if(argc < 3)
    {
        cout << "Usage:" << endl;
        cout << "<executable> <fileName> <word> [<word>,...]" << endl;
        return 1;
    }

    ifstream input(argv[1]);
    if(!input.is_open())
    {
        cerr << "Could not open file: " << argv[1] << endl;
        return 1;
    }

    vector<string> strings;
    for(int i = 2; i < argc; i++)
        strings.push_back(argv[i]);

    map<const string,int> stringCounts = countStringLines(&input, strings);

    for(map<const string,int>::iterator i = stringCounts.begin(); i != stringCounts.end(); i++)
    {
        cout << "'" << (i->first) << "' appears on " << (i->second) << " lines" << endl;
    }

    input.close();
}
