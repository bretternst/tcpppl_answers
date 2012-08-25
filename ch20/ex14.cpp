#include <string>
#include <iostream>
#include <time.h>

// Compile with:
// g++ -o ex14 ex14.cpp -lrt

namespace ch20 {
    using std::string;
    
    string complete_name(const string& first_name, const string& family_name) {
        string s = first_name;
        s += ' ';
        s += family_name;
        return s;
    }

    string complete_name2(const string& first_name, const string& family_name) {
        string s = family_name;
        s.insert(s.begin(), ' ');
        return s.insert(0, first_name);
    }

    string complete_name3(const string& first_name, const string& family_name) {
        return first_name + ' ' + family_name;
    }

    // This runs more than twice as fast as the next fastest version of the function
    // on my machine. The trick is reserving enough space for the whole string
    // to begin with so that re-allocation and copy isn't required.
    // This was the first attempt; can it be made even faster?
    string complete_name_fast(const string& first_name, const string& family_name) {
        string s;
        s.reserve(first_name.size() + family_name.size() + 1);
        s.append(first_name);
        s.push_back(' ');
        s.append(family_name);
        return s;
    }

    typedef string (*complfunc)(const string&, const string&);
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

void run_test(ch20::complfunc cmp, const char* func_name) {
    using namespace std;

    string first_name("John");
    string family_name("Smith");

    timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i < 1000000; i++) {
        string res = cmp(first_name, family_name);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    cout << func_name << ": " << diff(start, end) << endl;
}

int main() {
    using namespace ch20;

    run_test(&ch20::complete_name, "complete_name");
    run_test(&ch20::complete_name2, "complete_name2");
    run_test(&ch20::complete_name3, "complete_name3");
    run_test(&ch20::complete_name_fast, "complete_name_fast");
}
