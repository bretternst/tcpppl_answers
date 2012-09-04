#include <iostream>
#include <locale>
#include <ctime>

namespace xxd {
    struct Bad_date_time {
        const char* why;
        Bad_date_time(const char* why) : why(why) { }
    };

    struct Date {
        enum Month { jan=1, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec };

        Date(int dd, Month mm, int yy);
        Date() : d(0), m(Month(0)), y(0) { }

        int year() const { return y; }
        int month() const { return m; }
        int day() const { return d; }
        void make_tm(tm* p) const;

    private:
        char d;
        Month m;
        int y;
    };

    Date::Date(int dd, Month mm, int yy) : d(dd), m(mm), y(yy) {
        if(d == 0 && m == Month(0) && y == 0)
            return;
        if(mm < jan || mm > dec) throw Bad_date_time("bad month");
        if(dd < 1 || dd > 31) // already did this "correctly" in an earlier chapter
            throw Bad_date_time("bad day of month");
    }

    void Date::make_tm(tm* p) const {
        tm x = { 0 };
        *p = x;
        p->tm_year = y - 1900;
        p->tm_mday = d;
        p->tm_mon = m-1;
    }

    class Date_out : public std::locale::facet {
        std::string fmt;
    public:
        explicit Date_out(const std::string& fmt, int i = 0) : std::locale::facet(i), fmt(fmt) { }
        ~Date_out() { }

        void put(std::ostream& s, const Date& x) const;

        static std::locale::id id;
    };

    std::locale::id Date_out::id;

    void Date_out::put(std::ostream& s, const Date& d) const {
        tm x = { 0 };
        d.make_tm(&x);
        std::use_facet<std::time_put<char> >(s.getloc()).put(std::ostreambuf_iterator<char>(s), s, ' ', &x,
                                                             fmt.data(), fmt.data()+fmt.size());
    }

    std::ostream& operator<<(std::ostream& s, const Date& d) {
        typename std::ostream::sentry guard(s);
        if(guard)
            std::use_facet<Date_out>(s.getloc()).put(s, d);
        return s;
    }
}

int main() {
    using namespace std;
    using namespace xxd;

    Date d(31, Date::Month(12), 2012);
    locale loc(locale(), new Date_out("%b %d, %Y"));
    cout.imbue(loc);
    cout << d << endl;
}
