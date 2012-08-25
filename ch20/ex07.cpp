#include <string>
#include <iostream>

namespace ch20 {
    using namespace std;

    size_t find(const string& s, const string& pattern, int spos = 0, int ppos = 0) {
        while(spos < s.size()) {
            int i = spos;
            int j = ppos;

            while(i < s.size() && j < pattern.size()) {
                if (pattern[j] == '[') {
                    int end = pattern.find(']', j);
                    if(end == string::npos)
                        return string::npos; // unmatched
                    bool found = false;
                    for(int k = j+1; k < end; k++) {
                        if(s[i] == pattern[k]) {
                            found = true;
                            break;
                        }
                    }
                    if(found) {
                        i++;
                        j = end + 1;
                    }
                    else
                        return string::npos;
                }
                else if (pattern[j] == '*') {
                    for(int k = s.size() - 1; k >= i; --k) {
                        size_t npos = find(s, pattern, k, j + 1);
                        if(npos < string::npos)
                            return spos;
                    }
                    return string::npos;
                }
                else if (pattern[j] == '?' || pattern[j] == s[i]) {
                    i++;
                    j++;
                }
                else
                    break;
            }

            if(j == pattern.size())
                return spos;
            spos++;
        }
        return string::npos;
    }
}

int main() {
    using namespace ch20;

    string s("foo bar baz");
    cout << find(s, "foo") << endl; // 0
    cout << find(s, "bar") << endl; // 3
    cout << find(s, "oo*baz") << endl; // 1
    cout << find(s, "o?bar?b*") << endl; // 2
    cout << find(s, "ba[Rr]*z") << endl; // 4
}
