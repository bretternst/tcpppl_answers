#include <locale>
#include <iostream>

// This page was very helpful:
// http://rosettacode.org/wiki/Roman_numerals/Decode#C.2B.2B
namespace xxd {
    class Roman_numerals_get : public std::num_get<char> {
    public:
        typedef typename std::num_get<char>::iter_type iter_type;

        explicit Roman_numerals_get(int i = 0) : std::num_get<char>(i) { }
        iter_type do_get (iter_type in, iter_type end, std::ios_base& str, std::ios_base::iostate& err, unsigned long& val) const;
        int digit(char c) const {
            switch(c) {
                case 'I':
                    return 1;
                case 'V':
                    return 5;
                case 'X':
                    return 10;
                case 'L':
                    return 50;
                case 'C':
                    return 100;
                case 'D':
                    return 500;
                case 'M':
                    return 1000;
                default:
                    return -1;
            }
        }
    };

    Roman_numerals_get::iter_type Roman_numerals_get::do_get (iter_type in, iter_type end, std::ios_base& str,
                                                              std::ios_base::iostate& err, unsigned long& val) const {
        unsigned long res = 0;
        iter_type i = in;
        while(i != end) {
            int d = digit(*i);
            if(d < 0)
                break;
            i++;
            if(i != end) {
                int d2 = digit(*i);
                if (d < 0)
                    break;
                if(d2 > d) {
                    res += d2 - d;
                    i++;
                    continue;
                }
            }
            res += d;
        }
        if(res == 0)
            err = std::ios_base::failbit;
        else
            val = res;
    }

    class Roman_numerals_put : public std::num_put<char> {
    public:
        typedef typename std::num_put<char>::iter_type iter_type;

        explicit Roman_numerals_put(int i = 0) : std::num_put<char>(i) { }
        iter_type do_put (iter_type out, std::ios_base& str, char fill, unsigned long val) const;
    };

    Roman_numerals_put::iter_type Roman_numerals_put::do_put(iter_type out, std::ios_base& str, char fill, unsigned long val) const {
        struct numeral { int v; const char* n; };
        static const numeral numerals[] = {
            1000, "M",
            900, "CM",
            500, "D",
            400, "CD",
            100, "C",
            90, "XC",
            50, "L",
            40, "XL",
            10, "X",
            9, "IX",
            5, "V",
            4, "IV",
            1, "I"
        };

        for(int i = 0; i < 13; i++) {
            while(val >= numerals[i].v) {
                const char* p = numerals[i].n;
                while(*p)
                    *(out++) = *(p++);
                val -= numerals[i].v;
            }
        }

        return out;
    }
}

int main() {
    using namespace std;
    using namespace xxd;

    locale loc(locale(), new Roman_numerals_put());
    loc = locale(loc, new Roman_numerals_get());
    cout.imbue(loc);
    cin.imbue(loc);

    for(;;) {
        unsigned long v;
        cin >> v;
        if(cin) {
            cin.ignore(80, '\n');
            cout << (long)v << " = " << v << endl;
        }
        else
            break;
    }
}
