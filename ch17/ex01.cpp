#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <string>
#include <windows.h>

LARGE_INTEGER freq,start,end;

inline void StartTimer()
{
    QueryPerformanceCounter(&start);
}

inline void EndTimer(std::string msg)
{
    QueryPerformanceCounter(&end);
    std::cout << msg << ": " << static_cast<double>(end.QuadPart-start.QuadPart) /
        static_cast<double>(freq.QuadPart) * 1000.0  << "ms" << std::endl;
}

template<class Tcontainer> void FillContainer(Tcontainer& c, int n)
{
    c.clear();
    for(;n>=0;--n)
    {
        c.push_back(rand());
    }
}

template<class Tcontainer> void FillAssocContainer(Tcontainer& c, int n)
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

    if(!QueryPerformanceFrequency(&freq))
        return -1;

    vector<int> v;
    list<int> l;
    deque<int> q;
    map<int,int> m;

    StartTimer();
    FillContainer(v, 1000000);
    EndTimer("vector of 1m, initial fill");

    StartTimer();
    v.push_back(0);
    EndTimer("vector of 1m, back insert O(1)");

    StartTimer();
    v.insert(v.begin(), 0);
    EndTimer("vector of 1m, front insert O(n)");

    StartTimer();
    v.erase(v.begin()+500000);
    EndTimer("vector of 1m, erase middle O(n)");


    StartTimer();
    FillContainer(l, 1000000);
    EndTimer("list of 1m, initial fill");

    StartTimer();
    l.push_back(0);
    EndTimer("list of 1m, back insert O(1)");

    StartTimer();
    l.insert(l.begin(), 0);
    EndTimer("list of 1m, front insert O(1)");

    StartTimer();
    list<int>::iterator li = l.begin();
    for(int i = 0; i < 500000; i++) li++;
    l.erase(li);
    EndTimer("list of 1m, erase middle - includes time to find middle O(n)");


    StartTimer();
    FillContainer(q, 1000000);
    EndTimer("deque of 1m, initial fill");

    StartTimer();
    q.push_back(0);
    EndTimer("deque of 1m, back insert O(1)");

    StartTimer();
    q.insert(q.begin(), 0);
    EndTimer("deque of 1m, front insert O(n)");

    StartTimer();
    q.erase(q.begin()+500000);
    EndTimer("deque of 1m, erase middle O(n)");


    StartTimer();
    FillAssocContainer(m, 1000000);
    EndTimer("map of 1m, initial fill)");

    StartTimer();
    m[1000001] = 0;
    EndTimer("map of 1m, add high value O(log n)");

    StartTimer();
    m[500000] = 0;
    EndTimer("map of 1m, modify medium value O(log n)");

    StartTimer();
    m[0] = 0;
    EndTimer("map of 1m, modify low value O(log n)");

    StartTimer();
    map<int,int>::iterator mi = m.begin();
    for(int i = 0; i < 500000; i++) mi++;
    m.erase(mi);
    EndTimer("map of 1m, erase middle - includes time to find middle O(n)");
    return 0;
}
