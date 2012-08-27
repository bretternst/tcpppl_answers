#include <iostream>
#include <string>
#include <algorithm>
#include <cstddef>
#include <cctype>
#include <unistd.h>
#include <fcntl.h>

// This is an EXTREMELY simplified implementation of streambuf and streams.
// For example, seeking isn't supported, only basic input/output operations.
// Also, a real implementation would use char traits instead of being
// hard-coded to use char only. Many functions of ios and istream are
// missing.
// Additionally, a real implementation would need a lot more error handling.
namespace ch21 {
    typedef size_t streamsize;
    typedef size_t streampos;
    typedef size_t streamoff;

    const int EOF = -1;

    class ios_base {
    public:
        enum openmode { in = 0, out = 1 };
    };

    class streambuf {
    private:
        char* m_eback;
        char* m_gptr;
        char* m_egptr;

        char* m_pbase;
        char* m_pptr;
        char* m_epptr;
    public:
        streambuf() { }
        ~streambuf() { }

        // buffer management
        streambuf* pubsetbuf(char* s, streamsize n) { return setbuf(s, n); }
        int pubsync() { return sync(); }

        // input
        streamsize in_avail() {
            if (gptr() && gptr() < egptr())
                return egptr() - gptr();
            else
                return showmanyc();
        }

        int snextc() {
            if(!m_gptr || ++m_gptr == m_egptr)
                return uflow();
            return *m_gptr;
        };

        int sbumpc() {
            if(!m_gptr || m_gptr == m_egptr)
                return uflow();
            else
                return *(m_gptr++);
        };

        int sgetc() {
            if(!gptr() || gptr() == egptr()) {
                return underflow();
            }
            else {
                return *m_gptr;
            }
        }

        // output
        int sputc(char c) {
            if(!pptr() || pptr() == epptr())
                return overflow(c);
            else
                return *(m_pptr++) = c;
        }

    protected:
        // input sequence
        char* eback() const { return m_eback; }
        char* gptr() const { return m_gptr; }
        char* egptr() const { return m_egptr; }
        void gbump(int n) { m_gptr += n; };
        void setg(char* gbeg, char* gnext, char* gend) { m_eback = gbeg; m_gptr = gnext; m_egptr = gend; }

        // output sequence
        char* pbase() const { return m_pbase; }
        char* pptr() const { return m_pptr; }
        char* epptr() const { return m_epptr; }
        void pbump(int n) { m_pptr += n; }
        void setp(char* pbeg, char* pend) { m_pptr = m_pbase = pbeg; m_epptr = pend; }

        // buffer management
        virtual streambuf* setbuf(char* s, streamsize n) = 0;
        virtual int sync() = 0;

        // input functions
        virtual streamsize showmanyc() = 0;
        virtual int underflow() = 0;
        virtual int uflow() = 0;

        // output functions
        virtual int overflow(int c = EOF) = 0;
    };

    class filebuf : public streambuf {
        char* buf;
        streamsize sz;
        int fd;
        ios_base::openmode mode;

    public:
        filebuf() : fd(-1), buf(0), sz(0) { setp(0, 0); }
        ~filebuf() { close(); }

        // file management
        filebuf* open(const char* s, ios_base::openmode mode) {
            this->mode = mode;
            if(mode == ios_base::out)
                fd = ::open(s, O_CREAT | O_WRONLY, 0644);
            else
                fd = ::open(s, O_RDONLY);
            return fd >= 0 ? this : 0;
        }

        filebuf* close() {
            if(fd >= 0) {
                if(mode == ios_base::out && pptr() > pbase())
                    sync();
                ::close(fd);
                return this;
            }
            else
                return 0;
        }

        // buffer management
        streambuf* setbuf(char* s, streamsize n) {
            buf = s;
            sz = n;
            if(mode == ios_base::in)
                setg(0, 0, 0);
            else
                setp(buf, buf + sz);
            return this;
        }

        int sync() {
            if(!buf)
                return EOF;
            if(mode == ios_base::out) {
                if(::write(fd, pbase(), pptr()-pbase()) < 0)
                    return EOF;
                setp(buf, buf + sz);
                return 0;
            }
        }

        // input
        streamsize showmanyc() { return 0; }
        int underflow() {
            int count = read(fd, buf, sz);
            if(count <= 0)
                return EOF;
            setg(buf, buf, buf + count);
            return *buf;
        }

        int uflow() {
            int ch = underflow();
            if(ch != EOF)
                gbump(1);
        }

        // output
        int overflow(int c = EOF) {
            if(mode == ios_base::out) {
                if(!buf) {
                    char ch = static_cast<char>(c);
                    ::write(fd, &ch, 1);
                }
                else {
                    sync();
                    sputc(c);
                }
            }
        }
    };

    class ios : public ios_base {
        int state;

    public:
        enum iostate { eofbit = 1, failbit = 2 };

        bool good() { return !state; }
        bool eof() { return !(state & eofbit); }
        bool fail() { return !(state & failbit); }
        bool operator!() { return !good(); }
        operator void*() { return good() ? this : 0; }
        void clear() { state = 0; }
        void setstate(int f) { state |= f; }
    };

    class ostream : public ios {
        streambuf* sb;
    public:
        explicit ostream(streambuf* sb) : sb(sb) { }

        ostream& operator<<(char c) {
            sb->sputc(c);
            return *this;
        }

        ostream& operator<<(std::string s) {
            for(int i = 0; i < s.size(); i++)
                sb->sputc(s[i]);
            return *this;
        }

        ostream& operator<<(int x) {
            std::string tmp;
            if(x == 0)
                tmp.push_back('0');
            else {
                while(x > 0) {
                    tmp.push_back('0' + (x % 10));
                    x /= 10;
                }
                std::reverse(tmp.begin(), tmp.end());
            }
            *this << tmp;
            return *this;
        }
    };

    class istream : public ios {
        streambuf* sb;
    public:
        explicit istream(streambuf* sb) : sb(sb) { }

        istream& operator>>(char& c) {
            int ch = sb->sbumpc();
            if(ch == EOF)
                setstate(ios::eofbit);
            else
                c = ch;
            return *this;
        }

        istream& operator>>(int& x) {
            int ch;
            while(ch = sb->sgetc()) {
                if(ch == EOF) {
                    setstate(ios::failbit | ios::eofbit);
                    break;
                }
                else if(isspace(ch)) {
                    sb->sbumpc();
                    continue;
                }
                else if (isdigit(ch)) {
                    x = ch - '0';
                    sb->sbumpc();
                    break;
                }
                setstate(ios::failbit);
                return *this;
            }
            while(ch = sb->sgetc()) {
                if(ch == EOF) {
                    setstate(ios::eofbit);
                    break;
                }
                if(!isdigit(ch))
                    break;
                sb->sbumpc();
                x *= 10;
                x += ch - '0';
            }
            return *this;
        }
    };

    class ofstream : public ostream {
        filebuf fb;
        char buf[1024];
    public:
        explicit ofstream(const char* file) : ostream(&fb) {
            fb.setbuf(buf, 1024);
            fb.open(file, ios_base::out);
        }
    };

    class ifstream : public istream {
        filebuf fb;
        char buf[1024];
    public:
        explicit ifstream(const char* file) : istream(&fb) {
            fb.setbuf(buf, 1024);
            fb.open(file, ios_base::in);
        }
    };
}

int main() {
    using namespace ch21;

    ofstream out("ex21.txt");
    for(int i = 0; i < 5; i++)
        out << i << ' ';

    ifstream in("ex21.txt");
    int i;
    while(in >> i) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}
