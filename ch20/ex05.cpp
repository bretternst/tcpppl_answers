#include <algorithm>
#include <string>
#include <iostream>

// back_inserter already works with basic_string, am I missing something here?
// Anyway, here's a reimplementation of basic_string.
namespace ch20 {
    using namespace std;

    template<class Cont>
    class Back_inserter : public iterator<output_iterator_tag, void, void, void, void> {
    protected:
        Cont* c;
    public:
        typedef Cont container_type;
        explicit Back_inserter(Cont& x) : c(&x) { }
        Back_inserter& operator=(typename Cont::const_reference val) { c->push_back(val); return *this; }
        Back_inserter& operator*() { return *this; }
        Back_inserter& operator++() { return *this; }
        Back_inserter& operator++(int) { return *this; }
    };

    template<class Cont>
    Back_inserter<Cont> back_inserter(Cont& x) {
        return ch20::Back_inserter<Cont>(x);
    }
}

int main() {
    using namespace ch20;

    string s("Hello");
    const char* p = " world!";
    copy(p, p + 7, ch20::back_inserter(s));
    cout << s << endl;
}