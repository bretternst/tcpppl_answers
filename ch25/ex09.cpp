#include <iostream>

// One advantage of separating the checked interface is that users get to decide
// whether they want to use it or not. Checking has performance considerations
// and may not be appropriate in all circumstances. On the other hand, there are
// many scenarios where checking is desirable and performance micro-optimization
// is not a concern.
namespace ch25 {
    // Readonly
    class String {
        const char* s;
        size_t sz;
    public:
        String(const char* s, size_t sz) : s(s), sz(sz) { }
        char operator[](size_t pos) const { return s[pos]; }
        size_t size() const { return sz; }
    };

    class String_checked {
        String s;

    public:
        class Out_of_range { };
        String_checked(const char* s, size_t sz) : s(s, sz) { }
        String_checked(String& s) : s(s) { }
        char operator[](size_t pos) const { check(pos); return s[pos]; }
        size_t size() const { return s.size(); }
    private:
        void check(size_t pos) const {
            if(pos < 0 || pos >= s.size())
                throw Out_of_range();
        }
    };
}

int main() {
    using namespace std;
    using namespace ch25;

    String_checked s("foo bar", 7);
    char c = s[6];
    try {
        c = s[7];
    }
    catch(String_checked::Out_of_range) {
        cout << "caught String_checked::Out_of_range" << endl;
    }
}
