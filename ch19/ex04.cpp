#include <iostream>
#include <iterator>
#include <algorithm>

namespace ch19 {
    using namespace std;

    template<class T, class Ch = char, class Tr = char_traits<Ch> >
    class ostream_iterator : public iterator<output_iterator_tag, void, void, void, void> {
    public:
        typedef Ch char_type;
        typedef Tr traits_type;
        typedef basic_ostream<Ch,Tr> ostream_type;

        ostream_iterator(ostream_type& s) : s(s) { }
        ostream_iterator(ostream_type& s, const Ch* delim) : s(s), delim(delim) { }
        ostream_iterator(const ostream_iterator& x) : s(x.s), delim(x.delim) { }
        ~ostream_iterator() { }

        ostream_iterator& operator=(const T& val) { s << val; if (delim) s << delim; }

        ostream_iterator& operator*() { return *this; }
        ostream_iterator& operator++() { return *this; }
        ostream_iterator& operator++(int) { return *this; }
    private:
        ostream_type& s;
        const Ch* delim;
    };
}

int main() {
    using namespace std;

    int arr[] = { 1, 2, 3, 4, 5 };
    copy(arr, arr + 5, ch19::ostream_iterator<int>(cout, " "));
    cout << endl;
}