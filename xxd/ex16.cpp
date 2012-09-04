#include <locale>
#include <iostream>
#include <fstream>

namespace xxd {
    class Cvt_to_upper : public std::codecvt<char, char, mbstate_t> {
    public:
        explicit Cvt_to_upper(size_t r = 0) : std::codecvt<char,char,mbstate_t>(r) { }
    protected:
        result do_in(state_type& s, const char* from, const char* from_end, const char*& from_next,
                     char* to, char* to_end, char*& to_next) const;

        result do_out(state_type& s, const char* from, const char* from_end, const char *& from_next,
                      char* to, char* to_end, char*& to_next) const {
            return codecvt<char,char,mbstate_t>::do_out(s, from, from_end, from_next, to, to_end, to_next);
        }

        result do_unshift(state_type&, extern_type* to, extern_type* to_end, extern_type*& to_next) const { return ok; }

        int do_encoding() const throw() { return 1; }
        bool do_always_noconv() const throw() { return false; }

        int do_length(const state_type&, const extern_type* from, const extern_type* from_end, size_t max) const { 
            std::cout << "hmmm" << std::endl;
            return from_end - from;
        }
        int do_max_length() const throw() { std::cout << "hmm" << std::endl; return 1024; }
    };

    std::codecvt<char,char,mbstate_t>::result
    Cvt_to_upper::do_in(state_type& s, const char* from, const char* from_end, const char*& from_next, 
                        char* to, char* to_end, char*& to_next) const {
        const char* i;
        char* j;
        for(i = from, j = to; i != from_end && j != to_end; i++, j++) {
            *j = toupper(*i);
        }
        from_next = i;
        to_next = j;
        return i == from_end && j == to_end ? ok : partial;
    }
}

int main() {
    using namespace std;
    using namespace xxd;

    // On my system, codecvt cannot be used with cin/cout, only file streams.
    locale ulocale(locale(), new Cvt_to_upper());
    {
        ofstream out("ex16.txt");
        out << "hello world!";
    }
    ifstream in("ex16.txt");
    in.imbue(ulocale);
    cout << in.rdbuf() << endl;
}
