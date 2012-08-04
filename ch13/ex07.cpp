#include <iostream>
#include <map>
#include <string>

namespace Exercises
{
    // The type T must have a default constructor that initializes it to zero.
    // T must also implement the += operator with another T.
    // K must have a compare operator (<) and equality (==).
    // Both types must have stream input operators (>>).
    template<class K,class T> void sum_inputs()
    {
        using namespace std;

        map<K,T> store;

        cout << "Enter key value pairs, one per line, separated by a space. Type `.' when done." << endl;
        K key;
        T val;

        for(;;)
        {
            cin >> key;
            if(key==".") break;
            cin >> val;
            store[key] += val;
        }

        cout << "Here are the sums:" << endl;
        for(typename map<K,T>::const_iterator i = store.begin(); i != store.end(); i++)
        {
            cout << i->first << ": " << i->second << endl;
        }
    }
}

int main()
{
    using namespace std;
    using namespace Exercises;

    sum_inputs<string,int>();

    return 0;
}
