#include <iostream>
#include <ostream>
#include <sstream>

namespace ch21 {
    template<class T>
    class Bound_form;

    class Width_error { }; // thrown when a field can't be printed in the specified width without losing information

    class Form {
        template<class T>
        friend std::ostream& operator<<(std::ostream&, const Bound_form<T>&);

        int prc;
        int wdt;
        std::ios_base::fmtflags ffmt;
        char fc;
        std::ios_base::fmtflags ifmt;
        bool tr_width;
        bool throw_width;
        std::ios_base::fmtflags afmt;

    public:
        explicit Form(int p = 6) : prc(p), ffmt(std::ios_base::fmtflags(0)), ifmt(std::ios_base::fmtflags(0)),
            wdt(0), fc(' '), tr_width(false), throw_width(false), afmt(std::ios_base::right) { }

        template<class T>
        Bound_form<T> operator() (T val) const;

        Form& scientific() { ffmt = std::ios_base::scientific; return *this; }
        Form& fixed() { ffmt = std::ios_base::fixed; return *this; }
        Form& general() { ffmt = std::ios_base::fmtflags(0); return *this; }

        Form& uppercase() { ifmt |= std::ios_base::uppercase; return *this; }
        Form& lowercase() { ifmt &= ~std::ios_base::uppercase; return *this; }
        Form& precision(int p) { prc = p; return *this; }

        Form& width(int w, bool truncate = false) { wdt = w; tr_width = truncate; return *this; }
        Form& throw_on_overflow(bool th) { throw_width = th; return *this; }
        Form& fill(char c) { fc = c; return *this; }

        Form& plus(bool b = true) {
            if(b)
                ifmt |= std::ios_base::showpos;
            else
                ifmt &= ~std::ios_base::showpos;
            return *this;
        }
        Form& left() { afmt = std::ios_base::left; }
        Form& right() { afmt = std::ios_base::right; }
        Form& internal() { afmt = std::ios_base::internal; }
        Form& trailing_zeros(bool b = true); // not sure what this is supposed to do...
    };

    template<class T>
    struct Bound_form {
        const Form& f;
        T val;
        Bound_form(const Form& ff, T v) : f(ff), val(v) { }
    };

    template<class T>
    Bound_form<T> Form::operator() (T val) const { return Bound_form<T>(*this, val); }

    template<class T>
    std::ostream& operator<<(std::ostream& os, const Bound_form<T>& bf) {
        std::ostringstream s;
        s.precision(bf.f.prc);
        s.setf(bf.f.ffmt, std::ios_base::floatfield);
        s.width(bf.f.wdt);
        s.setf(bf.f.ifmt);
        s.fill(bf.f.fc);
        s.setf(bf.f.afmt, std::ios_base::adjustfield);
        s << bf.val;
        std::string tmp = s.str();
        if(tmp.size() > bf.f.wdt) {
            if(bf.f.throw_width)
                throw Width_error();
            else if (bf.f.tr_width) {
                if(s.flags() & std::ios_base::right)
                    tmp.erase(0, tmp.size() - bf.f.wdt);
                else
                    tmp.erase(bf.f.wdt);
            }
        }
        return os << tmp;        
    }
}

int main() {
    using namespace std;
    using namespace ch21;

    Form f;
    f.scientific().width(20, true).uppercase().fill('_').plus();
    cout << f(123.456) << endl;
    cout << f("foo") << endl;
    cout << f(123456) << endl;
    cout << f("abcdefghijklmnopqrstuvwxyz") << endl; // truncated
    f.throw_on_overflow(true);
    try {
        cout << f("abcdefghijklmnopqrstuvwxyz") << endl;
    }
    catch(Width_error) {
        cout << "caught Width_error" << endl;
    }
}
