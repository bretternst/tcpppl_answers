#include <locale>
#include <iostream>
#include <sstream>

namespace xxd {
    struct Zip {
        Zip() { }
        Zip(const std::string& zip) : z(zip) { }
        const std::string& zip() const { return z; }
    private:
        std::string z;
    };

    class Zip_io : public std::locale::facet {
    public:
        explicit Zip_io(int i = 0) : std::locale::facet(i) { }
        ~Zip_io() { }

        virtual void get(std::istream& s, Zip& x) const = 0;
        virtual void put(std::ostream& s, const Zip& x) const = 0;

        static std::locale::id id;
    };

    std::ostream& operator<<(std::ostream& s, const Zip& x) {
        typename std::ostream::sentry guard(s);
        if(guard)
            std::use_facet<Zip_io>(s.getloc()).put(s, x);
        return s;
    }

    std::istream& operator>>(std::istream& s, Zip& x) {
        typename std::istream::sentry guard(s);
        if(guard) {
            Zip z;
            std::use_facet<Zip_io>(s.getloc()).get(s, z);
            if(s || s.eof()) {
                x = z;
            }
        }
        return s;        
    }

    class Zip_usa : public Zip_io {
    public:
        explicit Zip_usa(int i = 0) : Zip_io(i) { }
        void get(std::istream& s, Zip& x) const;
        void put(std::ostream& s, const Zip& x) const;
    };

    void Zip_usa::get(std::istream& s, Zip& x) const {
        std::string tmp;
        if (s >> tmp && tmp.size() == 5) {
            int z;
            std::istringstream in(tmp);
            if(in >> z && z > 0 && z < 100000) {
                x = Zip(tmp);
                return;
            }
        }
        s.setstate(std::ios_base::failbit);
    }

    void Zip_usa::put(std::ostream& s, const Zip& x) const {
        std::string tmp = x.zip();
        if(tmp.size() > 5)
            tmp.erase(5, std::string::npos);
        s << tmp;
    }

    class Zip_canada : public Zip_io {
    public:
        explicit Zip_canada(int i = 0) : Zip_io(i) { }
        void get(std::istream& s, Zip& x) const;
        void put(std::ostream& s, const Zip& x) const;
    };

    void Zip_canada::get(std::istream& s, Zip& x) const {
        std::string area;
        std::string unit;

        if(s >> area && s >> unit && area.size() == 3 && unit.size() == 3) {
            std::ostringstream out;
            out << area << ' ' << unit;
            x = Zip(out.str());
            return;
        }
        s.setstate(std::ios_base::failbit);
    }

    void Zip_canada::put(std::ostream& s, const Zip& x) const {
        std::string tmp = x.zip();
        if(tmp.size() >= 7)
            s << tmp.substr(0, 3) << ' ' << tmp.substr(4, 3);
    }

    std::locale::id Zip_io::id;
}

int main() {
    using namespace std;
    using namespace xxd;

    locale loc(locale(), new Zip_usa());
    cin.imbue(loc);
    cout.imbue(loc);

    Zip z;
    cout << "Enter USA zip: ";
    cin >> z;
    if(!cin) {
        cout << "invalid" << endl;
        return 1;
    }
    cin.ignore(80, '\n');
    cout << "You entered: " << z << endl;

    loc = locale(locale(), new Zip_canada());
    cin.imbue(loc);
    cout.imbue(loc);
    cout << "Enter Canadian zip: ";
    cin >> z;
    if(!cin) {
        cout << "invalid" << endl;
        return 1;
    }
    cin.ignore(80, '\n');
    cout << "You entered: " << z << endl;
}
