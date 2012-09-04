#include <iostream>
#include <locale>
#include <ctime>

namespace xxd {
    template<class T>
    void handle_ioexception(T& s) {
        if(s.exceptions() & std::ios_base::badbit) {
            try {
                s.setstate(std::ios_base::badbit);
            } catch(...) { }
            throw;
        }
        s.setstate(std::ios_base::badbit);
    }

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

    std::ostream& operator<<(std::ostream& s, const Date& d) {
        typename std::ostream::sentry guard(s);
        if(guard) try {
            tm x = { 0 };
            d.make_tm(&x);
            std::use_facet<std::time_put<char> >(s.getloc()).put(std::ostreambuf_iterator<char>(s), s, ' ', &x, 'x');
        }
        catch(...) {
            handle_ioexception(s);
        }
        return s;
    }

    std::istream& operator>>(std::istream& s, Date& d) {
        typename std::istream::sentry guard(s);
        if(guard) try {
            tm x = { 0 };
            std::ios_base::iostate res = std::ios_base::goodbit;
            std::istreambuf_iterator<char> end;
            std::use_facet<std::time_get<char> >(s.getloc()).get_date(std::istreambuf_iterator<char>(s), end, s, res, &x);
            if(res == 0 || res == std::ios::goodbit || res == std::ios_base::eofbit)
                d = Date(x.tm_mday, Date::Month(x.tm_mon+1), x.tm_year+1900);
            else
                s.setstate(res);
        }
        catch(...) {
            handle_ioexception(s);
        }
        return s;
    }

    struct Time {
        Time(int hh, int nn, int ss);
        Time() : h(0), m(0), s(0) { }

        int hour() const { return h; }
        int minute() const { return m; }
        int second() const { return s; }
        void make_tm(tm* x) const;
    private:
        char h;
        char m;
        char s;
    };

    Time::Time(int hh, int mm, int ss) : h(hh), m(mm), s(ss) {
        if(hh < 0 || hh > 24)
            throw Bad_date_time("bad hour");
        if(mm < 0 || mm > 59)
            throw Bad_date_time("bad minute");
        if(ss < 0 || ss > 59)
            throw Bad_date_time("bad seconds");
    }

    void Time::make_tm(tm* p) const {
        tm x = { 0 };
        *p = x;
        p->tm_hour = h;
        p->tm_min = m;
        p->tm_sec = s;
    }

    std::ostream& operator<<(std::ostream& s, const Time& t) {
        typename std::ostream::sentry guard(s);
        if(guard) try {
            tm x = { 0 };
            t.make_tm(&x);
            std::use_facet<std::time_put<char> >(s.getloc()).put(std::ostreambuf_iterator<char>(s), s, ' ', &x, 'X');
        }
        catch(...) {
            handle_ioexception(s);
        }
        return s;
    }

    std::istream& operator>>(std::istream& s, Time& t) {
        typename std::istream::sentry guard(s);
        if(guard) try {
            tm x = { 0 };
            std::ios_base::iostate res = std::ios_base::goodbit;
            std::istreambuf_iterator<char> end;
            std::use_facet<std::time_get<char> >(s.getloc()).get_time(std::istreambuf_iterator<char>(s), end, s, res, &x);
            if(res == 0 || res == std::ios::goodbit || res == std::ios_base::eofbit)
                t = Time(x.tm_hour, x.tm_min, x.tm_sec);
            else {
                s.setstate(res);
            }
        }
        catch(...) {
            handle_ioexception(s);
        }
        return s;
    }

    struct Date_and_time : public Date, public Time {
        Date_and_time(int dd, Date::Month mm, int yy, int hh, int mi, int ss) : Date(dd, mm, yy), Time(hh, mi, ss) { }
        Date_and_time() { }
    };

    std::ostream& operator<<(std::ostream& s, const Date_and_time& dt) {
        const Date& d = dt;
        const Time& t = dt;

        if(s << d && s << ' ')
            s << t;

        return s;
    }

    std::istream& operator>>(std::istream& s, Date_and_time& dt) {
        Date d;
        Time t;

        if(s >> d && s >> t)
            dt = Date_and_time(d.day(), Date::Month(d.month()), d.year(), t.hour(), t.minute(), t.second());
        return s;
    }
}

int main() {
    using namespace std;
    using namespace xxd;

    // On my system, the format is, e.g.: 12/23/2012 07:30:15
    Date_and_time dt;
    cout << "Enter date and time: ";
    cin >> dt;
    if(!cin) {
        cout << "invalid" << endl;
        return 1;
    }
    cout << "You entered: " << dt << endl;
}
