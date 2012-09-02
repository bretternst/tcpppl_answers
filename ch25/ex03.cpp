#include <string>
#include <iostream>
#include <cstddef>
#include <time.h>

namespace ch25 {
    using std::string;

    // This is mostly busywork, so only adding a subset of the methods.
    class String {
        string s;
    public:
        typedef string::iterator iterator;
        typedef string::const_iterator const_iterator;
        typedef string::reverse_iterator reverse_iterator;
        typedef string::const_reverse_iterator const_reverse_iterator;

        String() { }
        String(const String& str) : s(str.s) { }
        String(const std::string str) : s(str) { }
        String(const char* str) : s(str) { }
        virtual ~String() { }

        virtual iterator begin() { return s.begin(); }
        virtual iterator end() { return s.end(); }
        virtual reverse_iterator rbegin() { return s.rbegin(); }
        virtual reverse_iterator rend() { return s.rend(); }
        virtual size_t size() { return s.size(); }
        size_t length() { return size(); }
        virtual void resize(size_t n, char c) { s.resize(n, c); }
        virtual void resize(size_t n) { s.resize(n); }
        virtual size_t capacity() { return s.capacity(); }
        virtual void reserve(size_t n) { return s.reserve(n); }
        virtual void clear() { return s.clear(); }
        virtual bool empty() { return s.empty(); }

        virtual const char& operator[](size_t pos) const { return s[pos]; }
        virtual char& operator[](size_t pos) { return s[pos]; }

        virtual String& append(const String& str) { s.append(str.s); return *this; }
        virtual String& append(const String& str, size_t pos, size_t n) { s.append(str.s, pos, n); return *this; }
        virtual void push_back(char c) { s.push_back(c); }
        virtual String& assign(const String& str) { s.assign(str.s); return *this; }
        virtual String& assign(const String& str, size_t pos, size_t n) { s.assign(str.s, pos, n); return *this; }
        virtual String& insert(size_t pos1, const String& str) { s.insert(pos1, str.s); return *this; }
        virtual String& insert(size_t pos1, const String& str, size_t pos2, size_t n) { s.insert(pos1, str.s, pos2, n); return *this; }
        virtual String& erase(size_t pos = 0, size_t n = string::npos) { s.erase(pos, n); return *this; }

        virtual String& replace(size_t pos1, size_t n1, const String& str) { s.replace(pos1, n1, str.s); return *this; }
        virtual String& replace(size_t pos1, size_t n1, const String& str, size_t pos2, size_t n2) { s.replace(pos1, n1, str.s, pos2, n2); return *this; }
        virtual void swap(String& str) { s.swap(str.s); }
    };

    // You might want to do this when you want to change some specific behavior
    // of a string, but not reimplement the entire thing. In general, this smells
    // like a bad idea; the existing methods have specific expected behaviors.
    // It might be useful for adding some custom string functions as member
    // methods that don't already exist, or for highly specialized rules for
    // string handling.
    class String2 : public String {
    public:
        String2(const char* str) : String(str) { }
    };
}

unsigned long diff(timespec start, timespec end) {
    timespec tmp;
    if((end.tv_nsec-start.tv_nsec)<0) {
        tmp.tv_sec = end.tv_sec-start.tv_sec-1;
        tmp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
        tmp.tv_sec = end.tv_sec-start.tv_sec;
        tmp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return tmp.tv_sec * 1000000000 + tmp.tv_nsec;
}

template<class T>
unsigned long measure() {
    T s("foo bar");
    timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i < 1000000; i++) {
        s[0] = 'X';
        s.push_back('0');
    }
    clock_gettime(CLOCK_REALTIME, &end);
    return diff(start, end);
}

int main() {
    using namespace std;

    // The point of this basically seems to be measuring virtual
    // method call overhead.

    cout << "Standard string: " << measure<string>() << endl;
    cout << "String2 with virtuals: " << measure<ch25::String2>() << endl; // A little slower, but not remarkably so.
}
