#include <string>
#include <iostream>
#include <time.h>

// Compile with:
// g++ -o ex13 ex13.cpp -lrt

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

int main() {
    using namespace std;
    string s(1024, 'x');

    timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i < 1000000; i++) {
        string s2(s);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    cout << "fast: " << diff(start, end) << endl;
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i < 1000000; i++) {
        string s2(s, 0, s.size());
    }
    clock_gettime(CLOCK_REALTIME, &end);
    cout << "slow: " << diff(start, end) << endl;}
