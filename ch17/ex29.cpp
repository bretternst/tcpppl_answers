#include <cstring>
#include <sstream>
#include <string>
#include <iostream>
#include <time.h>
#include "hash_map.h"

// Compile with:
// g++ -o ex29 ex29.cpp -lrt
namespace ch17 {
    struct St {
        int size;
        char type_indicator;
        char* buf;
        St(const St& x) : size(x.size), type_indicator(x.type_indicator) {
            buf = new char[x.size + 1];
            if(x.buf)
                strcpy(buf, x.buf);
        }
        St(const char* p) {
            type_indicator = 0;
            size = strlen(p);
            buf = new char[size + 1];
            strcpy(buf, p);
        }
        ~St() {
            delete buf;
        }
    };

    class St_hash {
    public:
        int operator()(const St& x) {
            int res = x.type_indicator;
            char* p = x.buf;
            while(*p) res = (res << 1)^*p++;
            return res;            
        }
    };

    class St_equals {
    public:
        bool operator()(const St& x, const St& y) {
            return x.size == y.size && x.type_indicator == y.type_indicator &&
                (x.buf == y.buf || strcmp(x.buf,y.buf) == 0);
        }
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

int main() {
    using namespace ch17;
    using namespace std;

    timespec start, stop;

    hash_map<St, int, St_hash, St_equals> hm;
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i < 1000; i++) {
        ostringstream os;
        os << i;
        St x(os.str().c_str());
        hm[x] = i;
    }
    clock_gettime(CLOCK_REALTIME, &stop);
    cout << "Time to populate: " << diff(start, stop) << endl;

    clock_gettime(CLOCK_REALTIME, &start);
    int sum;
    for(hash_map<St, int, St_hash, St_equals>::iterator i = hm.begin(); i != hm.end(); i++) {
        sum += i->second;
    }
    clock_gettime(CLOCK_REALTIME, &stop);
    cout << "Time to iterate: " << diff(start, stop) << endl;

    St x("500");
    clock_gettime(CLOCK_REALTIME, &start);
    sum = hm[x];
    clock_gettime(CLOCK_REALTIME, &stop);
    cout << "Time to access: " << diff(start, stop) << endl;
}
