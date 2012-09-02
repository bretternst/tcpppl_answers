#include <iostream>
#include <string>
#include <fstream>

// I feel like I don't understand what this exercise is really asking, so this is
// probably incomplete. Persistent has to be abstract because it doesn't know anything
// about how to write derived classes. Also, since derived virtual destructors are
// called first, they must call a protected function in Persistent which will write
// the object (but only once).
//
// Is Persistent also supposed to provide facilities for reading?
namespace ch25 {
    using std::string;
    using std::ostream;

    class Persistent {
        bool m_save;
        string loc;
    protected:
        void destruct() {
            if(m_save) {
                std::ofstream out(loc.c_str());
                write(out);
                m_save = false;
            }
        }
    public:
        Persistent() : m_save(false) { }
        void save(const string& location) {
            m_save = true;
            loc = location;
        }
        void no_save() {
            m_save = false;
        }
        virtual void write(ostream& out) = 0;
        virtual ~Persistent() { }
    };

    class Int : public Persistent {
        int x;
    public:
        Int(int x) : x(x) { }
        ~Int() { destruct(); }
        void write(ostream& out) {
            out << x;
        }
    };
}

int main() {
    using namespace std;
    using namespace ch25;

    {
        Int x(5);
        x.save("ex12.txt");
    }
    ifstream in("ex12.txt");
    int x;
    in >> x;
    cout << x << endl; // 5
}
