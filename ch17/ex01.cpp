#include <iostream>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <string>

// build with g++ -o ex01 ex01.cpp -lrt

timespec ts;

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

inline void start_timer()
{
    clock_gettime(CLOCK_REALTIME, &ts);
}

inline void end_timer(std::string msg)
{
    timespec end;
    clock_gettime(CLOCK_REALTIME, &end);
    std::cout << msg << ": " << diff(ts, end) << std::endl;
}

template<class Tcontainer> void fill_container(Tcontainer& c, int n)
{
    c.clear();
    for(;n>=0;--n)
    {
        c.push_back(rand());
    }
}

template<class Tcontainer> void fill_assoc_container(Tcontainer& c, int n)
{
    c.clear();
    for(;n>=0;--n)
    {
        c[n] = rand();
    }
}

int main()
{
    using namespace std;

    vector<int> v;
    list<int> l;
    deque<int> q;
    map<int,int> m;

    start_timer();
    fill_container(v, 1000000);
    end_timer("vector of 1m, initial fill");

    start_timer();
    v.push_back(0);
    end_timer("vector of 1m, back insert O(1)");

    start_timer();
    v.insert(v.begin(), 0);
    end_timer("vector of 1m, front insert O(n)");

    start_timer();
    v.erase(v.begin()+500000);
    end_timer("vector of 1m, erase middle O(n)");


    start_timer();
    fill_container(l, 1000000);
    end_timer("list of 1m, initial fill");

    start_timer();
    l.push_back(0);
    end_timer("list of 1m, back insert O(1)");

    start_timer();
    l.insert(l.begin(), 0);
    end_timer("list of 1m, front insert O(1)");

    start_timer();
    list<int>::iterator li = l.begin();
    for(int i = 0; i < 500000; i++) li++;
    l.erase(li);
    end_timer("list of 1m, erase middle - includes time to find middle O(n)");


    start_timer();
    fill_container(q, 1000000);
    end_timer("deque of 1m, initial fill");

    start_timer();
    q.push_back(0);
    end_timer("deque of 1m, back insert O(1)");

    start_timer();
    q.insert(q.begin(), 0);
    end_timer("deque of 1m, front insert O(n)");

    start_timer();
    q.erase(q.begin()+500000);
    end_timer("deque of 1m, erase middle O(n)");


    start_timer();
    fill_assoc_container(m, 1000000);
    end_timer("map of 1m, initial fill)");

    start_timer();
    m[1000001] = 0;
    end_timer("map of 1m, add high value O(log n)");

    start_timer();
    m[500000] = 0;
    end_timer("map of 1m, modify medium value O(log n)");

    start_timer();
    m[0] = 0;
    end_timer("map of 1m, modify low value O(log n)");

    start_timer();
    map<int,int>::iterator mi = m.begin();
    for(int i = 0; i < 500000; i++) mi++;
    m.erase(mi);
    end_timer("map of 1m, erase middle - includes time to find middle O(n)");
    return 0;
}
