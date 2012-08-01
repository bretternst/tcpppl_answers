#include <iostream>
#include <cstring>

using std::istream;
using std::ostream;

// Exercise 2 defines the String class.

// Exercise 7 adds the external iterator
// The big advantage of an external iterator over an internal
// one is the ability to hold multiple iterators pointing to
// different locations in the string at the same time. If
// the string kept an internal iterator, this would not be
// possible without making copies of the string.
//
// The disadvantage is that the external iterator could become
// invalidated if the string changes, unles there is a bunch
// of plumbing for the string to keep all of its external
// iterators updated. An internal iterator could be directly
// modified or invalidated if the string changes.

// Exercise 8 adds the non-mutable substring function.

// Exercise 9 adds the substring replace functionality.

// Exercise 10 adds the C-style string conversions.
//
// This string class implements the unary * operator which returns
// a C-style string (char *).
//
// One major disadvantage to performing the operation this way is that
// the memory allocation and de-allocation for the string is split up
// between the string class and the consumer of the string class.
//
// An alternative approach would be to have the consumer allocate memory
// and pass a pointer to the string class to fill.
//
// Further, a read-only C-style string could be returned similar to how
// std::string's c_str() function works. This would be problematic for our
// string class because if the consumer decided to modify the contents of
// that buffer after casting away the const-ness, all instances of the string
// with that same implementation would be affected.

// Exercise 11 adds a simple pattern matching function.
// The function returns true if the string matches the provided pattern and
// false if it does not. Wildcards (*) may be used in the pattern.

class String
{
    friend class StringIterator;

    struct Srep;
    Srep* rep;

    class Cref;
    class Sref;

    bool is_match(const String&, int, int);
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

    Sref operator()(int start, int count);
    String operator()(int start, int count) const;

    const char* c_str();
    char* operator*();

    bool is_match(const String&);
};

struct String::Srep
{
    char* s;
    int sz;
    int n;

    Srep(int nsz, const char* p)
    {
        n = 1;
        sz = nsz;
        s = new char[sz+1];
        strcpy(s,p);
    }

    ~Srep() { delete[] s; }

    Srep* copy()
    {
        if(n==1) return this;
        n--;
        return new Srep(sz, s);
    }

    void assign(int nsz, const char* p)
    {
        if(sz != nsz) {
            delete[] s;
            sz = nsz;
            s = new char[sz+1];
        }
        strncpy(s,p,sz);
        s[sz] = 0;
    }

private:
    Srep(const Srep&);
    Srep& operator=(const Srep&);
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

class String::Sref
{
    friend class String;
    String& s;
    int start, count;
    Sref(String& ss, int start, int count) : s(ss), start(start), count(count) {}
    Sref(const Sref& r) : s(r.s), start(r.start), count(r.count) {}
    Sref();
public:
    operator String() { String ret; ret.rep->assign(count, &s.rep->s[start]); return ret; }
    void operator=(const char* p)
    {
        s.rep = s.rep->copy();
        int newsz = strlen(p);
        int sz = s.rep->sz - count + newsz;
        char* tmp = new char[sz+1];
        char* after = &s.rep->s[start+count];
        strncpy(tmp, s.rep->s, start);
        strncpy(&tmp[start], p, newsz);
        strcpy(&tmp[start+newsz], after);
        s.rep->assign(sz, tmp);
        delete[] tmp;
    }
};

//
// String functions
//
String::String()
{
    rep = new Srep(0, "");
}

String::String(const char* s)
{
    rep = new Srep(strlen(s), s);
}

String::String(const String& s)
{
    s.rep->n++;
    rep = s.rep;
}

String& String::operator=(const char* s)
{
    if(rep->n == 1) {
        rep->assign(strlen(s), s);
    }
    else
    {
        rep->n--;
        rep = new Srep(strlen(s), s);
    }
    return *this;
}

String& String::operator=(const String& s)
{
    s.rep->n++;
    if(--(rep->n) == 0) delete rep;
    rep = s.rep;
    return *this;
}

String::~String()
{
    if(--(rep->n) == 0) delete rep;
}


String& String::operator+=(const String& x)
{
    return *this += x.rep->s;
}

String& String::operator+=(const char* x)
{
    int sz = rep->sz+strlen(x);
    char* s = new char[sz+1];
    strcpy(s, rep->s);
    strcat(s, x);
    rep->assign(sz, s);
    delete s;
    return *this;
}

String::Cref String::operator[](int i) { check(i); return Cref(*this, i); }
char String::operator[](int i) const { check(i); return rep->s[i]; }

String::Sref String::operator()(int start, int count) { return Sref(*this, start, count); }

String String::operator()(int pos, int n) const
{
    check(pos);
    check(pos+n-1);
    char* ss = new char[n+1];
    strncpy(ss, rep->s+pos, n);
    ss[n] = 0;
    return String(ss);
}

ostream& operator<<(ostream& o, const String& x)
{
    o << x.rep->s;
    return o;
}

const int INPUT_BUF = 255;
istream& operator>>(istream& i, String& x)
{
    // I purposely avoid using std::string here. Using string
    // would have made this code much more clear. Just another
    // example of why you shouldn't avoid STL.

    char* s = new char[INPUT_BUF+1];
    int n = 0;
    int sz = INPUT_BUF;
    char ch;

    while(i && i.get(ch) && !isspace(ch))
    {
        s[n++] = ch;
        if(n > sz)
        {
            s[n]=0;
            char* s2 = new char[sz * 2 + 1];
            strcpy(s2,s);
            delete s;
            s = s2;
            sz *= 2;
        }
    }
    s[n]=0;

    x = String(s);
    delete s;
    return i;
}

bool operator==(const String& x, const char* s) { return strcmp(x.rep->s, s)==0; }
bool operator==(const String& x, const String& y) { return strcmp(x.rep->s, y.rep->s)==0; }
bool operator!=(const String& x, const char* s) { return strcmp(x.rep->s, s)!=0; }
bool operator!=(const String& x, const String& y) { return strcmp(x.rep->s, y.rep->s)!=0; }

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

int String::size() const { return rep->sz; }
void String::check(int i) const { if(i<0 || i>=rep->sz) throw RangeError(); }
char String::read(int i) { return rep->s[i]; }
void String::write(int i, char c) { rep = rep->copy(); rep->s[i] = c; }

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
    return rep->s;
}

// Memory allocated here - needs to be freed by the consumer!
char* String::operator*()
{
    char* tmp = new char[rep->sz+1];
    strncpy(tmp,rep->s,rep->sz);
    tmp[rep->sz] = 0;
    return tmp;
}

bool String::is_match(const String& pattern, int pPos, int sPos)
{
    if(pattern.size() < pPos+1 ||
        (pattern.size() == pPos+1 && pattern[pPos] == '*')) return true;
    if(pattern[pPos] == '*')
    {
        ConstStringIterator p(pattern);
        ConstStringIterator i(*this);
        for(int idx = 0; idx < size()-sPos; idx++)
        {
            p.move(pPos+1);
            i.move(sPos+idx);
            bool match = true;
            while(!i.is_past_end() && !p.is_past_end())
            {
                if(*p == '*') return is_match(pattern, p.position(), i.position());
                if(*i != *p) {match = false; break;}
                i.next(); p.next();
            }
            if(match)
                return i.is_past_end() && (p.is_past_end() || (p.position()==pattern.size()-1 && *p=='*'));
        }
        return false;
    }
    else
    {
        ConstStringIterator p(pattern);
        ConstStringIterator i(*this);
        while(!i.is_past_end() && !p.is_past_end())
        {
            if(*p == '*') return is_match(pattern, p.position(), i.position());
            if(*i != *p) return false;
            i.next(); p.next();
        }
        return i.is_past_end() && (p.is_past_end() || (p.position()==pattern.size()-1 && *p=='*'));
    }
}

bool String::is_match(const String& pattern)
{
    return is_match(pattern, 0, 0);
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

    cout << endl << "Exercise 8:" << endl;
    String s2("hello world");
    cout << s2(1, 4) << endl;

    cout << endl << "Exercise 9:" << endl;
    String x2("Hello world!");
    const String y2("Hello world!");

    cout << y2 << endl;
    cout << y2(1,4) << endl;

    x2(6,5) = "everybody";
    cout << x2 << endl;

    x2(5,10) = "";
    cout << x2 << endl;

    cout << endl << "Exercise 10:" << endl;
    String s3("Hello world!");
    cout << s3.c_str() << endl;
    char* cs = *s3;
    cout << cs << endl;
    delete[] cs;

    cout << endl << "Exercise 11:" << endl;
    String ex11("Hello world!");
    cout << "Should be true: " << ex11.is_match("*") << endl;
    cout << "Should be true: " << ex11.is_match("Hello*") << endl;
    cout << "Should be true: " << ex11.is_match("Hello world!") << endl;
    cout << "Should be true: " << ex11.is_match("*lo world!") << endl;
    cout << "Should be true: " << ex11.is_match("Hel*orld!") << endl;
    cout << "Should be true: " << ex11.is_match("Hel*o*d!") << endl;
    cout << "Should be true: " << ex11.is_match("*ello worl*") << endl;
    cout << "Should be true: " << ex11.is_match("*Hello world!") << endl;
    cout << "Should be true: " << ex11.is_match("Hello world!*") << endl;
    cout << "Should be true: " << ex11.is_match("*") << endl;

    cout << "Should be false: " << ex11.is_match("Hello world") << endl;
    cout << "Should be false: " << ex11.is_match("*Hello world") << endl;
    cout << "Should be false: " << ex11.is_match("Hello* world") << endl;
    cout << "Should be false: " << ex11.is_match("x*") << endl;
    cout << "Should be false: " << ex11.is_match("*x") << endl;
    cout << "Should be false: " << ex11.is_match("Hel*x*orld!") << endl;
}
