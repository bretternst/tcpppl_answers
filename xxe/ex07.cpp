#include <iostream>
#include <cstring>

using std::istream;
using std::ostream;

// Copied from chapter 11 String class and modified.

// Since char doesn't have user defined operator= and copy ctor to worry about
// the only thing that can really throw is allocation. So we handle all
// allocations here in a careful order so that exceptions don't cause the string
// rep to become invalid.

class String
{
    friend class StringIterator;

    struct Srep {
        char* s;
        int sz;
        int n;

        std::allocator<char> alloc;

        Srep(int nsz, const char* p) : sz(nsz), n(1)
        {
            s = alloc.allocate(sz + 1);
            strcpy(s,p);
        }

        ~Srep() { alloc.deallocate(s, sz + 1); }

        void assign(int nsz, const char* p)
        {
            if(sz != nsz) {
                // reordered to preserve validity in case of exception
                char* snew = alloc.allocate(nsz + 1);
                alloc.deallocate(s, sz + 1);
                sz = nsz;
                s = snew;
            }
            strncpy(s,p,sz);
            s[sz] = 0;
        }

    private:
        Srep(const Srep&);
        Srep& operator=(const Srep&);

    };

    Srep rep;
    std::allocator<char> alloc;

    class Cref;

public:
    class RangeError {};

    String();
    String(const char*);
    String(const String&);
    String& operator=(const char*);
    String& operator=(const String&);
    ~String();

    String& operator+=(const String& x);
    String& operator+=(const char* x);
    Cref operator[](int i);
    char operator[](int i) const;

    friend ostream& operator<<(ostream&, const String&);
    friend istream& operator>>(istream&, String&);
    friend bool operator==(const String& x, const char* s);
    friend bool operator==(const String& x, const String& y);
    friend bool operator!=(const String& x, const char* s);
    friend bool operator!=(const String& x, const String& y);

    int size() const;
    void check(int i) const;
    char read(int i);
    void write(int i, char c);

    const char* c_str();
};

class String::Cref
{
    friend class String;
    friend class StringIterator;
    String& s;
    int i;
    Cref(String& ss, int ii) : s(ss), i(ii) {}
    Cref(const Cref& r) : s(r.s), i(r.i) {}
    Cref();
public:
    operator char() const { s.check(i); return s.read(i); }
    void operator=(char c) { s.write(i, c); }
};

//
// String functions
//
String::String() : rep(0, "") { }

String::String(const char* s) : rep(strlen(s), s) { }

String::String(const String& s) : rep(s.size(), s.rep.s)
{
}

String& String::operator=(const char* s)
{
    rep.assign(strlen(s), s);
    return *this;
}

String& String::operator=(const String& s)
{
    rep.assign(s.size(), s.rep.s);
    return *this;
}

String::~String()
{
}

String& String::operator+=(const String& x)
{
    return *this += x.rep.s;
}

String& String::operator+=(const char* x)
{
    int sz = rep.sz+strlen(x);
    char* s = alloc.allocate(sz + 1);
    strcpy(s, rep.s);
    strcat(s, x);
    rep.assign(sz, s);
    alloc.deallocate(s, sz + 1);
    return *this;
}

String::Cref String::operator[](int i) { check(i); return Cref(*this, i); }
char String::operator[](int i) const { check(i); return rep.s[i]; }

ostream& operator<<(ostream& o, const String& x)
{
    o << x.rep.s;
    return o;
}

bool operator==(const String& x, const char* s) { return strcmp(x.rep.s, s)==0; }
bool operator==(const String& x, const String& y) { return strcmp(x.rep.s, y.rep.s)==0; }
bool operator!=(const String& x, const char* s) { return strcmp(x.rep.s, s)!=0; }
bool operator!=(const String& x, const String& y) { return strcmp(x.rep.s, y.rep.s)!=0; }

String operator+(const String& x, const String& y)
{
    String s(x);
    s += y;
    return s;
}

String operator+(const String& x, const char* y)
{
    String s(x);
    s += y;
    return s;
}

int String::size() const { return rep.sz; }
void String::check(int i) const { if(i<0 || i>=rep.sz) throw RangeError(); }
char String::read(int i) { return rep.s[i]; }
void String::write(int i, char c) { rep.s[i] = c; }

using namespace std;

class StringIterator {
    int idx;
    String& s;
public:
    StringIterator(String& s) : s(s), idx(0) {}
    void next() { idx++; }
    void move(int i) { idx = i; }
    int position() { return idx; }
    bool is_past_end() { return idx >= s.size(); }
    String::Cref operator*() { return s[idx]; }
};

class ConstStringIterator {
    int idx;
    const String& s;
public:
    ConstStringIterator(const String& s) : s(s), idx(0) {}
    void next() { idx++; }
    void move(int i) { idx = i; }
    int position() { return idx; }
    bool is_past_end() { return idx >= s.size(); }
    char operator*() { return s[idx]; }
};

const char* String::c_str()
{
    return rep.s;
}

String f(String a, String b)
{
    a[2] = 'x';
    char c = b[3];
    cout << "in f: " << a << ' ' << b << ' ' << c << '\n';
    return b;
}

int main()
{
    cout << "Exercise 2:" << endl;
    String x("abcd"),y("defg");
    cout << "input: " << x << ' ' << y << endl;
    String z = x;
    y = f(x,y);
    if (x != z) cout << "x corrupted!" << endl;
    z[0] = '!';
    if(x==z) cout << "write failed!" << endl;
    cout << "exit: " << x << ' ' << y << ' ' << z << endl;

    cout << endl << "Exercise 7:" << endl;
    String s("hello world");
    StringIterator i(s);
    while(!i.is_past_end())
    {
        cout << *i;
        i.next();
    }
    cout << endl;
}
