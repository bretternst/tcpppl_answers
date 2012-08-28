#include <iostream>
#include <ostream>
#include <sstream>

namespace ch21 {
    template<class T>
    class Bound_form;

    class Form {
        template<class T>
        friend std::ostream& operator<<(std::ostream&, const Bound_form<T>&);

        int prc;
        int wdt;
        std::ios_base::fmtflags ffmt;
        char fc;
        std::ios_base::fmtflags ifmt;

    public:
        explicit Form(int p = 6) : prc(p), ffmt(std::ios_base::fmtflags(0)), ifmt(std::ios_base::fmtflags(0)),
            wdt(0), fc(' ') { }

        template<class T>
        Bound_form<T> operator() (T val) const;

        Form& scientific() { ffmt = std::ios_base::scientific; return *this; }
        Form& fixed() { ffmt = std::ios_base::fixed; return *this; }
        Form& general() { ffmt = std::ios_base::fmtflags(0); return *this; }

        Form& uppercase() { ifmt |= std::ios_base::uppercase; return *this; }
        Form& lowercase() { ifmt &= ~std::ios_base::uppercase; return *this; }
        Form& precision(int p) { prc = p; return *this; }

        Form& width(int w) { wdt = w; return *this; }
        Form& fill(char c) { fc = c; return *this; }

        Form& plus(bool b = true) {
            if(b)
                ifmt |= std::ios_base::showpos;
            else
                ifmt &= ~std::ios_base::showpos;
            return *this;
        }
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
        s << bf.val;
        return os << s.str();        
    }
}

int main() {
    using namespace std;
    using namespace ch21;

    Form f;
    f.scientific().width(20).uppercase().fill('_').plus();
    cout << f(123.456) << endl;
    cout << f("foo") << endl;
    cout << f(123456) << endl;
}
