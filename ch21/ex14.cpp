#include <fstream>
#include <iostream>
#include <string>

// The exercise wasn't clear on whether the [] should index on a per-object
// basis, or still index on a character basis but return different kinds of
// objects. If indexing on a per-object basis, things get a lot more tricky
// since there's no way to tell where to seek before reading, since an object
// could be of any length (e.g., integers "1" vs. "1000000").
//
// If the exercise wanted the latter, we'd have to sequentially read N-1 objects
// before reading the Nth (and maybe cache those for seeking backwards later).
// This would be slower, would take more memory, or both.
//
// For now, I"ll assume the index remains a char index.
namespace ch21 {
    template<class T>
    class File_random_access {
        std::fstream f;

    public:
        class File_pos_ref {
            std::fstream* f;
            std::streampos p;
        public:
            File_pos_ref(std::fstream* f, std::streampos p) : f(f), p(p) { }

            operator T() {
                f->seekg(p, std::ios::beg);
                T val;
                *f >> val;
                return val;
            }

            File_pos_ref& operator=(T val) {
                f->seekp(p, std::ios::beg);
                *f << val;
                return *this;
            }
        };

        File_random_access(const char* file) : f(file) {
            f.exceptions(std::ios::failbit | std::ios::eofbit);
        }

        File_pos_ref operator[](std::streampos pos) {
            return File_pos_ref(&f, pos);
        }
    };
}

int main() {
    using namespace std;
    using namespace ch21;

    ofstream out("ex14.txt");
    out << "foo bar baz bot foo2 bar2 baz2 bot2";
    out.close();

    File_random_access<string> f("ex14.txt");
    f[4] = "xyz";
    cout << "f[0] = " << (string)f[0] << endl;
    cout << "f[4] = " << (string)f[4] << endl;
    cout << "f[8] = " << (string)f[8] << endl;
    try {
        cout << "char 50 = " << (string)f[50] << endl;
    }
    catch(ios::failure) {
        cout << "caught ios::failure" << endl;
    }
}
