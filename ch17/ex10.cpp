#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstdio> // for EOF

namespace ch17
{
    class Date
    {
        unsigned int y;
        unsigned int m;
        unsigned int d;

        void read_full_year(std::istream& input) { if(!(input >> y)) throw FormatError(); }
        void read_three_letter_month(std::istream&);
        void read_month_number(std::istream& input) { if(!(input >> m)) throw FormatError(); }
        void read_day(std::istream& input) { if(!(input >> d)) throw FormatError(); }
        bool is_leap_year() { return ((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0); };
        bool is_date_valid();

        void read_mmmyyyy(std::istream& input);
        void read_mmddyyyy(std::istream& input);
        void read_mmmddyyyy(std::istream& input);
    public:
        class FormatError {};
        class InvalidDateError {};

        Date() : y(1900), m(1), d(1) {}
        explicit Date(const std::string& s);
        unsigned int year() const { return y; }
        unsigned int month() const { return m; }
        unsigned int day() const { return d; }

        bool operator<(const Date&) const;
    };

    void Date::read_three_letter_month(std::istream& input)
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

    bool Date::is_date_valid()
    {
        if(y < 1 || y > 9999) return false;
        if(m < 1 || m > 12) return false;

        static unsigned int monthLengths[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        if(m == 2 && is_leap_year()) return d <= 29;
        return d <= monthLengths[m-1];
    }
    
    void Date::read_mmmyyyy(std::istream& input)
    {
        d = 1;
        read_three_letter_month(input);
        read_full_year(input);
    }

    void Date::read_mmddyyyy(std::istream& input)
    {
        read_month_number(input);
        if(input.get() != '/') throw FormatError();
        read_day(input);
        if(input.get() != '/') throw FormatError();
        read_full_year(input);
    }

    void Date::read_mmmddyyyy(std::istream& input)
    {
        if(input.get() != '(') throw FormatError();
        read_three_letter_month(input);
        read_day(input);
        if(input.get() != ',') throw FormatError();
        read_full_year(input);
        if(input.get() != ')') throw FormatError();
    }

    Date::Date(const std::string& s)
    {
        std::stringstream input(s);

        // date format 1: Jan2008
        if(isalpha(input.peek()))
            read_mmmyyyy(input);
        // date format 2: 1/1/2008
        else if(isdigit(input.peek()))
            read_mmddyyyy(input);
        // date format 3: (Mmm DD, YYYY)
        else if(input.peek()=='(')
            read_mmmddyyyy(input);
        else
            throw FormatError();

        if(input.peek() != EOF) throw FormatError();
        if(!is_date_valid()) throw InvalidDateError();
    }

    bool Date::operator<(const Date& x) const
    {
        return (y < x.y) ||
            (y == x.y && m < x.m) ||
            (y == x.y && m == x.m && d < x.d);
    }

    std::ostream& operator<<(std::ostream& output, const Date& d)
    {
        return output << d.month() << '/' << d.day() << '/' << d.year();
    }
}

int main()
{
    using namespace std;
    using namespace ch17;

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
