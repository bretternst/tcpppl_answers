#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

bool citrus_filter(std::string s) {
    static const char* names[] = { "orange", "grapefruit", "lemon", "lime", "tangerine" };
    int end = sizeof(names) / sizeof(char*);
    return find(&names[0], &names[end], s) < &names[end];
}

int main()
{
    using namespace std;

    string fruit;
    vector<string> fruits;

    cout << "enter some types of fruit, blank line to end:" << endl;
    getline(cin,fruit);
    while(fruit.size() > 0)
    {
        fruits.push_back(fruit);
        getline(cin, fruit);
    }

    // the sort is just carried over from the last few exercises
    sort(fruits.begin(),fruits.end());

    fruits.erase(remove_if(fruits.begin(),fruits.end(),citrus_filter),fruits.end());

    for(vector<string>::const_iterator i = fruits.begin(); i != fruits.end(); i++)
    {
        cout << *i << endl;
    }

    return 0;
}
