#include <cctype>
#include <iostream>
#include <functional>
#include <limits>

namespace ch21 {
    template<class Out, class Pred>
    void print_all_chars(const char* category, Pred pred, char delim = 0) {
        std::cout << category;
        bool first = true;
        for(unsigned int i = 0; i < std::numeric_limits<unsigned char>::max(); i++) {
            if(pred(i)) {
                if(!first && delim)
                    std::cout << delim;
                first = false;
                Out c = static_cast<Out>(i);
                std::cout << c;
            }
        }
        std::cout << std::endl;
    }

    // $ is a valid identifier char for extensions on some compilers, but we'll stick
    // to standards here.
    // The question asked which chars can appear in an identifier, not how an identifier
    // must begin, so numbers are included.
    bool isidentifier(int c) {
        return isalnum(c) || c == '_';
    }
}

int main() {
    using namespace std;
    using namespace ch21;

    print_all_chars<char>("All lowercase: ", ::islower);
    print_all_chars<char>("All letters: ", ::isalpha);
    print_all_chars<char>("All letters and digits: ", ::isalnum);
    print_all_chars<char>("All chars from C++ identifers: ", ch21::isidentifier);
    print_all_chars<char>("All punctuation: ", ::ispunct);
    print_all_chars<int>("All control: ", ::iscntrl, ',');
    // commenting out because some whitespace chars screw up the terminal output
    // print_all_chars<char>("All whitespace (char): ", ::isspace, ',');
    print_all_chars<int>("All whitespace (int): ", ::isspace, ',');
    print_all_chars<char>("All printable: ", ::isprint);
}
