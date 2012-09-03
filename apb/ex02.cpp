#include <regex>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <iostream>
#include <string>

// On older versions of g++, compile with:
// g++ -o ex02 ex02.cpp -std=c++0x

// This is pretty sloppy, but it's just an appendix exercise
// and I don't want to write a lexer (again). It won't handle
// comments or strings.

void replace(std::string& s, const std::string& pattern, const std::string& with) {
    std::regex e(pattern);
    s = std::regex_replace(s, e, with);
}

int main() {
    using namespace std;

    ifstream in("ex02test.c");
    std::string s((istreambuf_iterator<char>(in)), (istreambuf_iterator<char>()));

    replace(s, "new", "c_new");

    cout << s << endl;
}
