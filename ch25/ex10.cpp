#include <istream>
#include <map>
#include <string>
#include <sstream>
#include <iostream>

namespace ch25 {
    using std::istream;
    using std::ostream;
    using std::map;
    using std::string;

    // For regular objects
    class Io_obj {
    public:
        virtual Io_obj* clone() const = 0;
        virtual ~Io_obj() {}
    };

    template<class T>
    class Io : public T, public Io_obj {
    public:
        Io* clone() const { return new Io(*this); }
        Io(istream& s) : T(s) { }
        static Io_obj* new_io(istream& s) { return new Io(s); }
    };

    // Specialization for int
    template<>
    class Io<int> : public Io_obj {
        int x;
    public:
        Io_obj* clone() const { return new Io(*this); }
        Io(istream& s) { s >> x; }
        operator int() const { return x; }
        static Io_obj* new_io(istream& s) { return new Io(s); }
    };

    // Specialization for string
    template<>
    class Io<string> : public Io_obj {
        string x;
    public:
        Io_obj* clone() const { return new Io(*this); }
        Io(istream& s) { s >> x; }
        operator string() const { return x; }
        static Io_obj* new_io(istream& s) { return new Io(s); }
    };

    // Simple test hierarchy
    class A {
        int x;
    public:
        A(istream& s) { s >> x; }
        virtual void put(ostream& os) const { os << x; }
    };

    class B : public A {
        string y;
    public:
        B(istream& s) : A(s) { s >> y; }
        void put(ostream& os) const { A::put(os); os << ',' << y; }
    };

    typedef Io_obj* (*PF) (istream&);
    map<string, PF> io_map;

    class No_class { };
    class Unknown_class { };

    bool get_word(istream& s, string& str) {
        return s >> str;
    }

    Io_obj* get_obj(istream& s) {
        string str;
        bool b = get_word(s, str);
        if(b == false) throw No_class();

        PF f = io_map[str];
        if(f == 0) throw Unknown_class();
        return f(s);
    }

    typedef Io<int> Io_int;
    typedef Io<string> Io_string;
}

int main() {
    using namespace std;
    using namespace ch25;

    io_map["A"] = Io<A>::new_io;
    io_map["B"] = Io<B>::new_io;
    io_map["int"] = Io_int::new_io;
    io_map["string"] = Io_string::new_io;

    string data = "int 43 string foo_bar_baz A 42 B 44 foo_bar";
    istringstream in(data);
    Io_obj* to = get_obj(in);
    int i = *dynamic_cast<Io<int>*>(to);
    cout << i << '\n'; // 43
    to = get_obj(in);
    string s = *dynamic_cast<Io<string>*>(to);
    cout << s << '\n'; // foo_bar_baz
    to = get_obj(in);
    A* t = dynamic_cast<A*>(to);
    t->put(cout); // 42
    cout << '\n';
    to = get_obj(in);
    t = dynamic_cast<A*>(to);
    t->put(cout); // 44,foo_bar
    cout << '\n';
}
