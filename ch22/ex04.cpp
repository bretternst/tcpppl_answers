#include <iostream>
#include <map>
#include <numeric>

using namespace std;

void readitems(map<string,int>& m) {
    string word;
    int val = 0;
    while(cin >> word >> val)
        m[word] += val;
}

int tally_word(int total, const pair<string,int>& p) {
    cout << p.first << '\t' << p.second << '\n';
    return total += p.second;
}

int main() {
    map<string,int> tbl;
    readitems(tbl);

    int total = 0;
    typedef map<string,int>::const_iterator CI;
    total = accumulate(tbl.begin(), tbl.end(), 0, tally_word);
    cout << "----------------\ntotal\t" << total << '\n';
    return !cin;    
}
