#include <iostream>
#include <istream>
#include <sstream>
#include <cstdarg>

// Instead of inventing a new type of patterm, I'm going to extend the previous
// exercise to capture the values from the stream fscanf-style.
//
// Using a varargs ellipsis is not best practice in C++. While it is somewhat
// useful in this particular exercise, it is VERY type-unsafe and should
// not be used this way for real. The caller has to be extremely careful to
// pass in the correct types or undefined and terrible things will happen.
namespace ch21 {
    bool match(std::istream& in, const char* fmt, ...) {
        va_list ap;
        va_start(ap, fmt);

        in >> std::noskipws;
        const char* p = fmt;
        char ch;
        while(in && *p) {
            if(*p == '%') {
                int i;
                std::string s;
                switch(*(++p)) {
                case 'd': {
                    in >> i;
                    int* ip = va_arg(ap, int*);
                    *ip = i;
                    p++;
                    continue;
                }
                case 's': {
                    in >> s;
                    std::string* sp = va_arg(ap, std::string*);
                    *sp = s;
                    p++;
                    continue;
                }
                default:
                    --p;
                }
            }
            
            if(in >> ch && ch != *p)
                break;
            p++;
        }

        va_end(ap);
        return !*p;
    }

    void test_match(const char* fmt) {
        std::string s("foo 1234 bar 555baz");
        std::istringstream in(s);
        std::cout << "testing \"" << fmt << "\" = " << std::boolalpha << match(in, fmt) << std::endl;
    }
}

int main() {
    using namespace ch21;
    using namespace std;

    string s("foo 1234 bar 555baz");
    istringstream in(s);
    int i1, i2;
    string s1;
    if(match(in, "foo %d %s %dbaz", &i1, &s1, &i2)) {
        cout << i1 << ' ' << s1 << ' ' << i2 << endl; // 1234 bar 555
    }
}
