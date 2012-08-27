#include <iostream>

namespace ch21 {
    using namespace std;

    struct complex {
        float re;
        float im;

        complex() : re(0), im(0) { }
        complex(float re, float im) : re(re), im(im) { }
    };

    ostream& operator<<(ostream& out, const complex& c) {
        return out << '(' << c.re << ',' << c.im << ')';
    }

    istream& operator>>(istream& in, complex& c) {
        float re = 0, im = 0;
        if(in >> re && in >> im)
            c = complex(re, im);
        return in;
    }
}

int main() {
    using namespace ch21;

    complex c;
    while(cin >> c)
        cout << c << endl;
}