#include <iostream>
#include <map>
#include <string>
#include <vector>

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

void printStats(Matrix data)
{
    cout << "Here are the stats so far:" << endl;
    for(Matrix::const_iterator i = data.begin(); i != data.end(); i++)
    {
        double sum = getSum((*i).second);
        cout << "Name = " << (*i).first << ", Sum = " << sum << ", Avg = " << sum / (*i).second.size() << endl;
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
