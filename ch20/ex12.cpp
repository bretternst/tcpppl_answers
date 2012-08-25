#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <ostream>

// Implemented a small subset of string functions so the point is illustrated.
// Yes, this would be somewhat faster because no allocations are required. With
// a small, static capacity, the same array can always be used and never has to
// be copied into a larger space.
//
// As with most things, there are tradeoffs. Some of the benefit could be
// combined with a general string by simply reserving more space. However, that
// improves efficiency at the cost of space.
namespace ch20 {
    using std::char_traits;
    using std::min;
    using std::out_of_range;
    using std::length_error;
    using std::ostream;

    template<class Ch, class Tr = char_traits<Ch>, size_t Capacity = 8>
    class String {
        Ch s[Capacity + 1];
        size_t sz;
    public:
        typedef Ch* iterator;
        typedef const Ch* const_iterator;
        typedef Ch& reference;
        typedef const Ch& const_reference;
        typedef Ch* pointer;
        typedef const Ch* const_pointer;

        String() : sz(0) { s[0] = Ch(); }
        // use default copy constructor
        String(const Ch* x) {
            sz = min(Capacity, Tr::length(x));
            Tr::copy(s, x, sz);
            s[sz] = 0;
        }

        iterator begin() { return s; }
        const_iterator begin() const { return s; }
        iterator end() { return s + sz; }
        const_iterator end() const { return s + sz; }
        size_t size() const { return sz; }
        size_t length() const { return sz; }
        size_t max_size() const { return Capacity; }
        size_t capacity() const { return Capacity; }
        void clear() { sz = 0; s[0] = 0; }
        bool empty() const { return sz == 0; }

        reference operator[](size_t pos) { return s[pos]; }
        const_reference operator[](size_t pos) const { return s[pos]; }
        reference at(size_t pos) { if(pos < 0 || pos >= sz) throw out_of_range("String::at"); return s[pos]; }
        const_reference at(size_t pos) const { if (pos < 0 || pos >= sz) throw out_of_range("String::at"); return s[pos]; }

        String& operator+=(const String& str);
        String& operator+=(const Ch* s);
        String& operator+=(Ch c);
        void push_back(Ch c) { *this += c; }

        const_pointer c_str() const { return s; }
        const_pointer data() const { return s; }
    };

    template<class Ch, class Tr, size_t Capacity>
    String<Ch,Tr,Capacity>& String<Ch,Tr,Capacity>::operator+=(const String& str) {
        *this += str.c_str();
    }

    template<class Ch, class Tr, size_t Capacity>
    String<Ch,Tr,Capacity>& String<Ch,Tr,Capacity>::operator+=(const Ch* x) {
        size_t n = Tr::length(x);
        if(n + sz > Capacity)
            throw length_error("String::operator+=");
        Tr::copy(s + sz, x, n);
        sz += n;
        s[sz] = 0;
    }

    template<class Ch, class Tr, size_t Capacity>
    String<Ch,Tr,Capacity>& String<Ch,Tr,Capacity>::operator+=(Ch c) {
        if(sz == Capacity)
            throw length_error("String::operator+=");
        s[sz++] = c;
        s[sz] = 0;
    }

    template<class Ch, class Tr, size_t Capacity>
    ostream& operator<<(ostream& out, const String<Ch,Tr,Capacity>& x) {
        out << x.c_str();
        return out;
    }
}

int main() {
    using namespace ch20;
    using namespace std;

    String<char> s("foo");
    s += " bar";
    cout << s << endl;
}