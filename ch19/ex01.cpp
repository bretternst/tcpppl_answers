#include <iterator>
#include <iostream>
#include <string>

namespace ch19 {
    template<class Bi> void reverse(Bi first, Bi last) {
        // two checks necessary to handle even and odd lengths
        while((first != last) && (first != --last)) {
            std::swap(*first++, *last);
        }
    }
}

int main() {
    using namespace ch19;
    using namespace std;

    string s("Hello World!");
    ch19::reverse(s.begin(), s.end());
    cout << s << endl;
    string s1("Hello World");
    ch19::reverse(s1.begin(), s1.end());
    cout << s1 << endl;
}
