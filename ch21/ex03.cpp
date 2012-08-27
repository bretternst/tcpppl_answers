#include <iostream>
#include <string>

namespace ch21 {
    using namespace std;

    struct Name_and_address {
        bool is_valid;
        string name;
        string address;
        string city;
        string state;
        unsigned int zip;
    };

    ostream& operator<<(ostream& out, const Name_and_address& x) {
        out << x.name << '\n' << x.address << '\n' << x.city << ", " << x.state << ' ' << x.zip;
    }

    // this was modified to more gracefully handle missing zip codes
    // and a meek attempt to recover from addresses with missing
    // or extra lines
    // the next answer will use proper delimiters and therefore be
    // much more robust.
    istream& operator>>(istream& in, Name_and_address& x) {
        Name_and_address tmp;
        if (getline(in, tmp.name)
            && getline(in, tmp.address)
            && getline(in, tmp.city, ',')
            && (in >> tmp.state)
            && (in >> tmp.zip)
            && in.ignore(80, '\n')) // eat the last newline
        {
            if(tmp.state.size() == 2) {
                tmp.state[0] = toupper(tmp.state[0]);
                tmp.state[1] = toupper(tmp.state[1]);
            }
            x = tmp;
            x.is_valid = true;
        } else if (!in.eof()) {
            x.is_valid = false;
            in.clear();
            in.ignore(-1, '\n');
        }
        return in;
    }
}

int main() {
    using namespace ch21;

    Name_and_address x;
    while(cin >> x) {
        if(x.is_valid)
            cout << x << endl << endl;
        x.is_valid = false;
    }
}
