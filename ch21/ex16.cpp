#include <iostream>
#include <istream>
#include <sstream>

// The exercise is too vague; it doesn't say whether values captured from
// the match operation need to be preserved or what. Is it just a match or
// no match thing, or more like fscanf?
namespace ch21 {
    bool match(std::istream& in, const char* fmt) {
        in >> std::noskipws;
        const char* p = fmt;
        char ch;
        while(in && *p) {
            if(*p == '%') {
                int i;
                std::string s;
                switch(*(++p)) {
                case 'd':
                    in >> i;
                    p++;
                    continue;
                case 's':
                    in >> s;
                    p++;
                    continue;
                default:
                    --p;
                }
            }
            
            if(in >> ch && ch != *p)
                break;
            p++;
        }
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

    test_match("foo %d %s %dbaz");
    test_match("%s %s %s %s");
    test_match("%s %d %s %d%s");
    test_match("foo %d bar 554baz");
    test_match("foo 1%d bar 5%dbaz");
}
