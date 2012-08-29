#include <vector>
#include <ios>
#include <iostream>

namespace ch21 {
    using std::ios;
    using std::ios_base;

    class Error_all {
        ios* s;
        ios::iostate old_state;

    public:
        explicit Error_all(ios* s) : s(s) {
            old_state = s->exceptions();
            s->exceptions(ios::eofbit | ios::failbit | ios::badbit);
        }
        ~Error_all() {
            s->exceptions(old_state);
            s->clear();
        }
    };

    void readints(std::vector<int>& s) {
        Error_all err(&std::cin);

        for(;;) {
            try {
                int i;
                std::cin >> i;
                s.push_back(i);
            }
            catch(ios::failure) {
                std::cout << "caught ios::failure" << std::endl;
                break;
            }
        }
    }
}

int main() {
    using namespace std;
    using namespace ch21;
    vector<int> v;
    readints(v);
}