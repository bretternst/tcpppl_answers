#include <iostream>
#include <string>

namespace ch21 {
    using namespace std;

    // Only supports US-style addresses, e.g.
    // John Smith
    // 123 Fake St. Suite 45
    // Seattle, WA 98101
    struct Name_and_address {
        string name;
        string address;
        string city;
        string state;
        unsigned int zip;
    };

    ostream& operator<<(ostream& out, const Name_and_address& x) {
        out << x.name << '\n' << x.address << '\n' << x.city << ", " << x.state << ' ' << x.zip;
    }

    istream& operator>>(istream& in, Name_and_address& x) {
        Name_and_address tmp;
        if (getline(in, tmp.name)
            && getline(in, tmp.address)
            && getline(in, tmp.city, ',')
            && (in >> tmp.state)
            && (in >> tmp.zip)
            && in.ignore(80, '\n'))
        {
            if(tmp.state.size() == 2) {
                tmp.state[0] = toupper(tmp.state[0]);
                tmp.state[1] = toupper(tmp.state[1]);
            }
            x = tmp;
        }
        return in;
    }
}

int main() {
    using namespace ch21;

    Name_and_address x;
    while(cin >> x)
        cout << x << endl;
}
