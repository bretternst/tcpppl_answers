#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

int main()
{
    using namespace std;

    string fruit;
    vector<string> fruits;

    cout << "enter some types of fruit, blank line to end:" << endl;
    getline(cin, fruit);
    while(fruit.size() > 0)
    {
        fruits.push_back(fruit);
        getline(cin, fruit);
    }

    cout << "sorted fruits:" << endl;
    sort(fruits.begin(),fruits.end());
    for(vector<string>::const_iterator i = fruits.begin(); i != fruits.end(); i++)
    {
        cout << *i << endl;
    }

    return 0;
}
