#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

namespace Exercises
{
    class Date
    {
        unsigned int y;
        unsigned int m;
        unsigned int d;

        void ReadFullYear(std::istream& input) { if(!(input >> y)) throw FormatError(); }
        void ReadThreeLetterMonth(std::istream&);
        void ReadMonthNumber(std::istream& input) { if(!(input >> m)) throw FormatError(); }
        void ReadDay(std::istream& input) { if(!(input >> d)) throw FormatError(); }
        bool IsLeapYear() { return ((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0); };
        bool IsDateValid();

        void ReadMmmYYYY(std::istream& input);
        void ReadMMDDYYYY(std::istream& input);
        void ReadMmmDDYYYY(std::istream& input);
    public:
        class FormatError {};
        class InvalidDateError {};

        Date() : y(1900), m(1), d(1) {}
        explicit Date(const std::string& s);
        unsigned int Year() const { return y; }
        unsigned int Month() const { return m; }
        unsigned int Day() const { return d; }

        bool operator<(const Date&);
    };

    void Date::ReadThreeLetterMonth(std::istream& input)
    {
        std::string s;
        for(int i = 0; i < 3 && input; i++) s.push_back(tolower(input.get()));
        if(s.length() != 3) throw FormatError();
        if(s=="jan") m=1; else if (s=="feb") m=2; else if (s=="mar") m=3;
        else if (s=="apr") m=4; else if (s=="may") m=5; else if (s=="jun") m=6;
        else if (s=="jul") m=7; else if (s=="aug") m=8; else if (s=="sep") m=9;
        else if (s=="oct") m=10; else if (s=="nov") m=11; else if (s=="dec") m=12;
        else throw InvalidDateError();
    }

    bool Date::IsDateValid()
    {
        if(y < 1 || y > 9999) return false;
        if(m < 1 || m > 12) return false;

        static unsigned int monthLengths[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        if(m == 2 && IsLeapYear()) return d <= 29;
        return d <= monthLengths[m-1];
    }
    
    void Date::ReadMmmYYYY(std::istream& input)
    {
        d = 1;
        ReadThreeLetterMonth(input);
        ReadFullYear(input);
    }

    void Date::ReadMMDDYYYY(std::istream& input)
    {
        ReadMonthNumber(input);
        if(input.get() != '/') throw FormatError();
        ReadDay(input);
        if(input.get() != '/') throw FormatError();
        ReadFullYear(input);
    }

    void Date::ReadMmmDDYYYY(std::istream& input)
    {
        if(input.get() != '(') throw FormatError();
        ReadThreeLetterMonth(input);
        ReadDay(input);
        if(input.get() != ',') throw FormatError();
        ReadFullYear(input);
        if(input.get() != ')') throw FormatError();
    }

    Date::Date(const std::string& s)
    {
        std::stringstream input(s);

        // date format 1: Jan2008
        if(isalpha(input.peek()))
            ReadMmmYYYY(input);
        // date format 2: 1/1/2008
        else if(isdigit(input.peek()))
            ReadMMDDYYYY(input);
        // date format 3: (Mmm DD, YYYY)
        else if(input.peek()=='(')
            ReadMmmDDYYYY(input);
        else
            throw FormatError();

        if(input.peek() != EOF) throw FormatError();
        if(!IsDateValid()) throw InvalidDateError();
    }

    bool Date::operator<(const Date& x)
    {
        return (y < x.y) ||
            (y == x.y && m < x.m) ||
            (y == x.y && m == x.m && d < x.d);
    }

    std::ostream& operator<<(std::ostream& output, const Date& d)
    {
        return output << d.Month() << '/' << d.Day() << '/' << d.Year();
    }
}

int main()
{
    using namespace std;
    using namespace Exercises;

    cout << "enter some dates (empty line to finish):" << endl;

    vector<Date> dates;
    string s;
    while(getline(cin,s) && !s.empty())
    {
        try
        {
            dates.push_back(Date(s));
        }
        catch(Date::FormatError)
        {
            cerr << "format error" << endl;
        }
        catch(Date::InvalidDateError)
        {
            cerr << "invalid date" << endl;
        }
    }

    sort(dates.begin(),dates.end());
    for(vector<Date>::const_iterator i = dates.begin(); i != dates.end(); i++)
    {
        cout << *i << endl;
    }
}
