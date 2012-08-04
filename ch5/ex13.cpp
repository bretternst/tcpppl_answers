#include <iostream>
#include <string>

using namespace std;

struct Date {

    Date(int y, int m, int d)
    {
        year = y;
        month = m;
        day = d;
    }

    int year;
    int month;
    int day;
};

void printDate(Date d)
{
    cout << d.year << "-" << d.month << "-" << d.day << endl;
}

Date readDate()
{
    int y, m, d;
    cin >> y >> m >> d;
    return Date(y, m, d);
}

int main()
{
    Date d = Date(2008, 12, 31);
    printDate(d);
    cout << "Enter a date (YYYY mm dd): ";
    d = readDate();
    cout << "You entered: ";
    printDate(d);
}
