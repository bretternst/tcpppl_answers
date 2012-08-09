#include <iostream>
#include <ctime>
#include <map>
#include "hash_map.h"

struct BadHashType
{
    int k;
    int x[8];
    void init() {for(int i=0;i<8;i++) x[i]=0;}
    BadHashType() : k(0) { init(); }
    BadHashType(int kk) : k(kk) { init(); }
    bool operator==(const BadHashType& x) const { return k==x.k; }
};

int main()
{
    using namespace std;
    using namespace ch17;

    typedef hash_map<int,int> IntInt;
    IntInt h;

    h[1] = 2;
    h[2] = 40;
    h[3] = 25;
    h[4] = 50;
    h[5] = 100;
    h[7] = 150;

    h.insert(IntInt::value_type(6, 125));

    vector<pair<int,int> > v;
    v.push_back(make_pair(10,1000));
    v.push_back(make_pair(15,1500));
    v.push_back(make_pair(20,2000));

    h.insert(v.begin(),v.end());

    IntInt::iterator i = h.find(4);
    h.erase(i);

    cout << "forward" << endl;
    for(IntInt::iterator i = h.begin(); i != h.end(); i++)
    {
        cout << i->first << '=' << i->second << endl;
    }

    h.erase(3);
    h[5] = 101;
    cout << "backward" << endl;
    for(IntInt::reverse_iterator i = h.rbegin(); i != h.rend(); i++)
    {
        cout << i->first << '=' << i->second << endl;
    }

    cout << h.count(0) << endl;
    cout << h.count(1) << endl;

    cout << h.lower_bound(1)->first << endl;
    cout << h.upper_bound(1)->first << endl;
    cout << (h.lower_bound(0) == h.end() && h.upper_bound(0) == h.end()) << endl;
    pair<IntInt::iterator,IntInt::iterator> pair = h.equal_range(1);
    cout << pair.first->first << pair.second->first << endl;

    h.clear();

    srand(static_cast<unsigned int>(time(0)));
    for(int i = 0; i < 10000; i++)
    {
        int r = rand();
        h[r] = r;
    }
    cout << "hash 1: bucket utilization: " << h.bucket_utilization() << endl;

    BadHashType bht;

    hash_map<BadHashType,int> bad;
    for(int i = 0; i < 10000; i++)
    {
        int r = rand();
        bad[BadHashType(r)] = r;
    }
    cout << "bad: bucket utilization: " << bad.bucket_utilization() << endl;
}
