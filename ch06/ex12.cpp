#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <numeric>

using namespace std;

typedef vector<double> ValueList;
typedef map<string,ValueList> Matrix;
typedef pair<string,ValueList> MatrixPair;

double getSum(ValueList list)
{
    double sum = 0;
    for(ValueList::const_iterator i = list.begin(); i != list.end(); i++)
        sum += *i;
    return sum;
}

double getMedian(ValueList list)
{
    int s = list.size();
    sort<ValueList::iterator>(list.begin(),list.end());

    if (s < 1) return 0.0;
    if (s < 2) return list[0];

    if (s % 2)
    {
        return list[s / 2];
    }
    else
    {
        return (list[s / 2 - 1] + list[s / 2]) / 2;
    }
}

void printStats(Matrix data)
{
    cout << "Here are the stats so far:" << endl;
    for(Matrix::const_iterator i = data.begin(); i != data.end(); i++)
    {
        ValueList values = (*i).second;
        double sum = getSum(values);
        cout << "Name = " << (*i).first << ", Sum = " << sum << ", Avg = " << sum / (*i).second.size() <<
            ", Median = " << getMedian((*i).second) << endl;
    }
}

int main()
{
    char *n;
    string name;
    double value;
    Matrix data;

    for(;;)
    {
        cout << "Enter 'name value':" << endl;

        name.clear();
        cin >> name;
        cin >> value;

        if (!data.count(name))
        {
            ValueList v;
            data.insert(MatrixPair(name,v));
        }
        data[name].push_back(value);

        printStats(data);
    }
}
