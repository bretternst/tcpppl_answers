#include <iostream>
#include <sstream>
#include "time.h"

// To measure actual function call time, prevent inlining:
// g++ -o ex17 ex17.cpp -fno-inline-functions -O0 -lrt

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

int dummy;

void f() { dummy++; }

struct A {
    virtual void f() { }
};

struct B : public A {
    void f() { dummy++; }
};

int main() {
    using namespace std;

    timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i < 1000000000; i++)
        f();
    clock_gettime(CLOCK_REALTIME, &end);
    cout << "function call time: " << (diff(start, end) / 1000000000) << endl;

    A* a = new B();
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i < 1000000000; i++)
        a->f();
    clock_gettime(CLOCK_REALTIME, &end);
    delete a;
    cout << "virtual function call time: " << (diff(start, end) / 1000000000) << endl;

    string s(1000000, 'x');
    char c;
    istringstream in(s);    
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i < 1000000; i++)
        in >> c;
    clock_gettime(CLOCK_REALTIME, &end);
    cout << "reading a char: " << (diff(start, end) / 1000000) << endl;

    s.clear();
    for(int i = 0; i < 1000000; i++)
        s += "1 ";
    int i;
    istringstream in2(s);
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i < 1000000; i++)
        in2 >> i;
    clock_gettime(CLOCK_REALTIME, &end);
    cout << "reading a 1-digit int: " << (diff(start, end) / 1000000) << endl;

    s.clear();
    for(int i = 0; i < 1000001; i++)
        s += "12345 ";
    istringstream in3(s);
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i < 1000000; i++)
        in3 >> i;
    clock_gettime(CLOCK_REALTIME, &end);
    cout << "reading a 5-digit int: " << (diff(start, end) / 1000000) << endl;

    s.clear();
    for(int i = 0; i < 1000001; i++)
        s += "1234.5 ";
    double d;
    istringstream in4(s);
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i < 1000000; i++)
        in4 >> d;
    clock_gettime(CLOCK_REALTIME, &end);
    cout << "reading a 5-digit double: " << (diff(start, end) / 1000000) << endl;

    s.clear();
    for(int i = 0; i < 1000001; i++)
        s += "x ";
    string s2;
    istringstream in5(s);
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i < 1000000; i++)
        in5 >> s2;
    clock_gettime(CLOCK_REALTIME, &end);
    cout << "reading a 1-char string: " << (diff(start, end) / 1000000) << endl;

    s.clear();
    for(int i = 0; i < 1000001; i++)
        s += "abcde ";
    istringstream in6(s);
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i < 1000000; i++)
        in6 >> s2;
    clock_gettime(CLOCK_REALTIME, &end);
    cout << "reading a 5-char string: " << (diff(start, end) / 1000000) << endl;

    s.clear();
    for(int i = 0; i < 1000001; i++)
        s += "abcdefghijklmnopqrstuvwxyzabcdefghijklmn ";
    istringstream in7(s);
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i < 1000000; i++)
        in7 >> s2;
    clock_gettime(CLOCK_REALTIME, &end);
    cout << "reading a 40-char string: " << (diff(start, end) / 1000000) << endl;
}
