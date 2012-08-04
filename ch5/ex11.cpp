#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace std;

int main()
{
    string s;
    vector<string> words;
    set<string> sortedWords;

    cin >> s;
    while (s != "Quit")
    {
        if (sortedWords.insert(s).second)
        {
            words.push_back(s);
        }
        cin >> s;
    }

    cout << "In entered order:" << endl;
    for(vector<string>::const_iterator i = words.begin(); i != words.end(); i++)
    {
        cout << *i << endl;
    }

    cout << "In sorted order: " << endl;
    for(set<string>::const_iterator i = sortedWords.begin(); i != sortedWords.end(); i++)
    {
        cout << *i << endl;
    }
}
