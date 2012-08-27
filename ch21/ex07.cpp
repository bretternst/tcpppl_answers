#include <iostream>
#include <cctype>
#include <algorithm>

namespace ch21 {
    char alnum_incr(char ch) {
        if(ch == 'z')
            return 'a';
        else if (ch == '9')
            return '0';
        else if (::isalnum(ch))
            return ch + 1;
        else
            return ch;
    }
}

const int BufSize = 80;

int main() {
    using namespace std;

    char buf[BufSize + 1];
    while(cin.getline(buf, BufSize)) {
        size_t n = cin.gcount();
        char* end = buf + n;
        end = remove_if(buf, end, ::isspace);
        transform(buf, end, buf, ch21::alnum_incr);
        *end = 0; // null terminate so we can print
        std::cout << buf << std::endl;
    }
}