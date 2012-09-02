#include <iostream>
#include <fstream>

// We would definitely want some error handling in a real implementation.
namespace ch25 {
    using std::string;
    using std::istream;
    using std::ostream;

    class Storable {
    public:
        void write_to(const char* location) {
            std::ofstream out(location);
            write_out(out);
        }

        void read_from(const char* location) {
            std::ifstream in(location);
            read_in(in);
        }

        template<class T>
        static T get(const char* location) {
            T val;
            val.read_from(location);
            return val;
        }

        virtual void write_out(ostream& out) = 0;
        virtual void read_in(istream& in) = 0;
    };

    class Storable_ints : public Storable {
        int x, y;
    public:
        Storable_ints() { }
        Storable_ints(int x, int y) : x(x), y(y) { }

        void read_in(istream& in) {
            in >> x >> y;
        }

        void write_out(ostream& out) {
            out << x << ' ' << y;
        }
    };

    class Storable_int_and_string : public Storable {
        int x;
        string y;
    public:
        Storable_int_and_string() { }
        Storable_int_and_string(int x, string y) : x(x), y(y) { }

        void read_in(istream& in) {
            in >> x >> y;
        }

        void write_out(ostream& out) {
            out << x << ' ' << y;
        }
    };
}

int main() {
    using namespace std;
    using namespace ch25;

    Storable_ints si(1, 2);
    Storable_int_and_string sis(3, "foo");

    si.write_to("ex11a.txt");
    sis.write_to("ex11b.txt");

    Storable_ints si2 = Storable::get<Storable_ints>("ex11a.txt");
    Storable_int_and_string sis2 = Storable::get<Storable_int_and_string>("ex11b.txt");
    si2.write_out(cout); // 1 2
    cout << '\n';
    sis2.write_out(cout); // 3 foo
    cout << '\n';
}
