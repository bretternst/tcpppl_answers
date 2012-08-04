#include <iostream>

enum Type {S, I};

class Entry
{
    char* name;
    Type t;
    union {
        const char* s;
        int i;
    };
public:
    class InvalidOperationError {};

    Entry() {}
    Entry(const char* s) : t(S), s(s) {}
    Entry(int i) : t(I), i(i) {}
    const char* get_s()
    {
        if(t != S)
            throw InvalidOperationError();
        return s;
    }
    int get_i()
    {
        if(t != I)
            throw InvalidOperationError();
        return i;
    }
    void set(const char* str)
    {
        t = S;
        s = str;
    }
    void set(int num)
    {
        t = I;
        i = num;
    }
};

int main()
{
    using namespace std;

    Entry e1("hello");
    Entry e2(4);
    cout << e1.get_s() << endl;
    cout << e2.get_i() << endl;
    e2.set("goodbye");
    cout << e2.get_s() << endl;
}
