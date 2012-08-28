#include <fstream>
#include <iostream>

namespace ch21 {
    class File_random_access {
        std::fstream f;

    public:
        class File_pos_ref {
            std::fstream* f;
            std::streampos p;
        public:
            File_pos_ref(std::fstream* f, std::streampos p) : f(f), p(p) { }

            operator char() {
                f->seekg(p, std::ios::beg);
                return f->peek();
            }

            File_pos_ref& operator=(char c) {
                f->seekp(p, std::ios::beg);
                f->put(c);
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

    ofstream out("ex13.txt");
    out << "abcdefghijklmnopqrstuvwxyz";
    out.close();

    File_random_access f("ex13.txt");
    f[4] = 'X';
    cout << "char 3 = " << f[3] << endl;
    cout << "char 4 = " << f[4] << endl;
    cout << "char 5 = " << f[5] << endl;
    try {
        cout << "char 50 = " << f[50] << endl;
    }
    catch(ios::failure) {
        cout << "caught ios::failure" << endl;
    }
}
