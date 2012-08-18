#include <iostream>
#include <algorithm>

int main(int argc, char** argv) {
    using namespace std;

    if(argc < 2)
        return 1;

    string s(argv[1]);
    sort(s.begin(), s.end());
    cout << s << endl;

    while(next_permutation(s.begin(), s.end())) {
        cout << s << endl;
    }
}