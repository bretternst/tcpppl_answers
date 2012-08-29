#include <iostream>
#include <sstream>
#include <cstddef>
#include <cstring>
#include <istream>
#include <cstdio>

namespace ch21 {
    class strstreambuf : public std::streambuf {
    public:
        strstreambuf(char* buf) {
            int sz = strlen(buf);
            setg(buf, buf, buf + sz);
        }
        strstreambuf(char* buf, size_t n) {
            setp(buf, buf + n - 1);
        }
        char* str() const {
            *pptr() = 0;
            return pbase();
        }
    };

    class ostrstream : public std::ostream {
        strstreambuf sb;

    public:
        ostrstream(char* buf, int sz) : sb(buf, sz), std::ostream(&sb) { }

        template<class T>
        ostrstream& operator<<(const T& x) {
            std::ostream out(&sb);
            out.copyfmt(*this);
            out << x;
            setstate(out.rdstate());
            return *this;
        }
    };

    std::ostream& operator<<(std::ostream& out, const ostrstream& x) {
        out << static_cast<strstreambuf*>(x.rdbuf())->str();
        return out;
    }

    class istrstream : public std::istream {
        strstreambuf sb;
    public:
        istrstream(char* buf) : sb(buf) { }

        template<class T>
        istrstream& operator>>(T& x) {
            std::istream in(&sb);
            in.copyfmt(*this);
            in >> x;
            setstate(in.rdstate());
            return *this;
        }
    };
}

int main() {
    using namespace std;
    using namespace ch21;

    char buf[1024];
    ostrstream out(buf, 1024);
    out << "foo bar " << 12345;
    cout << out << endl;

    istrstream in(buf);
    string s1, s2;
    int i;
    in >> s1 >> s2 >> i;
    cout << s1 << ' ' << s2 << ' ' << i << endl;
    if(in.eof())
        cout << "stream is eof" << endl;
}
