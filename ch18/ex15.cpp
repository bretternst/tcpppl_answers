#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <iterator>

int main(int argc, char** argv) {
    using namespace std;

    if(argc < 2)
        return 1;

    vector<string> words;
    istringstream in(argv[1]);
    copy(istream_iterator<string>(in), istream_iterator<string>(), back_inserter(words));
    sort(words.begin(), words.end());

    do {
        copy(words.begin(), words.end(), ostream_iterator<string>(cout, " "));
        cout << endl;
    } while (next_permutation(words.begin(), words.end()));
}
