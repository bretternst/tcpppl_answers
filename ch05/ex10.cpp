#include <iostream>
#include <string>

using namespace std;

const int NUM_MONTHS = 12;

void printMonths(string months[])
{
    for(int i = 0; i < NUM_MONTHS; i++)
    {
        cout << months[i] << endl;
    }
}

int main()
{
    string months[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

    for(int i = 0; i < NUM_MONTHS; i++)
    {
        cout << months[i] << endl;
    }

    printMonths(months);
}
