#include <string>
#include <iostream>
#include <cstring>
#include <regex.h>

// Uses POSIX C-style regex library (man regexec).
namespace ch20 {
    using namespace std;

    template<class Ch>
    class Basic_substring {
    public:
        typedef typename basic_string<Ch>::size_type size_type;

        Basic_substring(basic_string<Ch>& s, size_type i, size_type n);
        Basic_substring(basic_string<Ch>& s, const basic_string<Ch>& s2);
        Basic_substring(basic_string<Ch>& s, const Ch* p);
        Basic_substring(const Basic_substring<Ch>& x);
        ~Basic_substring();

        Basic_substring& operator=(const basic_string<Ch>& s);
        Basic_substring& operator=(const Basic_substring<Ch>& x);
        Basic_substring& operator=(const Ch* p);
        Basic_substring& operator=(Ch c);

        operator basic_string<Ch>() const;
        operator const Ch* () const;
    private:
        basic_string<Ch>* ps;
        size_type pos;
        size_type n;
        mutable Ch* cstr;
    };

    template<class Ch>
    Basic_substring<Ch>::Basic_substring(basic_string<Ch>& s, size_type i, size_type n) : ps(&s), pos(i), n(n), cstr(0) {
    }

    template<class Ch>
    Basic_substring<Ch>::Basic_substring(basic_string<Ch>& s, const basic_string<Ch>& s2) : ps(&s), n(s2.size()), cstr(0) {
        pos = s.find(s2);
    }

    template<class Ch>
    Basic_substring<Ch>::Basic_substring(basic_string<Ch>& s, const Ch* p) : ps(&s), n(strlen(p)), cstr(0) {
        pos = s.find(p);
    }

    template<class Ch>
    Basic_substring<Ch>::Basic_substring(const Basic_substring<Ch>& x) : ps(x.ps), pos(x.pos), n(x.n), cstr(0) {
    }

    template<class Ch>
    Basic_substring<Ch>::~Basic_substring() {
        if(cstr)
            delete[] cstr;
    }

    template<class Ch>
    Basic_substring<Ch>& Basic_substring<Ch>::operator=(const basic_string<Ch>& s) {
        ps->replace(pos, n, s);
    }

    template<class Ch>
    Basic_substring<Ch>& Basic_substring<Ch>::operator=(const Basic_substring<Ch>& s) {
        ps->replace(pos, n, s);
    }

    template<class Ch>
    Basic_substring<Ch>& Basic_substring<Ch>::operator=(const Ch* p) {
        ps->replace(pos, n, p);
    }

    template<class Ch>
    Basic_substring<Ch>& Basic_substring<Ch>::operator=(Ch c) {
        ps->replace(pos, n, 1, c);
    }

    template<class Ch>
    Basic_substring<Ch>::operator basic_string<Ch>() const {
        return basic_string<Ch>(*ps, pos, n);
    }

    template<class Ch>
    Basic_substring<Ch>::operator const Ch*() const {
        if(!cstr) {
            cstr = new Ch[n + 1];
            strcpy(cstr, basic_string<Ch>(*ps, pos, n).c_str());
        }
        return cstr;
    }

    class Regex_error { };

    class String : public basic_string<char> {
    public:
        String(const String& str, size_t pos, size_t n = basic_string<char>::npos) :
            basic_string<char>(str, pos, n) { }
        String(const char* s, size_t n) :
            basic_string<char>(s, n) { }
        String(const char* s) :
            basic_string<char>(s) { }
        String(size_t n, char c) :
            basic_string<char>(n, c) { }
        template<class In>
        String(In first, In last) :
            basic_string<char>(first, last) { }

        Basic_substring<char> regex_search(const basic_string<char>& pattern) {
            regoff_t i;
            const char* s = this->c_str();
            const char* p = pattern.c_str();
            regex_t preg;
            if (regcomp(&preg, p, 0))
                throw Regex_error();
            regmatch_t match;
            if(regexec(&preg, s, 1, &match, 0)) {
                // it would be a great idea to wrap this regex library in a class that
                // will properly free the compiled regex RAII-style regardless of whether
                // an exception is thrown.
                regfree(&preg);
                throw Regex_error();
            }
            regfree(&preg);
            if(match.rm_so < 0)
                return Basic_substring<char>(*this, string::npos, 0);
            else {
                return Basic_substring<char>(*this, match.rm_so, match.rm_eo - match.rm_so);
            }
        }
    };
}

int main() {
    using namespace ch20;

    String s("foo bar baz");
    Basic_substring<char> sub = s.regex_search("b[Aa]r.*");
    cout << sub << endl; // bar baz
}
