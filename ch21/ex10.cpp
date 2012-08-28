#include <cstdio>
#include <string>

// Similar to the last exercise, I don't want to implement
// the whole thing, so I'll go with a few basic I/O operations.
// As noted in exercise 8, a real implementation would use
// char_traits instead of supporting char only.

// When implementing things using C I/O functions, we'll
// replace streambuf with the FILE struct and use it for
// all operations intead.
namespace ch21 {
    class ios {
        int state;

    public:
        enum iostate { eofbit = 1, failbit = 2 };

        ios() : state(0) { }

        bool good() { return !state; }
        bool eof() { return !(state & eofbit); }
        bool fail() { return !(state & failbit); }
        bool operator!() { return !good(); }
        operator void*() { return good() ? this : 0; }
        void clear() { state = 0; }
        void setstate(int f) { state |= f; }
    };

    class ostream : public ios {
        FILE* f;

    public:
        ostream(FILE* f) : f(f) { }
        ~ostream() { }

        FILE* file() const { return f; }

        ostream& operator<<(char ch) {
            if(fputc(ch, f) == EOF)
                setstate(ios::failbit);
            return *this;
        }

        ostream& operator<<(const char* s) {
            if(fputs(s, f) == EOF)
                setstate(ios::failbit);
            return *this;
        }

        ostream& operator<<(std::string s) {
            if(fputs(s.c_str(), f) == EOF)
                setstate(ios::failbit);
            return *this;
        }

        ostream& operator<<(int x) {
            if(fprintf(f, "%d", x) == EOF)
                setstate(ios::failbit);
            return *this;
        }
    };

    class ofstream : public ostream {
        bool m_isopen;
    public:
        explicit ofstream(const char* file) : ostream(fopen(file, "w")) {
            m_isopen = true;
            if(!this->file()) {
                setstate(ios::failbit);
                m_isopen = false;
            }
        }

        ~ofstream() { close(); }
        void close() { if(m_isopen) fclose(file()); m_isopen = false; }
        bool is_open() const { return m_isopen; }
    };

    class istream : public ios {
        FILE* f;

    public:
        istream(FILE* f) : f(f) { }
        ~istream() { }

        FILE* file() const { return f; }

        istream& operator>>(char& ch) {
            int tmp;
            if ((tmp = fgetc(f)) == EOF) {
                setstate(ios::eofbit | ios::failbit);
            }
            else {
                ch = tmp;
            }
            return *this;
        }

        istream& operator>>(std::string& s) {
            char ch;
            while(*this >> ch) {
                if(!isspace(ch)) {
                    s.push_back(ch);
                    break;
                }
            }
            while(*this >> ch) {
                if(isspace(ch)) {
                    ungetc(ch, f);
                    break;
                }
                s.push_back(ch);
            }
            return *this;
        }

        istream& operator>>(int& x) {
            char ch;
            while(*this >> ch) {
                if(!isspace(ch)) {
                    if(!isdigit(ch)) {
                        ungetc(ch, f);
                        break;
                    }
                    x = ch - '0';
                    while(*this >> ch) {
                        if(!isdigit(ch)) {
                            ungetc(ch, f);
                            break;
                        }
                        x *= 10;
                        x += ch - '0';
                    }
                    return *this;
                }
            }
            setstate(ios::failbit);
            return *this;
        }
    };

    class ifstream : public istream {
        bool m_isopen;

    public:
        explicit ifstream(const char* file) : istream(fopen(file, "r")) {
            m_isopen = true;
            if(!this->file()) {
                setstate(ios::failbit);
                m_isopen = false;
            }
        }

        ~ifstream() { close(); }
        void close() { if(m_isopen) fclose(file()); m_isopen = false; }
        bool is_open() const { return m_isopen; };
    };
}

int main() {
    using namespace ch21;

    ofstream fout("ex10.txt");
    fout << "foo bar " << 12345 << "\n";
    fout.close();

    ifstream fin("ex10.txt");

    std::string s1;
    std::string s2;
    int x;
    fin >> s1;
    fin >> s2;
    fin >> x;

    ostream out(stdout);
    out << s1 << ' ' << s2 << ' ' << x << '\n';
}
