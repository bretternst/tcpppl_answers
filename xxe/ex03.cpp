#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>

namespace xxe {
    class Test_error { };

    struct Tester {
        static bool throw_construct;
        static bool throw_copy;
        static bool throw_assign;
        static bool throw_compare;
        static bool throw_destruct; // VERY VERY BAD!

        Tester() : v(0) { throw_if(throw_construct); }
        Tester(int v) : v(v) { throw_if(throw_construct); }
        Tester(const Tester& x) : v(x.v) { throw_if(throw_copy); }
        ~Tester() { throw_if(throw_destruct); }

        Tester& operator=(const Tester& x) { throw_if(throw_assign); v = x.v; return *this; }
        bool operator==(const Tester& x) const { throw_if(throw_compare); return v == x.v; }
        bool operator<(const Tester& x) const { throw_if(throw_compare); return v < x.v; }

        int val() const { return v; }

    private:
        int v;

        void throw_if(bool& cond) const {
            if(cond) {
                throw Test_error();
                cond = false;
            }
        }
    };

    std::ostream& operator<<(std::ostream& s, const Tester& x) {
        return s << x.val();
    }

    bool Tester::throw_construct = false;
    bool Tester::throw_copy = false;
    bool Tester::throw_assign = false;
    bool Tester::throw_compare = false;
    bool Tester::throw_destruct = false;
}

void print(std::vector<xxe::Tester>& v) {
    std::cout << "Count: " << v.size() << " = ";
    std::copy(v.begin(), v.end(), std::ostream_iterator<xxe::Tester>(std::cout, " "));
    std::cout << std::endl;
}

int main() {
    using namespace std;
    using namespace xxe;

    vector<Tester> v;
    v.push_back(Tester(2));
    v.push_back(Tester(1));
    v.push_back(Tester(5));
    v.push_back(Tester(4));
    v.push_back(Tester(6));

    Tester::throw_construct = true;
    try {
        v.push_back(Tester(7));
    }
    catch(Test_error) {
        print(v);
    }
    Tester::throw_construct = false;

    Tester::throw_assign = true;
    try {
        v.insert(v.begin() + 4, Tester(3));
    }
    catch(Test_error) {
        print(v);
    }
    try {
        v.erase(v.begin() + 4);
    }
    catch(Test_error) {
        print(v);
    }
    Tester::throw_assign = false;
    v.erase(v.begin() + 4);

    Tester::throw_compare = true;
    try {
        sort(v.begin(), v.end());
    }
    catch(Test_error) {
        print(v);
    }
}
