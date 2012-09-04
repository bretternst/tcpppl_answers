#include <locale>
#include <iostream>
#include <sstream>
#include <cctype>

namespace xxd {
    // to support the most different formats simultaneously, just store it as a string of digits.
    // this exercise is about input/output, not really building great phone number class.
    struct Phone_number {
        Phone_number() { }
        Phone_number(const std::string& number) : n(number) { }
        const std::string& number() const { return n; }
    private:
        std::string n;
    };

    class Phone_io : public std::locale::facet {
    public:
        explicit Phone_io(int i = 0) : std::locale::facet(i) { }
        ~Phone_io() { }

        virtual void get(std::istream& s, Phone_number& x) const;
        virtual void put(std::ostream& s, const Phone_number& x) const = 0;
        virtual int digits() const = 0;

        static std::locale::id id;
    };

    std::locale::id Phone_io::id;

    void Phone_io::get(std::istream& s, Phone_number& x) const {
        // don't be strict about the punctuation and spacing, but be strict about the # of digits
        std::string tmp;
        while(s && tmp.size() < digits()) {
            char c = s.peek();
            if (c == '-' || c == '(' || c == ')' || c == ' ') {
                s.get(c);
                continue;
            }
            if(!isdigit(c))
                break;
            s.get(c);
            tmp.push_back(c);
        }
        if(tmp.size() < digits())
            s.setstate(std::ios_base::failbit);
        else
            x = Phone_number(tmp);
    }

    std::ostream& operator<<(std::ostream& s, const Phone_number& x) {
        typename std::ostream::sentry guard(s);
        if(guard)
            std::use_facet<Phone_io>(s.getloc()).put(s, x);
        return s;
    }

    std::istream& operator>>(std::istream& s, Phone_number& x) {
        typename std::istream::sentry guard(s);
        if(guard) {
            Phone_number p;
            std::use_facet<Phone_io>(s.getloc()).get(s, p);
            if(s || s.eof()) {
                x = p;
            }
        }
        return s;        
    }

    class Phone_usa : public Phone_io {
    public:
        explicit Phone_usa(int i = 0) : Phone_io(i) { }
        void put(std::ostream& s, const Phone_number& x) const;
        int digits() const { return 10; }
    };

    void Phone_usa::put(std::ostream& s, const Phone_number& x) const {
        std::string tmp = x.number();
        if(tmp.size() >= 10)
            s << '(' << tmp.substr(0, 3) << ") " << tmp.substr(3, 3) << '-' << tmp.substr(6, 4);
    }

    class Phone_japan : public Phone_io {
    public:
        explicit Phone_japan(int i = 0) : Phone_io(i) { }
        void put(std::ostream& s, const Phone_number& x) const;
        int digits() const { return 10; }
    };

    void Phone_japan::put(std::ostream& s, const Phone_number& x) const {
        std::string tmp = x.number();
        if(tmp.size() >= 10)
            s << tmp.substr(0, 2) << '-' << tmp.substr(2, 4) << '-' << tmp.substr(6, 4);
    }
}

int main() {
    using namespace std;
    using namespace xxd;

    locale loc(locale(), new Phone_usa());
    cin.imbue(loc);
    cout.imbue(loc);

    Phone_number p;
    cout << "Enter USA phone: ";
    cin >> p;
    if(!cin) {
        cout << "invalid" << endl;
        return 1;
    }
    cin.ignore(80, '\n');
    cout << "You entered: " << p << endl;

    loc = locale(locale(), new Phone_japan());
    cin.imbue(loc);
    cout.imbue(loc);

    cout << "Enter Japan phone: ";
    cin >> p;
    if(!cin) {
        cout << "invalid" << endl;
        return 1;
    }
    cin.ignore(80, '\n');
    cout << "You entered: " << p << endl;
}
