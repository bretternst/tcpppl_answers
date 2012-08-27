#include <iostream>
#include <limits>
#include <cstdio>

// Implementing all of stdio.h is a lot more work than I want to do
// for one exercise, so here are a few basic C I/O operations. I'll
// use the non-file (console) versions, but it wouldn't be much harder
// to use the file versions of the functions (e.g., fputs) by passing
// in a struct that references an fstream.
//
// For the adventurous, you could translate a printf format string into
// a series of mutator operations and writes to a stream.
namespace ch21 {
    int getchar() {
        char ch;
        return (std::cin >> ch) ? ch : EOF;
    }

    // danger! beware buffer overflows!
    char* gets(char* str) {
        return std::cin.getline(str, std::numeric_limits<std::streamsize>::max()) ? str : 0;
    }

    int putchar(int ch) {
        return (std::cout << static_cast<char>(ch)) ? ch : EOF;
    }

    int puts(const char* str) {
        return (std::cout << str) ? 1 : EOF;
    }
}

int main() {
    using std::cout;
    using std::endl;
    using namespace ch21;

    cout << "Enter a char: ";
    int ch = ch21::getchar();
    cout << "You entered: ";
    ch21::putchar(ch);
    cout << endl;
    std::cin.getline(0, 0); // eat the newline;

    cout << "Enter a string: ";
    char str[512];
    ch21::gets(str);
    cout << "You entered: ";
    ch21::puts(str);
    cout << endl;
}