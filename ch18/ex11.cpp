#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <iterator>

// Use only alphanumeric characters in a word (ignore punctuation).
bool test_word(const std::string& x, const std::string& y) {
    std::string::const_iterator i1 = x.begin();
    std::string::const_iterator i2 = y.begin();
    for(;;) {
        while(i1 != x.end() && !isalnum(*i1)) i1++;
        while(i2 != y.end() && !isalnum(*i2)) i2++;
        if (i1 == x.end() || i2 == y.end())
            break;
        if(*i1++ != *i2++)
            return false;
    }
    if(i1 != x.end()) {
        while(i1 != x.end() && !isalnum(*i1)) i1++;
        return i1 == x.end();        
    }
    if(i2 != y.end()) {
        while(i2 != y.end() && !isalnum(*i2)) i2++;
        return i2 == y.end();
    }
    return true;
}

int main(int argc, char** argv) {
    using namespace std;

    if(argc != 2)
        return 1;
    ifstream in(argv[1]);
    if(!in)
        return 1;

    vector<string> v;
    copy(istream_iterator<string>(in), istream_iterator<string>(), back_inserter(v));
    v.erase(unique(v.begin(),v.end(),test_word), v.end());
    copy(v.begin(), v.end(), ostream_iterator<string>(cout, " "));
    cout << endl;
}
