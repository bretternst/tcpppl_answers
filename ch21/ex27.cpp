#include <iostream>

// This was a 2.5 point exercise, but it was really easy; did I miss something in the question?
namespace ch21 {
    std::ios_base& general(std::ios_base& s) {
        s.unsetf(std::ios_base::floatfield);
    }
}

int main() {
    using namespace std;
    using namespace ch21;

    cout << 12.345 << ' ' << scientific << 12.345 << ' ' << fixed << 12.345 << ' ' << general << 12.345 << endl;
}
