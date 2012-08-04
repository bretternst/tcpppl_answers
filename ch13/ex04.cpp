#include <iostream>

using std::istream;
using std::ostream;

// This is the string class as originally defined in chapter 11.
// Exercise 4 converts this class into a template.

template<class T> class String
{
    template<class> friend class StringIterator;

    struct Srep;
    Srep* rep;

    class Cref;
    class Sref;

    bool IsMatch(const String<T>&, int, int);
public:
    class RangeError {};

    String();
    String(const T*);
    String(const String<T>&);
    String& operator=(const T*);
    String<T>& operator=(const String<T>&);
    ~String();

    String<T>& operator+=(const String<T>& x);
    String& operator+=(const T* x);
    typename String<T>::Cref operator[](int i);
    T operator[](int i) const;

    template<class T> friend ostream& operator<<(ostream&, const String<T>&);
    bool operator==(const T* s);
    bool operator==(const String<T>& y);
    bool operator!=(const T* s);
    bool operator!=(const String<T>& y);

    int Size() const;
    void Check(int i) const;
    char Read(int i);
    void Write(int i, char c);

    Sref operator()(int start, int count);
    String<T> operator()(int start, int count) const;

    const T* c_str();
    T* operator*();

    bool IsMatch(const String<T>&);
};

template<class T> struct String<T>::Srep
{
    T* s;
    int sz;
    int n;

    Srep(int nsz, const T* p)
    {
        n = 1;
        sz = nsz;
        s = new T[sz+1];
        for(int i = 0; i < sz; i++) s[i] = p[i];
        s[sz]=0;
    }

    ~Srep() { delete[] s; }

    Srep* Copy()
    {
        if(n==1) return this;
        n--;
        return new Srep(sz, s);
    }

    void Assign(int nsz, const T* p)
    {
        if(sz != nsz) {
            delete[] s;
            sz = nsz;
            s = new char[sz+1];
        }
        for(int i = 0; i < sz; i++) s[i] = p[i];
        s[sz] = 0;
    }

private:
    Srep(const Srep&);
    Srep& operator=(const Srep&);
};

template<class T> class String<T>::Cref
{
    friend class String<T>;
    template<class> friend class StringIterator;
    String<T>& s;
    int i;
    Cref(String<T>& ss, int ii) : s(ss), i(ii) {}
    Cref(const Cref& r) : s(r.s), i(r.i) {}
    Cref();
public:
    operator T() const { s.Check(i); return s.Read(i); }
    void operator=(T c) { s.Write(i, c); }
};

template<class T> class String<T>::Sref
{
    friend class String<T>;
    String<T>& s;
    int start, count;
    Sref(String<T>& ss, int start, int count) : s(ss), start(start), count(count) {}
    Sref(const Sref& r) : s(r.s), start(r.start), count(r.count) {}
    Sref();
public:
    operator String<T>() { String<T> ret; ret.rep->Assign(count, &s.rep->s[start]); return ret; }
    operator T*() { return s.rep->s; }
    const Sref& operator=(T* x)
    {
        s.rep = s.rep->Copy();
        int xsz = 0;
        while(x[xsz++]);
        xsz--;

        int sz = s.rep->sz - count + xsz;
        T* tmp = new T[sz+1];

        int i = 0;
        for(int j = 0; j < start; j++) tmp[i++] = s.rep->s[j];
        for(int j = 0; j < xsz; j++) tmp[i++] = x[j];
        for(int j = start+count; j < s.rep->sz; j++) tmp[i++] = s.rep->s[j];

        s.rep->Assign(sz, tmp);
        delete[] tmp;
        return *this;
    }
};

//
// String functions
//
template<class T> String<T>::String()
{
    rep = new Srep(0, "");
}

template<class T> String<T>::String(const T* s)
{
    int xsz = 0;
    while(s[xsz++]);
    xsz--;

    rep = new Srep(xsz, s);
}

template<class T> String<T>::String(const String& s)
{
    s.rep->n++;
    rep = s.rep;
}

template<class T> String<T>& String<T>::operator=(const T* x)
{
    int xsz = 0;
    while(x[xsz++]);
    xsz--;

    if(rep->n == 1) {
        rep->Assign(xsz, s);
    }
    else
    {
        rep->n--;
        rep = new Srep(xsz, s);
    }
    return *this;
}

template<class T> String<T>& String<T>::operator=(const String& s)
{
    s.rep->n++;
    if(--(rep->n) == 0) delete rep;
    rep = s.rep;
    return *this;
}

template<class T> String<T>::~String()
{
    if(--(rep->n) == 0) delete rep;
}


template<class T> String<T>& String<T>::operator+=(const String<T>& x)
{
    return *this += x.rep->s;
}

template<class T> String<T>& String<T>::operator+=(const T* x)
{
    int xsz = 0;
    while(x[xsz++]);
    xsz--;

    int sz = rep->sz+xsz;
    T* s = new T[sz+1];
    int i = 0;
    for(int j = 0; j < rep->sz; j++) s[i++] = rep->s[j];
    for(int j = 0; j < xsz; j++) s[i++] = x[j];
    s[sz] = 0;
    rep->Assign(sz, s);
    delete s;
    return *this;
}

template<class T> typename String<T>::Cref String<T>::operator[](int i) { Check(i); return Cref(*this, i); }
template<class T> T String<T>::operator[](int i) const { Check(i); return rep->s[i]; }

template<class T> typename String<T>::Sref String<T>::operator()(int start, int count) { return Sref(*this, start, count); }

template<class T> String<T> String<T>::operator()(int pos, int n) const
{
    Check(pos);
    Check(pos+n-1);
    T* ss = new T[n+1];
    int i = 0;
    for(int j = pos; j < pos+n; j++) ss[i++] = rep->s[j];
    ss[n] = 0;
    return String<T>(ss);
}

template<class T> ostream& operator<<(ostream& o, const String<T>& x)
{
    o << x.rep->s;
    return o;
}

template<class T> bool String<T>::operator==(const T* s) { return *this == String<T>(s); }

template<class T> bool String<T>::operator==(const String<T>& y)
{
    if(y.rep->sz != rep->sz) return false;
    for(int i = 0; i < y.rep->sz; i++) if (rep->s[i] != y.rep->s[i]) return false;
    return true;
}

template<class T> bool String<T>::operator!=(const T* s) { return *this != String<T>(s); }
template<class T> bool String<T>::operator!=(const String<T>& y) { return !(*this == y); }

template<class T> String<T> operator+(const String<T>& x, const String<T>& y)
{
    String<T> s(x);
    s += y;
    return s;
}

template<class T> String<T> operator+(const String<T>& x, const char* y)
{
    String<T> s(x);
    s += y;
    return s;
}

template<class T> int String<T>::Size() const { return rep->sz; }
template<class T> void String<T>::Check(int i) const { if(i<0 || i>=rep->sz) throw RangeError(); }
template<class T> char String<T>::Read(int i) { return rep->s[i]; }
template<class T> void String<T>::Write(int i, char c) { rep = rep->Copy(); rep->s[i] = c; }

using namespace std;

template<class T> class StringIterator {
    int idx;
    String<T>& s;
public:
    StringIterator(String<T>& s) : s(s), idx(0) {}
    void Next() { idx++; }
    void Move(int i) { idx = i; }
    int Position() { return idx; }
    bool IsPastEnd() { return idx >= s.Size(); }
    typename String<T>::Cref operator*() { return s[idx]; }
};

template<class T> class ConstStringIterator {
    int idx;
    const String<T>& s;
public:
    ConstStringIterator(const String<T>& s) : s(s), idx(0) {}
    void Next() { idx++; }
    void Move(int i) { idx = i; }
    int Position() { return idx; }
    bool IsPastEnd() { return idx >= s.Size(); }
    char operator*() { return s[idx]; }
};

template<class T> const T* String<T>::c_str()
{
    return rep->s;
}

// Memory allocated here - needs to be freed by the consumer!
template<class T> T* String<T>::operator*()
{
    T* tmp = new T[rep->sz+1];
    for(int i = 0; i < rep->sz; i++) tmp[i] = rep->s[i];
    tmp[rep->sz] = 0;
    return tmp;
}

template<class T> bool String<T>::IsMatch(const String<T>& pattern, int pPos, int sPos)
{
    if(pattern.Size() < pPos+1 ||
        (pattern.Size() == pPos+1 && pattern[pPos] == '*')) return true;
    if(pattern[pPos] == '*')
    {
        ConstStringIterator<T> p(pattern);
        ConstStringIterator<T> i(*this);
        for(int idx = 0; idx < Size()-sPos; idx++)
        {
            p.Move(pPos+1);
            i.Move(sPos+idx);
            bool match = true;
            while(!i.IsPastEnd() && !p.IsPastEnd())
            {
                if(*p == '*') return IsMatch(pattern, p.Position(), i.Position());
                if(*i != *p) {match = false; break;}
                i.Next(); p.Next();
            }
            if(match)
                return i.IsPastEnd() && (p.IsPastEnd() || (p.Position()==pattern.Size()-1 && *p=='*'));
        }
        return false;
    }
    else
    {
        ConstStringIterator<T> p(pattern);
        ConstStringIterator<T> i(*this);
        while(!i.IsPastEnd() && !p.IsPastEnd())
        {
            if(*p == '*') return IsMatch(pattern, p.Position(), i.Position());
            if(*i != *p) return false;
            i.Next(); p.Next();
        }
        return i.IsPastEnd() && (p.IsPastEnd() || (p.Position()==pattern.Size()-1 && *p=='*'));
    }
}

template<class T> bool String<T>::IsMatch(const String<T>& pattern)
{
    return IsMatch(pattern, 0, 0);
}

template<class T> String<T> f(String<T> a, String<T> b)
{
    a[2] = 'x';
    char c = b[3];
    cout << "in f: " << a << ' ' << b << ' ' << c << '\n';
    return b;
}

int main()
{
    cout << "Exercise 2:" << endl;
    String<char> x("abcd"),y("defg");
    cout << "input: " << x << ' ' << y << endl;
    String<char> z = x;
    y = f(x,y);
    if (x != z) cout << "x corrupted!" << endl;
    z[0] = '!';
    if(x==z) cout << "write failed!" << endl;
    cout << "exit: " << x << ' ' << y << ' ' << z << endl;

    cout << endl << "Exercise 7:" << endl;
    String<char> s("hello world");
    StringIterator<char> i(s);
    while(!i.IsPastEnd())
    {
        cout << *i;
        i.Next();
    }
    cout << endl;

    cout << endl << "Exercise 8:" << endl;
    String<char> s2("hello world");
    cout << s2(1, 4) << endl;

    cout << endl << "Exercise 9:" << endl;
    String<char> x2("Hello world!");
    const String<char> y2("Hello world!");

    cout << y2 << endl;
    cout << y2(1,4) << endl;

    x2(6,5) = "everybody";
    cout << x2 << endl;

    x2(5,10) = "";
    cout << x2 << endl;

    cout << endl << "Exercise 10:" << endl;
    String<char> s3("Hello world!");
    cout << s3.c_str() << endl;
    char* cs = *s3;
    cout << cs << endl;
    delete[] cs;

    cout << endl << "Exercise 11:" << endl;
    String<char> ex11("Hello world!");
    cout << "Should be true: " << ex11.IsMatch("*") << endl;
    cout << "Should be true: " << ex11.IsMatch("Hello*") << endl;
    cout << "Should be true: " << ex11.IsMatch("Hello world!") << endl;
    cout << "Should be true: " << ex11.IsMatch("*lo world!") << endl;
    cout << "Should be true: " << ex11.IsMatch("Hel*orld!") << endl;
    cout << "Should be true: " << ex11.IsMatch("Hel*o*d!") << endl;
    cout << "Should be true: " << ex11.IsMatch("*ello worl*") << endl;
    cout << "Should be true: " << ex11.IsMatch("*Hello world!") << endl;
    cout << "Should be true: " << ex11.IsMatch("Hello world!*") << endl;
    cout << "Should be true: " << ex11.IsMatch("*") << endl;

    cout << "Should be false: " << ex11.IsMatch("Hello world") << endl;
    cout << "Should be false: " << ex11.IsMatch("*Hello world") << endl;
    cout << "Should be false: " << ex11.IsMatch("Hello* world") << endl;
    cout << "Should be false: " << ex11.IsMatch("x*") << endl;
    cout << "Should be false: " << ex11.IsMatch("*x") << endl;
    cout << "Should be false: " << ex11.IsMatch("Hel*x*orld!") << endl;
}
