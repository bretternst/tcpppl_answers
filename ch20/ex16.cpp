#include <iostream>
#include <string>
#include <algorithm>

namespace ch20 {
    using std::string;
    using std::reverse;

    string itos(int i) {
        int b = 10;
        if(i == 0)
            return "0";
        bool neg = false;
        if(i < 0) {
            neg = true;
            i = -i;
        }
        string s;
        while(i) {
            int r = i % b;
            s.push_back('0' + r);
            i /= b;
        }
        if(neg)
            s.push_back('-');
        reverse(s.begin(), s.end());
        return s;
    }
}

int main() {
    using namespace std;
    using namespace ch20;

    cout << itos(0) << endl;
    cout << itos(1) << endl;
    cout << itos(12345) << endl;
    cout << itos(2000001) << endl;
    cout << itos(-54321) << endl;
}
