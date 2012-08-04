#include <iostream>
#include <string>

using namespace std;

void print(int i, int base = 10)
{
    string s;

    do
    {
        int digit = i % base;
        s.push_back(digit > 10 ? 'A' + digit - 10 : '0' + digit);
        i /= base;
    } while (i > 0);

    cout << string(s.rbegin(),s.rend()) << endl;
}

int main(int argc, char* argv[])
{
    print(31);
    print(31,10);
    print(31,16);
    print(31,2);
}
