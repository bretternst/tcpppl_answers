#include <ios>
#include <cstdio>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <inttypes.h>

// The biggest pitfall here will be binary incompatibility between platforms; two of
// the most common problems can be solved by determining the platform's endianness and
// writing/reading in a consistent byte order, and also by using an explicitly-sized int
// type.
//
// We're going to assume here that float is always 32-bits.
namespace ch15 {
    typedef union {
        uint32_t i;
        char c[4];
    } intbytes;

    typedef union {
        float f;
        char c[4];
    } floatbytes;

    // Adapted from http://stackoverflow.com/questions/1001307/detecting-endianness-programmatically-in-a-c-program
    int is_big_endian() {
        intbytes test = {0x01020304};
        return test.c[0] == 1;
    }

    class ostream : public std::ios {
    public:
        explicit ostream(std::streambuf* sb) : std::ios(sb) { }

        ostream& put (char c) {
            int n = rdbuf()->sputc(c);
            if(n == EOF)
                setstate(std::ios::failbit);
            return *this;
        }

        ostream& operator<<(uint32_t x) {
            intbytes bytes;
            bytes.i = x;
            if(!is_big_endian())
                std::reverse(bytes.c, bytes.c + 4);
            for(int i = 0; i < 4; i++) {
                if(!put(bytes.c[i])) {
                    setstate(std::ios::failbit);
                    break;
                }
            }
            return *this;
        }

        ostream& operator<<(int x) {
            return *this << static_cast<uint32_t>(x);
        }

        ostream& operator<<(float x) {
            floatbytes bytes;
            bytes.f = x;
            if(!is_big_endian())
                std::reverse(bytes.c, bytes.c + 4);
            for(int i = 0; i < 4; i++) {
                if(!put(bytes.c[i])) {
                    setstate(std::ios::failbit);
                    break;
                }
            }
            return *this;
        }
    };

    class istream : public std::ios {
    public:
        explicit istream(std::streambuf* sb) : std::ios(sb) { }

        int get() {
            int c = rdbuf()->sbumpc();
            if(c == EOF)
                setstate(std::ios::eofbit);
            return c;
        }

        istream& operator>>(uint32_t& x) {
            intbytes bytes;
            for(int i = 0; i < 4; i++) {
                if((bytes.c[i] = get()) == EOF) {
                    setstate(std::ios::failbit);
                    return *this;
                }
            }
            if(!is_big_endian())
                std::reverse(bytes.c, bytes.c + 4);
            x = bytes.i;
            return *this;
        }

        istream& operator>>(float& x) {
            floatbytes bytes;
            for(int i = 0; i < 4; i++) {
                if((bytes.c[i] = get()) == EOF) {
                    setstate(std::ios::failbit);
                    return *this;
                }
            }
            if(!is_big_endian())
                std::reverse(bytes.c, bytes.c + 4);
            x = bytes.f;
            return *this;
        }
    };
}

int main() {
    using namespace std;
    {
        filebuf buf;
        buf.open("ex15.txt", ios::out);
        ch15::ostream out(&buf);
        out << 1;
        out << 1234;
        out << 987654321;
        out << 4.32f;
        out << 0.0f;
        out << 1.0f / 0.0f;
    }
    {
        filebuf buf;
        buf.open("ex15.txt", ios::in);
        ch15::istream in(&buf);
        uint32_t x, y, z;
        in >> x;
        in >> y;
        in >> z;
        float a, b, c;
        in >> a;
        in >> b;
        in >> c;
        cout << x << ' ' << y << ' ' << z << ' ' << a << ' ' << b << ' ' << c << endl;
    }
}