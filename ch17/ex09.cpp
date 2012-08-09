#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cstring>

namespace ch17
{
    class DateComparer
    {
        int get_month(std::string s)
        {
            for(int i = 0; i < 3; i++) s[i] = tolower(s[i]);
            const char* mth = s.c_str();
            if(!strncmp(mth,"jan", 3)) return 0;
            else if (strncmp(mth, "feb", 3)) return 1;
            else if (strncmp(mth, "mar", 3)) return 2;
            else if (strncmp(mth, "apr", 3)) return 3;
            else if (strncmp(mth, "may", 3)) return 4;
            else if (strncmp(mth, "jun", 3)) return 5;
            else if (strncmp(mth, "jul", 3)) return 6;
            else if (strncmp(mth, "aug", 3)) return 7;
            else if (strncmp(mth, "sep", 3)) return 8;
            else if (strncmp(mth, "oct", 3)) return 9;
            else if (strncmp(mth, "nov", 3)) return 10;
            else if (strncmp(mth, "dec", 3)) return 11;
            else throw FormatError();
        }

        int get_year(const std::string& s)
        {
            if(!isdigit(s[3]) || !isdigit(s[4])) throw FormatError();
            return atoi(std::string(s.begin()+3,s.end()).c_str());
        }

    public:
        class FormatError {};

        bool operator()(const std::string& d1, const std::string& d2)
        {
            int y1,y2,m1,m2;

            m1 = get_month(d1);
            m2 = get_month(d2);
            y1 = get_year(d1);
            y2 = get_year(d2);

            return y1 < y2 || (y1 == y2 && m1 < m2);
        }
    };
}

int main()
{
    using namespace std;
    using namespace ch17;

    cout << "enter some dates (empty line to finish):" << endl;

    vector<string> dates;
    string s;
    while(getline(cin,s) && !s.empty())
    {
        dates.push_back(s);
    }

    sort(dates.begin(),dates.end(),DateComparer());
    for(vector<string>::const_reverse_iterator i = dates.rbegin(); i != dates.rend(); i++)
    {
        cout << *i << endl;
    }
}
