#include <iostream>
#include <iterator>
#include <algorithm>
#include <iterator>
#include <sstream>

namespace ch19 {
    using namespace std;

    template<class T, class Ch = char, class Tr = char_traits<Ch>, class Dist = ptrdiff_t>
    class istream_iterator : public iterator<input_iterator_tag, T, Dist, const T*, const T&> {
    public:
        typedef Ch char_type;
        typedef Tr traits_type;
        typedef basic_istream<Ch,Tr> istream_type;

        istream_iterator() : s(0) { }
        istream_iterator(istream_type& s) : s(&s) { if(*this->s) *this->s >> val; if (!*this->s) end = true; }
        istream_iterator(const istream_iterator& x) : s(x.s), val(x.val) { }
        ~istream_iterator() { }

        const T& operator*() const { return val; }
        const T* operator->() const { return &val; }
        istream_iterator& operator++() { *s >> val; if(!*s) end = true; return *this; }
        istream_iterator operator++(int) { istream_iterator tmp = *this; ++*this; return tmp; }
        bool operator==(const istream_iterator& x) { return end && x.end || s == x.s; }
        bool operator!=(const istream_iterator& x) { return !(*this == x); }
    private:
        istream_type* s;
        T val;
        bool end;
    };
}

int main() {
    using namespace std;

    string s("1 2 3 4 5 6 7 8 9 10");
    istringstream in(s);
    copy(ch19::istream_iterator<int>(in), ch19::istream_iterator<int>(), std::ostream_iterator<int>(cout, " "));
    cout << endl;
}
