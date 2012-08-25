#include <string>
#include <iostream>
#include <time.h>

// Compile with:
// g++ -o ex15 ex15.cpp -lrt
// Run with:
// ./ex15 < /usr/share/dict/words
// ./ex15 fast < /usr/share/dict/words

namespace ch20 {
    using std::istream;
    using std::string;

    // shortcoming: if any words are larger than the entire buffer, they'll be split
    // on my system, this runs about 3.5 times faster than operator>>
    // I assume that input streams are already buffered, so that's a bit of a mystery.
    bool read_word(istream& in, string& s, char* buf, size_t sz, size_t& n, size_t& pos) {
        if(pos >= n) {
            if(!in)
                return false;
            pos = 0;
            in.read(buf, sz);
            n = in.gcount();
        }
        if(n == 0)
            return false;
        while(isspace(buf[pos]) && pos < n)
            pos++;
        if(pos == n)
            return read_word(in, s, buf, sz, n, pos);

        int i = pos;
        while(i < n && !isspace(buf[i]))
            i++;
        s.assign(buf + pos, i - pos);
        pos = i;
        if(pos == n && in) {
            in.read(buf, sz);
            n = in.gcount();
            pos = 0;
            if(n > 0) {
                while(pos < n && !isspace(buf[pos])) {
                    s.push_back(buf[pos++]);
                }
            }
        }
        return true;
    }
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

int main(int argc, char** argv) {
    using namespace std;
    using namespace ch20;

    timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    string s;
    if(argc > 1) {
        char buf[1024];
        size_t pos = 0, n = 0;
        while(read_word(cin, s, buf, 1024, n, pos));
    }
    else {
        while(cin) {
            cin >> s;
        }
    }
    clock_gettime(CLOCK_REALTIME, &end);
    cout << "time: " << diff(start, end) << endl;
}
